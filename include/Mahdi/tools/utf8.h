#ifndef __MAHDI_UTF8_H
#define __MAHDI_UTF8_H  1
//***************************************
#define IS_ONE_BYTE(X) (X>=0x0&&X<=0x7F)
#define CALC_UTF8_ONE_BYTES(X1)             (X1)
#define CALC_UTF8_TWO_BYTES(X1, X2)          ((X1 - 192) * 64 + X2 - 128)
#define CALC_UTF8_THREE_BYTES(X1, X2, X3)     ((X1-224) * 4096 + (X2 - 128) * 64 + X3 - 128)
#define CALC_UTF8_FOUR_BYTES(X1, X2, X3, X4)   ((X1 - 240) * 262144 + (X2 - 128) * 4096 + (X3 - 128) * 64 + X4 - 128)

//***************************************

//=>utf8 functions
int32 UTF_read_line(FILE *fp, UString *chars, uint32 *size);
uint8 UTF_need_bytes(uint8 c);
uint8 UTF_decode_bytes(String out, uint32 utf);
UString UTF_encode_bytes(String s);
FILE * UTF_file_open(String filename, String mode);
//utf8 string functions
uint8 USTR_max_bytes(UString s, Boolean is_just_ascii);
UString USTR_substring(UString s, uint32 start, uint32 end);
UString USTR_append(UString s1, UString s2);
uint32 USTR_length(UString s);
UString USTR_trim_space(UString s);
UString USTR_reverse(UString s);
String USTR_to_STR(UString s);
String USTR_to_unicode_str(UString s);
void USTR_print(String name, UString s, Boolean is_print_char);
void USTR_init(UString *s, UString val);
Boolean USTR_equal(UString s1, UString s2);
UString USTR_replace(UString s, UString old_s, UString new_s, int8 num);
UString USTR_simple_replace(UString s, String old, String new, int8 num);
String USTR_to_bytes_str(UString s);
//=>utf8 char functions
UString UCH_append(UString s, uint32 c);
//utf8 string list functions
void USLIST_init(UStrList *s, UStrList val, uint32 len);
void USLIST_append(UStrList *s, UString s1, uint32 len);


#endif /*__MAHDI_UTF8_H*/