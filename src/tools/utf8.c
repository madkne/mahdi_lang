#include <Mahdi/system.h>


//=>utf8 functions
//***************************************
int32 UTF_read_line(FILE *fp, UString *chars, uint32 *size)
{
	int32 ret = 0;
	uint8 max_bytes = 1;
	(*size) = 0;
	(*chars) = 0;
	for (;;)
	{
		uint8 buf[4] = {0, 0, 0, 0};
		uint8 arr[1];
		uint32 code_point = 0;
		//--------------------get 1-4 bytes of utf8
		ret = fread(arr, sizeof(uint8), 1, fp);
		if (ret == 0)
		{
			ret = EOF;
			break;
		}
		if (arr[0] == '\n')
		{
			ret = 0;
			break;
		}
		uint8 count = UTF_need_bytes(arr[0]);
		if (count > max_bytes)
			max_bytes = count;
		if (count == 0)
		{
			ret = -10;
			break;
		}
		if (count >= 1)
			buf[0] = arr[0];
		if (count >= 2)
		{
			ret = fread(arr, sizeof(uint8), 1, fp);
			if (ret == 0)
			{
				ret = -10;
				break;
			}
			buf[1] = arr[0];
		}
		if (count >= 3)
		{
			ret = fread(arr, sizeof(uint8), 1, fp);
			if (ret == 0)
			{
				ret = -10;
				break;
			}
			buf[2] = arr[0];
		}
		if (count == 4)
		{
			ret = fread(arr, sizeof(uint8), 1, fp);
			if (ret == 0)
			{
				ret = -10;
				break;
			}
			buf[3] = arr[0];
		}
		//--------------------generate code point of utf8 char
		if (count == 1)
		{
			code_point = buf[0];
		}
		else if (count == 2)
		{
			code_point = CALC_UTF8_TWO_BYTES(buf[0], buf[1]);
		}
		else if (count == 3)
		{
			code_point = CALC_UTF8_THREE_BYTES(buf[0], buf[1], buf[2]);
		}
		else if (count == 4)
		{
			code_point = CALC_UTF8_FOUR_BYTES(buf[0], buf[1], buf[2], buf[3]);
		}
		//--------------------add to chars
		(*size)++;
		(*chars) = UCH_append((*chars), code_point);
		//printf("CCCC:%i=>\\u%X(%i,%i,%i,%i) : %i\n",count,code_point,buf[0],buf[1],buf[2],buf[3],(*chars)[(*size)-1]);
	}
	return ret;
}

//***************************************
UString UTF_encode_bytes(String s)
{
	uint8 max_bytes = 1;
	UString chars = 0;
	uint32 len = STR_length(s);
	uint32 size = 0;
	for (;;)
	{
		if (size == len)
			break;
		uint8 buf[4] = {0, 0, 0, 0};
		uint32 code_point = 0;

		//--------------------get 1-4 bytes of utf8
		uint8 ch = s[size++];
		if (ch == 0)
		{
			break;
		}
		uint8 count = UTF_need_bytes(ch);
		if (count > max_bytes)
			max_bytes = count;
		if (count == 0)
			break;
		if (count >= 1)
			buf[0] = ch;
		if (count >= 2 && size < len)
		{
			ch = s[size++];
			if (ch == 0)
				break;
			buf[1] = ch;
		}
		if (count >= 3 && size < len)
		{
			ch = s[size++];
			if (ch == 0)
				break;
			buf[2] = ch;
		}
		if (count == 4 && size < len)
		{
			ch = s[size++];
			if (ch == 0)
				break;
			buf[3] = ch;
		}
		//--------------------generate code point of utf8 char
		if (count == 1)
		{
			code_point = buf[0];
		}
		else if (count == 2)
		{
			code_point = CALC_UTF8_TWO_BYTES(buf[0], buf[1]);
		}
		else if (count == 3)
		{
			code_point = CALC_UTF8_THREE_BYTES(buf[0], buf[1], buf[2]);
		}
		else if (count == 4)
		{
			code_point = CALC_UTF8_FOUR_BYTES(buf[0], buf[1], buf[2], buf[3]);
		}
		//--------------------add to chars
		chars = UCH_append(chars, code_point);
	}
	return chars;
}



