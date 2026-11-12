#!/usr/bin/env python3

"""
pip install pyyaml
"""

import yaml
import sys

def get_index_from_list(li, value):
    idx = len(li)
    if value in li:
        idx = li.index(value)
    else:
        li.append(value)
    return idx

def value_to_string(value, doQuote = False):
    mapping_bool = {'True': 'true', 'False': 'false'}
    value = str(value)
    value = mapping_bool.get(value, value)
    if doQuote:
        value = '"' + value + '"'
    return value


def get_member_info(key, record, preset_names, counters, fqn):
    type_replacements = {
        'int': 'int64_t',
        'bool': 'bool',
        'str': 'std::string',
    }
    qt_type_map = {
        'int' : 'PropertyInt_Qt',
        'bool': 'PropertyBool_Qt',
        'str' : 'PropertyString_Qt',
    }
    property_getter = {
        'int' : 'getPropertyInt',
        'bool': 'getPropertyBool',
        'str' : 'getPropertyString',
    }
    
    member_name = f'm_{key}'
    
    type_name_orig = record.get('type', '')
    type_name = type_replacements.get(type_name_orig, '')
    
    add = 'addScalar'
    is_struct = False
    is_enum   = False
    if type_name_orig == 'struct':
        is_struct = True
        type_name = record['name']
        add = 'addStruct'
    if type_name_orig == 'enum':
        is_enum = True
        type_name = record['name']
        add = 'addEnum'
    is_primitive = not is_enum and not is_struct
    
    default_value_arg = ''
    member_initializer = '{}'
    setters = ''
    do_quote = type_name_orig == 'str'
    
    if is_primitive:
        if 'min' in record and 'max' in record:
            min_value = record.get('min', '0')
            max_value = record.get('max', '0')
            setters = setters + f'.setRange({min_value}, {max_value})'
        if 'zero' in record:
            zero_value = value_to_string(record['zero'], do_quote)
            member_initializer = '{ ' + zero_value + ' }'
            setters = setters + '.setZero(' + zero_value + ')'
        if 'recc' in record:
            recc_value = value_to_string(record['recc'], do_quote)
            setters = setters + '.setRecc(' + recc_value + ')'
        if 'percent' in record:
            setters = setters +  f'.setPercent()'
        if 'logScale' in record:
            setters = setters +  f'.setLogScale()'
        if 'constant' in record:
            setters = setters +  f'.setConstant()'
        if 'skipPresets' in record:
            setters = setters +  f'.skipPresets()'
        if 'presets' in record:
            for preset_name, preset_value in record['presets'].items():
                idx = get_index_from_list(preset_names, preset_name)
                preset_value = value_to_string(preset_value)
                setters = setters +  '.addPreset(' + str(idx) + ', ' + preset_value + ')'

    member_name = 'm_' + key

    result = {

    }
    result['registration_cpp'] = f'{add}("{key}", {member_name}){setters};'
    result['declaration_cpp'] = f'{type_name} {member_name}{member_initializer};'
    
    if is_primitive:
        child_qt_type = qt_type_map[type_name_orig]
        if record.get('constant', ''):
            child_qt_type = "Constant" + child_qt_type
        prop_index = counters[type_name_orig]
        counters[type_name_orig] = prop_index + 1
        prop_getter = property_getter[type_name_orig]

        result['qt_init'] = f'{member_name}(data.{prop_getter}({prop_index}))'
    else:
        child_qt_type = f'{fqn.replace("::", "_")}_{type_name}_Qt'
        result['qt_init'] = f'{member_name}(data.{member_name})'

    result['qt_property'] = f'Q_PROPERTY({child_qt_type}* {key} READ get_{key} CONSTANT)'
    result['qt_getter']   = f'{child_qt_type}* get_{key}() {{ return &{member_name}; }}'
    result['qt_member']   = f'{child_qt_type} {member_name};'

    
    return result;


