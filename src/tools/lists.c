#include <Mahdi/system.h>


// Longint list functions
//******************************************
Longint LLIST_delete_first(LintList *s, uint32 len) {
  if (len == 0) {
    (*s) = 0;
    return 0;
  }
  LintList tmp = 0;
  Longint ret = 0;
  LLIST_init(&tmp, (*s), len);
  free((*s));
  (*s) = (LintList) malloc((len - 1) * sizeof(LintList));
  uint32 ind = 0;
  for (uint32 i = 1; i < len; i++) {
    //str_init(&(*s)[i],val[i]);
    (*s)[ind++] = tmp[i];
  }
  ret = tmp[0];
  free(tmp);
  return ret;
}

//******************************************
void LLIST_append(LintList *s, uint32 len, Longint n) {
  LintList tmp = 0;
  LLIST_init(&tmp, *s, len);
  //  free (*s);
  (*s) = (LintList) malloc((len + 1) * sizeof(LintList));
  if (*s == 0)return;
  for (uint32 i = 0; i < len; i++) {
    (*s)[i] = tmp[i];
  }
  (*s)[len] = n;
}

//*************************************************************
uint32 LLIST_join(LintList *s, LintList a1, uint32 a1_len, LintList a2, uint32 a2_len) {
  //if(s!=0)free(*s);
  uint32 ind = 0;
  (*s) = (LintList) malloc((a1_len + a2_len) * sizeof(LintList));
  if (*s == 0)return 0;
  for (uint32 i = 0; i < a1_len; i++) {
    (*s)[ind++] = a1[i];
  }
  for (uint32 i = 0; i < a2_len; i++) {
    (*s)[ind++] = a2[i];
  }
  return ind;
}

//*************************************************************
void LLIST_init(LintList *s, LintList val, uint32 len) {
  if (len == 0) {
    (*s) = 0;
    return;
  }
  (*s) = (LintList) malloc(len * sizeof(LintList));
  for (uint32 i = 0; i < len; i++) {
    //str_init(&(*s)[i],val[i]);
    (*s)[i] = val[i];
  }
}

//*************************************************************
String LLIST_print(LintList s, uint32 len) {
  if (len == 0) return "[NULL]";
  String ret = 0;
  ret = CH_append(ret, '{');
  for (uint32 i = 0; i < len; i++) {
    //printf("%s,%i\n",s[i],str_length(s[i]));
    ret = STR_append(ret, STR_from_Longint(s[i]));
    if (i + 1 < len) ret = CH_append(ret, ',');
  }
  ret = CH_append(ret, '}');
  return ret;
}

//=>int32 List Functions
//******************************************
void ILIST_init(IntList *s, IntList val, uint32 len) {
  if (len == 0 || val==0) {
    (*s) = 0;
    return;
  }
  if(*s!=NULL){
    free(*s);
  }
  (*s) = (IntList) malloc(len * sizeof(IntList));
  for (uint32 i = 0; i < len; i++) {
    (*s)[i] = val[i];
  }
}
//******************************************
void ILIST_reset(IntList s,uint32 len){
  // printf("reset:%i\n",len);
  for (uint32 i = 0; i < len; i++){
    s[i]=0;
  }
}
//******************************************
String ILIST_print(IntList s,uint32 len){
  String ret=0;
  STR_init(&ret,"[");
  for (uint32 i = 0; i < len; i++){
    ret=STR_append(ret,STR_from_int32(s[i]));
    if(i+1<len)ret=CH_append(ret,',');
  }
  return CH_append(ret,']');
}
//******************************************
Boolean ILIST_search(IntList s,int32 s2,uint32 len){
  for (uint32 i = 0; i < len; i++){
    if(s[i]==s2)return true;
  }
  return false;
}
//*************************************************************
void ILIST_append(IntList *s, int32 s1, uint32 len) {
  // printf("CCCC:%s,%i\n",s1,len);
  IntList tmp = 0;
  ILIST_init(&tmp, *s, len);
  //if(*s!=NULL)free(*s);
  (*s) = (IntList) malloc((len + 1) * sizeof(IntList));
  if (*s == 0)return;
  for (uint32 i = 0; i < len; i++) {
    (*s)[i] = tmp[i];
  }
  (*s)[len] = s1;
  //printf("CCVVV:%s,%i\n",(*s)[len],str_length((*s)[len]));
}

