
#include <Mahdi/system.h>
//**********************************************************
Boolean APP_start() {
  //=>init global vars includes normal vars and magic macros entries
  APP_init_global_vars();
  //=>if BuildMode is true, so stop and go to builder
//  print_magic_macros(2);
  if (build_mode) {
    // start_builder();
    APP_exit(EXIT_NORMAL);
  }
  //=>init global runtime variables
  entry_table.func_index++; //add one for main() function
  //add_to_prev_fins_array(0)
  entry_table.cur_fid = 0;
  entry_table.cur_fin = entry_table.func_index;
  entry_table.cur_sid = 0;
  entry_table.parent_fin = 0;
  entry_table.cur_order = 1;
  entry_table.Rsrc = 0;
  entry_table.Rline = 0;
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
Boolean APP_init_global_vars() {
//   long_int order = 1;
//   entry_table.cur_fin = 0;
//   entry_table.cur_sid = 0;
//   entry_table.cur_fid = 0;
//   instru *st = entry_table.instru_start;
//   if (st == 0) return false;
//   for (;;) {
//     if (st->func_id == 0 && st->order == order) {
//       String Rcode = str_trim_space(st->code);
//       entry_table.Rline = st->line;
//       Boolean is_done = true;
//       uint8 state = labeled_instruction(Rcode);
//       if (is_programmer_debug > 0) {
//         printf("@###############GLOBAL_INST(State:%i,fin:%i,line:%i):\n%s\n@###############\n", state,
//                entry_table.cur_fin, entry_table.Rline, Rcode);
//       }
//       //********************
//       switch (state) {
//         case DEF_VARS_LBL_INST: {
//           Rcode = define_vars(Rcode);
//           if (str_equal(Rcode, "bad")) is_done = false;
//           break;
//         }
//         case ALLOC_MAGIC_MACROS_LBL_INST: {
//           Rcode = alloc_magic_macros(Rcode);
//           if (str_equal(Rcode, "bad")) is_done = false;
//           break;
//         }
//         default: {
//           is_done = false;
//           exception_handler("incorrect_global_inst", __func__, Rcode, 0);
//           break;
//         }
//       }
//       //********************is DONE!!
//       if (is_programmer_debug >= 2) {
//         if (is_done) {
//           printf("~~~~~~~~~~~~~~~~~~>DONE :)\n\n");
//         } else {
//           printf("~~~~~~~~~~~~~~~~~~>BREAK :(\n\n");
//         }
//         //show_prev_fins_array()
//       }
//       order++;
//     }
//     st = st->next;
//     if (st == 0) break;
//   }
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