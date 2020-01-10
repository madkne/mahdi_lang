//
// Created by madkne on 2020.1.10
//

#ifndef __MODULES_CORE_H
#define __MODULES_CORE_H  1

#include <Mahdi/system.h>
#include "console.h"

#define MOD_CORE_VERSION    "0.10"

//------------------------------------------used for detect core module packages
#define   CORE_CONSOLE_MODID             1
#define   CORE_APP_MODID                 3
#define   CORE_OS_MODID                  5

struct entry_table_struct *MOD_CORE_TABLE;
//------------------------------------------core module global functions
Boolean MOD_CORE_init(struct entry_table_struct *e);
uint32 MOD_CORE_detect_package(String modpack_name);
Boolean MOD_CORE_init_package(uint32 packid);


#endif /* __MODULES_CORE_H */