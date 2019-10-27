//
// Created by madkne on 2019.10.24
//

#ifndef MAHDI_CONSTANTS_H
#define MAHDI_CONSTANTS_H       1

//**************************************************define interpreter constants
#define  APP_NAME                       "Mahdi Programming Language"
#define  VERSION                        "0.0.23"    //X.Y.Z=> Z<100,Y<10
#define  VERSION_NUMBER                 2          //XYZ=>(X*100)+(Y*10)+(Z/10)
#define  VERSION_NAME                   "BETA"
#define  SLOGAN                         "Assemble all the good features of programming languages" //اجتماع تمام ویژگی های خوب زبان های برنامه نویسی

#define  FIRST_BUILD                    "2019.10.26"
#define  BETA_LBUILD                    "undefined"
#define  RC_LBUILD                      "undefined"
#define  LICENCE                        "MIT"
#define  OFFICIAL_WEBSITE               "http://mahdi-lang.ir"
#define  COPYRIGHT                      "Copyright (C) 2019"
#define  C_CORE_CODE_LINES              0       //0.6.31 - 0.6.90
#define  H_CORE_CODE_LINES              0
#define  MODULES_CODE_LINES             0           //+=16757
#define  LANGUAGE_CREATOR               "madkne"
#define  CREATOR_SIGNATURE              "603556816519829596"
#define  TRUST_LEVEL                    0          //100%
#define  WINDOWS_COMPATIBLE             false
#define  LINUX_COMPATIBLE               true
#define  X64_COMPATIBLE                 false
#define  X32_COMPATIBLE                 true
//**************************************************define other constants
#if x64_OS == true
#define OS_ARCH                         "x64"
#define HASH_MEM_SIZE                   (250000/2)+1
#elif x86_OS == true
#define OS_ARCH                         "x86"
#define HASH_MEM_SIZE                   (500000/2)+1
#endif
//------------------------------------------
#if LINUX_PLATFORM == true
#define   OS_SEPARATOR                  '/'
#define   OS_TYPE                       "linux"
#define   MAX_INT_NUMBER                10000000
#define   MAX_FLOAT_NUMBER              1000000000
#define   BUILDER_LIB_NAME              "builder.so"
#define   APP_EXTENSION                 ""
#define   LIB_EXTENSION                 ".so"
#elif WINDOWS_PLATFORM == true
#define   OS_SEPARATOR                  '\\'
#define   OS_TYPE                       "windows"
#define   MAX_INT_NUMBER                INT_MAX
#define   MAX_FLOAT_NUMBER              DBL_MAX
#define   BUILDER_LIB_NAME              "builder.dll"
#define   APP_EXTENSION                 ".exe"
#define   LIB_EXTENSION                 ".dll"
#endif
//------------------------------------------
#define   HASH_VCACHE_SIZE              10001 //must be first number
#define   HASH_PCACHE_SIZE              50001
#define   INT_USED_BYTES                sizeof(MAX_INT_NUMBER)
#define   FLOAT_USED_BYTES              sizeof(MAX_FLOAT_NUMBER)
#define   MAX_ARRAY_DIMENSIONS          5
#define   MAX_VAR_ALLOC_INSTRUCTIONS    15
#define   MAX_PARAMS_STRUCT             30
#define   MAX_INTO_IN_STRUCTURES        20
#define   MAX_INTO_IN_FUNCTION_CALLS    30
#define   MAX_OPENED_FILES              20
#define   END_OF_FILE                  -10
#define   SYSTEM_STRING_DEFINE          "SYS__STR_"
#define   RETURN_TMP_NAME               "@ret"
#define   RETURN_TMP_POINTER_ID         2
#define   STRUCTURES_LABEL              "@@@STRU_"
#define   SWITCH_LABEL                  "@@@SWITCH_"
#define   UTF8_ID_LABEL                 "__!U8!_"
#define   UTF8_ID_LBL_LEN               6
#define   PARAM_ARRAY_NAME              "__ArrayParam_"
#define   USER_DEFINED_EXCEPTION        "user"
#define   WHITE_SPACES                  " \t\n\v\f\r"
#define   BAD_CODE                      "@!BAD!@"
//------------------------------------------
#define   DEBUG_MODE                    "DebugMode"
#define   DEBUG_QUESTION                "[mdebug]: "
#define   DEBUG_ANSWER                  "[mdebug]$ "
//------------------------------------------import struct
#define IMPORT_FILE                     1
#define IMPORT_PACKAGE                  2
#define IMPORT_EMBEDDED                 3
#define IMPORT_MODULE                   4
//------------------------------------------used as exit status for exit syscall
#define   EXIT_NORMAL                   0
#define   EXIT_ERROR                    1
#define   EXIT_FATAL                    2
//------------------------------------------used as exception status
#define   ERROR_ID                     -2
#define   WARNING_ID                   -3
#define   FATAL_ID                     -1
#define   CANCEL_ID                     0
//------------------------------------------used as instruction types
#define   UNKNOWN_LBL_INST              0
#define   STRUCTURE_LBL_INST            1
#define   DEF_VARS_LBL_INST             2
#define   ALLOC_VARS_LBL_INST           3
#define   FUNC_CALL_LBL_INST            4
#define   ALLOC_SHORT_LBL_INST          5
#define   RETURN_LBL_INST               6
#define   DEF_PACKAGE_LBL_INST          7
#define   NEXT_BREAK_LBL_INST           9