// Str List Functions
//******************************************
uint32 SLIST_size(StrList s) {
  if (s == 0)return 0;
  uint32 len = 0;
  for (;;) {
    if (s[len++] == NULL) break;
  }
  return len - 1;
}
//******************************************
String SLIST_print(StrList s, uint32 len) {
  if (len == 0||s==0) return "[NULL]";
  String ret = 0;
  ret = CH_append(ret, '{');
  for (uint32 i = 0; i < len; i++) {
    //printf("%s,%i\n",s[i],str_length(s[i]));
    ret = STR_append(ret, s[i]);
    if (i + 1 < len) ret = CH_append(ret, ',');
  }
  ret = CH_append(ret, '}');
  return ret;
}
//******************************************
int32 SLIST_search(StrList s, uint32 len,String find) {
  if (len == 0) return -1;
  String ret = 0;
  for (uint32 i = 0; i < len; i++) {
    if(STR_equal(s[i],find)) return i;
  }
  return -1;
}
//*************************************************************
void SLIST_append(StrList *s, String s1, uint32 len) {
  // printf("CCCC:%s,%i\n",s1,len);
  StrList tmp = 0;
  SLIST_init(&tmp, *s, len);
  //if(*s!=NULL)free(*s);
  (*s) = (StrList) malloc((len + 1) * sizeof(StrList));
  if (*s == 0)return;
  for (uint32 i = 0; i < len; i++) {
    (*s)[i] = tmp[i];
  }
  (*s)[len] = STR_append(0, s1);
  //printf("CCVVV:%s,%i\n",(*s)[len],str_length((*s)[len]));
}
//*************************************************************
void SLIST_init(StrList *s, StrList val, uint32 len) {
  if (len == 0||val==0) {
    (*s) = 0;
    return;
  }
  // printf("ghjghjk:%i,%i,%i\n",&s,val,len);
  if(*s!=NULL){
    free(*s);
  }
  
  (*s) = (StrList) malloc(len * sizeof(StrList));
  for (uint32 i = 0; i < len; i++) {
    //str_init(&(*s)[i],val[i]);
    uint32 le = STR_length(val[i]);
    (*s)[i] = (String) malloc((le + 1) * sizeof(String));
    for (uint32 ii = 0; ii < le; ii++) (*s)[i][ii] = val[i][ii];
    (*s)[i][le] = 0;
  }
}
//*************************************************************
void SLIST_delete_first(StrList *s, uint32 len) {
  if (len == 0) {
    (*s) = 0;
    return;
  }
  StrList tmp = 0;
  SLIST_init(&tmp, (*s), len);
  free((*s));
  (*s) = (StrList) malloc((len - 1) * sizeof(StrList));
  uint32 ind = 0;
  for (uint32 i = 1; i < len; i++) {
    //str_init(&(*s)[i],val[i]);
    STR_init(&(*s)[ind++], tmp[i]);
  }
  free(tmp);
}
//*************************************************************
uint32 SLIST_join(StrList *s, StrList a1, uint32 a1_len, StrList a2, uint32 a2_len) {
  //if(s!=0)free(*s);
  uint32 ind = 0;
  (*s) = (StrList) malloc((a1_len + a2_len) * sizeof(StrList));
  if (*s == 0)return 0;
  for (uint32 i = 0; i < a1_len; i++) {
    STR_init(&(*s)[ind++], a1[i]);
  }
  for (uint32 i = 0; i < a2_len; i++) {
    STR_init(&(*s)[ind++], a2[i]);
  }
  return ind;
}

//*************************************************************
Boolean SLIST_equal(StrList s1, uint32 len1, StrList s2, uint32 len2) {
  if (len1 != len2) return false;
  if (len1 == 0)return true;
  for (uint32 i = 0; i < len1; ++i) {
    if (!STR_equal(s1[i], s2[i]))return false;
  }
  return true;
}