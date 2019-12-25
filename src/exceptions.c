
#include<Mahdi/system.h>

void EXP_init() {
  entry_table.exceptions_count = 0;
  entry_table.exli_start = 0;
  //-----------------------------------------
  EXP_define(0, FATAL_ID, 0, 0, 0);
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
  //DebuggerError
  EXP_define(1, ERROR_ID, "not_enough_params", DebuggerError, "your input has not enough parameters");
  EXP_define(2,ERROR_ID,"not_exist_source",
                       DebuggerError,
                       "'!1@1!' source file is not a part of this program");
  EXP_define(3,ERROR_ID,"invalid_line_number",
                       DebuggerError,
                       "'!1@1!' is invalid as line number of a source file");
  EXP_define(4,ERROR_ID,"not_execute_line_code",
                       DebuggerError,
                       "in line number'!1@1!' in '!2@2!' file, not exist any executable instruction");
  EXP_define(5, ERROR_ID, "unknown_command", DebuggerError, "'!1@1!' is unknown command for mdebug");
  EXP_define(6,ERROR_ID,"not_find_breakpoint",
                       DebuggerError,
                       "not find any breakpoints in line'!1@1!' of '!2@2!' file");
  EXP_define(7,ERROR_ID,"undefined_var",
                       DebuggerError,
                       "'!1@1!' as a variable in current line is undefined");
  EXP_define(8, ERROR_ID, "can_not_run", DebuggerError, "line '!1@1!' completed and could not run again!");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
  //CommandError
  EXP_define(1, FATAL_ID, "bad_exit", CommandError, "returned 1 exit status");
  EXP_define(2, ERROR_ID, "unknown_opt", CommandError, "unknown option '!1@1!'");
  EXP_define(3, FATAL_ID, "not_exist_file", CommandError, "can't open file '!1@1!',No such file.");
  EXP_define(4, ERROR_ID, "not_exist_docs_file", CommandError, "can not open docs file for '!1@1!'");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//ImportError
  EXP_define(1, ERROR_ID, "bad_place_using_utf8", ImportError,
                       "using UTF-8 in bad place or bad character in `!1@1!`");
  EXP_define(2, ERROR_ID, "not_open_file", ImportError, "can't open file '!1@1!'");
  EXP_define(3, ERROR_ID, "file_not_support_utf8", ImportError, "'!1@1!' file not support UTF-8");
  EXP_define(4, ERROR_ID, "import_syntax_error", ImportError,
                       "A syntax error occurs for import instruction");
  EXP_define(5, ERROR_ID, "import_not_support_protocol", ImportError,
                       "not support the protocol in this path '!1@1!' import instruction");
  EXP_define(6, ERROR_ID, "can_not_load_module", ImportError, "can not load a module in '!1@1!' path");
  EXP_define(7,ERROR_ID,"not_support_module",
                       ImportError,
                       "mahdi not support '!1@1!' as a module in '!2@2!' path");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//InterruptedError
  EXP_define(1, ERROR_ID, "zero_division", InterruptedError, "division by zero in '!1@1!' expression");
  EXP_define(2, ERROR_ID, "wrong_remainder", InterruptedError,
                       "remainder(%%) is not determined for '!1@1!' numbers and expressions");
  EXP_define(3, ERROR_ID, "out_of_range_index", InterruptedError,
                       "'!1@1!' out of range for '!2@2!' variable");
  EXP_define(4,ERROR_ID,"out_of_range_break",
                       InterruptedError,
                       "'break' instruction parameter is not between 1 .. !1@1!");
  EXP_define(5,ERROR_ID,"out_of_range_integer",
                       InterruptedError,
                       "result of '!1@1!' expression is out of range of integer");
  EXP_define(6,ERROR_ID,"out_of_range_float",
                       InterruptedError,
                       "result of '!1@1!' expression is out of range of float");
  EXP_define(7,ERROR_ID,"array_index_out_of_range",
                       InterruptedError,
                       "Index Array '!1@1!' is out of range from '0' to '!2@2!'");
  EXP_define(8,ERROR_ID,"not_using_next_break",
                       InterruptedError,
                       "not using next and break instructions outside of a loop structure");
  EXP_define(9, WARNING_ID, "array_index_underflow", InterruptedError, "Index Array '!1@1!' is underflow");
  EXP_define(10,ERROR_ID,"array_index_overflow",
                       InterruptedError,
                       "Index Array '!1@1!' is overflow from '!2@2!'");

  EXP_define(11, ERROR_ID, "review_array_out_of_range", InterruptedError,
                       "review array can not assign '!1@1!' to more than '!2@2!' variables");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//SyntaxError
  EXP_define(1, ERROR_ID, "non_dec_stat", SyntaxError,
                       "'!1@1!' is non-declaration statement outside a body");
  EXP_define(2, FATAL_ID, "invalid_name_block", SyntaxError,
                       "'!1@1!' is not a valid name for a function or package");
  EXP_define(3, FATAL_ID, "redeclared_package", SyntaxError, "'!1@1!' redeclared as a package before");
  EXP_define(4, FATAL_ID, "param_def_val", SyntaxError,
                       "'!1@1!' as a '!2@2!' function parameter can not have any default value");
  EXP_define(5, ERROR_ID, "reiterative_attr", SyntaxError, "'!1@1!' reiterative before as a attribute");
  EXP_define(6, FATAL_ID, "not_end_acod", SyntaxError, "expected '}' at end of input");
  EXP_define(7, FATAL_ID, "not_start_acod", SyntaxError, "expected '{' at start of input");
  EXP_define(8, ERROR_ID, "not_defined_struct", SyntaxError, "'!1@1!' struct can not defined in a structure");
  EXP_define(9, FATAL_ID, "define_func_in", SyntaxError, "'!1@1!' is defined into another function");
  EXP_define(10, ERROR_ID, "redeclared_var", SyntaxError,
                       "'!1@1!' redeclared as a variable before in this function or package");
  EXP_define(11, ERROR_ID, "redeclared_func", SyntaxError,
                       "'!1@1!' redeclared as a function before with same parameters");
  EXP_define(12, ERROR_ID, "wrong_def_var", SyntaxError,
                       "'!1@1!' is a wrong expression for variable declaration");
  EXP_define(13, ERROR_ID, "invalid_name_var", SyntaxError, "'!1@1!' is not a valid name for a variable");
  EXP_define(14, ERROR_ID, "invalid_next_inst", SyntaxError, "'next' instruction not accept any parameters");
  EXP_define(15, FATAL_ID, "define_pack_in", SyntaxError, "'!1@1!' is defined into another package");
  EXP_define(16, FATAL_ID, "invalid_predefined_method", SyntaxError,
                      "'!1@1!' is a predefined method name and not used in globally");
  EXP_define(17, ERROR_ID, "invalid_predefined_attr_method", SyntaxError,
                        "'!1@1!' is a predefined method and can not have private or static attributes");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//ValueError
  EXP_define(1, ERROR_ID, "val_def_var", ValueError,
                       "'!1@1!' cannot define as type '!2@2!' in variable declaration");
  EXP_define(2, ERROR_ID, "invalid_exp_val", ValueError, "'!1@1!' is invalid as a '!2@2!' expression");
  EXP_define(3, ERROR_ID, "not_alloc_val", ValueError, "'!1@1!' can not get '!2@2!' as itself value");
  EXP_define(4, ERROR_ID, "not_equal_vals_vars", ValueError,
                       "count of variables is not equal with count of initialization values");
  EXP_define(5, ERROR_ID, "not_valid_val_param", ValueError,
                       "'!1@1!' is not a valid value as a function parameter");
  EXP_define(6, ERROR_ID, "not_defined_array", ValueError,
                       "'!1@1!' defined by one value and its assigned value is an array");
  EXP_define(7, ERROR_ID, "invalid_value", ValueError,
                       "'!1@1!' is an invalid value for define a variable");
  EXP_define(8, ERROR_ID, "not_defined_mm_key", ValueError,
                       "'!1@1!' not defined in '!2@2!' magic macro");
  EXP_define(9, ERROR_ID, "reinitialized_in__define_mm", ValueError,
                       "'!1@1!' was defined in '$def' magic macro. you can not Re-initialized it");
  EXP_define(10, ERROR_ID, "not_global__config_mm", ValueError,
                       "you can not edit '__config' magic macro items inside of a function");
  EXP_define(11, ERROR_ID, "not_float_array_index", ValueError,
                       "Array index can not be as a float or huge number");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//NotExistError
  EXP_define(1, ERROR_ID, "not_exist_func", NotExistError,
                       "'!1@1!' with '!2@2!' parameters, not exist as a function");
  EXP_define(2, ERROR_ID, "wrong_type_var", NotExistError,
                       "'!1@1!' is not a valid type for variable declaration");
  EXP_define(3, ERROR_ID, "not_found_main", NotExistError,
                       "Not found 'main()' function in your source files");
  EXP_define(4, ERROR_ID, "not_exist_var", NotExistError, "'!1@1!' is not exist as a variable");
  EXP_define(5, ERROR_ID, "not_exist_struct_name", NotExistError, "Not exist name for struct");
  EXP_define(6,
                       ERROR_ID,
                       "not_exist__config_mm",
                       NotExistError,
                       "Not exist any items by name '!1@1!' in '$con' magic macro");
  EXP_define(7,
                       ERROR_ID,
                       "not_exist_func_param",
                       NotExistError,
                       "Not exist '!1@1!' as a parameter for '!2@2!' function");
  EXP_define(8, ERROR_ID, "not_exist_mod_func", NotExistError,
                       "not found '!1@1!' function in '!2@2!' module");
  EXP_define(9, ERROR_ID, "not_load_module", NotExistError,
                       "'!1@1!' module not load in memory and not available for using");
  EXP_define(10, ERROR_ID, "not_found_inherit", NotExistError,
  "'!1@1!' package can not find to inherit from it");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//RuntimeError
  EXP_define(1, ERROR_ID, "unknown_instruction", RuntimeError, "'!1@1!' is not a valid instruction");
  EXP_define(2,
                       ERROR_ID,
                       "incorrect_global_inst",
                       RuntimeError,
                       "'!1@1!' is an invalid instruction in global");
  EXP_define(3,
                       ERROR_ID,
                       "missing_func_param",
                       RuntimeError,
                       "'!1@1!' is a parameter of '!2@2!' function and must be in calling this function");
}
//**************************************************************
void EXP_set_errcode(uint32 errcode){
  errcodes_len++;
  if(errcodes_len+1>MAX_ERROR_CODES){
    //=>reset all fields
    for (uint8 i = 0; i < MAX_ERROR_CODES; i++){
      errcodes[i]=NOT_ERROR_ERRC;
    }
    errcodes_len=0;
  }
  errcodes[errcodes_len]=errcode;
}
//**************************************************************
Boolean EXP_check_errcode(uint32 errcode){
  return (errcodes[errcodes_len]==errcode);
}
//**************************************************************
int8 EXP_handler(String lbl_err, const char func_occur[], String rep1, String rep2) {
//  printf("FFFFFFFFFF:%s\n",lbl_err);
  //---------------------init vars
  int8 ret_num = WARNING_ID;
  Longint excep_id = 0;
  String func_occur_err = STR_from_const(func_occur);
  Boolean is_user_defined = false;
  if (lbl_err[0] == '#')is_user_defined = true;
  //---------------------search in exceptions_list
  if (!is_user_defined)
    for (Longint i = 0; i < entry_table.exceptions_count; i++) {
      exli st = EXP_get_exli(i);
      if (STR_equal(lbl_err, st.lbl)) {
        //fmt.Println("SSSSS:", st)
        ret_num = st.type;
        excep_id = i;
        break;
      }
    }
  exli st = EXP_get_exli(excep_id);
  //---------------------if exception is user-defined
  if (is_user_defined) {
    ret_num = (int8) STR_to_int32(rep1);
    excep_id = 1;
    st.type = (int8) STR_to_int32(rep1);
    st.group = UserDefinedError;
    st.text = rep2;
    st.lbl = lbl_err;
//    printf("EEWWW:%s;%s\n",lbl_err,rep2);
  }
  //---------------------search for manage structure
  String manage_exception_store = 0;
  //TODO:
//   stst *tmp1 = entry_table.stst_start;
//   //print_struct(PRINT_STRUCTURES_STACK_ST);
//   if (tmp1 != 0) {
//     for (;;) {
//       // printf("##SSSSS:%i,%s\n", tmp1->type, tmp1->extra);
//       if (tmp1->type == MANAGE_STRU_ID && tmp1->fin == entry_table.cur_fin && tmp1->sid == entry_table.cur_sid) {
//         str_init(&manage_exception_store, tmp1->extra);
//         break;
//       }
//       tmp1 = tmp1->next;
//       if (tmp1 == 0) break;
//     }
//   }
//   //---------------------print error;
//   if (manage_exception_store == 0 && excep_id > 0) {
    ret_num = EXP_print_error(entry_table.Rline, st.lbl, entry_table.Rsrc, rep1, rep2, func_occur_err);
//   }
//     //---------------------manage structure
//   else if (manage_exception_store != 0 && excep_id > 0) {
//     //---------clear rep1,rep2 if is_user_defined
//     if (is_user_defined) {
//       rep1 = 0;
//       rep2 = 0;
//     }
//     //---------modes
//     switch (ret_num) {
//       case ERROR_ID: ret_num = errors_mode;
//         break;
//       case WARNING_ID: ret_num = warnings_mode;
//         break;
//     }
// //    printf("@@@@:%i,%s\n",ret_num,manage_exception_store);
//     if (ret_num == CANCEL_ID) {
//       return ret_num;
//     }
//     //---------if manage_exception_store not 'null'
//     if (!str_equal(manage_exception_store, "null")) {
//       uint8 sub = 0;
//       String struct_id = calculate_struct_expression(create_exli_struct(st, rep1, rep2), "exception", &sub);
//       print_vaar(get_stde(str_to_long_int(struct_id)).st);
//       _MPL_TYPE__push(find_index_var_memory(str_to_long_int(manage_exception_store)),
//                       struct_id,
//                       "exception",
//                       RIGHT_DIRECT);
//     }
//     //---------if was error
//     if (ret_num == ERROR_ID) {
//       //printf("@@@@WWWWW:\n");
//       //cur_sid = last_sid
//       //cur_order = last_order
//       entry_table.is_occur_error_exception = true;
//     }
//       //---------if was fatal
//     else if (ret_num == FATAL_ID) {
//       exit_runtime(EXIT_FATAL);
//     }
//   }

  //---------------------return
  return ret_num;
}

