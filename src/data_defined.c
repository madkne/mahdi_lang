

#include <Mahdi/system.h>

String interpreter_level = 0;

String project_root = 0;

String main_source_name = 0;

String interpreter_path = 0; //=>mahdi dir path
String interpreter_tmp_path = 0; //mahdi-root/tmp-dir
String os_separator = 0; //char_to_str(OS_SEPARATOR);
uint8 MAX_INT_LEN = 0;

uint8 MAX_FLOAT_LEN = 0;

uint8 max_estimate_divide_huge = 8;

uint8 max_float_estimate_huge_X0 = 4;

String new_line_char = "\n";

String os_tmp_dir = 0;

uint32 BufferSize = 4096;

String hostname = 0;

String os_version = 0;

uint64 os_total_memory = 0; //in bytes
uint64 os_total_disk = 0; //in kilo-bytes
long_int max_size_id = 0;

uint32 max_mpl_modules_instance_len = 100;
Boolean is_real_mpl = false;
//-------------------------------------------------------defualt for config entries
int8 errors_mode = ERROR_ID;
int8 warnings_mode = WARNING_ID;
uint8 is_programmer_debug = 2;
uint8 max_steps_estimate_huge = 8;
uint32 max_decimal_has_huge = 10;
Boolean safe_mode = true;
Boolean session_mode = true;
Boolean optimize_mode = false;
Boolean overwrite_builtin_mode = false;
Boolean debug_mode = false;
Boolean package_mode = false;
Boolean access_vars_mode = true;
String logfile_path = 0;
String namespace = 0;
String bytecode_path = 0;
Boolean build_mode = false;
String buildfile_path = 0;
//-------------------------------------------------------
String exceptions_group[] = {
    "ImportError",          //0
    "MahlibError",          //1
    "SyntaxError",          //2
    "InterruptedError",     //3
    "ValueError",           //4
    "NotExistError",        //5
    "RuntimeError",         //6
    "IOError",              //7
    "NotAccessError",       //8
    "OSError",              //9
    "OverflowError",        //10
    "TypeError",            //11
    "FloatingPointError",   //12
    "CommandError",         //13
    "DebuggerError",        //14
    "BuilderError"          //15
};

String exceptions_type[4] = {"CANCEL", "FATAL", "ERROR", "WARNING"};

String keywords[18] = {"func", "true", "false", "null", "if", "elif", "else", "loop", "manage", "next", "break",
    "return", "num", "str", "bool", "struct", "vars", "import"};

String keywords_out[13] = {"func", "true", "false", "null", "if", "elif", "else", "loop", "manage", "next", "break",
    "return", "import"};

String magic_macros[3] = {"$con", "$def", "$ses"};

String block_instructions[5] = {"loop", "if", "elif", "else", "manage"};

//uint8 keywords_splitter[5] = {':', ' ', '{', '=', '('};
uint8 single_operators[] = {'+', '-', '*', '/', '%', '^'};

String comparative_operators[] = {"==", ">=", "<=", ">", "<", "!="};

String alloc_operators[11] = {"+=", "-=", "*=", "/=", "%=", "^=", ":=", "|=", "&=", "~=", "!="};

String boolean_operators[] = {"&&", "||", "~~"};

String basic_types[3] = {"str", "num", "bool"};

uint8 golden_bytes[] = {7, 11, 27, 127, 223};

uint8 splitters[] = {' ', '(', ')', '=', '[', ']', '{', '}', ';', ','};

uint8 words_splitter[] = {':', ' ', '{', '=', '(', ',', '.', '[', ']', '}', ')', ';'};

String sub_types[5] = {"s", "b", "i", "f", "h"};

String control_chars[5] = {"\\n", "\\t", "\\", "\\\\", "\\b"};

Mpoint *hash_pointers[HASH_MEM_SIZE] = {0};

utf8_str_list source_paths;

str_list program_argvs = 0;
uint32 argvs_len = 0;

str_list installed_modules = 0;
uint32 installed_modules_len = 0;

