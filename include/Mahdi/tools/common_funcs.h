#ifndef __MAHDI_COMMON_FUNCS_H
#define __MAHDI_COMMON_FUNCS_H    1


String COM_convert_mahdipath_to_abspath(String mahdipath,String extension,String *mahdiname);
// String COM_get_Mahdi_dir_path();
double COM_calculate_period_time(Longint start_time, String *unit);
void COM_print_struct(uint8 which);
// Boolean functions
//******************************************
Boolean BOOL_switch(Boolean b);
Boolean BOOL_switch_str(String b);
// int32 functions
//******************************************
double I32_power(double base, int32 power);
/*
void msg(String format, ...);

long_int longint_list_delete_first(longint_list *s, uint32 len);

void longint_list_append(longint_list *s, uint32 len, long_int n);

uint32 longint_list_join(longint_list *s, longint_list a1, uint32 a1_len, longint_list a2, uint32 a2_len);

void longint_list_init(longint_list *s, longint_list val, uint32 len);

String longint_list_print(longint_list s, uint32 len);

double int32_power(double base, int32 power);

String print_str_list(str_list s, uint32 len);

void print_struct(uint8 which);
void print_vaar(vaar_en s);
uint32 source_paths_search(str_utf8 path);

String validate_path(String s);

void str_list_append(str_list *s, String s1, uint32 len);

void str_list_init(str_list *s, str_list val, uint32 len);

void str_list_delete_first(str_list *s, uint32 len);

uint32 str_list_join(str_list *s, str_list a1, uint32 a1_len, str_list a2, uint32 a2_len);

Boolean str_list_equal(str_list s1, uint32 len1, str_list s2, uint32 len2);

void utf8_str_list_append(utf8_str_list *s, str_utf8 s1, uint32 len);

String remove_incorrect_pars(String str, int32 *invalid_pars);

FILE *utf8_file_open(String filename, String mode);

String replace_in_expression(String exp, String rep, int32 start, int32 end, Boolean remove_pars, Boolean is_trim);

String make_valid_double(String s);

String limit_decimal_huge_numbers(String s);

Boolean is_equal_arrays_indexes(String s1, String s2);

Boolean has_two_limiting(String s, uint8 l1, uint8 l2, Boolean ignore_space);
String replace_control_chars(String val);
String set_valid_control_chars(String val);
Boolean is_equal_data_types(uint8 t1, uint8 t2);
int32 search_int32_array(int32 a[], uint32 al, int32 n);
Boolean delete_int32_element_array(int32 a[], uint32 al, int32 n, Boolean delete_last);
uint8 convert_index_to_int32(String ind, int32 ret[], Boolean manage_ques);
String find_first_var_name(String exp, uint32 start, Boolean is_inverse, uint32 *endpoint);
String format_int32_array(int32 s[], uint32 start, uint32 end);
String return_type_structure(uint8 t);
String return_file_name_extension_path(String path, String *ext,Boolean must_ext);
String convert_sub_type_to_type(uint8 sub_type);
Boolean has_suffix_for_array(String s, str_list ar, uint32 ar_len);
*/
#endif /*__MAHDI_COMMON_FUNCS_H*/