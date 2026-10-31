#!/usr/bin/env python3

"""
pip install pyyaml
"""

import yaml


def visit_struct(struct_ident, schema, fqn_prefix, tab, preset_names, lines_cpp, lines_qt):
    next_tab   = tab + "    "
    next_tab_2 = next_tab + "    "

    # FQN setup
    fqn = fqn_prefix + struct_ident
    qt_class_name = fqn.replace("::", "_") + "_Qt" 

    lines_cpp.append(f'{tab}struct {struct_ident} : public CommonStruct {{')
    
    mapping_bool = {'True': 'true', 'False': 'false', True: 'true', False: 'false'}
    type_replacements = {
        'int': 'int64_t',
        'percent': 'int64_t',
        'bool': 'bool',
        'str': 'std::string',
    }
    type_defaults = {
        'int': '0',
        'percent': '0',
        'bool': 'false',
        'str': '',
    }
    def get_default_from_record(record):
        type_name = record.get('type', '')
        default_value = type_defaults.get(type_name, '')
        default_value = str(record.get('def', default_value))
        default_value = mapping_bool.get(default_value, default_value)
        if type_name == 'str':
            default_value = '"' + default_value + '"'
        return default_value

    def get_type_from_record(record):
        type_name = record.get('type', '?missingtype?')
        type_name = type_replacements.get(type_name, type_name)
        return type_name
    
    def is_scalar(record):
        type_name = record.get('type', '?missingtype?')
        return type_name in type_replacements

    def is_percent(record):
        type_name = record.get('type', '?missingtype?')
        return type_name == 'percent'

    if 'structs' in schema:
        for key, value in schema['structs'].items():
            visit_struct(key, value, fqn + "::", next_tab, preset_names, lines_cpp, lines_qt)
    
    constructor_lines = []
    
    for key, record in schema['members'].items():
        member_name = 'm_' + key
        default_value = get_default_from_record(record)
        type_name = get_type_from_record(record)

        range = ''
        if 'min' in record and 'max' in record:
            min_value = record.get('min', '0')
            max_value = record.get('max', '0')
            range = f'.setRange({min_value}, {max_value})'

        if is_percent(record):
            range = f'.setPercent()'

        if 'presets' in record:
            for preset_name, preset_value in record['presets'].items():
                preset_name = '"' + preset_name + '"';
                idx = len(preset_names)
                if preset_name in preset_names:
                    idx = preset_names.index(preset_name)
                else:
                    preset_names.append(preset_name)
                range = range + '.addPreset(' + str(idx) + ', ' + str(mapping_bool.get(preset_value, preset_value)) + ')'

        if is_scalar(record):
            constructor_lines.append(f'{next_tab_2}add("{key}", {member_name}, {default_value}){range};')
        else:
            constructor_lines.append(f'{next_tab_2}add("{key}", {member_name});')


        if default_value:
            default_value = ' ' + default_value + ' '
        lines_cpp.append(f'{next_tab}{type_name} {member_name}{{{default_value}}};')
        lines_cpp.append('')
    
    lines_cpp.append(f'{next_tab}{struct_ident}()')
    lines_cpp.append(f'{next_tab}{{')
    lines_cpp.extend(constructor_lines);


    lines_cpp.append(f'{next_tab}}}')
    lines_cpp.append(f'{tab}}};')


    qt_type_map = {
        'int': 'qlonglong',
        'bool': 'bool',
        'str': 'QString',
    }

    qt_properties = []
    qt_methods = []
    init_lists = []
    private_members = []

    for key, value in schema.get('members', {}).items():
        t = value.get('type', '')
        member_name = f'm_{key}'
        
        is_primitive = t in qt_type_map
        
        if is_primitive:
            qt_type = qt_type_map[t]
            qt_arg_type = qt_type
            if qt_type == 'QString':
                qt_arg_type = 'const QString&'

            qt_properties.append(f'Q_PROPERTY({qt_type} {key} READ get_{key} WRITE set_{key} NOTIFY dataChanged)')

            qt_methods.append(f'{qt_type} get_{key}() const {{ return get(m_data.{member_name}); }}')
            qt_methods.append(f'void set_{key}({qt_arg_type} val) {{ return set(m_data.{member_name}, val); }}')
            
            if t == 'int' or t == 'percent':
                qt_properties.append(f'Q_PROPERTY(qlonglong {key}_min READ get_{key}_min CONSTANT)')
                qt_properties.append(f'Q_PROPERTY(qlonglong {key}_max READ get_{key}_max CONSTANT)')
                qt_properties.append(f'Q_PROPERTY(bool {key}_isPercent READ get_{key}_isPercent CONSTANT)')
                
                min_val = value.get('min', '0')
                max_val = value.get('max', '0')
                is_percent = 'false'
                if t == 'percent':
                    is_percent = 'true';
                qt_methods.append(f'qlonglong get_{key}_min() const {{ return {min_val}; }}')
                qt_methods.append(f'qlonglong get_{key}_max() const {{ return {max_val}; }}')
                qt_methods.append(f'bool get_{key}_isPercent() const {{ return {is_percent}; }}')
        else:
            child_qt_type = f'{fqn.replace("::", "_")}_{t}_Qt'
            
            qt_properties.append(f'Q_PROPERTY({child_qt_type}* {key} READ get_{key} CONSTANT)')
            
            init_lists.append(f', {member_name}(data.{member_name})')
            qt_methods.append(f'{child_qt_type}* get_{key}() {{ return &{member_name}; }}')
            private_members.append(f'{child_qt_type} {member_name};')

    lines_qt.append(f'class {qt_class_name} : public BaseStruct_Qt {{')
    lines_qt.append('    Q_OBJECT')
    for prop in qt_properties:
        lines_qt.append(f'    {prop}')
    
    lines_qt.append('\npublic:')
    lines_qt.append(f'    explicit {qt_class_name}({fqn}& data)')
    lines_qt.append('        : BaseStruct_Qt(data)')
    lines_qt.append('        , m_data(data)')
    for init in init_lists:
        lines_qt.append(f'        {init}')
    lines_qt.append('    {')
    lines_qt.append('    }\n')
    
    for method in qt_methods:
        lines_qt.append(f'    {method}')
        lines_qt.append('')
        
    lines_qt.append('private:')
    lines_qt.append(f'    {fqn}& m_data;')
    for p_mem in private_members:
        lines_qt.append(f'')
        lines_qt.append(f'    {p_mem}')
    lines_qt.append('};\n')


def main():
    input_file = 'schema.yaml'
    with open(input_file, 'r') as file:
        schema = yaml.safe_load(file)

    with open("Config.hpp", "w") as output_cpp, open("ConfigQt.hpp", "w") as output_qt:
        header = f"#pragma once\n// Do not edit, this file was generated from {input_file} using update.py\n"
        
        lines_cpp = []
        lines_qt = []
        lines_cpp.append(header)
        
        lines_cpp.append('#include "StructBaseCpp.hpp"\n')
        lines_cpp.append('namespace D2ModGen {\n')
        
        lines_qt.append(header)
        lines_qt.append('#include "Config.hpp"')
        lines_qt.append('#include "StructBaseQt.hpp"\n')
        lines_qt.append('namespace D2ModGen {\n')
        
        preset_names = []

        visit_struct('Config', schema, '', '', preset_names, lines_cpp, lines_qt)
        
        lines_cpp.append('');
        lines_cpp.append('static inline std::vector<std::string> s_presetNames{ ' + ', '.join(preset_names) + " };")

        lines_cpp.append('}')
        lines_qt.append('}')
        output_cpp.write("\n".join(lines_cpp) + "\n")
        output_qt.write("\n".join(lines_qt) + "\n")


if __name__ == "__main__":
    main()