//***************************************
uint8 UTF_need_bytes(uint8 c)
{
	if (c >= 0x0 && c <= 0x7F)
		return 1;
	if (c >= 0xC2 && c <= 0xDF)
		return 2;
	if (c == 0xE0 || c == 0xED || (c >= 0xE1 && c <= 0xEC) || (c >= 0xEE && c <= 0xEF))
		return 3;
	if (c == 0xF0 || c == 0xF4 || (c >= 0xF1 && c <= 0xF3))
		return 4;

	return 0;
}

//***************************************
FILE * UTF_file_open(String filename, String mode) {
  #if LINUX_PLATFORM == 1
  return fopen(filename,mode);
  #elif WINDOWS_PLATFORM == 1
  wchar_t wfile[260];
  wchar_t wmode[32];
  MultiByteToWideChar(CP_UTF8, 0, filename, -1, wfile, 260);
  MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, 32);
  return _wfopen(wfile, wmode);
  #endif
  return NULL;
}


//=>utf8 string functions
//***************************************
uint8 USTR_max_bytes(UString s, Boolean is_just_ascii)
{
	uint8 max = 1;
	uint32 len = USTR_length(s);
	for (uint32 i = 0; i < len; ++i)
	{
		uint8 count = UTF_decode_bytes(0, s[i]);
		if (is_just_ascii && count > 1)
			return count;
		if (count > max)
			max = count;
		else if (count == 0)
			return 0;
	}
	return max;
}
//******************************************
UString USTR_substring(UString s, uint32 start, uint32 end)
{
	uint32 s_len = USTR_length(s);
	if (end == 0 && start > 0)
		end = s_len;
	if (start >= end)
		return 0;
	if (end > s_len)
		end = s_len;
	UString ret = 0;
	for (uint32 i = start; i < end; i++)
	{
		ret = UCH_append(ret, s[i]);
	}
	return ret;
}
//******************************************
UString USTR_append(UString s1, UString s2)
{
	uint32 len1 = USTR_length(s1);
	uint32 len2 = USTR_length(s2);
	UString s3;
	s3 = (UString)malloc(sizeof(uint32) * (len1 + len2 + 1));
	//---------------------------
	if (s3 != 0)
	{
		for (uint32 i = 0; i < len1; i++)
		{
			s3[i] = s1[i];
		}
		for (uint32 i = len1; i < len1 + len2; i++)
		{
			s3[i] = s2[i - len1];
		}
		s3[len1 + len2] = 0;
	}
	//printf("CCC:%s,%i,%i\n",s3,len1,len2);
	return s3;
}

//***************************************
uint32 USTR_length(UString s)
{
	if (s == NULL)
		return 0;
	uint32 len = 0;
	while (s[len++] != 0)
		;
	return len - 1;
}

//***************************************
UString USTR_trim_space(UString s)
{
	uint32 len = USTR_length(s);
	if (len == 0)
		return 0;
	UString tmp1 = 0, tmp2 = 0, ret = 0;
	Boolean is_first = false, is_last = false;
	//trim from first
	for (uint32 i = 0; i < len; i++)
	{
		if (is_first || !IS_ONE_BYTE(s[i]) || !CH_search(WHITE_SPACES, (uint8)s[i],STR_length(WHITE_SPACES)))
		{
			is_first = true;
			tmp1 = UCH_append(tmp1, s[i]);
		}
	}
	//config
	len = USTR_length(tmp1);
	if (len == 0)
		return 0;
	USTR_init(&tmp2, tmp1);
	free(tmp1);
	tmp2 = USTR_reverse(tmp2);
	//trim from last
	for (uint32 i = 0; i < len; i++)
	{
		if (is_last || !IS_ONE_BYTE(tmp2[i]) || !CH_search(WHITE_SPACES, (uint8)tmp2[i],STR_length(WHITE_SPACES)))
		{
			is_last = true;
			ret = UCH_append(ret, tmp2[i]);
		}
	}
	//config
	ret = USTR_reverse(ret);

	return ret;
}

