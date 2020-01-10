
#include <Mahdi/system.h>

uint32 MODULE_return_module_id(String module_name) {
  if (STR_equal(module_name, "core")) return CORE_MODULE_ID;
  else if (STR_equal(module_name, "db")) return DB_MODULE_ID;
  return 0;
}
//**************************************************
/**
 * if module path is just name of module, then check if exist in mahdi/mods dir and return its path
 * @author madkne
 * @version 1.0
 * @since 2020.1.9
 * @param modname
 * @return String : absolute path of module
 */ 
String MODULE_return_mahdi_modules(String modname){
    //=>create absolute path by modname
    String modpath=STR_multi_append(COM_get_Mahdi_dir_path(),os_separator,"mods",os_separator,modname,LIB_EXTENSION);
    //=>check if exist module file in mahdi/mods dir
    if(CALL_exist_file(modpath)){
        return modpath;
    }
    //=>if not exist, return modname
    return modname;

}
//**************************************************
uint32 MODULE_return_modpack_id(String modpack_name) {
    //=>if core package is loaded
    if(mahdi_modules_instance[CORE_MODULE_ID]!=0){
        uint32 ret=MODULE_call_type2("MOD_CORE_detect_package",CORE_MODULE_ID,modpack_name);
        if(ret>0)return ret;
    }
    //TODO:the other module packages
  return 0;
}
//**************************************************
Boolean MODULE_load_module(String path,StrList packs,uint32 packs_len, uint32 line, String src) {
    //if module supported by mahdi
    String mod_name = COM_get_path_name_ext(path, 0, true);
    uint32 mahdi_module_id = MODULE_return_module_id(mod_name);
    if (mahdi_module_id == 0) {
        EXP_print_error(line, "not_support_module", src, mod_name, path, "MODULE_load_module");
        return false;
    }
    // printf("@@:%s,%s\n", path, mod_name);
    //=>if module not loaded
    if(mahdi_modules_instance[mahdi_module_id]==0){
        #if WINDOWS_PLATFORM == true
            //=>Get a handle to the .DLL module.
            HINSTANCE hinstLib = LoadLibrary(TEXT(path));
            //=>If the handle is not valid
            if (hinstLib == NULL) {
                EXP_print_error(line, "can_not_load_module", src, path, 0, "MODULE_load_module");
                return false;
            }
            //=>save module instance
            mahdi_modules_instance[mahdi_module_id] = hinstLib;
            SLIST_append(&installed_modules, path, installed_modules_len++);
        //-----------------------------------
        #elif LINUX_PLATFORM == true
            // Get a handle to the .SO module.
            void *handle= dlopen(path, RTLD_NOW | RTLD_GLOBAL);
            //=>If the handle is not valid
            if (!handle){
                EXP_print_error(line, "can_not_load_module", src, path, 0, "MODULE_load_module");
                // printf("The error is %s\n", dlerror());
                return false;
            }
            //save module instance
            mahdi_modules_instance[mahdi_module_id] = handle;
            SLIST_append(&installed_modules, path, installed_modules_len++);
        #endif
    }
    //=>call module init
    #if WINDOWS_PLATFORM == true
        //TODO:
        // FUNCTYPE1 Proc = (FUNCTYPE1) GetProcAddress(mahdi_modules_instance[mpl_module_id], "MOD_CORE_init");
        // if (NULL != Proc) return (uint32) Proc(entry_table);
    #elif LINUX_PLATFORM == 1
        FUNCTYPEINIT proc = dlsym(mahdi_modules_instance[mahdi_module_id], "MOD_CORE_init");
        if(proc !=NULL) (*proc)(&entry_table);
    #endif
    // printf("VVVVV:%s\n",SLIST_print(packs,packs_len));
    //=>init module package functions
    for (uint32 i = 0; i < packs_len; i++){
        //=>get module package id, if exist
        uint32 modpackid=MODULE_return_modpack_id(packs[i]);
        // printf("WWWEEW:%s=>%i\n",packs[i],modpackid);
        //=>if not exist package
        if(modpackid==0){
            //TODO:error
            printf("mod#465687\n");
            continue;
        }
        //=>init module package
        MODULE_call_type3("MOD_CORE_init_package",mahdi_module_id,modpackid);
        printf("DDFSFSFD12:%i,%i\n",entry_table.mpfu_start,entry_table.mpfu_id);
        // MODULE_call_type2(mahdi_module_id,entry_table);
        // MODULE_init_package_functions(mahdi_module_id,modpackid);
    }
    
    return true;
}

