#!/usr/bin/env python3

"""
Usage:
    python tables_update.py Config
    python tables_update.py Config.yaml

Reads Config.yaml and writes Config.hpp and Config.cpp.
Requires PyYAML.
"""

import sys
from pathlib import Path

import yaml


CPP_TYPES = {
    "string": "std::string",
    "bool": "std::optional<bool>",
    "int": "std::optional<int>",
}


def cpp_string(value):
    """Return a quoted C++ string literal."""
    value = value.replace("\\", "\\\\")
    value = value.replace('"', '\\"')
    return f'"{value}"'


def validate_column_name(value, context):
    if not isinstance(value, str) or not value:
        raise ValueError(
            f"{context}: column name must be a non-empty string"
        )

    if value != value.lower():
        raise ValueError(
            f"{context}: column name '{value}' must be lowercase"
        )

    return value


def validate_positive_size(value, context, property_name):
    if not isinstance(value, int) or value <= 0:
        raise ValueError(
            f"{context}: {property_name} must be a positive integer"
        )

    return value


def validate_start(value, context):
    if not isinstance(value, int):
        raise ValueError(f"{context}: start must be an integer")

    return value


def validate_pattern(value, context):
    if not isinstance(value, str) or "{i}" not in value:
        raise ValueError(f"{context}: column must contain '{{i}}'")

    return value


def fixed_array_member(field_name, shape, flat_index):
    indices = []

    for dimension in reversed(shape):
        indices.append(flat_index % dimension)
        flat_index //= dimension

    indices.reverse()

    return field_name + "".join(
        f"[{index}]" for index in indices
    )


def process_fixed_scalar_array(table_name, field_name, record):
    context = f"table '{table_name}', field '{field_name}'"
    array = record["fixed_array"]

    type_name = record.get("type")
    if type_name not in CPP_TYPES:
        raise ValueError(
            f"{context}: type must be one of {', '.join(CPP_TYPES)}"
        )

    cpp_type = CPP_TYPES[type_name]
    explicit_columns = array.get("columns")
    shape = array.get("shape")

    if explicit_columns is not None:
        if not isinstance(explicit_columns, list) or not explicit_columns:
            raise ValueError(
                f"{context}: fixed_array.columns must be a non-empty list"
            )

        canonical_columns = [
            validate_column_name(column, context)
            for column in explicit_columns
        ]

        if shape is None:
            shape = [len(canonical_columns)]
        else:
            if not isinstance(shape, list) or not shape:
                raise ValueError(
                    f"{context}: fixed_array.shape must be a non-empty list"
                )

            for dimension in shape:
                validate_positive_size(
                    dimension,
                    context,
                    "fixed_array.shape dimensions",
                )

            element_count = 1

            for dimension in shape:
                element_count *= dimension

            if element_count != len(canonical_columns):
                raise ValueError(
                    f"{context}: fixed_array.shape describes "
                    f"{element_count} elements, but "
                    f"{len(canonical_columns)} columns were provided"
                )

        if "size" in array and array["size"] != len(canonical_columns):
            raise ValueError(
                f"{context}: fixed_array.size does not match columns"
            )
    else:
        if shape is not None:
            raise ValueError(
                f"{context}: fixed_array.shape currently requires "
                "an explicit columns list"
            )

        size = validate_positive_size(
            array.get("size"),
            context,
            "fixed_array.size",
        )

        start = validate_start(
            array.get("start", 1),
            f"{context}: fixed_array",
        )

        pattern = validate_pattern(
            array.get("column"),
            f"{context}: fixed_array",
        )

        canonical_columns = []

        for array_index in range(size):
            csv_index = start + array_index
            column = pattern.format(i=csv_index)

            canonical_columns.append(
                validate_column_name(column, context)
            )

        shape = [size]

    cpp_array_type = cpp_type

    for dimension in reversed(shape):
        cpp_array_type = f"std::array<{cpp_array_type}, {dimension}>"

    missing = array.get("missing")

    if missing is not None:
        if type_name != "bool" or not isinstance(missing, bool):
            raise ValueError(
                f"{context}: fixed_array.missing is currently supported "
                "only as a boolean value for bool arrays"
            )

    columns = []
    read = []
    write = []

    for flat_index, column in enumerate(canonical_columns):
        enum_name = f"c_{field_name}{flat_index}"
        member = fixed_array_member(
            field_name,
            shape,
            flat_index,
        )

        columns.append((enum_name, column))

        read_arguments = f"Column::{enum_name}, {member}"

        if missing is not None:
            read_arguments += ", true" if missing else ", false"

        read.append(f"reader.read({read_arguments});")
        write.append(
            f"writer.write(Column::{enum_name}, {member});"
        )

    return {
        "declaration": f"{cpp_array_type} {field_name};",
        "columns": columns,
        "read": read,
        "write": write,
    }


