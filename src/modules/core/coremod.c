#include "Modules/core/coremod.h"

struct entry_table_struct *MOD_CORE_TABLE=0;
//*********************************************
Boolean MOD_CORE_init(struct entry_table_struct *e){
    // printf("DDDD23:%i;%i\n",e,e->mpfu_id);
    MOD_CORE_TABLE=e;
    // printf("@@@@RSCR:%s,%s\n",entry_table.sources_list[0],e.sources_list[0]);
    return true;
}
//*********************************************
uint32 MOD_CORE_detect_package(String modpack_name){
    if(STR_equal(modpack_name,"console")) return CORE_CONSOLE_MODID;
    else if(STR_equal(modpack_name,"app")) return CORE_APP_MODID;
    else if(STR_equal(modpack_name,"os")) return CORE_OS_MODID;
    return 0;
}
//*********************************************
Boolean MOD_CORE_init_package(uint32 packid){
    uint32 loaded_count=0;
    if(packid==CORE_CONSOLE_MODID){
        return (MOD_CORE_CONSOLE_INIT()==0)?false:true;
    }
    //TODO:
    return false;
}