//***************************************
UString USTR_reverse(UString s)
{
	uint32 len = USTR_length(s);
	if (len == 0)
		return 0;
	UString ret = (UString)malloc(sizeof(uint32) * (len + 1));
	uint32 ind = len - 1;
	for (uint32 i = 0; i < len; i++)
	{
		ret[ind--] = s[i];
	}
	ret[len] = 0;
	return ret;
}

//***************************************
String USTR_to_STR(UString s)
{
	uint32 len = USTR_length(s);
	String ret = 0;
	for (uint32 i = 0; i < len; i++)
	{
		//printf("FFFF:%c,%i\n",s[i],IS_ONE_BYTE(s[i]));
		if (IS_ONE_BYTE(s[i]))
			ret = CH_append(ret, s[i]);
	}
	return ret;
}

//***************************************
String USTR_to_unicode_str(UString s)
{
	String ret = 0;
	uint32 len = USTR_length(s);
	for (uint32 i = 0; i < len; i++)
	{
		if (IS_ONE_BYTE(s[i]))
			ret = CH_append(ret, s[i]);
		else
		{
			String v = malloc(6 * sizeof(uint8));
			sprintf(v, "\\u%4.4X", s[i]);
			ret = STR_append(ret, v);
		}
	}
	return ret;
}

//***************************************
void USTR_print(String name, UString s, Boolean is_print_char)
{
	uint32 len = USTR_length(s);
	if (!is_print_char)
	{
		if (name == 0) STR_init(&name, "UTF8-BIN");
		printf("%s[%4i]: ", name, len);
		for (uint32 i = 0; i < len; i++)
		{
			printf("%i ", s[i]);
		}
		printf("\n");
	}
	else
	{
		if (name == 0)
			STR_init(&name, "UTF8-CH");
		printf("%s[%4i]: ", name, len);
		for (uint32 i = 0; i < len; i++)
		{
			if (IS_ONE_BYTE(s[i]))
				printf("%c", s[i]);
			else
			{
				printf("\\u%4.4X", s[i]);
			}
		}
		printf("\n");
	}
}

//***************************************
void USTR_init(UString *s, UString val)
{
	uint32 len = USTR_length(val);
	(*s) = (UString)malloc(sizeof(uint32) * (len + 1));
	for (uint32 i = 0; i < len; i++)
	{
		(*s)[i] = val[i];
	}
	(*s)[len] = 0;
}

//***************************************
Boolean USTR_equal(UString s1, UString s2)
{
	uint32 len = USTR_length(s1);
	if (len != USTR_length(s2))
		return false;
	while (len > 0)
	{
		len--;
		if (s1[len] != s2[len])
			return false;
	}
	return true;
}

//***************************************
UString USTR_replace(UString s, UString old_s, UString new_s, int8 num)
{
	UString ret = 0;
	uint32 len = USTR_length(s);
	uint32 old_len = USTR_length(old_s);
	uint32 new_len = USTR_length(new_s);
	for (uint32 i = 0; i < len; i++)
	{
		Boolean is_exist = true;
		if (i + old_len < len)
		{
			uint32 ind = 0;
			for (uint32 j = i; j < i + old_len; j++)
			{
				if (s[j] != old_s[ind++])
				{
					is_exist = false;
					break;
				}
			}
			if (is_exist)
			{
				if (num == -1 || num > 0)
				{
					ret = USTR_append(ret, new_s);
					i += old_len - 1;
					continue;
				}
				else
				{
					is_exist = false;
				}
				if (num > 0)
					num--;
			}
		}
		else
		{
			is_exist = false;
		}
		if (!is_exist)
		{
			ret = UCH_append(ret, s[i]);
		}
	}
	//printf("PPP:%s=>%s\n",s,ret);
	return ret;
}

//***************************************
UString USTR_simple_replace(UString s, String old, String new, int8 num)
{
	UString old_s = 0;
	STR_to_utf8(&old_s, old);
	UString new_s = 0;
	STR_to_utf8(&new_s, new);
	return USTR_replace(s, old_s, new_s, num);
}

