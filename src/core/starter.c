
#include <Mahdi/system.h>
//**********************************************************
Boolean APP_start() {
  //=>init global vars includes global vars and config vars
  APP_init_global();
  //=>if BuildMode is true, so stop and go to builder
  //  print_magic_macros(2);
  if (build_mode) {
    // start_builder();
    APP_exit(EXIT_NORMAL);
  }
  //=>init global runtime variables
  // entry_table.func_index++; //add one for main() function
  //add_to_prev_fins_array(0)
  // entry_table.cur_fid = 0;
  // entry_table.cur_fin = entry_table.func_index;
  // entry_table.cur_sid = 0;
  // entry_table.parent_rrss = 0;
  // entry_table.cur_order = 1;
  // entry_table.Rsrc = 0;
  // entry_table.Rline = 0;
  //=>find main function
  //   blst main_func = search_lbl_func("main", 0, 0);
  //   if (main_func.id == 0) {
  //     print_error(0, "not_found_main", "stdin", 0, 0, "start_runtime");
  //     return false;
  //   }
  //   entry_table.cur_fid = main_func.id;
    //printf("LLLL:%i,%s\n",main_func.id,main_func.lbl);

    //*************start debugger if enabled
  //  print_magic_macros(CONFIG_MAGIC_MACRO_TYPE);
  if (debug_mode) {
    // start_debugger();
  }
  //*************start program from fid=2,fin=2,sid=0,order=1
  APP_controller();
  //*************return
  return true;
}

//**********************************************************
int8 APP_controller() {
//   //--------------search for current instruction
//   instru *st = entry_table.instru_start;
//   if (st == 0) return false;
//   for (;;) {
//     //-------is is_occur_error_exception is true
//     if (entry_table.is_occur_error_exception) {
//       //msg("&BREAKS:", break_count)
//       entry_table.is_occur_error_exception = false;
//       return STOP_RETURN_APP_CONTROLLER;
//     }
//     // printf("XSSSS:%s>>%i<>%i,%i<>%i,%i<>%i\n",st->code, st->func_id, entry_table.cur_fid , st->stru_id, entry_table.cur_sid , st->order, entry_table.cur_order);
//     //-------get current instruction for executing
//     if (st->func_id == entry_table.cur_fid
//         && st->stru_id == entry_table.cur_sid
//         && st->order == entry_table.cur_order) {
//       //-------set global vars
//       entry_table.Rorder = st->order;
//       //-------run instruction
//       int8 ret0 = INSTRUCTION_EXECUTOR(st->id);
//       // printf("RUN_inst:\n",st->code);
//       if (ret0 == FAILED_EXECUTE_INSTRUCTION)return BAD_RETURN_APP_CONTROLLER;
//       //-------if is next instruction
//       if (entry_table.next_break_inst == NEXT_INST) {
//         entry_table.next_break_inst = 0;
//         return STOP_RETURN_APP_CONTROLLER;
//       }
//         //-------if is break instruction
//       else if (entry_table.next_break_inst == BREAK_INST) {
//         entry_table.break_count--;
//         if (entry_table.break_count == 0) entry_table.next_break_inst = 0;
//         return BREAK_RETURN_APP_CONTROLLER;
//       }
//       //-------increase cur_order
//       entry_table.cur_order++;
//     }
//     st = st->next;
//     if (st == 0) break;
//   }

  return NORMAL_RETURN_APP_CONTROLLER;
}

