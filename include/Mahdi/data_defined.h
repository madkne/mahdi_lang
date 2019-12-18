//
// Created by madkne on 2019.10.24
//

#ifndef __MAHDI_DATA_DEFINED_H
#define __MAHDI_DATA_DEFINED_H  1

//******************************data values
clock_t AppStartedClock;
String AppStartedTime;
String project_root;
String stdin_source_path;
String interpreter_level;
String interpreter_path;
String interpreter_tmp_path;
String os_separator;
uint8 MAX_INT_LEN;
uint8 MAX_FLOAT_LEN;
uint8 max_estimate_divide_huge;
uint8 max_float_estimate_huge_X0;
String new_line_char;
String os_tmp_dir;
uint32 BufferSize;
String hostname;
String os_version;
uint64 os_total_memory;
uint64 os_total_disk;
Longint max_size_id;
uint32 max_mahdi_modules_instance_len;
Boolean is_real_mahdi;
String program_command;
//******************************config values
uint8 is_programmer_debug;
String logfile_path;
int8 errors_mode;
int8 warnings_mode;
uint8 max_steps_estimate_huge;
uint32 max_decimal_has_huge;
// Boolean safe_mode;
// Boolean optimize_mode;
Boolean debug_mode;
// String bytecode_path;
Boolean build_mode;
String buildfile_path;

//******************************
#if WINDOWS_PLATFORM == true
HINSTANCE mahdi_modules_instance[100];
#elif LINUX_PLATFORM == true
void * mahdi_modules_instance[100];
#endif
String exceptions_group[16];
String exceptions_type[4];
String keywords[26];
// String keywords_out[13];
String block_instructions[6];
uint8 single_operators[7];
String comparative_operators[6];
String alloc_operators[8];
String boolean_operators[2];
String basic_types[3];
uint8 golden_bytes[5];
uint8 splitters[10];
uint8 words_splitter[12];
uint8 sub_types[6];
String control_chars[5];
StrList program_argvs;
uint32 argvs_len;
StrList installed_modules;
uint32 installed_modules_len;
//****************************import struct
typedef struct import_inst_struct {
  Longint id;
  uint8 type;
  Boolean is_active;
  String name;
  String path;
  StrList packages;
  StrList functions;
  uint32 pack_len;
  uint32 func_len;
  uint8 err_code;

  uint32 line;
  uint32 source_index; //=>index of entry_table.sources_list StrList

  struct import_inst_struct *next;
} imin;
//****************************source_code struct
typedef struct source_code_struct {
  uint32 line;
  String code;

  struct source_code_struct *next;
} soco;
//****************************exceptions_list struct
typedef struct exceptions_list_struct {
  uint32 id;
  int8 type;
  uint8 group;
  String lbl;
  String text;

  struct exceptions_list_struct *next;
} exli;

//****************************virtual memory
//runtime
typedef struct var_memory_struct {
  Longint id;
  Longint pointer_id;
  Longint stru_id;     //(0:global vars or pack vars or func direct vars)
  Longint func_id;     //(0:global vars or pack vars)
  Longint func_index;  
  Longint pack_id;     //(0:global vars in outer or inner of global functions)
  Longint pack_index;
  Longint type_id;
  Boolean is_static;
  Boolean is_private;
  uint8 flag;
  String  name;
  struct var_memory_struct *next;
} Mvar;
typedef struct pointer_memory_struct {
  Longint id;
  uint8 type; //('i':int , 'f':float , 'h':huge , 's':string , 'u':utf8 , 'b':boolean , 'p':Pointer To Mpoint , 'v':Pointer To Mvar)
  String data;
  struct pointer_memory_struct *next;
} Mpoint;
//static
Mpoint *hash_pointers[HASH_MEM_SIZE];
Mpoint *hash_pointers_end[HASH_MEM_SIZE];
Longint hash_pointers_len[HASH_MEM_SIZE];

//****************************utf8_strings struct
/**
 * store utf8 strings like str r='سلام' => str s='!UTF8!'
 */
typedef struct utf8_strings_struct {
  Longint id;
  uint32 line;
  UString utf8_string;
  uint8 max_bytes_per_char;

  struct utf8_strings_struct *next;
} utst;
//****************************instructions struct
/**
 * store all instructions except block headers
 */ 
typedef struct instructions_struct {
  Longint id;
  Longint pack_id;
  Longint func_id;
  Longint stru_id;
  uint32 order;
  String code;
  uint8 type;

  uint32 line;
  uint32 source_id;
  struct instructions_struct *next;
} instru;
//****************************instructions_order struct
typedef struct instructions_order_struct {
  Longint pid;
  Longint fid;
  Longint sid;
  uint32 order;

  struct instructions_order_struct *next;
} inor;
//****************************block_structures struct
typedef struct block_structures_struct {
  Longint id;
  Longint pack_id;
  Longint func_id;
  Longint stru_id;
  uint8 type; //loop,if,elif,else,manage,choose,func,pack
  String label;
  StrList params; //for packs, is attributes vars
  uint32 params_len;

  uint32 line;
  uint32 source_id;
  struct block_structures_struct *next;
} blst;
// //****************************data_types struct
/**
 * (parser) store all valid data types, basic and user defined by packages
 */ 
