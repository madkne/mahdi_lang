#include <Mahdi/system.h>

//************************************************prototypes
Boolean INTR_start();

void INTR_init();

uint8 interpreter_mode = 0;

//************************************************
int main(int argc, char **argv) {
  //-------------------------time of start program
  AppStartedClock = clock();
  AppStartedTime = STR_from_Longint(CALL_unix_time());
  is_real_mahdi = true;
  //-------------------------init exceptions list
  EXP_init();
  //--------------------------get Argvs,analyze it
  //=>save program command
  program_command=argv[0];
  //=>no any argv, show help usage
  if (argc < 2) {
    MHELP_usage();
  }
  //=>-v argv, show help version 
  else if (argv[1][0] == '-' && argv[1][1] == 'v') {
    MHELP_version();
    interpreter_mode = 'v';
  }
  //=>-i argv, show help info  
  else if (argv[1][0] == '-' && argv[1][1] == 'i') {
    MHELP_info();
    interpreter_mode = 'i';
  } 
  //=>-h argv, show mhelp (with params)
  else if (argv[1][0] == '-' && argv[1][1] == 'h') {
    StrList help_argvs = 0;
    uint32 help_argvs_len = 0;
    //=>append the other argvs to StrList
    for (uint32 i = 2; i < argc; i++)
      SLIST_append(&help_argvs, STR_trim_space(argv[i]), help_argvs_len++);
    //=>run mhelp program
    MHELP_starter(help_argvs, help_argvs_len);
    interpreter_mode = 'h';
  }
  //=>if any argv that start with '-', then error! 
  else if (argv[1][0] == '-') {
    EXP_print_error(0, "unknown_opt", "stdin", argv[1], 0, "main");
    MHELP_usage();
  }
  //=>if argv not an option, check for exist a file 
  else {
    //=>get argv as a file path and absolute path
    STR_init(&stdin_source_path, argv[1]);
    stdin_source_path = CALL_abspath(stdin_source_path);
    //=>check if source path is exist
    if(stdin_source_path==0){
      EXP_print_error(0, "not_exist_file", 0, argv[1], 0, "main");
    }else{
      //=>store arguments of program
      if (argc > 2){
        for (int ii = 2; ii < argc; ++ii)
          SLIST_append(&program_argvs, argv[ii], argvs_len++);
      }
      Boolean ret = INTR_start();
      interpreter_mode = 'f';
      if (!ret)
        EXP_print_error(0, "bad_exit", "stdin", 0, 0, "main");
    }
  }

//  String ss = 0;
//  str_init(&ss, "Hello Amin.");
//  printf("encode:%s\n>>>%s\n>>>%s$\n", ss, MPLV1_encode(ss),MPLV1_decode(MPLV1_encode(ss)));
//  console_color_reverse();
//  printf("Hello World:%s\n",argv[1]);
//  console_color_reset();
//  printf("Hello World:%s\n",argv[1]);
  //  print_struct(PRINT_CONDITION_LEVEL_ST);
  //	print_magic_macros(CONFIG_MAGIC_MACRO_TYPE);
//  print_struct(PRINT_UTF8_ST);
  //-------------------------time of end program
  APP_exit(EXIT_NORMAL);
  return 0;
}
//************************************************
Boolean INTR_start() {
  //-----------------------init interpreter
  STR_init(&interpreter_level, "init");
  INTR_init();
//   //-----------------------parsing source codes
//   str_init(&interpreter_level, "parse");
//   Boolean ret0 = import_all_files();
//   //printf("VVVVVV:%i\n",ret0);
//   if (!ret0) return false;
//   //-----------------------meaning&running instructions
//   str_init(&interpreter_level, "runtime");
//   Boolean ret3 = start_runtime();
//   if (!ret3) return false;
//   //show debug  lists info
//   if (is_programmer_debug > 0) {
//     show_memory(0);
// //    print_magic_macros(2);
// //    print_struct(PRINT_INSTRU_ST);
//   }
//   //-----------------------free memory
//   str_init(&interpreter_level, "free");
}

//************************************************
void INTR_init() {
  //********************
  DEF_init();
//   //********************
//   init_memory();
//   //********************
//   init_built_in_funcs();
//   //********************
//   init_magic_define();
//   //********************
//   init_magic_config();
  //********************
  //get_basic_system_info()
}

//************************************************