//*************************************************************
//*********************public functions************************
//*************************************************************
void init_data_defined() {

  //---------------init project_root
  str_list entries, filename;
  uint32 size = char_split(stdin_source_path, OS_SEPARATOR, &entries, true);
  char_split(entries[size - 1], '.', &filename, true);
  String Fsrc = filename[0];
  str_init(&main_source_name, Fsrc);
  project_root = char_join(entries, OS_SEPARATOR, size - 1, true);
//  printf("GGGGG:%s,%s\n",project_root,stdin_source_path);
  #if LINUX_PLATFORM == true
  project_root = str_append("/", project_root);
  #endif
  //---------------init max_int ,max_float
  MAX_INT_LEN = INT_USED_BYTES * 2;
  MAX_FLOAT_LEN = (FLOAT_USED_BYTES * 2) - 2;

  //---------------get mpl path
  os_separator = char_to_str(OS_SEPARATOR);
  if (interpreter_path == 0) interpreter_path = get_mpl_dir_path();
  if (is_real_mpl) {
    interpreter_tmp_path = str_multi_append(interpreter_path, os_separator, "tmp", os_separator, 0, 0);
    if (fopen(str_append(interpreter_tmp_path, "TMPDIR"), "r") == NULL) {
      mkdir(interpreter_tmp_path);
      fopen(str_append(interpreter_tmp_path, "TMPDIR"), "w");
    }
  }
  //---------------init mpl_modules_instance
  for (uint32 i = 0; i < max_mpl_modules_instance_len; i++) {
    mpl_modules_instance[i] = 0;
  }
  //---------------init entry_table
  entry_table.import_start = 0;
  entry_table.import_id = 1;
  entry_table.import_inst_count = 0;

  entry_table.utst_start = 0;
  entry_table.utf8_strings_id = 1;

  entry_table.soco_main_count = 0;
  entry_table.source_counter = 0;
  entry_table.soco_tokens_count = 0;
  entry_table.soco_tokens_start = 0;

  entry_table.blst_func_start = 0;
  entry_table.blst_stru_start = 0;
  entry_table.func_id = 1;
  entry_table.stru_id = 0;
  entry_table.func_index = 1;

  entry_table.parent_fin = 0;
  entry_table.cur_sid = 0;
  entry_table.cur_fin = 0;
  entry_table.cur_fid = 0;

  entry_table.datas_start = 0;
  entry_table.datas_id = 1;

  entry_table.instru_start = 0;
  entry_table.inst_id = 1;

  entry_table.inor_start = 0;
  entry_table.inor_count = 0;

  entry_table.bifs_start = 0;
  //entry_table.bifs_len=0;

  entry_table.mama_start = 0;
  entry_table.mama_id = 0;

  entry_table.stde_start = 0;
  entry_table.stde_id = 0;

  entry_table.stst_start = 0;
  entry_table.stst_len = 0;

  entry_table.cole_start = 0;
  entry_table.cole_end = 0;
  entry_table.cole_len = 0;

  entry_table.lole_start = 0;
  entry_table.lole_end = 0;
  entry_table.lole_len = 0;

  entry_table.next_break_inst = 0;
  entry_table.break_count = 0;

  entry_table.Rsrc = 0;
  entry_table.return_fin = 0;
  entry_table.Rorder = 0, entry_table.Rline = 0;
  entry_table.is_stop_APP_CONTROLLER = false;
  entry_table.is_next_inst_running = false;
  entry_table.is_occur_error_exception = false;

  entry_table.sources_list = 0;
  entry_table.sources_list_len = 0;

  entry_table.debr_start = 0;
  entry_table.debr_len = 0;

  entry_table.debug_is_run = false;
  entry_table.debug_is_next = false;
  str_utf8 stdin_src;
  str_to_utf8(&stdin_src, "stdin");
  utf8_str_list_append(&source_paths, stdin_src, entry_table.source_counter++);
  //---------------init post short_alloc
  entry_table.post_short_alloc = 0;
  entry_table.post_short_alloc_len = 0;
  //---------------init data_types
  datas tmp1 = {0, 0, "str", MAIN_DATA_TYPE, 0, 0};
  append_datas(tmp1);
  datas tmp2 = {0, 0, "num", MAIN_DATA_TYPE, 0, 0};
  append_datas(tmp2);
  datas tmp3 = {0, 0, "bool", MAIN_DATA_TYPE, 0, 0};
  append_datas(tmp3);
  datas tmp4 = {0, 0, "struct", MAIN_DATA_TYPE, 0, 0};
  append_datas(tmp4);
  datas tmp5 = {0, 0, "vars", MAIN_DATA_TYPE, 0, 0};
  append_datas(tmp5);
  //struct::exception(num id,str msg,str group,num type,str src,num line)
  str_list exception_params = 0;
  uint32 exception_params_len = 0;
  str_list_append(&exception_params, "num;id;", exception_params_len++);
  str_list_append(&exception_params, "str;msg;", exception_params_len++);
  str_list_append(&exception_params, "str;group;", exception_params_len++);
  str_list_append(&exception_params, "num;type;", exception_params_len++);
  str_list_append(&exception_params, "str;src;", exception_params_len++);
  str_list_append(&exception_params, "num;line;", exception_params_len++);
  datas tmp6 = {0, 0, "exception", STRUCT_DATA_TYPE, exception_params, exception_params_len};
  append_datas(tmp6);

}

