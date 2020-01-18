
#include <Mahdi/system.h>

//**************************************************
int32 MODULE_call_function(uint32 mod_id,uint32 pack_id,String func_name,map *params_type,StrList params,uint32 params_len,StrList *rets_mod) {
  //=>init vars
  uint32 rets_mod_len = 0;
  Longint mpfu_id=0;
  mpfu mod_func;
  int32 ret_func_response = 0;
  mpfu *st = entry_table.mpfu_start;
  StrList call_params=0;
  if (st == 0) {
    EXP_set_errcode(INVALID_MODULE_FUNC_ERRC);
    return 0;
  }
  //=>iterate mpfu nodes
  for(;;){
    //=>check for modid and modpackid and func_name
    if(st->mod_id==mod_id && st->pack_id==pack_id && STR_equal(st->func_name,func_name)){
      //=>check for params
      if(RUN_check_func_params(params_type,params,params_len,st->params_name,st->params_type,st->params_default,st->params_len)){
        mpfu_id=st->id;
        mod_func=*(st);
        break;
      }
    }
    st = st->next;
    if (st == 0) break;
  }
  //=>if not found function
  if(mpfu_id==0){
    //TODO:
    EXP_set_errcode(INVALID_MODULE_FUNC_ERRC);
    return 0;
  }
  printf("found module package function:%s\n",mod_func.func_name);
  //=>convert and normalize params to values
  map* argvs=RUN_func_params_to_values(params_type,params,params_len);
  //=>if is an error
  if(argvs==0 && EXP_check_errcode(BAD_FUNC_PARAMS_ERRC)){
    //TODO:
    EXP_set_errcode(INVALID_MODULE_FUNC_ERRC);
    return 0;
  }
  //=>generate function name
  String fname=MODULE_generate_func_name(mod_id,pack_id,func_name);
  //=>if func name is null
  if(fname==0){
    //TODO:
    EXP_set_errcode(INVALID_MODULE_FUNC_ERRC);
    return 0;
  }
  printf("func argvs:%s(%s)\n",fname,_map_print(argvs));
  //=>create params by params type,is_ref
  for (uint32 i = 0; i < mod_func.params_len; i++){
    // printf("call param:%s;%s;%i\n", mod_func.params_type[i],mod_func.params_default[i],mod_func.params_ref[i]);
    //=>if param is reference
    if(mod_func.params_ref[i]){
      //TODO:not default
    }
    //=>if param is value
    else{
      //=>get argv by param name
      String argv=_map_get(argvs,mod_func.params_name[i]);
      if(argv==0){
        //=>get argv by param index
        STR_init(&argv,_map_get(argvs,STR_from_int32(i)));
        //=>if not argv not found,get default value
        if(argv==0){
          STR_init(&argv,mod_func.params_default[i]);
        }
      }
      //=>append argv to call_params
      SLIST_append(&call_params,argv,i);
    }
  }
  //=>call function by returns types and params type
  MODULE_call_by_adapter(fname,mod_id,call_params,mod_func);
  //TODO:
  //-----------------------return
  if (ret_func_response > 0)rets_mod_len = ret_func_response;
  return rets_mod_len;
}
//**************************************************
uint32 MODULE_call_by_adapter(String fname,uint32 mid,StrList params,mpfu func_struct){
  //=>init vars
  String adp_type=0;
  
  //=>generate adapter params type
  for (uint32 i = 0; i < func_struct.params_len; i++){
    if(STR_equal(func_struct.params_type[i],"string")){
      adp_type=STR_append(adp_type,"STR");
    }else if(STR_equal(func_struct.params_type[i],"boolean") || STR_equal(func_struct.params_type[i],"numberi")){
      adp_type=STR_append(adp_type,"NUMI");
    }else if(STR_equal(func_struct.params_type[i],"numberf")){
      adp_type=STR_append(adp_type,"NUMF");
    }else if(STR_equal(func_struct.params_type[i],"numberh")){
      adp_type=STR_append(adp_type,"NUMH");
    }
    if(i+1<func_struct.params_len)adp_type=CH_append(adp_type,'_');
  }
  //=>if not have any params
  if(adp_type==0)adp_type=STR_append(adp_type,"NULL");
  adp_type=CH_append(adp_type,'2');
  //=>generate adapter returns type
  for (uint32 i = 0; i < func_struct.returns_len; i++){
    if(STR_equal(func_struct.returns_type[i],"string")){
      adp_type=STR_append(adp_type,"STR");
    }else if(STR_equal(func_struct.returns_type[i],"boolean") || STR_equal(func_struct.returns_type[i],"numberi")){
      adp_type=STR_append(adp_type,"NUMI");
    }else if(STR_equal(func_struct.returns_type[i],"numberf")){
      adp_type=STR_append(adp_type,"NUMF");
    }else if(STR_equal(func_struct.returns_type[i],"numberh")){
      adp_type=STR_append(adp_type,"NUMH");
    }
    if(i+1<func_struct.returns_len)adp_type=CH_append(adp_type,'_');
  }
  //=>if not have any returns
  if(func_struct.returns_len==0)adp_type=STR_append(adp_type,"NULL");
  printf("call adp:%s(%s)=>%s\n",fname,SLIST_print(params,func_struct.params_len),adp_type);
  //=>find best adapter by adp_type
  if(STR_equal(adp_type,"NULL2NUMI")){
    //TODO:
  }
  else if(STR_equal(adp_type,"STR2NUMI")){
    MODULE_adapt_STR2NUMI(fname,mid,params[0]);
  }
  else if(STR_equal(adp_type,"NUMI2NUMI")){
    //TODO:
  }
  // if(mod_func.returns_len==1){
  //   if(mod_func.params_len==0){
  //     //TODO:
  //   }else if(mod_func.params_len==1){
  //     //=>numberi func(string) OR boolean func(string)
  //     if(STR_equal(mod_func.params_type[0],"string") && (STR_equal(mod_func.returns_type[0],"numberi") || STR_equal(mod_func.returns_type[0],"boolean"))){
  //       MODULE_call_type110(fname,mod_id,_map_get(argvs,"0"));
  //     }
  //   }
  //   //TODO:
  // }
  return 0;
}
//**************************************************
String MODULE_generate_func_name(uint32 modid,uint32 packid,String funcname){
  String fname=0;
  switch (modid){
    case CORE_MODULE_ID:
      STR_init(&fname,"MOD_CORE_");
      switch (packid){
      case CORE_CONSOLE_MODID:
        fname=STR_append(fname,"CONSOLE_");
        break;
      //TODO:
      default: return 0;
      }
      break;
  //TODO:
  default: return 0;
  }
  //=>append func name
  fname=STR_append(fname,funcname);
  return fname;
}
//**************************************************
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
/**
 * get a module package name like 'console', search it to all loaded modules and return module id and module pack id 
 * @author madkne
 * @version 1.0
 * @param modpack_name
 * @param modid : (pointer)
 * @return uint32
 */ 
