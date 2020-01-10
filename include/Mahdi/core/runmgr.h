//
// Created by madkne on 2020.1.2
//

#ifndef __MAHDI_RUNMGR_H
#define __MAHDI_RUNMGR_H   1

String RUN_define_vars(String inst);
String RUN_normal_func_call(String inst);
String RUN_init_calling_function(String pname,String fname,StrList params,uint32 param_len,String index);

#endif //__MAHDI_RUNMGR_H
