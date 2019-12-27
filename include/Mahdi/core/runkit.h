//
// Created by madkne on 2019.12.18
//


#ifndef __MAHDI_RUNKIT_H
#define __MAHDI_RUNKIT_H  1

//********************************
Boolean RUNKIT_is_valid_name(String name, Boolean is_array);
uint8 RUNKIT_defvars_analyzing(StrList defvars,uint32 defvars_len, defvar vars_store[],Boolean just_syntax,Boolean just_basic_types);
void RUNKIT_calculate_value(String value, String type, String *ret_value, uint8 *ret_subtype);
String RUNKIT_calc_number_exp(String exp, uint8 target_type, uint8 *rettype);
String RUNKIT_determine_number(String s,uint8 main_type,uint8 *sub_type);
String RUNKIT_determine_string(String s,uint8 *sub_type);
String RUNKIT_determine_boolean(String s);
String RUNKIT_is_radix_number(String buf, uint8 type_exp);
String RUNKIT_radix_to_dec(String base, uint8 radix, uint8 type_exp);
uint8 RUNKIT_determine_type_numbers(String *num11, String *num22);
uint8 RUNKIT_determine_type_number(String num);
String RUNKIT_remove_unused_parenthesis(String value);
String RUNKIT_detect_basic_type_value(String value,uint8 *sub_type);
String RUNKIT_get_firstitem_listormap(String value);
uint32 RUNKIT_get_list_items(String list,StrList *items);
uint32 RUNKIT_get_map_items(String map,StrList *values,StrList *keys);
uint8 RUNKIT_priority_number_types(uint8 type_exp, uint8 tmp1);
String RUNKIT_calc_two_numbers(String num1, String num2, uint8 op, uint8 type_exp);
String RUNKIT_resize_to_int(String str_val);
String RUNKIT_resize_to_float(String str_val);
String RUNKIT_calc_string_exp(String exp, uint8 *sub_type);
String RUNKIT_calc_two_strings(String str1, String str2, uint8 op);
String RUNKIT_calc_boolean_exp(String exp);

#endif /*__MAHDI_RUNKIT_H*/


