//
// Created by madkne on 2019.12.18
//


#ifndef __MAHDI_RUNKIT_H
#define __MAHDI_RUNKIT_H  1

//********************************
Boolean RUNKIT_is_valid_name(String name, Boolean is_array);
uint8 RUNKIT_defvars_analyzing(StrList defvars,uint32 defvars_len, defvar vars_store[],Boolean just_syntax);
String RUNKIT_detect_basic_type_value(String value,uint8 *sub_type);
String RUNKIT_get_firstitem_listormap(String value);
uint32 RUNKIT_get_list_items(String list,StrList *items);
uint32 RUNKIT_get_map_items(String map,StrList *values,StrList *keys);

#endif /*__MAHDI_RUNKIT_H*/


