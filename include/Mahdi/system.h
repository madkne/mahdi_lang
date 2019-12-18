//
// Created by madkne on 2019.10.24
//

#ifndef __MAHDI_SYSTEM_H
#define __MAHDI_SYSTEM_H    1
//---------------------------------settings information
#include "settings.h"
//---------------------------------standard C functions
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>
//---------------------------------platform based C functions
#if LINUX_PLATFORM == 1
#include <linux/limits.h>
#include <stdint.h>
#include <errno.h>
#include <dlfcn.h>
#elif WINDOWS_PLATFORM == 1
#include <ctype.h>
#include <stdint.h>
#include <float.h>
#include <windows.h>

#endif
//---------------------------------basics informations
#include "constants.h"
#include "data_defined.h"
//---------------------------------normal functions
#include "exceptions.h"
#include "mahdi_builtin.h"
#include "mahdi_help.h"
// #include "debugger.h"
// #include "builder.h"
//---------------------------------core functions
#include "core/importer.h"
#include "core/parser.h"
#include "core/starter.h"

#include "core/runkit.h"
// #include "core/run_mgr.h"
#include "core/memory.h"
// #include "core/magic_macro.h"
//---------------------------------tools functions
#include "tools/common_funcs.h"
#include "tools/strings.h"
#include "tools/chars.h"
#include "tools/lists.h"
#include "tools/utf8.h"
#include "tools/syscalls.h"
// #include "tools/encoder.h"
#include "tools/console.h"
// //---------------------------------built-in functions
// #include "built_in/mpl_builtin.h"
// #include "built_in/os_builtin.h"
// #include "built_in/data_builtin.h"
// #include "built_in/fs_builtin.h"
// //---------------------------------modules
// #include "module.h"
// #include "modules/sqlite_interface.h"
#endif //__MAHDI_SYSTEM_H
