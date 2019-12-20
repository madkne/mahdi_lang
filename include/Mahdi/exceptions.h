//
// Created by madkne on 2019.10.26
//

#ifndef __MAHDI_EXCEPTIONS_H
#define __MAHDI_EXCEPTIONS_H  1

void EXP_init();
void EXP_set_errcode(uint32 errcode);
Boolean EXP_check_errcode(uint32 errcode);
int8 EXP_handler(String lbl_err, const char func_occur[], String rep1, String rep2);
exli EXP_search_lbl_exli(String lbl);
exli EXP_get_exli(Longint i);
String EXP_create_exli(exli s, String rep1, String rep2);
void EXP_define(uint32 id, int8 type, String lbl, uint8 group, String text);
int8 EXP_print_error(Longint line_err, String name_err, String file_err, String rep1, String rep2, String occur_func);
Boolean EXP_set_to_logfile(String exception_msg);
Boolean EXP_user_handler(int8 err_type, String err_name, String err_des, String err_func);
#endif /*__MAHDI_EXCEPTIONS_H*/