uint32 MODULE_return_modpack_id(String modpack_name,uint32 *modid) {
  //=>if core package is loaded
  if(mahdi_modules_instance[CORE_MODULE_ID]!=0){
      uint32 ret=MODULE_adapt_STR2NUMI("MOD_CORE_detect_package",CORE_MODULE_ID,modpack_name);
      if(ret>0){
        if(modid!=0) *(modid)=CORE_MODULE_ID;
        return ret;
      }
  }
  //TODO:the other module packages
  if(modid!=0) *(modid)=0;
  return 0;
}
//**************************************************
Boolean MODULE_load_module(String path,StrList packs,uint32 packs_len, uint32 line, String src) {
    //if module supported by mahdi
    String mod_name = COM_get_path_name_ext(path, 0, true);
    uint32 mahdi_module_id = MODULE_return_module_id(mod_name);
    if (mahdi_module_id == 0) {
        EXP_print_error(line, "not_support_module", src, mod_name, path,"MODULE_load_module");
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
        uint32 modpackid=MODULE_return_modpack_id(packs[i],0);
        // printf("WWWEEW:%s=>%i\n",packs[i],modpackid);
        //=>if not exist package
        if(modpackid==0){
            //TODO:error
            printf("mod#465687\n");
            continue;
        }
        //=>init module package
        MODULE_adapt_NUMI2NUMI("MOD_CORE_init_package",mahdi_module_id,modpackid);
    }
    
    return true;
}

//**************************************************
/**
 * call a module function like: numberi func() and return a int32
 */ 
int32 MODULE_adapt_NULL2NUMI(String func_name, uint32 mahdi_module_id) {
  #if WINDOWS_PLATFORM == true
    FUNCTYPE1 Proc = (FUNCTYPE1) GetProcAddress(mahdi_modules_instance[mpl_module_id], func_name);
    if (NULL != Proc) return (uint32) Proc();
  #elif LINUX_PLATFORM == 1
    NULL2NUMI proc = dlsym(mahdi_modules_instance[mahdi_module_id], func_name);
    if(proc !=NULL) return (*proc)();
  #endif
  //TODO:error
  printf("M#ERR343:%s,%i\n", func_name, mahdi_module_id);
  return 0;
}
//**************************************************
/**
 * call a module function like: numberi func(String) and return a int32
 */ 
int32 MODULE_adapt_STR2NUMI(String func_name, uint32 mahdi_module_id,String s) {
  #if WINDOWS_PLATFORM == true
  //TODO:
    // FUNCTYPE1 Proc = (FUNCTYPE1) GetProcAddress(mahdi_modules_instance[mpl_module_id], func_name);
    // if (NULL != Proc) return (uint32) Proc();
  #elif LINUX_PLATFORM == 1
    STR2NUMI proc = dlsym(mahdi_modules_instance[mahdi_module_id], func_name);
    if(proc !=NULL) return (*proc)(s);
  #endif
  //TODO:error
  printf("M#ERR34345:%s,%s,%i\n", func_name,s, mahdi_module_id);
  return 0;
}
//**************************************************
/**
 * call a module function like: numberi func(numberi) and return a int32
 */ 
int32 MODULE_adapt_NUMI2NUMI(String func_name, uint32 mahdi_module_id,int32 s) {
  #if WINDOWS_PLATFORM == true
  //TODO:
    // FUNCTYPE1 Proc = (FUNCTYPE1) GetProcAddress(mahdi_modules_instance[mpl_module_id], func_name);
    // if (NULL != Proc) return (uint32) Proc();
  #elif LINUX_PLATFORM == 1
    NUMI2NUMI proc = dlsym(mahdi_modules_instance[mahdi_module_id], func_name);
    if(proc !=NULL) return (*proc)(s);
  #endif
  //TODO:error
  printf("M#ERR3434rs5:%s,%i,%i\n", func_name,s, mahdi_module_id);
  return 0;
}