//***************************************
uint8 UTF_decode_bytes(String out, uint32 utf)
{
	if (utf <= 0x7F)
	{
		// Plain ASCII
		if (out != 0)
		{
			out[0] = (char)utf;
			out[1] = 0;
		}
		return 1;
	}
	else if (utf <= 0x07FF)
	{
		// 2-byte unicode
		if (out != 0)
		{
			out[0] = (char)(((utf >> 6) & 0x1F) | 0xC0);
			out[1] = (char)(((utf >> 0) & 0x3F) | 0x80);
			out[2] = 0;
		}
		return 2;
	}
	else if (utf <= 0xFFFF)
	{
		// 3-byte unicode
		if (out != 0)
		{
			out[0] = (char)(((utf >> 12) & 0x0F) | 0xE0);
			out[1] = (char)(((utf >> 6) & 0x3F) | 0x80);
			out[2] = (char)(((utf >> 0) & 0x3F) | 0x80);
			out[3] = 0;
		}
		return 3;
	}
	else if (utf <= 0x10FFFF)
	{
		// 4-byte unicode
		if (out != 0)
		{
			out[0] = (char)(((utf >> 18) & 0x07) | 0xF0);
			out[1] = (char)(((utf >> 12) & 0x3F) | 0x80);
			out[2] = (char)(((utf >> 6) & 0x3F) | 0x80);
			out[3] = (char)(((utf >> 0) & 0x3F) | 0x80);
			out[4] = 0;
		}
		return 4;
	}
	else
	{
		// error - use replacement character
		if (out != 0)
		{
			out[0] = (char)0xEF;
			out[1] = (char)0xBF;
			out[2] = (char)0xBD;
			out[3] = 0;
		}
		return 0;
	}
}

//***************************************
String USTR_to_bytes_str(UString s)
{
	String ret = 0;
	uint32 len = USTR_length(s);
	for (uint32 i = 0; i < len; i++)
	{
		uint8 bytes[4];
		uint8 count = UTF_decode_bytes(bytes, s[i]);
		for (int j = 0; j < count; j++)
		{
			//printf("GGGG:%i,%x\n",bytes[j],bytes[j]);
			ret = CH_append(ret, bytes[j]);
		}
	}
	return ret;
}



//utf8 char functions
//***************************************
UString UCH_append(UString s, uint32 c)
{
	UString ret = 0;
	uint32 len = USTR_length(s);
	ret = (UString)malloc(sizeof(uint32) * (len + 2));
	//---------------------------
	if (ret != 0)
	{
		for (uint32 i = 0; i < len; i++)
		{
			ret[i] = s[i];
		}
		ret[len] = c;
		ret[len + 1] = 0;
	}
	//printf("CCC:%s,%i,%i\n",s1,len);
	return ret;
}


//=>utf8 string list functions
//***************************************
void USLIST_init(UStrList *s, UStrList val, uint32 len)
{
	if (len == 0)
	{
		(*s) = 0;
		return;
	}
	(*s) = (UStrList)malloc(len * sizeof(UStrList));
	for (uint32 i = 0; i < len; i++)
	{
		//str_init(&(*s)[i],val[i]);
		uint32 le = USTR_length(val[i]);
		(*s)[i] = (UString)malloc(le * sizeof(UString));
		for (uint32 ii = 0; ii < le; ii++)
			(*s)[i][ii] = val[i][ii];
	}
}
//***************************************
void USLIST_append(UStrList *s, UString s1, uint32 len) {
  //printf("CCCC:%s,%i\n",s1,len);
  UStrList tmp = 0;
  USLIST_init(&tmp, *s, len);
  free(*s);
  (*s) = (UStrList) malloc((len + 1) * sizeof(UStrList));
  if (*s == 0)return;
  for (uint32 i = 0; i < len; i++) {
    (*s)[i] = tmp[i];
  }
  (*s)[len] = USTR_append(0, s1);
  //printf("CCVVV:%s,%i\n",(*s)[len],str_length((*s)[len]));
}