//**************************************************
Boolean MODULE_init_package_functions(uint32 modid,uint32 modpackid){
    //     // printf("init mod pack:%i,%i=>%i\n",modid, modpackid,mahdi_modules_instance[modid]);
    //     //=>init vars
    //     String func_name="MOD_";
    //     //=>if is core module
    //     if(modid==CORE_MODULE_ID){
    //         func_name=STR_append(func_name,"CORE_");
    //         //=>if is core console package
    //         if(modpackid==CORE_CONSOLE_MODID){
    //             func_name=STR_append(func_name,"CONSOLE_");
    //         }
    //         //=>if is core bit package
    //         else if(modpackid==CORE_BIT_MODID){
    //             func_name=STR_append(func_name,"BIT_");
    //         }
    //         //=>if is core app package
    //         else if(modpackid==CORE_APP_MODID){
    //             func_name=STR_append(func_name,"APP_");
    //         }
    //         //=>if is core file package
    //         else if(modpackid==CORE_FILE_MODID){
    //             func_name=STR_append(func_name,"FILE_");
    //         }
    //         //TODO:
    //     }
    //     //=>complete func name
    //     func_name=STR_append(func_name,"INIT");
    //     //TODO:
    //     //=>call init function by its name
    //     #if WINDOWS_PLATFORM == 1
    //         //TODO:
    //     #elif LINUX_PLATFORM == 1
    //         uint32 func_loaded=MODULE_call_type1(func_name,modid);
    //         //=>if func_loaded is zero
    //         if(func_loaded==0){
    //             //TODO:error
    //             printf("mod#465687\n");
    //             return false;
    //         }
    //     #endif
    //     return true;
}
//**************************************************
/**
 * call a module function like: uint32 func() and return a uint32
 */ 
uint32 MODULE_call_type1(String func_name, uint32 mahdi_module_id) {
  #if WINDOWS_PLATFORM == true
    FUNCTYPE1 Proc = (FUNCTYPE1) GetProcAddress(mahdi_modules_instance[mpl_module_id], func_name);
    if (NULL != Proc) return (uint32) Proc();
  #elif LINUX_PLATFORM == 1
    FUNCTYPE1 proc = dlsym(mahdi_modules_instance[mahdi_module_id], func_name);
    if(proc !=NULL) return (*proc)();
  #endif
  //TODO:error
  printf("M#ERR343:%s,%i\n", func_name, mahdi_module_id);
  return 0;
}
//**************************************************
/**
 * call a module function like: uint32 func(String) and return a uint32
 */ 
uint32 MODULE_call_type2(String func_name, uint32 mahdi_module_id,String s) {
  #if WINDOWS_PLATFORM == true
  //TODO:
    // FUNCTYPE1 Proc = (FUNCTYPE1) GetProcAddress(mahdi_modules_instance[mpl_module_id], func_name);
    // if (NULL != Proc) return (uint32) Proc();
  #elif LINUX_PLATFORM == 1
    FUNCTYPE2 proc = dlsym(mahdi_modules_instance[mahdi_module_id], func_name);
    if(proc !=NULL) return (*proc)(s);
  #endif
  //TODO:error
  printf("M#ERR34345:%s,%s,%i\n", func_name,s, mahdi_module_id);
  return 0;
}
//**************************************************
/**
 * call a module function like: Boolean func(uint32) and return a Boolean
 */ 
Boolean MODULE_call_type3(String func_name, uint32 mahdi_module_id,uint32 s) {
  #if WINDOWS_PLATFORM == true
  //TODO:
    // FUNCTYPE1 Proc = (FUNCTYPE1) GetProcAddress(mahdi_modules_instance[mpl_module_id], func_name);
    // if (NULL != Proc) return (uint32) Proc();
  #elif LINUX_PLATFORM == 1
    FUNCTYPE3 proc = dlsym(mahdi_modules_instance[mahdi_module_id], func_name);
    if(proc !=NULL) return (*proc)(s);
  #endif
  //TODO:error
  printf("M#ERR3434r5:%s,%i,%i\n", func_name,s, mahdi_module_id);
  return 0;
}