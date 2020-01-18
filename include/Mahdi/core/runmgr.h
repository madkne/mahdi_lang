//
// Created by madkne on 2020.1.2
//

#ifndef __MAHDI_RUNMGR_H
#define __MAHDI_RUNMGR_H   1

String RUN_define_vars(String inst);
String RUN_normal_func_call(String inst);
String RUN_init_calling_function(String pname,String fname,StrList params,uint32 param_len,String index);
map* RUN_detect_type_name_func_params(StrList *params,uint32 params_len);
Boolean RUN_check_func_params(map *Sparams_type,StrList Sparams,uint32 Sparams_len,StrList Dparams_name,StrList Dparams_type,StrList Dparams_def,uint32 Dparams_len);
map* RUN_func_params_to_values(map *params_type,StrList params,uint32 params_len);

#endif //__MAHDI_RUNMGR_H
