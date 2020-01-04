#include <Mahdi/system.h>
//**********************************************************
String RUN_define_vars(String inst) {
    //=>init vars
    defvar vars_store[MAX_VAR_ALLOC_INSTRUCTIONS];
    StrList vars=0;
    Boolean is_static=false;
    //=>reset errcode
    EXP_clear_errcode();
    //=>simplify define vars
    uint32 vars_count=RUNKIT_simplify_define_vars(inst,&vars,&is_static);
    //=>check for errors
    if(vars_count==0 || EXP_check_errcode(BAD_DEFINE_VARS_ERRC)){
        return 0;
    }
    //=>analyzing define vars
    uint8 store_count=RUNKIT_defvars_analyzing(vars,vars_count,vars_store,false,false);
    //=>check for errors
    if(store_count==0 || EXP_check_errcode(BAD_DEFINE_VARS_ERRC)){
        return 0;
    }
    //=>add vars store to memory
    for (uint8 i = 0; i < store_count; i++) {
        printf("DEF_VAR:%s;;%s;;%s\n",vars_store[i].name_var,vars_store[i].type_var,vars_store[i].value_var);
        //=>register var in memory (default is public var)
        Longint ret1 = VM_set_var(entry_table.current, vars_store[i].name_var, vars_store[i].value_var, vars_store[i].type_var,is_static,false, true);
        if (ret1 == 0) {
            EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
            return 0;
        }
    }
  
    //show_memory(0);
    return 0;
}