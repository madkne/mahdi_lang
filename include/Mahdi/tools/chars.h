#ifndef __MAHDI_CHARS_H
#define __MAHDI_CHARS_H    1

#include <Mahdi/system.h>

//******************************************
#define ChArraySize(x) (sizeof(x)/sizeof(uint8))
//******************************************
String CH_append(String s, uint8 c);
String CH_join(StrList s, uint8 sp, uint32 size, Boolean is_remove_empty);
uint32 CH_split(String s, uint8 sp, StrList *ret, Boolean is_remove_empty);
Boolean CH_search(uint8 list[], uint8 c,uint32 size);
int32 CH_search_index(uint8 list[], uint8 c,uint32 size);
uint32 CH_search_count(String s, uint8 c,uint32 size);
String CH_to_string(uint8 c);
int32 CH_last_indexof(String s, uint8 ch,uint32 size);
String CH_backspace(String s,uint32 size);
uint8 CH_to_uint8(uint8 c, Boolean *has_err);
Boolean CH_isnum(uint8 c);
Boolean CH_isspace(uint8 c);

#endif /*__MAHDI_CHARS_H*/