#define   LOGIC_CALC_LBL_INST           20
#define   REVIEW_ARRAY_LBL_INST         21
//------------------------------------------used as exception types (groups)
#define   ImportError                   0
#define   MahlibError                   1
#define   SyntaxError                   2
#define   InterruptedError              3
#define   ValueError                    4
#define   NotExistError                 5
#define   RuntimeError                  6
#define   IOError                       7
#define   NotAccessError                8
#define   OSError                       9
#define   OverflowError                 10
#define   TypeError                     11
#define   FloatingPointError            12
#define   CommandError                  13
#define   DebuggerError                 14
#define   BuilderError                  15
#define   UserDefinedError              100
//------------------------------------------
#define STDIN_SOURCE_PATH               0
//------------------------------------------
#define PRINT_IMPORT_ST                 1
#define PRINT_EXCEPTIONS_ST             2
#define PRINT_UTF8_ST                   3
#define PRINT_MAIN_SOURCE_ST            4
#define PRINT_TOKENS_SOURCE_ST          5
#define PRINT_FUNC_ST                   6
#define PRINT_STRUCT_ST                 7
#define PRINT_INSTRU_ST                 8
#define PRINT_STRU_ST                   9
#define PRINT_MAGIC_MACROS_ST           10
#define PRINT_STRUCT_DES_ST             11
#define PRINT_FUNCTIONS_STACK_ST        12
#define PRINT_STRUCTURES_STACK_ST       13
#define PRINT_CONDITION_LEVEL_ST        14
#define PRINT_LOOP_LEVEL_ST             15
//------------------------------------------
#define   BOOL_SUB_TYPE_ID              1
#define   STR_SUB_TYPE_ID               2
#define   UTF8_SUB_TYPE_ID              3
#define   INT_SUB_TYPE_ID               4
#define   FLOAT_SUB_TYPE_ID             5
#define   HUGE_SUB_TYPE_ID              6
//------------------------------------------used in parser-determine type of statements and block codes
#define   FUNC_BLOCK_ID                 1
#define   PACK_BLOCK_ID                 2
#define   LOOP_STRU_ID                  3
#define   IF_STRU_ID                    4
#define   ELIF_STRU_ID                  5
#define   ELSE_STRU_ID                  6
#define   MANAGE_STRU_ID                7
#define   CHOOSE_STRU_ID                8

//------------------------------------------used in data_defined,runtime-determine type data types that is basic or package
#define   BASIC_DATA_TYPE                1
#define   PACKAGE_DATA_TYPE              2
//------------------------------------------
#define   FAILED_EXECUTE_INSTRUCTION    1
#define   SUCCESS_EXECUTE_INSTRUCTION   2
//------------------------------------------used in APP_controller return
#define   BAD_RETURN_APP_CONTROLLER     0
#define   NORMAL_RETURN_APP_CONTROLLER  1
#define   STOP_RETURN_APP_CONTROLLER    2
#define   BREAK_RETURN_APP_CONTROLLER   3
//------------------------------------------used in built-in input,fread functions
#define   READ_CHAR_INPUT_TYPE          "1"
#define   READ_LINE_INPUT_TYPE          "2"
#define   READ_ALL_INPUT_TYPE           "3"
//------------------------------------------used for entry_table.next_break_inst
#define   NEXT_INST                     1
#define   BREAK_INST                    2
//------------------------------------------used for mahdi_modules_instance array
#define   EXCEPTION_MODULE_ID           2
#define   CONSOLE_MODULE_ID             3
#define   STRING_MODULE_ID              4
#define   MATH_MODULE_ID                5
#define   OS_MODULE_ID                  6
#define   BIT_MODULE_ID                 7
#define   APP_MODULE_ID                 8
#define   FILE_MODULE_ID                9
//------------------------------------------used for function call
#define   FUNC_TYPE_NORMAL              1
#define   FUNC_TYPE_MODULE              2
#define   FUNC_TYPE_PACKAGE             3
//**************************************************typedef
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
#if LINUX_PLATFORM == true && x64_OS == true
typedef __int64_t           int64;
typedef __uint64_t          uint64;
#elif LINUX_PLATFORM == true && x86_OS == true
typedef __int64_t           uint64;
typedef __uint64_t          int64;
#elif WINDOWS_PLATFORM == true && x64_OS == true
typedef unsigned long int   uint64;
typedef signed long int     int64;
#elif WINDOWS_PLATFORM == true && x86_OS == true
typedef uint64_t uint64;
typedef int64_t int64;
#else
typedef unsigned long long int uint64;
typedef signed long long int int64;
#endif

// typedef long int num_int;
// typedef double num_float;

typedef unsigned char* String;
typedef unsigned char** StrList;
typedef uint32** UStrList;
typedef uint32* UString;
typedef uint64 Longint;
typedef Longint* LintList;
typedef unsigned char Boolean;

#endif //MAHDI_CONSTANTS_H