def process_fixed_record_array(table_name, field_name, record):
    context = f"table '{table_name}', field '{field_name}'"
    array = record["fixed_array"]

    size = validate_positive_size(
        array.get("size"),
        context,
        "record fixed_array.size",
    )

    item_name = array.get("name")

    if not isinstance(item_name, str) or not item_name:
        raise ValueError(
            f"{context}: record fixed_array requires a name"
        )

    fields = array.get("fields")

    if not isinstance(fields, dict) or not fields:
        raise ValueError(
            f"{context}: record fixed_array.fields must be "
            "a non-empty mapping"
        )

    declaration_lines = [
        f"struct {item_name} {{",
        f"    bool operator==(const {item_name}&) "
        "const noexcept = default;",
    ]

    processed_fields = []

    for subfield_name, subfield_record in fields.items():
        subfield_context = f"{context}, field '{subfield_name}'"

        if not isinstance(subfield_record, dict):
            raise ValueError(
                f"{subfield_context}: expected a mapping"
            )

        type_name = subfield_record.get("type")

        if type_name not in CPP_TYPES:
            raise ValueError(
                f"{subfield_context}: type must be one of "
                f"{', '.join(CPP_TYPES)}"
            )

        subfield_columns = subfield_record.get("columns")

        if not isinstance(subfield_columns, list):
            raise ValueError(
                f"{subfield_context}: record fixed_array fields "
                "require a columns list"
            )

        if len(subfield_columns) != size:
            raise ValueError(
                f"{subfield_context}: expected {size} columns, "
                f"got {len(subfield_columns)}"
            )

        subfield_columns = [
            validate_column_name(column, subfield_context)
            for column in subfield_columns
        ]

        cpp_type = CPP_TYPES[type_name]

        declaration_lines.append(
            f"{cpp_type} {subfield_name};"
        )

        processed_fields.append({
            "name": subfield_name,
            "columns": subfield_columns,
        })

    declaration_lines.extend([
        "};",
        f"std::array<{item_name}, {size}> {field_name};",
    ])

    columns = []
    read = []
    write = []

    for array_index in range(size):
        for subfield in processed_fields:
            subfield_name = subfield["name"]
            column = subfield["columns"][array_index]

            enum_name = (
                f"c_{field_name}{array_index}_{subfield_name}"
            )
            member = (
                f"{field_name}[{array_index}].{subfield_name}"
            )

            columns.append((enum_name, column))

            read.append(
                f"reader.read(Column::{enum_name}, {member});"
            )
            write.append(
                f"writer.write(Column::{enum_name}, {member});"
            )

    return {
        "declaration": "\n".join(declaration_lines),
        "columns": columns,
        "read": read,
        "write": write,
    }