def visit_struct(struct_ident, schema, fqn_prefix, tab, preset_names, lines_cpp, lines_qt):
    next_tab   = tab + "    "
    next_tab_2 = next_tab + "    "

    # FQN setup
    fqn = fqn_prefix + struct_ident
    qt_class_name = fqn.replace("::", "_") + "_Qt" 

    lines_cpp.append(f'{tab}struct {struct_ident} : public CommonStruct {{')
    
    

    if 'structs' in schema:
        for key, value in schema['structs'].items():
            visit_struct(key, value, fqn + "::", next_tab, preset_names, lines_cpp, lines_qt)
            
    cpp_members = []
    cpp_construction = []
    qt_properties = []
    qt_methods = []
    qt_init_list = []
    qt_members = []

    counters = {
        'int' : 0,
        'bool': 0,
        'str' : 0,
    }
    
    for key, record in schema['members'].items():
        info = get_member_info(key, record, preset_names, counters, fqn)

        cpp_members.append(next_tab + info['declaration_cpp'])
        cpp_members.append('')
        cpp_construction.append(next_tab_2 + info['registration_cpp'])
        qt_init_list.append(info['qt_init'])
        qt_properties.append(info['qt_property'])
        qt_methods.append(info['qt_getter'])
        qt_members.append(info['qt_member'])

    lines_cpp.extend(cpp_members);
    lines_cpp.append(f'{next_tab}{struct_ident}()')
    lines_cpp.append(f'{next_tab}{{')
    lines_cpp.extend(cpp_construction);
    lines_cpp.append(f'{next_tab}}}')
    lines_cpp.append(f'{tab}}};')

    qt_methods.append(f'Q_INVOKABLE void assign({qt_class_name}* another) {{ m_data.assign(another->m_data, true); }}')

    lines_qt.append(f'class {qt_class_name} : public CommonStruct_Qt {{')
    lines_qt.append('    Q_OBJECT')
    for prop in qt_properties:
        lines_qt.append(f'    {prop}')

    lines_qt.append('')
    lines_qt.append('public:')
    lines_qt.append(f'    explicit {qt_class_name}({fqn}& data)')
    lines_qt.append('        : CommonStruct_Qt(data)')
    lines_qt.append('        , m_data(data)')
    for init_record in qt_init_list:
        lines_qt.append(f'        , {init_record}')
    lines_qt.append('    {')
    lines_qt.append('    }')
    lines_qt.append('')
    
    for method in qt_methods:
        lines_qt.append(f'    {method}')
        lines_qt.append('')
        
    lines_qt.append('private:')
    lines_qt.append(f'    {fqn}& m_data;')
    for p_mem in qt_members:
        lines_qt.append(f'')
        lines_qt.append(f'    {p_mem}')
    lines_qt.append('};')
    lines_qt.append('')


def main():
    if len(sys.argv) < 2:
        print("Usage: python update.py <baseFilename>")
        sys.exit(1)

    base_filename = sys.argv[1]

    input_file = base_filename + '.yaml'
    with open(input_file, 'r') as file:
        schema = yaml.safe_load(file)

    with open(base_filename + ".hpp", "w") as output_cpp, open(base_filename + "Qt.hpp", "w") as output_qt:
        header = f"#pragma once\n// Do not edit, this file was generated from {input_file} using update.py\n"
        
        lines_cpp = []
        lines_qt = []
        lines_cpp.append(header)
        
        lines_cpp.append('#include "StructBaseCpp.hpp"\n')
        lines_cpp.append('namespace D2ModGen {\n')
        
        lines_qt.append(header)
        lines_qt.append(f'#include "{base_filename}.hpp"')
        lines_qt.append('#include "StructBaseQt.hpp"\n')
        lines_qt.append('namespace D2ModGen {\n')
        
        preset_names = []
        lines_cpp_impl = []
        lines_qt_impl = []

        visit_struct(base_filename, schema, '', '', preset_names, lines_cpp_impl, lines_qt_impl)

        if base_filename == 'Config':
            lines_cpp.append('[[maybe_unused]] static inline bool s_presetNamesSetter_unused = CommonStruct::setPresetNames({ "' + '", "'.join(preset_names) + '" });')
            lines_cpp.append('');

        lines_cpp.extend(lines_cpp_impl)
        lines_cpp.append('}')

        lines_qt.extend(lines_qt_impl)
        lines_qt.append('}')
        output_cpp.write("\n".join(lines_cpp) + "\n")
        output_qt.write("\n".join(lines_qt) + "\n")


if __name__ == "__main__":
    main()