//**********************************************************
int8 APP_executor(Longint index) {
  //---------------------init
  //   instru st = get_instru_by_id(index);
  //   String Rcode = str_trim_space(st.code);
  //   entry_table.Rline = st.line;
  //   entry_table.Rsrc = utf8_to_bytes_string(source_paths[st.source_id]);
  //   Boolean is_done = true;
  //   //---------------------check breakpoints1
  //   if (is_find_debr(entry_table.Rline, entry_table.Rsrc) || entry_table.debug_is_next) {
  //     on_breakpoint_interrupt(Rcode, true);
  //   }
  //   //---------------------show memory in programmer debug
  //   if (is_programmer_debug >= 1 && str_equal(Rcode, "_@_")) {
  //     show_memory(0);
  //     return SUCCESS_EXECUTE_INSTRUCTION;
  //   } else if (is_programmer_debug >= 1 && str_equal(Rcode, "_u@_")) {
  //     print_struct(PRINT_UTF8_ST);
  //     return SUCCESS_EXECUTE_INSTRUCTION;
  //   }
  //   //---------------------
  //   if (str_equal(Rcode, "null")) return SUCCESS_EXECUTE_INSTRUCTION;
  //   while ((Rcode = str_trim_space(Rcode)) != 0) {
  //     //msg("&Rcode", Rcode)
  //     uint8 state = labeled_instruction(Rcode);
  //     if (is_programmer_debug >= 1) {
  //       printf("@###############INST(fid:%i,sid:%i,order:%i,state:%i,fin:%i,line:%i):\n%s\n", entry_table.cur_fid,
  //              entry_table.cur_sid, entry_table.Rorder, state, entry_table.cur_fin, entry_table.Rline, Rcode);
  //     }
  //     //---------------------analyzing all states
  //     if (state == UNKNOWN_LBL_INST) {
  //       is_done = false;
  //       if (str_equal(Rcode, "null")) return SUCCESS_EXECUTE_INSTRUCTION;
  //       exception_handler("unknown_instruction", __func__, Rcode, "");
  //     } else if (state == DEF_VARS_LBL_INST) Rcode = define_vars(Rcode);
  //     else if (state == ALLOC_MAGIC_MACROS_LBL_INST)Rcode = alloc_magic_macros(Rcode);
  //     else if (state == FUNC_CALL_LBL_INST)Rcode = function_call(Rcode);
  //     else if (state == ALLOC_VARS_LBL_INST) {
  //       is_done = vars_allocation(Rcode);
  //       Rcode = 0;
  //     } else if (state == RETURN_LBL_INST) {
  //       Boolean ret = function_return(Rcode);
  //       Rcode = 0;
  //       /*if Rcode == "true"
  //       {
  //           return 3
  //       } else if Rcode == "false"
  //       {
  //           return 1
  //       }*/
  //     } else if (state == ALLOC_SHORT_LBL_INST) {
  //       Rcode = vars_allocation_short(Rcode);
  //     } else if (state == STRUCTURE_LBL_INST) {
  //       Boolean ret = init_structures(Rcode);
  //       Rcode = 0;
  //       if (!ret)is_done = false;
  //     } else if (state == NEXT_BREAK_LBL_INST) {
  //       is_done = structure_loop_next_break(Rcode);
  //       Rcode = 0;
  //     } else {
  //       is_done = false;
  //       //exception_handler("unknown_instruction", "INSTRUCTION_EXECUTOR", Rcode, "")
  //     }
  //     if (str_equal(Rcode, BAD_CODE)) is_done = false;
  //     else if (entry_table.is_occur_error_exception) is_done = false;
  //     //********************
  //     if (!is_done) {
  //       break;
  //     }
  //   }
  //   //garbage_collector('@')
  //   //********************is DONE!!
  //   if (is_programmer_debug >= 2) {
  //     if (is_done) {
  //       printf("~~~~~~~~~~~~~~~~~~>DONE :)\n\n");
  // //       printf("---------------:%i,%s\n",entry_table.post_short_alloc_len,entry_table.post_short_alloc[0]);
  //       //check post short alloc
  //       is_done = check_post_short_alloc();
  //     }
  //     if (!is_done) {
  //       printf("~~~~~~~~~~~~~~~~~~>BREAK :(\n\n");
  //       return FAILED_EXECUTE_INSTRUCTION;
  //     }
  //     //show_prev_fins_array()
  //   }
  //   //---------------------check breakpoints2
  //   if (entry_table.debug_is_run) {
  //     on_breakpoint_interrupt(st.code, false);
  //   }
  //show_memory(0)
  //printf("###QWER:%i\n",entry_table.is_stop_APP_CONTROLLER);
  return SUCCESS_EXECUTE_INSTRUCTION;
}