typedef struct data_types_struct {
  Longint id;
  Longint fid;
  Longint pack_id;
  uint8 type; //basic,package
  String name;

  struct data_types_struct *next;
} datas;
//****************************built_in_funcs struct
typedef struct built_in_funcs_struct {
  uint32 id;
  uint32 parent_type;
  String func_name;
  String params;
  uint8 params_len;
  String returns;
  uint8 returns_len;

  struct built_in_funcs_struct *next;
} bifs;
//****************************structures_stack struct
/**
 * (runtime) when multi structures init in complex or basic, need to call back before and before structure with this struct
 */ 
typedef struct structures_stack_struct {
  uint8 type;
  Longint pid;
  Longint pin;
  Longint fid;
  Longint fin;
  Longint sid;
  Longint parent_sid;
  Longint order;
  String extra;

  struct structures_stack_struct *next;
} stst;
//****************************condition_level struct
/**
 * (runtime) when a condition structure like if,elif,else in complex init, then use this struct to store states of conditions
 */ 
typedef struct condition_level_struct {
  uint32 id;
  Longint pin;
  Longint fin;
  Longint sid;
  Boolean is_complete;

  struct condition_level_struct *prev;
  struct condition_level_struct *next;
} cole;
//****************************loop_level struct
/**
 * (runtime) when a loop structure in complex init, then use this struct to store states of loops
 */ 
typedef struct loop_level_struct {
  uint32 id;
  Longint pin;
  Longint fin;
  Longint sid;

  struct loop_level_struct *prev;
  struct loop_level_struct *next;
} lole;

//****************************debug_breakpoints struct
/**
 * (debug) when in debug mode, user can define brakepoints based on source program and store in this struct 
 */ 
typedef struct debug_breakpoints_struct {
  uint32 line_number;
  String source_path;
  struct debug_breakpoints_struct *next;
} debr;
//****************************stru_to_in struct
/**
 * (parser) store if,elif,else,loop,manage,... structures that can be complex and in to the other
 */ 
typedef struct stru_to_in_struct {
  Longint id;
  uint32 stru_pars;
  Boolean is_active;
  Boolean is_inline;
} stoi;
// //****************************def_var struct
// typedef struct def_var_struct {
//   String main_type;
//   uint8 sub_type;
//   String name_var;
//   String index_var;
//   String value_var;
//   Longint fid;
//   Longint sid;

// } def_var_s;

// //**********************vals_array_struct
// typedef struct vals_array_struct {
//   Longint data_id;
//   uint8 sub_type;
//   String value;
//   String index;
//   struct vals_array_struct *next;
// } vaar;
// typedef struct vals_array_entries {
//   vaar *start;
//   vaar *end;
//   uint32 count;
// } vaar_en;


// //****************************modules_funcs struct
// typedef struct modules_funcs_struct {
//   uint32 id;
//   uint8 mod_id;
//   String func_name;
//   String params;
//   uint8 params_len;
//   String returns;
//   uint8 returns_len;

//   struct modules_funcs_struct *next;
// } mofu;
// //****************************functions_stack struct
// typedef struct functions_stack_struct {
//   Longint fid;
//   Longint fin;
//   Longint sid;
//   Longint order;
//   Longint parent_fin;

//   struct functions_stack_struct *next;
// } fust;


// //****************************alloc_var struct
// struct alloc_var_struct {
//   String type;
//   String name;
//   String alloc;
//   String index;
// };


// //****************************call_func struct
// /*typedef struct call_func_struct {
//     String func_name;
//     String func_index;
//     String func_rets;
//     str_list params;
//     uint32 params_len;
//     Boolean is_core_func;
// } call_func_s;
// //****************************vars_table struct
// typedef struct vars_table_table {
//     Longint id;
//     String name;
//     uint8 var_type;
//     String pack_name;
//     Boolean is_static;
//     Boolean is_const;
//     Boolean is_private;
//     Longint sid;
//     Longint fid;
//     Longint pid;

//     struct vars_table_table *next;
// } vata;
// //****************************core_lib struct
// typedef struct core_lib_struct {
//     uint32 id;
//     uint8 pack_id;
//     String func_name;
//     String params;
//     String returns;
//     uint8 params_len;
//     uint8 returns_len;
//     Boolean is_static;
//     Boolean is_implement;

//     struct core_lib_struct *next;
// } coli;*/
// //****************************entry_table struct
struct entry_table_struct {
  imin *import_start;
  imin *import_end;
  Longint import_id;
  uint32 import_active_count;

  soco *soco_main_start;
  soco *soco_main_end;
  String current_source_path;
  Longint soco_main_count;

  soco *soco_tokens_start;
  soco *soco_tokens_end;
  Longint soco_tokens_count;

  exli *exli_start;
  exli *exli_end;
  Longint exceptions_count;

  utst *utst_start;
  utst *utst_end;
  Longint utf8_strings_id;