//**************************************************************
exli EXP_search_lbl_exli(String lbl) {
  exli null = {0, 0, 0, 0, 0};
  exli *tmp1 = entry_table.exli_start;
  for (;;) {
    if (STR_equal(tmp1->lbl, lbl)) {
      return (*tmp1);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return null;
}

//**************************************************************
exli EXP_get_exli(Longint i) {
  exli null = {0, 0, 0, 0, 0};
  Longint counter = 0;
  exli *tmp1 = entry_table.exli_start;
  for (;;) {
    if (i == counter) {
      return (*tmp1);
    }
    tmp1 = tmp1->next;
    counter++;
    if (tmp1 == 0) break;
  }
  return null;
}
//**************************************************************
String EXP_create_exli(exli s, String rep1, String rep2) {
  String ret = 0;
//replace rep1,rep2
  s.text = STR_replace(s.text, "!1@1!", rep1, -1);
  s.text = STR_replace(s.text, "!2@2!", rep2, -1);
  //create struct value
  //struct::exception(num id,str msg,str group,num type,str src,num line)
  ret = STR_append("struct(", STR_from_Longint((Longint) s.id));
  ret = CH_append(ret, ',');
  ret = STR_multi_append(ret, "\"", s.text, "\"", 0, 0);
  ret = CH_append(ret, ',');
  ret = STR_multi_append(ret, "\"", exceptions_group[s.group], "\"", 0, 0);
  ret = CH_append(ret, ',');
  ret = STR_append(ret, STR_from_int32((int32) s.type));
  ret = CH_append(ret, ',');
  ret = STR_multi_append(ret, "\"", entry_table.Rsrc, "\"", 0, 0);
  ret = CH_append(ret, ',');
  ret = STR_append(ret, STR_from_int32((int32) entry_table.Rline));

  return ret;
}
//**************************************************************
void EXP_define(uint32 id, int8 type, String lbl, uint8 group, String text) {
  //id;type;group;label;text
  exli *q;
  q = (exli *) malloc(sizeof(exli));
  if (q == 0) return;
  q->id = id;
  q->type = type;
  q->group = group;
  STR_init(&q->lbl, lbl);
  STR_init(&q->text, text);
  q->next = 0;
  entry_table.exceptions_count++;
  if (entry_table.exli_start == 0)
    entry_table.exli_start = entry_table.exli_end = q;
  else {
    entry_table.exli_end->next = q;
    entry_table.exli_end = q;
  }
}

//**************************************************************
int8 EXP_print_error(Longint line_err, String name_err, String file_err, String rep1, String rep2, String occur_func) {
  uint8 group_err = RuntimeError;
  int8 type_err = ERROR_ID;
  String text_err = 0;
  Longint id_err = 0;
  exli search;
  Boolean is_user_defined = false;
  if (name_err[0] == '#')is_user_defined = true;
  //---------------------search in exceptions_list
  if (!is_user_defined) search = EXP_search_lbl_exli(name_err);
  //---------------------
  if (is_user_defined) {
    type_err = (int8) STR_to_int32(rep1);
    text_err = rep2;
    group_err = UserDefinedError;
    rep1 = 0;
    rep2 = 0;
  } else {
    id_err = search.id;
    text_err = search.text;
    type_err = search.type;
    group_err = search.group;
  }

  //----------------------modes
  switch (type_err) {
    case ERROR_ID: type_err = errors_mode;
      break;
    case WARNING_ID: type_err = warnings_mode;
      break;
  }
  if (type_err == CANCEL_ID) {
    return type_err;
  }
  //printf("%s,%s,%i,\n%s\n",rep1,rep2,group_err,print_str_list(exceptions_group,14));
  //----------------------replace items
  if (rep1 != 0)text_err = STR_replace(text_err, "!1@1!", rep1, -1);
  if (rep2 != 0)text_err = STR_replace(text_err, "!2@2!", rep2, -1);
  //-----------------------print exception
  String exception_msg = malloc(7 + STR_length(file_err) + (2 * sizeof(Longint)) + STR_length(text_err) +
      STR_length(exceptions_group[group_err]) + 1);
  if (line_err == 0 && !is_user_defined)
    sprintf(exception_msg,
            "%s : %s (%s::Errno%li)",
            exceptions_type[-type_err], text_err,
            exceptions_group[group_err], id_err);
  else if (line_err == 0 && is_user_defined)
    sprintf(exception_msg,
            "%s : %s (%s::%s)",
            exceptions_type[-type_err],
            text_err, occur_func, name_err);
  else if (is_user_defined)
    sprintf(exception_msg,
            "%s[%s:%li] %s (%s::%s)",
            exceptions_type[-type_err],
            file_err, line_err,
            text_err, occur_func, name_err);
  else
    sprintf(exception_msg,
            "%s[%s:%li] %s (%s::Errno%li)",
            exceptions_type[-type_err],
            file_err, line_err, text_err,
            exceptions_group[group_err], id_err);

  //=>change console color to red
  Boolean is_color_mode = false;
  if ((is_color_mode = CMD_enable_color_mode())) {
    if (type_err == ERROR_ID || type_err == FATAL_ID)CMD_color_red(stderr);
    else if (type_err == WARNING_ID)CMD_color_yellow(stderr);
  }

  if (is_programmer_debug >= 1 && !is_user_defined) {
    //y, mo, d := time.Now().Date()
    // h, mi, s := time.Now().Clock()
    //time_now := fmt.Sprintf("%v:%v:%v - %v.%v.%v", h, mi, s, y, mo, d)
    // exception_msg += new_line_char + fmt.Sprintf("Occurs in:[%s] [%s]", occur_func, time_now)
    fprintf(stderr, "%s {occur_func:%s}\n", exception_msg, occur_func);
  } else {
    fprintf(stderr, "%s\n", exception_msg);
  }
  //=>reset console color
  if (is_color_mode)CMD_color_reset(stderr);
  //=>add exeption message to logfile is enabled!
  if (logfile_path != 0) EXP_set_to_logfile(exception_msg);

  //-----------------------handle fatal
  if (type_err == FATAL_ID) {
    APP_exit(EXIT_FATAL);
  }
  return type_err;
}
//**************************************************************
Boolean EXP_set_to_logfile(String exception_msg) {
  //TODO:
//   String clk = __syscall_datetime(1);
// //  printf("logfile:%s\n",logfile_path);
//   FILE *fp = fopen(logfile_path, "a");
//   if (fp == NULL) {
//     //TODO:error
//     printf("E#ERR5654756\n");
//     return false;
//   }
//   //=>write to file
//   fwrite(clk, sizeof(uint8), str_length(clk), fp);
//   fwrite(" *** ", sizeof(uint8), 5, fp);
//   fwrite(exception_msg, sizeof(uint8), str_length(exception_msg), fp);
//   fwrite("\n", sizeof(uint8), 1, fp);
//   fclose(fp);
//   return true;
}
//**************************************************************
Boolean EXP_user_handler(int8 err_type, String err_name, String err_des, String err_func) {
  if (err_type != ERROR_ID && err_type != WARNING_ID)return false;
  EXP_handler(STR_append("#", err_name), err_func, STR_from_int32((int32) err_type), err_des);
  return true;
}