def process_struct(table_name, field_name, record):
    context = f"table '{table_name}', field '{field_name}'"
    struct_schema = record["struct"]

    struct_name = struct_schema.get("name")

    if not isinstance(struct_name, str) or not struct_name:
        raise ValueError(f"{context}: struct requires a name")

    fields = struct_schema.get("fields")

    if not isinstance(fields, dict) or not fields:
        raise ValueError(
            f"{context}: struct.fields must be a non-empty mapping"
        )

    declaration_lines = [
        f"struct {struct_name} {{",
        f"    bool operator==(const {struct_name}&) "
        "const noexcept = default;",
    ]

    columns = []
    read = []
    write = []

    for subfield_name, subfield_record in fields.items():
        subfield_context = f"{context}, field '{subfield_name}'"

        if not isinstance(subfield_record, dict):
            raise ValueError(
                f"{subfield_context}: expected a mapping"
            )

        type_name = subfield_record.get("type")

        if type_name not in CPP_TYPES:
            raise ValueError(
                f"{subfield_context}: type must be one of "
                f"{', '.join(CPP_TYPES)}"
            )

        column = validate_column_name(
            subfield_record.get("column"),
            subfield_context,
        )

        cpp_type = CPP_TYPES[type_name]
        enum_name = f"c_{field_name}_{subfield_name}"
        member = f"{field_name}.{subfield_name}"

        declaration_lines.append(
            f"{cpp_type} {subfield_name};"
        )

        columns.append((enum_name, column))

        read.append(
            f"reader.read(Column::{enum_name}, {member});"
        )
        write.append(
            f"writer.write(Column::{enum_name}, {member});"
        )

    declaration_lines.extend([
        "};",
        f"{struct_name} {field_name};",
    ])

    return {
        "declaration": "\n".join(declaration_lines),
        "columns": columns,
        "read": read,
        "write": write,
    }


def process_dynamic_array(table_name, field_name, record):
    context = f"table '{table_name}', field '{field_name}'"
    array = record.get("dynamic_array")

    if not isinstance(array, dict):
        raise ValueError(
            f"{context}: dynamic_array must be a mapping"
        )

    size = validate_positive_size(
        array.get("max"),
        context,
        "dynamic_array.max",
    )

    start = validate_start(
        array.get("start", 1),
        f"{context}: dynamic_array",
    )

    fields = array.get("fields")

    if fields is None:
        type_name = record.get("type")

        if type_name != "string":
            raise ValueError(
                f"{context}: primitive dynamic_array currently "
                "requires type: string"
            )

        pattern = validate_pattern(
            array.get("column"),
            f"{context}: dynamic_array",
        )

        columns = []
        read = [
            f"{field_name}.clear();",
            f"{field_name}.reserve({size});",
        ]
        write = []

        for array_index in range(size):
            csv_index = start + array_index
            column = validate_column_name(
                pattern.format(i=csv_index),
                context,
            )
            enum_name = f"c_{field_name}{array_index}"

            columns.append((enum_name, column))

            read.extend([
                "{",
                "    std::string value;",
                f"    reader.read(Column::{enum_name}, value);",
                "    if (!value.empty())",
                f"        {field_name}.push_back(value);",
                "}",
            ])

            write.extend([
                f"if ({field_name}.size() > {array_index})",
                "{",
                f"    writer.write(Column::{enum_name}, "
                f"{field_name}[{array_index}]);",
                "}",
            ])

        return {
            "declaration": f"std::vector<std::string> {field_name};",
            "columns": columns,
            "read": read,
            "write": write,
        }

    if not isinstance(fields, dict) or not fields:
        raise ValueError(
            f"{context}: dynamic_array.fields must be "
            "a non-empty mapping"
        )

    item_name = array.get("name")

    if not isinstance(item_name, str) or not item_name:
        raise ValueError(
            f"{context}: record dynamic_array requires a name"
        )

    nonempty = array.get("nonempty")

    if nonempty not in fields:
        raise ValueError(
            f"{context}: nonempty must name one of the record fields"
        )

    declaration_lines = [
        f"struct {item_name} {{",
        f"    bool operator==(const {item_name}&) "
        "const noexcept = default;",
    ]

    processed_fields = []

    for subfield_name, subfield_record in fields.items():
        subfield_context = f"{context}, field '{subfield_name}'"

        if not isinstance(subfield_record, dict):
            raise ValueError(
                f"{subfield_context}: expected a mapping"
            )

        type_name = subfield_record.get("type")

        if type_name not in CPP_TYPES:
            raise ValueError(
                f"{subfield_context}: type must be one of "
                f"{', '.join(CPP_TYPES)}"
            )

        pattern = validate_pattern(
            subfield_record.get("column"),
            subfield_context,
        )

        cpp_type = CPP_TYPES[type_name]

        declaration_lines.append(
            f"{cpp_type} {subfield_name};"
        )

        processed_fields.append({
            "name": subfield_name,
            "pattern": pattern,
            "type": type_name,
        })

    nonempty_type = next(
        field["type"]
        for field in processed_fields
        if field["name"] == nonempty
    )

    if nonempty_type != "string":
        raise ValueError(
            f"{context}: nonempty field '{nonempty}' must be a string"
        )

    exclude = array.get("exclude", [])

    if not isinstance(exclude, list):
        raise ValueError(
            f"{context}: dynamic_array.exclude must be a list"
        )

    valid_condition = f"!item.{nonempty}.empty()"

    for excluded_value in exclude:
        if not isinstance(excluded_value, str):
            raise ValueError(
                f"{context}: dynamic_array.exclude values "
                "must be strings"
            )

        valid_condition += (
            f" && item.{nonempty} != {cpp_string(excluded_value)}"
        )

    declaration_lines.extend([
        "};",
        f"std::vector<{item_name}> {field_name};",
    ])

    columns = []
    read = [
        f"{field_name}.clear();",
        f"{field_name}.reserve({size});",
    ]
    write = []

    for array_index in range(size):
        csv_index = start + array_index

        read.append("{")
        read.append(f"    {item_name} item;")

        write.append(
            f"if ({field_name}.size() > {array_index})"
        )
        write.append("{")

        for subfield in processed_fields:
            subfield_name = subfield["name"]
            column = subfield["pattern"].format(i=csv_index)
            column = validate_column_name(column, context)

            enum_name = (
                f"c_{field_name}{array_index}_{subfield_name}"
            )

            columns.append((enum_name, column))

            read.append(
                f"    reader.read(Column::{enum_name}, "
                f"item.{subfield_name});"
            )

            write.append(
                f"    writer.write(Column::{enum_name}, "
                f"{field_name}[{array_index}].{subfield_name});"
            )

        read.extend([
            f"    if ({valid_condition})",
            f"        {field_name}.push_back(item);",
            "}",
        ])

        write.append("}")

    return {
        "declaration": "\n".join(declaration_lines),
        "columns": columns,
        "read": read,
        "write": write,
    }


