//
// Created by madkne on 2019.10.24
//

#ifndef __MAHDI_LISTS_H
#define __MAHDI_LISTS_H    1

// Longint list functions
//******************************************
Longint LLIST_delete_first(LintList *s, uint32 len);
void LLIST_append(LintList *s, uint32 len, Longint n);
uint32 LLIST_join(LintList *s, LintList a1, uint32 a1_len, LintList a2, uint32 a2_len);
void LLIST_init(LintList *s, LintList val, uint32 len);
String LLIST_print(LintList s, uint32 len);
//************************************************************************
// Str List Functions
uint32 SLIST_size(StrList s);
String SLIST_print(StrList s, uint32 len);
void SLIST_append(StrList *s, String s1, uint32 len);
void SLIST_init(StrList *s, StrList val, uint32 len);
void SLIST_delete_first(StrList *s, uint32 len);
uint32 SLIST_join(StrList *s, StrList a1, uint32 a1_len, StrList a2, uint32 a2_len);
Boolean SLIST_equal(StrList s1, uint32 len1, StrList s2, uint32 len2);

#endif /*__MAHDI_LISTS_H*/