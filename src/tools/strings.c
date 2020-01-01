#include <Mahdi/system.h>

//******************************************
uint32 STR_length(String s) {
  if (s == NULL)return 0;
  uint32 len = 0;
  while (s[len++] != 0);
  return len - 1;
}
//******************************************
Boolean STR_equal(String s1, String s2) {
  uint32 len = STR_length(s1);
  if (len != STR_length(s2)) return false;
  while (len > 0) {
    len--;
    if (s1[len] ^ s2[len]/*OR s1[len] != s2[len]*/) return false;
  }
  return true;
}
//******************************************
Boolean STR_is_const(String s){
  if(s==0)return false;
  if(s[0]!='\0' && (s[0] >= 'A' && s[0] <= 'Z')) return true;
  return false;
}
//******************************************
String STR_convert_from(String s) {
  String ret = 0;
  ret = STR_append("\"", s);
  ret = CH_append(ret, '\"');
  return ret;
}

//******************************************
String STR_reomve_quotations(String s, String type) {
  if ((!STR_CH_equal(type, 's') && !STR_equal(type, "str")))return s;
  uint32 s_len = STR_length(s);
  if (s_len >= 2 && s[0] == '\"' && s[s_len - 1] == '\"') {
    return STR_substring(s, 1, s_len - 1);
  }
  return s;
}



//******************************************
String STR_append(String s1, String s2) {
  uint32 len1 = STR_length(s1);
  uint32 len2 = STR_length(s2);
  //=>if both is null
  if(len1==0 && len2==0){
    return 0;
  }
  String s3;
  s3 = (String) malloc(sizeof(uint8) * (len1 + len2 + 1));
  //---------------------------
  if (s3 != 0) {
    if(len1>0){
      for (uint32 i = 0; i < len1; i++) {
        s3[i] = s1[i];
      }
    }
    for (uint32 i = len1; i < len1 + len2; i++) {
      s3[i] = s2[i - len1];
    }
    s3[len1 + len2] = '\0';
  }
  //printf("CCC:%s,%i,%i\n",s3,len1,len2);
  return s3;
}

//******************************************
void STR_init(String *s, String val) {
  if (val == 0) {
    *s = 0;
    return;
  }
  //if(s!=0)free(*s);
  uint32 len = STR_length(val);
  //printf("VVVVV:%s,%i\n",val,len);
  *s = (String) malloc(sizeof(uint8) * (len + 1));
  for (uint32 i = 0; i < len; i++) {
    (*s)[i] = val[i];
  }
  (*s)[len] = 0;
}

//******************************************
String STR_replace(String s, String old_s, String new_s, int8 num) {
  String ret = 0;
  uint32 len = STR_length(s);
  uint32 old_len = STR_length(old_s);
  uint32 new_len = STR_length(new_s);
  for (uint32 i = 0; i < len; i++) {
    Boolean is_exist = true;
    if (i + old_len <= len) {
      uint32 ind = 0;
      for (uint32 j = i; j < i + old_len; j++) {
        if (s[j] != old_s[ind++]) {
          is_exist = false;
          break;
        }
      }
      if (is_exist) {
        if (num == -1 || num > 0) {
          ret = STR_append(ret, new_s);
          i += old_len - 1;
          continue;
        } else {
          is_exist = false;
        }
        if (num > 0)num--;
      }
    } else {
      is_exist = false;
    }
    if (!is_exist) {
      ret = CH_append(ret, s[i]);
    }
  }
  //printf("PPP:%s=>%s\n",s,ret);
  return ret;
}

//******************************************
Boolean STR_search(StrList list, String s,uint32 size) {
  if (s == 0)return false;
  for (uint32 i = 0; i < size; i++) {
    if (STR_equal(s, list[i]))
      return true;
  }
  return false;
}
//******************************************
int32 STR_search_index(StrList list, String s,uint32 size) {
  if (s == 0)return -1;
  for (uint32 i = 0; i < size; i++) {
    if (STR_equal(s, list[i]))
      return i;
  }
  return -1;
}

//******************************************
Boolean STR_CH_equal(String s1, uint8 s2) {
  uint32 len = STR_length(s1);
  if (len != 1) return false;
  if (s1[0] != s2)return false;
  return true;
}



