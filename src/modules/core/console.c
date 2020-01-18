
#include "Modules/core/coremod.h"
//************************************************
uint32 MOD_CORE_CONSOLE_INIT(){
    //=>assign entry_table
    entry_table=*MOD_CORE_TABLE;
    //=>init vars
    uint32 lcount=0;
    //=>append console functions to mpfu
    if(_mpfu_append(CORE_MODULE_ID,CORE_CONSOLE_MODID,"print",true,"msg","string","","0",1,"boolean",1))lcount++;
    if(_mpfu_append(CORE_MODULE_ID,CORE_CONSOLE_MODID,"printLn",true,"msg","string","","0",1,"boolean",1))lcount++;
    //=>update entry table indexes
    MOD_CORE_TABLE->mpfu_id=entry_table.mpfu_id;
    MOD_CORE_TABLE->mpfu_start=entry_table.mpfu_start;
    MOD_CORE_TABLE->mpfu_end=entry_table.mpfu_end;
    return lcount;
}
//************************************************
Boolean MOD_CORE_CONSOLE_print(String msg){
    printf(COM_replace_ctrl_chars(msg));
    return true;
}
//************************************************
Boolean MOD_CORE_CONSOLE_printLn(String msg){
    Boolean ret=MOD_CORE_CONSOLE_print(msg);
    printf("\n");
    return ret;
}