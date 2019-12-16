
//
// Created by madkne on 2019.10.29
//

#ifndef __MAHDI_BUILTIN_H
#define __MAHDI_BUILTIN_H  1

void BUILT_init();
//******************************defines
#define _BUILT_PARENT_GLOBAL       1
#define _BUILT_PARENT_LIST         2
#define _BUILT_PARENT_MAP          3

#define _BUILT_LEN                1
#define _BUILT_TYPEOF             2
#define _BUILT_SUBTYPEOF          3
#define _BUILT_EXEC               4
#define _BUILT_ERRHAND            5
#define _BUILT_DEL                6
#define _BUILT_NIL                7
#define _BUILT_STR                17
#define _BUILT_NUM                18
#define _BUILT_INUM               19

#define _BUILT_PUSH               1
#define _BUILT_POP                2
#define _BUILT_TOP                3
#define _BUILT_POPFIRST           4
#define _BUILT_TOPFIRST           5
#define _BUILT_PUSHFIRST          6
#define _BUILT_FILL               7

#define _BUILT_GET                1
#define _BUILT_SET                2

//=>bifs functions
void _bifs_append(uint32 parent,uint32 id,String func_name, String params, String returns);

#endif /*__MAHDI_BUILTIN_H*/