//**********************************************************
Boolean APP_init_global() {
  //=>init vars
  Longint order = 1;
  entry_table.current=_rrss_null();
  //=>iterate all instructions
  instru *st = entry_table.instru_start;
  if (st == 0) return false;
  for (;;) {
    //=>if instruction is out of any package or function
    if (st->func_id == 0 && st->pack_id==0 && st->order == order) {
      String Rcode = STR_trim_space(st->code);
      entry_table.Rline = st->line;
      Boolean is_done = true;
      uint8 state = APP_labeled_instruction(Rcode);
      if (is_programmer_debug > 0) {
        printf("@###############GLOBAL_INST(State:%i,pin:%i,fin:%i,line:%i):\n%s\n@###############\n", state,
              entry_table.current.pin, entry_table.current.fin, entry_table.Rline, Rcode);
      }
      //********************
      switch (state) {
        case DEF_VARS_LBL_INST: {
          Rcode = RUN_define_vars(Rcode);
          if (EXP_check_errcode(BAD_DEFINE_VARS_ERRC)) is_done = false;
          break;
        }
        default: {
          is_done = false;
          EXP_handler("incorrect_global_inst", __func__, Rcode, 0);
          break;
        }
      }
      //=>if is DONE or failed
      if (is_programmer_debug >= 2) {
        if (is_done) {
          printf("~~~~~~~~~~~~~~~~~~>DONE :)\n\n");
        } else {
          printf("~~~~~~~~~~~~~~~~~~>BREAK :(\n\n");
        }
        //show_prev_fins_array()
      }
      order++;
    }
    st = st->next;
    if (st == 0) break;
  }
}
//**********************************************************
/**
 * get an instruction code, analyze it and determine its type like FUNC_CALL_LBL_INST or ...
 * @author madkne
 * @version 1.0
 * @since 2020.1.2
 * @param code : instruction code
 * @return uint8 : instruction label type
 */ 