  instru *instru_start;
  instru *instru_end;
  Longint inst_id;

  blst *blst_stru_start;
  blst *blst_stru_end;
  Longint stru_id;

  blst *blst_pack_start;
  blst *blst_pack_end;
  Longint pack_id;
  Longint pack_index;

  blst *blst_func_start;
  blst *blst_func_end;
  Longint func_id;
  Longint func_index;

  datas *datas_start;
  datas *datas_end;
  Longint datas_id;

  inor *inor_start;
  inor *inor_end;
  Longint inor_count;

  bifs *bifs_start;
  bifs *bifs_end;
  uint32 bifs_len;

  // fust *fust_start;
  // fust *fust_end;
  // uint32 fust_len;

  stst *stst_start;
  stst *stst_end;
  uint32 stst_len;

  cole *cole_start;
  cole *cole_end;
  uint32 cole_len;

  lole *lole_start;
  lole *lole_end;
  uint32 lole_len;

  debr *debr_start;
  debr *debr_end;
  uint32 debr_len;

  Boolean debug_is_run;
  Boolean debug_is_next;

  Mvar *var_memory_start;
  Mvar *var_memory_end;
  Mpoint *pointer_memory_start;
  Mpoint *pointer_memory_end;
  Longint var_mem_id;
  Longint var_mem_len;
  Longint pointer_mem_id;
  Longint pointer_mem_len;

  int8 next_break_inst;
  uint32 break_count;

  Longint cur_pid;
  Longint cur_pin;
  Longint cur_fid;
  Longint cur_fin;
  Longint cur_sid;
  Longint cur_order;
  Longint parent_fin;

  String Rsrc;
  Longint return_fin;
  uint32 Rorder, Rline;
  Boolean is_stop_APP_controller, is_next_inst_running, is_occur_error_exception;

  StrList sources_list;
  uint32 sources_len;

  StrList post_short_alloc;
  uint32 post_short_alloc_len;

};
struct entry_table_struct entry_table;

//****************************functions
void DEF_init();

//=>soco functions
void _soco_append(uint8 type, uint32 line,String code);
void _soco_clear(uint8 type);
soco _soco_get(uint8 type, uint32 ind);

// soco get_soco(uint8 type, uint32 ind);
// Boolean edit_soco(uint8 type, uint32 line, String new_data);
//=>stoi functions
void _stoi_empty(stoi s[], uint32 size);
//=>utst functions
void _utst_append(utst s);
Longint _utst_add(uint32 line, UString str, uint8 max_bytes);
// Longint add_to_utst(uint32 line,str_utf8 str,uint8 max_bytes);
// utst get_utst(Longint id);
// utst get_utst_by_string(String s);
// utst get_utst_by_label(String s);
// //-------------------------blst funcs
// void append_blst(blst s);

// blst search_lbl_func(String lbl, str_list params, uint32 par_len);
// blst get_func_by_id(Longint id);
// blst search_lbl_stru(String lbl);
// //-------------------------datas funcs
void _datas_append(Longint fid,Longint pack_id,uint8 type,String name);

// datas _datas_get(Longint id);

// datas _datas_search(String name, Longint fid, Boolean is_all);

// //-------------------------instru funcs
// void append_instru(instru s);

// instru get_instru_by_id(Longint id);
// instru get_instru_by_params(Longint fid, Longint sid, uint32 order);
// instru get_instru_by_source(String source_file, uint32 source_line);
// //-------------------------inor funcs
// void append_inor(inor s);

// uint32 get_order(Longint fid, Longint sid);

// void set_order(Longint fid, Longint sid, uint32 order);

// //-------------------------stoi funcs
// void empty_stoi(stoi s[], uint32 size);

// //-------------------------bifs funcs
// void append_bifs(bifs s);

// void add_to_bifs(Longint id, uint8 type, String func_name, String params, String returns);
// //-------------------------mofu funcs
// //void append_mofu(mofu s, mofu *start, mofu *end);
// //-------------------------mama funcs
// void append_mama(mama s);

// void add_to_mama(uint8 type, String value_type, String key, String value);

// mama get_mama(uint8 type, String key);
// String get_config_mama_value(String key);
// //-------------------------vaar funcs
// void append_vaar(vaar s, vaar_en *s1);

// //-------------------------stde funcs
// void append_stde(stde s);

// stde get_stde(Longint id);

// //-------------------------fust funcs
// void append_fust(fust s);

// fust get_last_fust();

// void delete_last_fust();
// //-------------------------stst funcs
// void append_stst(stst s);
// stst get_last_stst();
// void delete_last_stst();
// //-------------------------cole funcs
// void append_cole(cole s);
// cole get_cole_by_id(uint32 id);
// Boolean set_cole_complete(uint32 id);
// cole pop_last_cole();
// //-------------------------lole funcs
// void append_lole(lole s);
// lole pop_last_lole();
// //-------------------------debr funcs
// void append_debr(uint32 line, String path);
// Boolean is_find_debr(uint32 line, String path);
// Boolean delete_debr(uint32 line, String path);
#endif //__MAHDI_DATA_DEFINED_H