//******************************************
String STR_from_Longint(Longint num) {
  if (num == 0) return "0";
  String ret = 0;
  Boolean is_neg = false;
  uint32 ind = 0;
  while (num > 0) {
    Longint r = num % 10;
    num /= 10;
    ret = CH_append(ret, r + '0');
  }
  //reverse array
  ret = STR_reverse(ret);
  return ret;
}

//******************************************
int32 STR_to_int32(String s) {
  uint32 len = STR_length(s);
  if (len == 0)return 0;
  int32 ret = 0;
  uint32 ten = 1;
  Boolean is_neg = false;
  if (s[0] == '-') {
    len--;
    is_neg = true;
  }
  s = STR_reverse(s);
  for (uint32 i = 0; i < len; i++) {
    if (s[i] >= '0' && s[i] <= '9') {
      int8 c = s[i] - '0';
      ret += ten * c;
      //printf("LL:%c=>%i=>%i\n",s[i],c,ret);
      ten *= 10;
    }
  }
  if (is_neg) ret *= -1;
  return ret;
}
//******************************************
Longint STR_to_Longint(String s) {
  uint32 len = STR_length(s);
  if (len == 0)return 0;
  Longint ret = 0;
  uint32 ten = 1;
  uint32 start = 0;
  if (s[0] == '-') {
    start = 1;
  }
  s = STR_reverse(s);
  for (uint32 i = 0; i < len; i++) {
    if (s[i] >= '0' && s[i] <= '9') {
      int8 c = s[i] - '0';
      ret += ten * c;
      //printf("LL:%c=>%i=>%i\n",s[i],c,ret);
      ten *= 10;
    }
  }
  return ret;
}

//******************************************
Boolean STR_is_int32(String s) {
  uint32 len = STR_length(s);
  if (len == 0)return false;
  for (uint32 i = 0; i < len; ++i) {
    if (i == 0 && (s[i] == '-' || s[i] == '+'))continue;
    if (s[i] >= '0' && s[i] <= '9')continue;
    return false;
  }
  return true;
}
//******************************************
Boolean STR_is_bool(String s){
  if(s==0)return false;
  if(STR_equal(s,"true")||STR_equal(s,"false"))return true;
  return false;
}
//******************************************
Boolean STR_is_decimal(String str){
  uint32 len = STR_length(str);
  if (len == 1 && (str[0] < '0' || str[0] > '9')) {
    return false;
  }
  if (len > 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'b' || str[1] == 'o')) {
    return false;
  }
  for (uint32 i = 0; i < len; i++) {
    if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.' || (i == 0 && (str[i] == '+' || str[i] == '-'))) {
      continue;
    } else if (i == len - 1 && len > 1 && (str[i] == 'i' || str[i] == 'f' || str[i] == 'd' || str[i] == 'h') &&
        (str[i - 1] >= '0' && str[i - 1] <= '9')) {
      continue;
    }  else {
      return false;
    }
  }
  return true;
}
//******************************************
Boolean STR_is_num(String str) {
  uint32 len = STR_length(str);
  if (len == 1 && (str[0] < '0' || str[0] > '9')) {
    return false;
  }
  Boolean is_hex = false;
  if (len > 2 && str[0] == '0' && str[1] == 'x') {
    is_hex = true;
  }
  //msg("&&EE", str, is_hex)
  for (uint32 i = 0; i < len; i++) {
    if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.' || (i == 0 && (str[i] == '+' || str[i] == '-') ||
        (i == 1
            && (str[i] == 'x' || str[i] == 'b' || str[i] == 'o')))) {
      continue;
    } else if (i == len - 1 && len > 1 && (str[i] == 'i' || str[i] == 'f' || str[i] == 'd' || str[i] == 'h') &&
        (str[i - 1] >= '0' && str[i - 1] <= '9')) {
      continue;
    } else if (is_hex &&
        (str[i] == 'a' || str[i] == 'b' || str[i] == 'c' || str[i] == 'd' || str[i] == 'e' || str[i] == 'f')) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

//******************************************
String STR_reverse(String s) {
  uint32 len = STR_length(s);
  if (len == 0)return 0;
  String ret = (String) malloc(sizeof(uint8) * (len + 1));
  uint32 ind = len - 1;
  for (uint32 i = 0; i < len; i++) {
    ret[ind--] = s[i];
  }
  ret[len] = 0;
  return ret;
}