def process_affix_list(table_name, field_name, record):
    context = f"table '{table_name}', field '{field_name}'"
    affix_list = record.get("affix_list")

    if not isinstance(affix_list, dict):
        raise ValueError(
            f"{context}: affix_list must be a mapping"
        )

    dynamic_schema = dict(affix_list)
    dynamic_schema["name"] = "AffixProp_codegen"
    dynamic_schema["nonempty"] = "code"

    size = validate_positive_size(
        dynamic_schema.get("max"),
        context,
        "affix_list.max",
    )

    result = process_dynamic_array(
        table_name,
        field_name,
        {
            "dynamic_array": dynamic_schema,
        },
    )

    limit_name = f"{field_name}_limit"

    result["declaration"] = "\n".join([
        f"std::vector<AffixProp_codegen> {field_name};",
        f"std::size_t {limit_name} = {size};",
    ])

    limit_read = [
        f"{limit_name} = 0;",
    ]

    fields = dynamic_schema.get("fields", {})

    for array_index in range(size):
        available_checks = []

        for subfield_name in fields:
            enum_name = (
                f"c_{field_name}{array_index}_{subfield_name}"
            )
            available_checks.append(
                f"reader.available(Column::{enum_name})"
            )

        if not available_checks:
            continue

        condition = " || ".join(available_checks)

        limit_read.extend([
            f"if ({condition})",
            f"    {limit_name} = {array_index + 1};",
        ])

    result["read"] = limit_read + result["read"]

    return result


