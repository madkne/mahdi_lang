//
// Created by madkne on 2019.10.24
//

#ifndef __MAHDI_STRINGS_H
#define __MAHDI_STRINGS_H    1

#define StrArraySize(x) (sizeof(x)/sizeof(String))

//************************************************************************
uint32 STR_length(String s);
Boolean STR_equal(String s1, String s2);
String STR_convert_from(String s);
String STR_reomve_quotations(String s, String type);
String STR_append(String s1, String s2);
void STR_init(String *s, String val);
String STR_replace(String s, String old_s, String new_s, int8 num);
Boolean STR_search(StrList list, String s,uint32 size);
int32 STR_search_index(StrList list, String s,uint32 size);
Boolean STR_CH_equal(String s1, uint8 s2);
String STR_from_Longint(Longint num);
int32 STR_to_int32(String s);
Longint STR_to_Longint(String s);
Boolean STR_is_int32(String s);
Boolean STR_is_bool(String s);
Boolean STR_is_num(String str);
String STR_reverse(String s);
String STR_trim_space(String s);
String STR_trim_optimized_boolean(String str);
String STR_trim_number(String str);
String STR_trim_last_float(String str);
void STR_empty(String *s);
Boolean STR_is_empty(String s);
void STR_swap(String *s1, String *s2);
String STR_substring(String s, uint32 start, uint32 end);
int32 STR_indexof(String s, String s1, uint32 start);
String STR_to_upper_case(String text);
String STR_to_lower_case(String text);
String STR_from_int32(int32 x);
String STR_from_bool(Boolean x);
String STR_from_int64(int64 x);
Boolean STR_to_bool(String s);
double STR_to_double(String s);
String STR_from_double(double n, uint8 afterpoint);
String STR_multi_append(String s, String s1, String s2, String s3, String s4, String s5);
void STR_to_utf8(UString *ret, String val);
String STR_from_const(const char s[]);
Boolean STR_has_suffix(String s, String find);
String STR_set_negative_number(String s);
void STR_detachment_float(String s, String *s1, String *s2);
String STR_join(StrList s, uint32 len, String delimiter);
String STR_to_validate(String s);
String STR_from_path(String path);

//************************************************************************


#endif /*__MAHDI_STRINGS_H*/