//******************************************
String STR_trim_space(String s) {
  uint32 len = STR_length(s);
  if (len == 0)return 0;
  String tmp1 = 0, tmp2 = 0, ret = 0;
  Boolean is_first = false, is_last = false;
  //trim from first
  for (uint32 i = 0; i < len; i++) {
    if (is_first || !CH_search(WHITE_SPACES, s[i],STR_length(WHITE_SPACES))) {
      is_first = true;
      tmp1 = CH_append(tmp1, s[i]);
    }
  }
  //config
  len = STR_length(tmp1);
  if (len == 0)return 0;
  STR_init(&tmp2, tmp1);
  free(tmp1);
  tmp2 = STR_reverse(tmp2);
  //trim from last
  for (uint32 i = 0; i < len; i++) {
    if (is_last || !CH_search(WHITE_SPACES, tmp2[i],STR_length(WHITE_SPACES))) {
      is_last = true;
      ret = CH_append(ret, tmp2[i]);
    }
  }
  //config
  ret = STR_reverse(ret);

  return ret;
}
//******************************************
String STR_trim_optimized_boolean(String str) {
  String final = 0;
  Boolean is_string = false;
  for (uint32 i = 0; i < STR_length(str); i++) {
    if (str[i] == '\"' && (i == 0 || str[i - 1] != '\\')) {
      is_string = BOOL_switch(is_string);
    }
    if (!is_string && str[i] != ')' && str[i] != '(' && str[i] != '!') {
      final = CH_append(final, str[i]);
    }
  }
  return final;
}

//******************************************
String STR_trim_number(String str) {
  String final = 0;
  Boolean is_num = false;
  uint32 str_len = STR_length(str);
  for (uint32 i = 0; i < str_len; i++) {
    if ((!is_num && str[i] != '0') || is_num) {
      final = CH_append(final, str[i]);
    }
    if (!is_num && ((str[i] >= '1' && str[i] <= '9') || (i + 1 < str_len && str[i] == '0' && str[i + 1] == '.'))) {
      if (str[i] == '0')final = CH_append(final, str[i]);
      is_num = true;
    }
  }
  return final;
}

//******************************************
String STR_trim_last_float(String str) {
  String final = 0;
  Boolean is_point = false;
  int32 point_ind = -1, zero_ind = -1;
  uint32 str_len = STR_length(str);
  for (uint32 i = 0; i < str_len; i++) {
    if (str[i] == '.') {
      is_point = true;
      point_ind = i;
      break;
    }
  }
  //-------------------------
  if (!is_point) return str;
  //-------------------------
  for (int32 i = str_len - 1; i >= point_ind; i--) {
    if (str[i] != '0') {
      if (str[i] == '.') {
        zero_ind = i - 1;
      } else {
        zero_ind = i;
      }
      break;
    }
  }
  //-------------------------
  final = STR_substring(str, 0, zero_ind + 1);
  return final;
}
//******************************************
void STR_empty(String *s) {
  if (s == 0 || !*s)return;
  //printf("$RR:%s,%i,%i\n",*s,*s,s);
  //free(*s); //is correct
  *s = NULL;
}

//******************************************
Boolean STR_is_empty(String s) {
  if (s == 0) return true;
  if (s[0] == 0) return true;
  return false;
}

//******************************************
void STR_swap(String *s1, String *s2) {
  String s3 = 0;
  STR_init(&s3, (*s1));
  STR_init(&(*s1), (*s2));
  STR_init(&(*s2), s3);
}

//******************************************
String STR_substring(String s, uint32 start, uint32 end) {
  uint32 s_len = STR_length(s);
  if (end == 0 && start > 0)end = s_len;
  if (start >= end) return 0;
  if (end > s_len)end = s_len;
  String ret = 0;
  for (uint32 i = start; i < end; i++) {
    ret = CH_append(ret, s[i]);
  }
  return ret;
}