def process_field(table_name, field_name, record):
    context = f"table '{table_name}', field '{field_name}'"

    if not isinstance(record, dict):
        raise ValueError(f"{context}: expected a mapping")

    fixed_array = record.get("fixed_array")
    dynamic_array = record.get("dynamic_array")
    struct_schema = record.get("struct")
    affix_list = record.get("affix_list")

    variant_count = sum(
        variant is not None
        for variant in (
            fixed_array,
            dynamic_array,
            struct_schema,
            affix_list,
        )
    )

    if variant_count > 1:
        raise ValueError(
            f"{context}: fixed_array, dynamic_array, struct, and "
            "affix_list are mutually exclusive"
        )

    if dynamic_array is not None:
        return process_dynamic_array(
            table_name,
            field_name,
            record,
        )

    if struct_schema is not None:
        if not isinstance(struct_schema, dict):
            raise ValueError(
                f"{context}: struct must be a mapping"
            )

        return process_struct(
            table_name,
            field_name,
            record,
        )

    if affix_list is not None:
        return process_affix_list(
            table_name,
            field_name,
            record,
        )

    if fixed_array is not None:
        if not isinstance(fixed_array, dict):
            raise ValueError(
                f"{context}: fixed_array must be a mapping"
            )

        if "fields" in fixed_array:
            return process_fixed_record_array(
                table_name,
                field_name,
                record,
            )

        return process_fixed_scalar_array(
            table_name,
            field_name,
            record,
        )

    type_name = record.get("type")

    if type_name not in CPP_TYPES:
        raise ValueError(
            f"{context}: type must be one of {', '.join(CPP_TYPES)}"
        )

    cpp_type = CPP_TYPES[type_name]
    column = validate_column_name(
        record.get("column"),
        context,
    )
    enum_name = f"c_{field_name}"

    read_arguments = f"Column::{enum_name}, {field_name}"
    missing = record.get("missing")

    if missing is not None:
        if type_name != "bool" or not isinstance(missing, bool):
            raise ValueError(
                f"{context}: missing is currently supported only "
                "as a boolean value for bool fields"
            )

        read_arguments += ", true" if missing else ", false"

    return {
        "declaration": f"{cpp_type} {field_name};",
        "columns": [(enum_name, column)],
        "read": [
            f"reader.read({read_arguments});"
        ],
        "write": [
            f"writer.write(Column::{enum_name}, {field_name});"
        ],
    }


def resolve_record(record_name, record, bases, stack=()):
    if record_name in stack:
        cycle = " -> ".join(stack + (record_name,))
        raise ValueError(f"Cyclic extends chain: {cycle}")

    if not isinstance(record, dict):
        raise ValueError(f"'{record_name}': expected a mapping")

    extends = record.get("extends", [])

    if isinstance(extends, str):
        extends = [extends]

    if not isinstance(extends, list):
        raise ValueError(
            f"'{record_name}': extends must be a string or a list"
        )

    fields = {}
    aliases = {}

    for base_name in extends:
        if base_name not in bases:
            raise ValueError(
                f"'{record_name}': unknown base '{base_name}'"
            )

        base = resolve_record(
            base_name,
            bases[base_name],
            bases,
            stack + (record_name,),
        )

        fields.update(base["fields"])
        aliases.update(base["aliases"])

    own_fields = record.get("fields", {})
    own_aliases = record.get("aliases", {})

    if not isinstance(own_fields, dict):
        raise ValueError(
            f"'{record_name}': fields must be a mapping"
        )

    if not isinstance(own_aliases, dict):
        raise ValueError(
            f"'{record_name}': aliases must be a mapping"
        )

    fields.update(own_fields)
    aliases.update(own_aliases)

    return {
        "fields": fields,
        "aliases": aliases,
    }