//*************************************************************
//*******************import_inst functions**********************
//*************************************************************
void append_imin(imin s) {
  imin *q;
  q = (imin *) malloc(sizeof(imin));
  if (q == 0) return;
  q->id = s.id;
  q->is_active = true;
  q->type = s.type;
  utf8_str_init(&q->path, s.path);
  q->line = s.line;
  q->err_code = 0;
  q->max_bytes_per_char = s.max_bytes_per_char;
  q->source_id = s.source_id;
  q->next = 0;
  if (entry_table.import_start == 0) {
    entry_table.import_start = entry_table.import_end = q;
  } else {
    entry_table.import_end->next = q;
    entry_table.import_end = q;
  }
  entry_table.import_inst_count++;
}

//*************************************************************
imin get_imin(long_int id) {
  imin ret = {0, 0, false, 0, 0, 0, 0};
  imin *tmp1 = entry_table.import_start;
  for (;;) {
    if (tmp1->id == id) {
      ret = *tmp1;
      break;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return ret;
}

//*************************************************************
imin get_first_active_imin() {
  imin ret = {0, 0, false, 0, 0, 0, 0};
  imin *tmp1 = entry_table.import_start;
  for (;;) {
    if (tmp1->is_active) {
      ret = *tmp1;
      break;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return ret;
}

//*************************************************************
void delete_imin(long_int id, Boolean is_delete) {
  imin *tmp1 = entry_table.import_start;
  for (;;) {
    if (tmp1->is_active && tmp1->id == id) {
      entry_table.import_inst_count--;
      if (!is_delete) {
        tmp1->is_active = false;
      } else {
        //TODO: delete import node
      }
      break;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
}

//*************************************************************
//*******************source_code functions*********************
//*************************************************************
void append_soco(uint8 type, soco s) {
  soco *q;
  q = (soco *) malloc(sizeof(soco));
  if (q == 0) return;
  q->line = s.line;
  str_init(&q->code, s.code);
  q->next = 0;
  //soco_main
  if (type == 1) {
    //printf("QW!!!:%s,%i,%s\n", s.code, entry_table.soco_main_start, q->code);
    if (entry_table.soco_main_start == 0) {
      entry_table.soco_main_start = entry_table.soco_main_end = q;
    } else {
      entry_table.soco_main_end->next = q;
      entry_table.soco_main_end = q;
    }
    entry_table.soco_main_count++;
  }
    //soco tokens
  else if (type == 2) {
    if (entry_table.soco_tokens_start == 0)
      entry_table.soco_tokens_start = entry_table.soco_tokens_end = q;
    else {
      entry_table.soco_tokens_end->next = q;
      entry_table.soco_tokens_end = q;
    }
    entry_table.soco_tokens_count++;
  }
}

//*************************************************************
void clear_soco(uint8 type) {
  //soco_main
  if (type == 1 && entry_table.soco_main_start > 0) {
    entry_table.soco_main_count = 0;
    soco *tmp1;
    entry_table.soco_main_end = entry_table.soco_main_start;
    entry_table.soco_main_start = 0;
    for (;;) {
      tmp1 = entry_table.soco_main_end;
      entry_table.soco_main_end = entry_table.soco_main_end->next;
      free(tmp1);
      if (entry_table.soco_main_end == 0) break;
    }
  } else if (type == 2 && entry_table.soco_tokens_start > 0) {
    entry_table.soco_tokens_count = 0;
    soco *tmp1;
    entry_table.soco_tokens_end = entry_table.soco_tokens_start;
    entry_table.soco_tokens_start = 0;
    for (;;) {
      tmp1 = entry_table.soco_tokens_end;
      entry_table.soco_tokens_end = entry_table.soco_tokens_end->next;
      free(tmp1);
      if (entry_table.soco_tokens_end == 0) break;
    }
  }
}

//*************************************************************
soco get_soco(uint8 type, uint32 ind) {
  soco ret = {0, 0, 0};
  uint32 ii = 0;
  soco *tmp1;
  //soco_main
  if (type == 1) {
    tmp1 = entry_table.soco_main_start;
  }
    //soco_tokens
  else if (type == 2) {
    tmp1 = entry_table.soco_tokens_start;
  }
  for (;;) {
    if (ii++ == ind) {
      ret = *tmp1;
      break;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }

  return ret;
}
//*************************************************************
Boolean edit_soco(uint8 type, uint32 line, String new_data) {
  soco *tmp1;
  //soco_main
  if (type == 1)tmp1 = entry_table.soco_main_start;
    //soco_tokens
  else if (type == 2)tmp1 = entry_table.soco_tokens_start;
  for (;;) {
    if (tmp1->line == line) {
      (*tmp1).code = new_data;
      return true;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }

  return false;
}
//*************************************************************
//******************utf8_strings functions*********************
//*************************************************************
void append_utst(utst s) {
  utst *q;
  q = (utst *) malloc(sizeof(utst));
  if (q == 0) return;
  q->id = s.id;
  q->max_bytes_per_char = s.max_bytes_per_char;
  utf8_str_init(&q->utf8_string, s.utf8_string);
  q->line = s.line;
  q->next = 0;
  if (entry_table.utst_start == 0)
    entry_table.utst_start = entry_table.utst_end = q;
  else {
    entry_table.utst_end->next = q;
    entry_table.utst_end = q;
  }
}
//*************************************************************
long_int add_to_utst(uint32 line, str_utf8 str, uint8 max_bytes) {
  if (max_bytes == 0)max_bytes = utf8_str_max_bytes(str, false);
  utst tmp1 = {++entry_table.utf8_strings_id, line, str, max_bytes};
  append_utst(tmp1);
  return entry_table.utf8_strings_id;
}
//*************************************************************
utst get_utst(long_int id) {
  utst ret = {0, 0, 0, 0};
  utst *tmp1 = entry_table.utst_start;
  for (;;) {
    if (tmp1->id == id) {
      ret = *tmp1;
      break;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return ret;
}
//*************************************************************
utst get_utst_by_string(String s) {
  utst nil = {0, 0, 0};
  uint32 len = str_length(s);
  String val = 0;
  if (len > 2 && s[0] == '\"' && s[len - 1] == '\"') {
    val = str_substring(s, 1, len - 1);
  } else {
    str_init(&val, s);
  }
  //------------
//   printf("OOOOO1:%s,%s,%i\n", s, val, str_is_int32(val));
  if (str_is_int32(val)) {
    int32 utst_id = str_to_int32(val);
//    print_struct(PRINT_UTF8_ST);
//    printf("OOOOOO2:%i\n", utst_id);
    utst ut = get_utst((long_int) utst_id);//TODO:
    return ut;
    //printf("UTT:%s=>%s,%i\n", s, utf8_to_unicode_str(ut.utf8_string), ut.max_bytes_per_char);
    //utf8_str_print("UTF8", ut.utf8_string, false);
  } else
    return nil;

}
//*************************************************************
utst get_utst_by_label(String s) {
  utst ret = {0, 0, 0, 0};
  long_int uid = str_to_long_int(str_substring(s, UTF8_ID_LBL_LEN, 0));
  if (uid == 0)return ret;
  utst *tmp1 = entry_table.utst_start;
  for (;;) {
    if (tmp1->id == uid) {
      return (*tmp1);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return ret;
}
//*************************************************************
//****************block_structures functions*******************
//*************************************************************
void append_blst(blst s) {
  blst *q;
  q = (blst *) malloc(sizeof(blst));
  if (q == 0) return;
  q->func_id = s.func_id;
  q->stru_id = s.stru_id;
  q->type = s.type;
  q->line = s.line;
  q->source_id = s.source_id;
  q->params_len = s.params_len;
  str_init(&q->lbl, s.lbl);
  str_list_init(&q->params, s.params, s.params_len);
  q->next = 0;
  //blst_func
  if (s.type == FUNC_BLOCK_ID) {
    q->id = ++entry_table.func_id;
    if (entry_table.blst_func_start == 0)
      entry_table.blst_func_start = entry_table.blst_func_end = q;
    else {
      entry_table.blst_func_end->next = q;
      entry_table.blst_func_end = q;
    }
  }
    //blst_stru
  else {
    q->id = ++entry_table.stru_id;
    if (entry_table.blst_stru_start == 0)
      entry_table.blst_stru_start = entry_table.blst_stru_end = q;
    else {
      entry_table.blst_stru_end->next = q;
      entry_table.blst_stru_end = q;
    }
  }
}

//*************************************************************
blst search_lbl_func(String lbl, str_list params, uint32 par_len) {
  blst null = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  blst *tmp1;
  tmp1 = entry_table.blst_func_start;
  //------------------
  for (;;) {
    if (par_len == tmp1->params_len && str_equal(tmp1->lbl, lbl) &&
        str_list_equal(tmp1->params, tmp1->params_len, params, par_len)) {
      return (*tmp1);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return null;
}
//*************************************************************
blst get_func_by_id(long_int id) {
  blst null = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  blst *tmp1;
  tmp1 = entry_table.blst_func_start;
  //------------------
  for (;;) {
    if (id == tmp1->id) {
      return (*tmp1);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return null;
}
//*************************************************************
blst search_lbl_stru(String lbl) {
  blst null = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  blst *tmp1;
  tmp1 = entry_table.blst_stru_start;
  if (lbl == 0 || lbl[0] != '@')return null;
  //------------------
  for (;;) {
    if (str_equal(tmp1->lbl, lbl)) {
      return (*tmp1);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return null;
}

//*************************************************************
//*******************data_types functions**********************
//*************************************************************
void append_datas(datas s) {
  datas *q;
  q = (datas *) malloc(sizeof(datas));
  if (q == 0) return;
  q->id = ++entry_table.datas_id;

  q->type = s.type;
  q->fid = s.fid;
  str_init(&q->name, s.name);
  q->params_len = s.params_len;
  //printf("##SSSSS:%s,%i\n",s.name,s.params_len);
  //if(s.params_len>0)
  //printf("RRRRRR:%s,%s,%i\n",s.name,print_str_list(s.params,s.params_len),s.params_len);
  str_list_init(&q->params, s.params, s.params_len);
  q->next = 0;
  if (entry_table.datas_start == 0)
    entry_table.datas_start = entry_table.datas_end = q;
  else {
    entry_table.datas_end->next = q;
    entry_table.datas_end = q;
  }
}

//*************************************************************
datas get_datas(long_int id) {
  datas null = {0, 0, 0, 0, 0, 0};
  datas *st = entry_table.datas_start;
  if (st == 0) return null;
  for (;;) {
    if (st->id == id) return (*st);
    st = st->next;
    if (st == 0) break;
  }
  return null;
}

//*************************************************************
datas search_datas(String name, long_int fid, Boolean is_all) {
  datas null = {0, 0, 0, 0, 0, 0};
  //printf("!!!!!SSSSS:%s\n",name);
  datas *st = entry_table.datas_start;
  if (st == 0) return null;
  for (;;) {
    if (is_all && str_equal(st->name, name)) return (*st);
    else if ((st->fid == 0 || st->fid == fid) && str_equal(st->name, name)) return (*st);
    st = st->next;
    if (st == 0) break;
  }

  return null;
}

//*************************************************************
//******************instructions functions*********************
//*************************************************************
void append_instru(instru s) {
  instru *q;
  q = (instru *) malloc(sizeof(instru));
  if (q == 0) return;
  q->id = entry_table.inst_id++;
  q->func_id = s.func_id;
  q->stru_id = s.stru_id;
  q->order = s.order;
  q->line = s.line;
  q->type = s.type;
  q->source_id = s.source_id;
  str_init(&q->code, s.code);
  q->next = 0;
  if (entry_table.instru_start == 0)
    entry_table.instru_start = entry_table.instru_end = q;
  else {
    entry_table.instru_end->next = q;
    entry_table.instru_end = q;
  }
}

//*************************************************************
instru get_instru_by_id(long_int id) {
  instru null = {0, 0, 0, 0, 0, 0, 0, 0};
  instru *st = entry_table.instru_start;
  if (st == 0) return null;
  for (;;) {
    if (st->id == id) {
      return (*st);
    }
    st = st->next;
    if (st == 0) break;
  }
  return null;
}
//*************************************************************
instru get_instru_by_params(long_int fid, long_int sid, uint32 order) {
  instru null = {0, 0, 0, 0, 0, 0, 0, 0};
  instru *st = entry_table.instru_start;
  if (st == 0) return null;
  for (;;) {
    if (st->func_id == fid && st->stru_id == sid && st->order == order) {
      return (*st);
    }
    st = st->next;
    if (st == 0) break;
  }
  return null;
}
//*************************************************************
instru get_instru_by_source(String source_file, uint32 source_line) {
  instru null = {0, 0, 0, 0, 0, 0, 0, 0};
  instru *st = entry_table.instru_start;
  if (st == 0) return null;
  for (;;) {
    if (st->line == source_line && str_equal(utf8_to_bytes_string(source_paths[st->source_id]), source_file)) {
      return (*st);
    }
    st = st->next;
    if (st == 0) break;
  }
  return null;
}
//*************************************************************
//***************instructions_order functions******************
//*************************************************************
void append_inor(inor s) {
  inor *q;
  q = (inor *) malloc(sizeof(inor));
  if (q == 0) return;
  q->fid = s.fid;
  q->sid = s.sid;
  q->order = s.order;
  q->next = 0;
  entry_table.inor_count++;
  if (entry_table.inor_start == 0) {
    entry_table.inor_start = entry_table.inor_end = q;
  } else {
    entry_table.inor_end->next = q;
    entry_table.inor_end = q;
  }
}

//*************************************************************
uint32 get_order(long_int fid, long_int sid) {
  inor *tmp1 = entry_table.inor_start;
  if (tmp1 == 0) return 0;
  for (;;) {
    if (tmp1->fid == fid && tmp1->sid == sid) {
      return tmp1->order;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return 0;
}

//*************************************************************
void set_order(long_int fid, long_int sid, uint32 order) {
  uint32 index = 0;
  Boolean is_exist = false;
  //***************search
  inor *tmp1 = entry_table.inor_start;
  if (tmp1 != 0) {
    for (;;) {
      index++;
      if (tmp1->fid == fid && tmp1->sid == sid) {
        is_exist = true;
        break;
      }
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
  }
  //**************set
  if (!is_exist) {
    inor tmp2 = {fid, sid, order, 0};
    append_inor(tmp2);
  } else {
    uint32 ind = 0;
    inor *tmp1 = entry_table.inor_start;
    for (;;) {
      ind++;
      if (ind == index) {
        tmp1->order = order;
        break;
      }
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
  }
}

//*************************************************************
//***************stru_to_in_struct functions******************
//*************************************************************
void empty_stoi(stoi s[], uint32 size) {
  for (uint32 i = 0; i < size; i++) {
    s[i].id = 0;
    s[i].is_active = false;
    s[i].is_inline = false;
    s[i].stru_pars = 0;
  }
}

//*************************************************************
//****************built_in_funcs functions*********************
//*************************************************************
void append_bifs(bifs s) {
  bifs *q;
  q = (bifs *) malloc(sizeof(bifs));
  if (q == 0) return;
  q->id = s.id;
  q->type = s.type;
  q->params_len = s.params_len;
  q->returns_len = s.returns_len;
  str_init(&q->func_name, s.func_name);
  str_init(&q->params, s.params);
  str_init(&q->returns, s.returns);
  q->next = 0;
  //entry_table.bifs_len++;
  if (entry_table.bifs_start == 0) {
    entry_table.bifs_start = entry_table.bifs_end = q;
  } else {
    entry_table.bifs_end->next = q;
    entry_table.bifs_end = q;
  }
}

//*************************************************************
void add_to_bifs(long_int id, uint8 type, String func_name, String params, String returns) {
  uint8 par_len = 0, ret_len = 0;
  par_len = char_search_count(params, ';');
  ret_len = char_search_count(returns, ';');
  bifs tmp = {id, type, func_name, params, par_len, returns, ret_len};
  append_bifs(tmp);
}
//*************************************************************
//*****************modules_funcs functions*********************
//*************************************************************
//void append_mofu(mofu s, mofu *start, mofu *end) {
//  mofu *q;
//  q = (mofu *) malloc(sizeof(mofu));
//  if (q == 0) return;
//  q->id = s.id;
//  q->mod_id = s.mod_id;
//  q->params_len = s.params_len;
//  q->returns_len = s.returns_len;
//  str_init(&q->func_name, s.func_name);
//  str_init(&q->params, s.params);
//  str_init(&q->returns, s.returns);
//  q->next = 0;
//  if (start == 0 /*|| (*start) == 0*/) {
//    //*(&start) = *(&end) = &q;
//    start = end = q;
//  } else {
////    (*end)->next = q;
////    (*end) = q;
//    end->next = q;
//    end = q;
//  }
////  printf("mod_func:%i,%s,%i,%i\n", s.id, s.func_name, (*start), (*end));
//}

//*************************************************************
//******************magic_macros functions*********************
//*************************************************************
void append_mama(mama s) {
  mama *q;
  q = (mama *) malloc(sizeof(mama));
  if (q == 0) return;
  q->id = s.id;
  q->type = s.type;
  q->value_type = s.value_type;
  str_init(&q->key, s.key);
  str_init(&q->value, s.value);
  q->next = 0;
  if (entry_table.mama_start == 0) {
    entry_table.mama_start = entry_table.mama_end = q;
  } else {
    entry_table.mama_end->next = q;
    entry_table.mama_end = q;
  }
}

//*************************************************************
void add_to_mama(uint8 type, String value_type, String key, String value) {
  mama tmp = {++entry_table.mama_id, type, value_type, key, value, 0};
  append_mama(tmp);
}

//*************************************************************
mama get_mama(uint8 type, String key) {
  mama null = {0, 0, 0, 0, 0, 0};
  mama *tmp1 = entry_table.mama_start;
  if (tmp1 == 0) return null;
  for (;;) {
    if (tmp1->type == type && str_equal(tmp1->key, key)) {
      return (*tmp1);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return null;
}
//*************************************************************
String get_config_mama_value(String key) {
  mama *tmp1 = entry_table.mama_start;
  if (tmp1 == 0) return 0;
  for (;;) {
    if (tmp1->type == CONFIG_MAGIC_MACRO_TYPE && str_equal(tmp1->key, key)) {
      return (*tmp1).value;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return 0;
}
//*************************************************************
//********************vals_array functions*********************
//*************************************************************
void append_vaar(vaar s, vaar_en *s1) {
  vaar *q;
  q = (vaar *) malloc(sizeof(vaar));
  if (q == 0) return;
  //printf("FFFFF:%s,%s\n",s.value,s.index);
  q->data_id = s.data_id;
  q->sub_type = s.sub_type;
  str_init(&q->value, s.value);
  str_init(&q->index, s.index);
  q->next = 0;
  if ((*s1).start == 0) {
    (*s1).start = (*s1).end = q;
  } else {
    (*s1).end->next = q;
    (*s1).end = q;
  }
}

//*************************************************************
//***************struct_descriptor functions*******************
//*************************************************************
void append_stde(stde s) {
  stde *q;
  q = (stde *) malloc(sizeof(stde));
  if (q == 0) return;
  q->id = s.id;
  str_init(&q->type, s.type);
  q->st = s.st;
  q->next = 0;
  if (entry_table.stde_start == 0) {
    entry_table.stde_start = entry_table.stde_end = q;
  } else {
    entry_table.stde_end->next = q;
    entry_table.stde_end = q;
  }
}

//*************************************************************
stde get_stde(long_int id) {
  stde null = {0, 0, 0};
  stde *tmp1 = entry_table.stde_start;
  if (tmp1 == 0) return null;
  for (;;) {
    if (tmp1->id == id) {
      return (*tmp1);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return null;
}

//*************************************************************
//***************functions_stack functions*********************
//*************************************************************
void append_fust(fust s) {
  fust *q;
  q = (fust *) malloc(sizeof(fust));
  if (q == 0) return;
  q->sid = s.sid;
  q->fid = s.fid;
  q->fin = s.fin;
  q->parent_fin = s.parent_fin;
  q->order = s.order;
  q->next = 0;
  if (entry_table.fust_start == 0) {
    entry_table.fust_start = entry_table.fust_end = q;
  } else {//printf("!!!:%i,%i\n",s.fin,entry_table.fust_end);
    entry_table.fust_end->next = q;
    entry_table.fust_end = q;
  }
  entry_table.fust_len++;
}

//*************************************************************
fust get_last_fust() {
  fust null = {0, 0, 0};
  if (entry_table.fust_end == 0)return null;
  return (*entry_table.fust_end);
}

//*************************************************************
void delete_last_fust() {
  if (entry_table.fust_end == 0)return;
  //get last-1 item
  fust *tmp1 = entry_table.fust_start;
  if (tmp1 == 0) return;
  for (;;) {
    if (tmp1->next == entry_table.fust_end) {
      tmp1->next = 0;
      break;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  free(entry_table.fust_end);
  entry_table.fust_end = tmp1;
  if (entry_table.fust_end == 0)entry_table.fust_start = 0;
  entry_table.fust_len--;
}

//*************************************************************
//***************structures_stack functions********************
//*************************************************************
void append_stst(stst s) {
  stst *q;
  q = (stst *) malloc(sizeof(stst));
  if (q == 0) return;
  q->sid = s.sid;
  q->parent_sid = s.parent_sid;
  q->fid = s.fid;
  q->fin = s.fin;
  q->type = s.type;
  q->order = s.order;
  str_init(&q->extra, s.extra);
  q->next = 0;
//  printf("####-----:%i\n",entry_table.stst_end);
  if (entry_table.stst_start == 0) {
    entry_table.stst_start = entry_table.stst_end = q;
  } else {
    entry_table.stst_end->next = q;
    entry_table.stst_end = q;
  }
  entry_table.stst_len++;
}

//*************************************************************
stst get_last_stst() {
  stst null = {0, 0, 0, 0, 0, 0, 0, 0};
  if (entry_table.stst_end == 0)return null;
  return (*entry_table.stst_end);
}
//*************************************************************
void delete_last_stst() {
  if (entry_table.stst_end == 0)return;
  //get last-1 item
  stst *tmp1 = entry_table.stst_start;
  if (tmp1 == 0) return;
  for (;;) {
    if (tmp1->next == entry_table.stst_end) {
      tmp1->next = 0;
      break;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  free(entry_table.stst_end);
  entry_table.stst_end = tmp1;
  if (entry_table.stst_end == 0)entry_table.stst_start = 0;
  entry_table.stst_len--;
}
//*************************************************************
//***************condition_level functions*********************
//*************************************************************
void append_cole(cole s) {
  cole *q;
  q = (cole *) malloc(sizeof(cole));
  if (q == 0) return;
  q->id = ++entry_table.cole_len;
  q->fin = s.fin;
  q->sid = s.sid;
  q->is_complete = s.is_complete;
  q->next = 0;
  q->prev = entry_table.cole_end;
//  printf("$$$:%i,%i,%i\n", s.fin, s.sid,entry_table.cole_end!=0?entry_table.cole_end->fin:-1);
  if (entry_table.cole_start == 0) {
    entry_table.cole_start = entry_table.cole_end = q;
  } else {
    entry_table.cole_end->next = q;
    entry_table.cole_end = q;
  }
}
//*************************************************************
cole get_cole_by_id(uint32 id) {
  cole null = {0, 0, 0, 0, 0, 0};
  cole *tmp1 = entry_table.cole_start;
  if (tmp1 == 0) return null;
  for (;;) {
    if (tmp1->id == id) {
      return (*tmp1);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return null;
}
//*************************************************************
Boolean set_cole_complete(uint32 id) {
  cole *tmp1 = entry_table.cole_start;
  if (tmp1 == 0) return false;
  for (;;) {
    if (tmp1->id == id) {
      tmp1->is_complete = true;
      return true;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return false;
}
//*************************************************************
cole pop_last_cole() {
  cole ret = {0, 0, 0, 0, 0, 0};
  cole *tmp1 = entry_table.cole_end;
  if (tmp1 == 0) return ret;
  ret.id = (*tmp1).id;
  ret.fin = (*tmp1).fin;
  ret.sid = (*tmp1).sid;
  ret.is_complete = (*tmp1).is_complete;
//  printf("#####:%i,%i,%i,%i\n", ret.fin, tmp1->prev, entry_table.cole_end, entry_table.cole_start);
  entry_table.cole_len--;
  //if exist just one item
  if (entry_table.cole_start == tmp1 || tmp1->prev == 0) {
    free(tmp1);
    entry_table.cole_start = entry_table.cole_end = 0;
  }
    //else exist more items
  else {
    entry_table.cole_end = tmp1->prev;
    entry_table.cole_end->next = 0;
    free(tmp1);
  }

  return ret;
}
//*************************************************************
//********************loop_level functions*********************
//*************************************************************
void append_lole(lole s) {
  lole *q;
  q = (lole *) malloc(sizeof(lole));
  if (q == 0) return;
  q->id = ++entry_table.lole_len;
  q->fin = s.fin;
  q->sid = s.sid;
  q->next = 0;
  q->prev = entry_table.lole_end;
//  printf("$$$:%i,%i,%i\n", s.fin, s.sid,entry_table.cole_end!=0?entry_table.cole_end->fin:-1);
  if (entry_table.lole_start == 0) {
    entry_table.lole_start = entry_table.lole_end = q;
  } else {
    entry_table.lole_end->next = q;
    entry_table.lole_end = q;
  }
}
//*************************************************************
lole pop_last_lole() {
  lole ret = {0, 0, 0, 0, 0};
  lole *tmp1 = entry_table.lole_end;
  if (tmp1 == 0) return ret;
  ret.id = (*tmp1).id;
  ret.fin = (*tmp1).fin;
  ret.sid = (*tmp1).sid;
  entry_table.lole_len--;
  //if exist just one item
  if (entry_table.lole_start == tmp1 || tmp1->prev == 0) {
    free(tmp1);
    entry_table.lole_start = entry_table.lole_end = 0;
  }
    //else exist more items
  else {
    entry_table.lole_end = tmp1->prev;
    entry_table.lole_end->next = 0;
    free(tmp1);
  }

  return ret;
}
//*************************************************************
//***************debug_breakpoints functions*******************
//*************************************************************
void append_debr(uint32 line, String path) {
  debr *q;
  q = (debr *) malloc(sizeof(debr));
  if (q == 0) return;
  q->line_number = line;
  str_init(&q->source_path, path);
  q->next = 0;
  if (entry_table.debr_start == 0) {
    entry_table.debr_start = entry_table.debr_end = q;
  } else {
    entry_table.debr_end->next = q;
    entry_table.debr_end = q;
  }
  entry_table.debr_len++;
}
//*************************************************************
Boolean is_find_debr(uint32 line, String path) {
  debr null = {0, 0, 0};
  debr *tmp1 = entry_table.debr_start;
  if (tmp1 == 0) return false;
  for (;;) {
    if (tmp1->line_number == line && str_equal(path, tmp1->source_path)) {
      return true;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return false;
}
//*************************************************************
Boolean delete_debr(uint32 line, String path) {
  debr *tmp1 = entry_table.debr_start;
  if (tmp1 == 0) return false;
  for (;;) {
    if (tmp1->line_number == line && str_equal(path, tmp1->source_path)) {
      tmp1->line_number = 0;
      tmp1->source_path = 0;
      return true;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return false;
}