//******************************************
int32 STR_indexof(String s, String s1, uint32 start) {
  if (s == 0 || s1 == 0)return -1;
  uint32 len = STR_length(s);
  uint32 len1 = STR_length(s1);
  if (start >= len || len1 > len)return -1;
  for (uint32 i = start; i < len; i++) {
    Boolean is_exist = true;
    if (i + len1 <= len) {
      uint32 ind = 0;
      //printf("WWWW:%i,%i,%i\n", i, i + len1, len);
      for (uint32 j = i; j < i + len1; j++) {
        if (s[j] != s1[ind++]) {
          is_exist = false;
          break;
        }
      }
      if (is_exist) {
        return i;
      }
    } else {
      is_exist = false;
    }
  }
  return -1;
}
//******************************************
Boolean STR_equal_indexof(String s, String s1, uint32 index) {
  if (s == 0 || s1 == 0)return false;
  uint32 len = STR_length(s);
  uint32 len1 = STR_length(s1);
  if (index + len1 > len || len1 > len)return false;
  uint32 ind = 0;
  for (uint32 j = index; j < index + len1; j++) {
    if (s[j] != s1[ind++]) {
      return false;
    }
  }
  return true;
}
//******************************************
int32 STR_last_indexof(String s, String s1) {
  if (s == 0 || s1 == 0)return -1;
  uint32 len = STR_length(s);
  uint32 len1 = STR_length(s1);
  if (len1 > len)return -1;
  for (uint32 i = len; i >=0; i--) {
    if(i==-1) break;
    Boolean is_exist = true;
    if (i + len1 <= len) {
      uint32 ind = 0;
      //printf("WWWW:%i,%i,%i\n", i, i + len1, len);
      for (uint32 j = i; j < i + len1; j++) {
        if (s[j] != s1[ind++]) {
          is_exist = false;
          break;
        }
      }
      if (is_exist) {
        return i;
      }
    } else {
      is_exist = false;
    }
  }
  return -1;
}

//******************************************
String STR_to_upper_case(String text) {
  String s = 0;
  for (; *text; text++) {
    if (('a' <= *text) && (*text <= 'z'))
      s = CH_append(s, 'A' + (*text - 'a'));
    else s = CH_append(s, *text);
  }
  return s;
}

//******************************************
String STR_to_lower_case(String text) {
  String s = 0;
  for (; *text; text++) {
    if (('A' <= *text) && (*text <= 'Z'))
      s = CH_append(s, 'a' + (*text - 'A'));
    else s = CH_append(s, *text);
  }
  return s;
}

//******************************************
String STR_from_int32(int32 x) {
  if (x == 0)return "0";
  Boolean is_neg = false;
  String ret = 0;
  if (x < 0) {
    x *= -1;
    is_neg = true;
  }

  while (x) {
    ret = CH_append(ret, (x % 10) + '0');
    x = x / 10;
  }
  if (is_neg) ret = CH_append(ret, '-');
  ret = STR_reverse(ret);
  return ret;
}
//******************************************
String STR_from_bool(Boolean x) {
  if (x)return "true";
  return "false";
}
//******************************************
String STR_from_int64(int64 x) {
  if (x == 0)return "0";
  Boolean is_neg = false;
  String ret = 0;
  if (x < 0) {
    x *= -1;
    is_neg = true;
  }
  while (x) {
    ret = CH_append(ret, (x % 10) + '0');
    x = x / 10;
  }
  
  if (is_neg) ret = CH_append(ret, '-');
  ret = STR_reverse(ret);
  return ret;
}
//******************************************
Boolean STR_to_bool(String s) {
  if (STR_CH_equal(s, '0') || STR_equal(s, "false") || STR_equal(STR_to_lower_case(s), "false")) return false;
  return true;
}
//******************************************
double STR_to_double(String s) {
  double val, power;
  int i, sign;
  for (i = 0; CH_isspace(s[i]); i++); /* skip white space */

  sign = (s[i] == '-') ? -1 : 1;
  if (s[i] == '+' || s[i] == '-')
    i++;
  for (val = 0.0; CH_isnum(s[i]); i++) {
    val = 10.0 * val + (s[i] - '0');
  }
  if (s[i] == '.')
    i++;
  for (power = 1.0; CH_isnum(s[i]); i++) {
    val = 10.0 * val + (s[i] - '0');
    power *= 10;
  }
  return sign * val / power;
}