def expand_affix_lists(table_schema, records):
    for field_name, field_schema in table_schema["fields"].items():
        if "affix_list" not in field_schema:
            continue

        context = f"field '{field_name}'"
        affix_list = field_schema["affix_list"]

        if not isinstance(affix_list, dict):
            raise ValueError(
                f"{context}: affix_list must be a mapping"
            )

        record_name = affix_list.get("record", "AffixProp")

        if record_name not in records:
            raise ValueError(
                f"{context}: unknown affix record '{record_name}'"
            )

        record_fields = records[record_name].get("fields")

        if not isinstance(record_fields, dict):
            raise ValueError(
                f"{context}: record '{record_name}' has no fields"
            )

        column_patterns = affix_list.get("columns")

        if not isinstance(column_patterns, dict) or not column_patterns:
            raise ValueError(
                f"{context}: affix_list.columns must be "
                "a non-empty mapping"
            )

        expanded_fields = {}

        for subfield_name, pattern in column_patterns.items():
            if subfield_name not in record_fields:
                raise ValueError(
                    f"{context}: '{subfield_name}' is not a member "
                    f"of record '{record_name}'"
                )

            record_field = record_fields[subfield_name]

            expanded_fields[subfield_name] = {
                "column": pattern,
                "type": record_field["type"],
            }

        expanded_field_schema = dict(field_schema)
        expanded_affix_list = dict(affix_list)

        expanded_affix_list.pop("columns", None)
        expanded_affix_list.pop("record", None)
        expanded_affix_list["fields"] = expanded_fields

        expanded_field_schema["affix_list"] = expanded_affix_list
        table_schema["fields"][field_name] = expanded_field_schema

    return table_schema


def emit_record(record_name, record_schema):
    context = f"record '{record_name}'"

    if not isinstance(record_schema, dict):
        raise ValueError(f"{context}: expected a mapping")

    fields = record_schema.get("fields")

    if not isinstance(fields, dict) or not fields:
        raise ValueError(
            f"{context}: fields must be a non-empty mapping"
        )

    struct_name = f"{record_name}_codegen"
    lines = [
        f"struct {struct_name} {{",
        f"    bool operator==(const {struct_name}&) "
        "const noexcept = default;",
    ]

    for field_name, field_schema in fields.items():
        field_context = f"{context}, field '{field_name}'"

        if not isinstance(field_schema, dict):
            raise ValueError(
                f"{field_context}: expected a mapping"
            )

        type_name = field_schema.get("type")

        if type_name not in CPP_TYPES:
            raise ValueError(
                f"{field_context}: type must be one of "
                f"{', '.join(CPP_TYPES)}"
            )

        lines.append(
            f"    {CPP_TYPES[type_name]} {field_name};"
        )

    lines.append("};")
    lines.append("")

    return lines


def emit_table(table_name, table_schema):
    context = f"table '{table_name}'"

    if not isinstance(table_schema, dict):
        raise ValueError(f"{context}: expected a mapping")

    fields = table_schema.get("fields")

    if not isinstance(fields, dict) or not fields:
        raise ValueError(
            f"{context}: fields must be a non-empty mapping"
        )

    declarations = []
    columns = []
    read_statements = []
    write_statements = []

    for field_name, field_schema in fields.items():
        field = process_field(
            table_name,
            field_name,
            field_schema,
        )

        declarations.append(field["declaration"])
        columns.extend(field["columns"])
        read_statements.extend(field["read"])
        write_statements.extend(field["write"])

    canonical_names = [
        column
        for _, column in columns
    ]

    if len(canonical_names) != len(set(canonical_names)):
        raise ValueError(
            f"{context}: duplicate canonical CSV column"
        )

    aliases = table_schema.get("aliases", {})

    if not isinstance(aliases, dict):
        raise ValueError(f"{context}: aliases must be a mapping")

    normalized_aliases = []

    for alias, canonical in aliases.items():
        alias = validate_column_name(
            alias,
            f"{context}, aliases",
        )
        canonical = validate_column_name(
            canonical,
            f"{context}, aliases",
        )

        if canonical not in canonical_names:
            raise ValueError(
                f"{context}: alias '{alias}' refers to unknown "
                f"canonical column '{canonical}'"
            )

        normalized_aliases.append((alias, canonical))

    struct_name = f"DataRow_{table_name}_codegen"

    hpp = [
        f"struct {struct_name} {{",
        f"    bool operator==(const {struct_name}&) "
        "const noexcept = default;",
        "",
    ]

    for declaration in declarations:
        for declaration_line in declaration.split("\n"):
            if declaration_line:
                hpp.append(f"    {declaration_line}")

    hpp.extend([
        "",
        "private:",
        "    enum Column : std::size_t;",
        "",
        "public:",
        "    static const GeneratedTableSchema& tableSchema();",
        "",
        "    void readData(const CsvRowReader& reader);",
        "",
        "    void writeData(CsvRowWriter& writer) const;",
        "};",
        "",
    ])

    cpp = [
        f"enum {struct_name}::Column : std::size_t {{",
    ]

    for enum_name, _ in columns:
        cpp.append(f"    {enum_name},")

    cpp.extend([
        "};",
        "",
        f"const GeneratedTableSchema& {struct_name}::tableSchema()",
        "{",
        "    static const GeneratedTableSchema schema{",
        "        .columns = {",
    ])

    for _, column in columns:
        cpp.append(f"            {cpp_string(column)},")

    cpp.append("        },")
    cpp.append("        .aliases = {")

    for alias, canonical in normalized_aliases:
        cpp.append(
            "            { "
            f"{cpp_string(alias)}, {cpp_string(canonical)} "
            "},"
        )

    cpp.extend([
        "        },",
        "    };",
        "    return schema;",
        "}",
        "",
        f"void {struct_name}::readData(const CsvRowReader& reader)",
        "{",
    ])

    for statement in read_statements:
        if statement:
            cpp.append(f"    {statement}")
        else:
            cpp.append("")

    cpp.extend([
        "}",
        "",
        f"void {struct_name}::writeData(CsvRowWriter& writer) const",
        "{",
    ])

    for statement in write_statements:
        if statement:
            cpp.append(f"    {statement}")
        else:
            cpp.append("")

    cpp.extend([
        "}",
        "",
    ])

    return {
        "hpp": hpp,
        "cpp": cpp,
    }