uint8 APP_labeled_instruction(String code) {
  /**
   * instruction lables (by priority):
    1- STRUCTURE_LBL_INST
    1- NEXT_BREAK_LBL_INST
    2- ALLOC_SHORT_LBL_INST
    3- DEF_PACKAGE_LBL_INST
    4- FUNC_CALL_LBL_INST
    5- DEF_VARS_LBL_INST
    5- ALLOC_VARS_LBL_INST
    5- RETURN_LBL_INST
    6- UNKNOWN_LBL_INST
  */
  //=>init variables
  IntList states=0;
  uint32 states_len=0;
  ILIST_append(&states,UNKNOWN_LBL_INST,states_len++);
  Boolean is_string = false, is_equal = false, is_ret = false;
  uint16 par = 0/*count of parenthesis */, bra = 0/*count of brackets*/, aco = 0/*count of acolads*/, store_counter = 0,
      tmp_short_alloc = 0;
  String word = 0/*just create by words_splitter*/, case_word = 0/*create by words_splitter,single_operators*/,
      com_word = 0/*create by words_splitter,single_operators and skip brackets*/, buffer = 0;
  int16 last_pars = -1;
  String word_store[10];
  uint8 last_sep = 0;
  uint32 len = STR_length(code);
  //=>iterate code chars
  for (uint32 i = 0; i < len; i++) {
    //=>check is string
    if (code[i] == '\"' && (i == 0 || code[i - 1] != '\\')) {
      is_string = BOOL_switch(is_string);
    }
    //=>count parenthesis,brackets,acolads
    if (!is_string) {
      if (i + 1 < len && code[i] == ' ' && code[i + 1] == '(') continue;
      //=>count parenthesis
      else if (code[i] == '(') par++;
      else if (code[i] == ')') par--;
      //=>count brackets
      else if (code[i] == '[')bra++;
      else if (code[i] == ']')bra--;
      //=>count acolads
      else if (code[i] == '{')aco++;
      else if (code[i] == '}')aco--;
    }
    //=>is return keyword
    if (!is_string && (code[i] == ' ' || code[i] == '(' || i + 1 == len) && word != 0 &&STR_equal(word, "return")) {
      ILIST_append(&states,RETURN_LBL_INST,states_len++);
      is_ret = true;
      //break
    }
    //=>is next or break keyword
    if (!is_string && ((code[i] == ' ' || code[i] == '(' || i + 1 == len) && word != 0 &&(STR_equal(word, "next") || STR_equal(word, "break") || i + 1 == len))) {
      Boolean is_not = false;
      if (i + 1 == len) {
        is_not = true;
        word = CH_append(word, code[i]);
        if (STR_equal(word, "next") || STR_equal(word, "break")) {
          is_not = false;
        }
      }
      if (!is_not) {
        ILIST_append(&states,NEXT_BREAK_LBL_INST,states_len++);
        //break
      }
    }
    //=>is define variables
    if (!is_string && !is_equal && STR_equal(word,"def") && code[i] == ' ' && !is_ret) {
      ILIST_append(&states,DEF_VARS_LBL_INST,states_len++);
      //break
    }
    //=>is '='
    if (!is_string && code[i] == '=') {
      is_equal = true;
    }
    //=>is structure
    if (!is_string && word != 0 && STR_indexof(word, STRUCTURES_LABEL, 0) == 0) {
      ILIST_append(&states,STRUCTURE_LBL_INST,states_len++);
      break;
    }
    //=>is function call
    if (!is_string && code[i] == '(' && case_word != 0 && RUNKIT_is_valid_name(case_word, false)) {
      ILIST_append(&states,FUNC_CALL_LBL_INST,states_len++);
      last_pars = par - 1;
      //break
    } else if (last_pars > -1 && last_pars == par && states[states_len-1] == FUNC_CALL_LBL_INST) {
      break;
    }
    //=>is define package
    if (!is_string && code[i] == '(' && case_word != 0 && _datas_search(case_word,0,true).name!=0) {
      ILIST_append(&states,DEF_PACKAGE_LBL_INST,states_len++);
      last_pars = par - 1;
      //break
    }
    //=>is ++ or --
    if (!is_string && i + 1 < len && ((code[i] == '+' && code[i + 1] == '+') || (code[i] == '-' && code[i + 1] == '-'))) {
      tmp_short_alloc = i + 1;
    }
    if (!is_string && com_word != 0 && (RUNKIT_is_valid_name(com_word, true) || (com_word == 0 && RUNKIT_is_valid_name(CH_to_string(code[i]), true))) && ((tmp_short_alloc > 0) || (i + 1 < len && ((code[i] == '+' && code[i + 1] == '+') || (code[i] == '-' && code[i + 1] == '-'))))) {
      //printf ("TTT:%s\n", com_word);
      tmp_short_alloc = 0;
      ILIST_append(&states,ALLOC_SHORT_LBL_INST,states_len++);
      break;
    }
    //=>append to buffer
    buffer = CH_append(buffer, code[i]);
    //=>append to word
    if (!is_string && CH_search(words_splitter, code[i],ChArraySize(words_splitter))) {
      word_store[store_counter++] = STR_trim_space(word);
      last_sep = code[i];
      if (store_counter >= 10) {
        store_counter = 0;
      }
      word = 0;
    } else {
      word = CH_append(word, code[i]);
    }
    //=>append to case_word
    if (!is_string && (CH_search(words_splitter, code[i],ChArraySize(words_splitter)) || CH_search(single_operators, code[i],ChArraySize(single_operators)))) {
      case_word = 0;
    } else {
      case_word = CH_append(case_word, code[i]);
    }
    //=>append to com_word
    if (!is_string && (CH_search(words_splitter, code[i],ChArraySize(words_splitter)) || CH_search(single_operators, code[i],ChArraySize(single_operators))) &&
        bra == 0 && code[i] != '[' && code[i] != ']') {
      //msg("COM_WORD:", com_word)
      com_word = 0;
    } else {
      com_word = CH_append(com_word, code[i]);
    }

  }
  //=>final switch by instruction type priority
  if (is_equal && states[states_len-1] == UNKNOWN_LBL_INST) {
    ILIST_append(&states,ALLOC_VARS_LBL_INST,states_len++);
    //case state == labeled_inst["func_call"]:
  }
  //=>return state
  return states[states_len-1];
}
//**********************************************************
void APP_exit(int32 i) {
  //=>show exit message if in program debug
  if (is_programmer_debug >= 1) {
    //exit state
    String exit_state = 0, unit = 0;
    double time_taken = COM_calculate_period_time((Longint) AppStartedClock, &unit);
    if (i == EXIT_NORMAL)STR_init(&exit_state, "EXIT_SUCCESS");
    else STR_init(&exit_state, "EXIT_FAILURE");
    printf("Process finished during %.6f %s with exit code %i (%s)\n", time_taken, unit, i, exit_state);
  }
  //=>call exit system call with i parameter
  exit(i);
}