//******************************************
String STR_from_double(double n, uint8 afterpoint) {
  String ret = 0;
  // Extract integer part
  int64 ipart = (int64) n;
  // convert integer part to string
  STR_init(&ret, STR_from_int64(ipart));
  if (n < 0) {
    ipart *= -1;
    n *= -1;
  }
  // Extract floating part
  double fpart = n - (double) ipart;
  //printf("CCCC:%f=>%lld,%s,%f\n",n,ipart,ret,fpart);
  // check for display option after point
  if (afterpoint > 0 && fpart != 0) {
    ret = CH_append(ret, '.');// add dot
    double power = I32_power(10, afterpoint); //calc power of 10
    double one_unit = (double) 1 / power; //calc one unit like 0.01 or 0.0001
    fpart = (fpart * power) + one_unit;
    String fpart_s = STR_from_int32((int) fpart);
    uint32 fpart_s_len = STR_length(fpart_s);
    //add pre zeros like 0.005
    if (afterpoint > fpart_s_len) {
      for (uint32 i = 0; i < afterpoint - fpart_s_len; ++i) {
        ret = CH_append(ret, '0');
      }
    }
    //printf("OOOOO:%f=>%f,%f,%i\n",n,fpart,power,afterpoint);
    ret = STR_append(ret, fpart_s);
  }
  return ret;
}

//******************************************
String STR_multi_append(String s, String s1, String s2, String s3, String s4, String s5) {
  String ret = 0;
  STR_init(&ret, s);
  if (s1 != 0)ret = STR_append(ret, s1); else return ret;
  if (s2 != 0)ret = STR_append(ret, s2); else return ret;
  if (s3 != 0)ret = STR_append(ret, s3); else return ret;
  if (s4 != 0)ret = STR_append(ret, s4); else return ret;
  if (s5 != 0)ret = STR_append(ret, s5);
  return ret;
}

//******************************************
void STR_to_utf8(UString *ret, String val) {
  uint32 len = STR_length(val);
  (*ret) = (UString) malloc(sizeof(uint32) * (len + 1));
  for (uint32 i = 0; i < len; i++) {
    (*ret)[i] = val[i];
  }
  (*ret)[len] = 0;
}

//******************************************
String STR_from_const(const char s[]) {
  String ret = 0;
  if (s == NULL)return 0;
  uint32 len = 0;
  while (s[len++] != 0);
  if (len == 0)return 0;
  //printf("VVVVV:%s,%i\n",val,len);
  ret = (String) malloc(sizeof(uint8) * (len + 1));
  for (uint32 i = 0; i < len; i++) {
    ret[i] = s[i];
  }
  ret[len] = 0;
  return ret;
}



//******************************************
Boolean STR_has_suffix(String s, String find) {
  int32 ind = STR_indexof(s, find, 0);
  if (ind == -1)return false;
  //printf("@#@SSSS:%s,%s[%i,%i,%i]\n",s,find,ind,ind + str_length(find) + 1,str_length(s));
  if (ind + STR_length(find) == STR_length(s)) return true;
  return false;
}

//******************************************
String STR_set_negative_number(String s) {
  uint32 s_len = STR_length(s);
  if (s_len > 1 && s[0] == '-')s[0] = '+';
  else if (s_len > 1 && s[0] == '+')s[0] = '-';
  else if (s_len > 0)s = STR_append("-", s);
  return s;
}

//******************************************
void STR_detachment_float(String s, String *s1, String *s2) {
  Boolean is_point = false;
  String int_s = 0, float_s = 0;
  for (uint32 i = 0; i < STR_length(s); i++) {
    if (s[i] == '.') {
      is_point = true;
      continue;
    }
    if (!is_point) {
      int_s = CH_append(int_s, s[i]);
    } else {
      float_s = CH_append(float_s, s[i]);
    }
  }
  if (s1 != 0)STR_init(&(*s1), int_s);
  if (s2 != 0)STR_init(&(*s2), float_s);
}

//******************************************
String STR_join(StrList s, uint32 len, String delimiter) {
  String ret = 0;
  for (uint32 i = 0; i < len; i++) {
    ret = STR_append(ret, s[i]);
    if (i + 1 < len)ret = STR_append(ret, delimiter);
  }
  return ret;
}
//******************************************
String STR_to_validate(String s) {
  String ret = 0;
  for (uint32 i = 0; i < STR_length(s); i++) {
    if (s[i] == '\"' && (i == 0 || s[i - 1] != '\\')) {
      ret = STR_append(ret, "\\\"");
    } else ret = CH_append(ret, s[i]);
  }
  return ret;
}
//******************************************
String STR_from_path(String path){
  String ret = 0;
  for (uint32 i = 0; i < STR_length(path); i++) {
    if (path[i] == '\\' && (i == 0 || path[i - 1] != '\\')) {
      ret = STR_append(ret, "\\\\");
    } else ret = CH_append(ret, path[i]);
  }
  return ret;
}