def main():
    if len(sys.argv) != 2:
        print("Usage: python tables_update.py <schema-base-name>")
        print("Example: python tables_update.py Config")
        return 1

    base_path = Path(sys.argv[1])
    input_path = base_path.with_suffix(".yaml")
    hpp_path = base_path.with_suffix(".hpp")
    cpp_path = base_path.with_suffix(".cpp")

    with input_path.open("r", encoding="utf-8") as input_file:
        schema = yaml.safe_load(input_file)

    if not isinstance(schema, dict):
        raise ValueError("Schema root must be a mapping")

    tables = schema.get("tables")

    if not isinstance(tables, dict) or not tables:
        raise ValueError(
            "Schema must contain a non-empty 'tables' mapping"
        )

    bases = schema.get("bases", {})

    if not isinstance(bases, dict):
        raise ValueError("'bases' must be a mapping")

    records = schema.get("records", {})

    if not isinstance(records, dict):
        raise ValueError("'records' must be a mapping")

    generated_comment = (
        f"// Do not edit. Generated from {input_path.name} "
        "using tables_update.py."
    )

    hpp_output = [
        "#pragma once",
        "",
        generated_comment,
        "",
        '#include "TablesBaseCpp.hpp"',
        "",
        "namespace D2ModGen {",
        "// clang-format off",
        "",
    ]

    cpp_output = [
        generated_comment,
        "",
        f'#include "{hpp_path.name}"',
        "",
        "namespace D2ModGen {",
        "// clang-format off",
        "",
    ]

    for record_name, record_schema in records.items():
        hpp_output.extend(
            emit_record(record_name, record_schema)
        )

    for table_name, table_schema in tables.items():
        resolved_schema = resolve_record(
            table_name,
            table_schema,
            bases,
        )

        resolved_schema = expand_affix_lists(
            resolved_schema,
            records,
        )

        emitted = emit_table(
            table_name,
            resolved_schema,
        )

        hpp_output.extend(emitted["hpp"])
        cpp_output.extend(emitted["cpp"])

    hpp_output.extend([
        "// clang-format on",
        "} // namespace D2ModGen",
        "",
    ])

    cpp_output.extend([
        "// clang-format on",
        "} // namespace D2ModGen",
        "",
    ])

    hpp_path.write_text(
        "\n".join(hpp_output),
        encoding="utf-8",
    )

    cpp_path.write_text(
        "\n".join(cpp_output),
        encoding="utf-8",
    )

    return 0


if __name__ == "__main__":
    sys.exit(main())
