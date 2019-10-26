#ifndef __UTF8_H
#define __UTF8_H
//***************************************
#define IS_ONE_BYTE(X) (X>=0x0&&X<=0x7F)
#define CALC_UTF8_ONE_BYTES(X1)             (X1)
#define CALC_UTF8_TWO_BYTES(X1, X2)          ((X1 - 192) * 64 + X2 - 128)
#define CALC_UTF8_THREE_BYTES(X1, X2, X3)     ((X1-224) * 4096 + (X2 - 128) * 64 + X3 - 128)
#define CALC_UTF8_FOUR_BYTES(X1, X2, X3, X4)   ((X1 - 240) * 262144 + (X2 - 128) * 4096 + (X3 - 128) * 64 + X4 - 128)

//***************************************
int32 utf8_read_line(FILE *fp, str_utf8 *chars, uint32 *size);

uint8 utf8_str_max_bytes(str_utf8 s, Boolean is_just_ascii);

uint8 utf8_need_bytes(uint8 c);

str_utf8 utf8_char_append(str_utf8 s, uint32 c);
str_utf8 utf8_str_substring(str_utf8 s, uint32 start, uint32 end);
uint32 utf8_str_length(str_utf8 s);

str_utf8 utf8_str_trim_space(str_utf8 s);

str_utf8 utf8_str_reverse(str_utf8 s);

String utf8_to_str(str_utf8 s);

String utf8_to_unicode_str(str_utf8 s);

void utf8_str_print(String name, str_utf8 s, Boolean is_print_char);

void utf8_str_init(str_utf8 *s, str_utf8 val);

str_utf8 utf8_str_append(str_utf8 s1, str_utf8 s2);

Boolean utf8_str_equal(str_utf8 s1, str_utf8 s2);

void utf8_str_list_init(utf8_str_list *s, utf8_str_list val, uint32 len);

str_utf8 utf8_str_replace(str_utf8 s, str_utf8 old_s, str_utf8 new_s, int8 num);

str_utf8 utf8_str_simple_replace(str_utf8 s, String old, String new, int8 num);

uint8 utf8_decode_bytes(String out, uint32 utf);

String utf8_to_bytes_string(str_utf8 s);

str_utf8 utf8_encode_bytes(String s);

#endif /*__UTF8_H*/