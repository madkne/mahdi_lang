//
// Created by madkne on 2020.1.5
//

#ifndef __MAHDI_MODULE_H
#define __MAHDI_MODULE_H  1
//******************************************constants
#define EXPORT                          __declspec(dllexport)
//******************************************typedef
#if WINDOWS_PLATFORM == true
//TODO:
// typedef uint32 (WINAPI *FUNCTYPE1)(void);
// typedef String (WINAPI *FUNCTYPE2)(String);
// typedef String (WINAPI *FUNCTYPE3)(int32);
// typedef Boolean (WINAPI *FUNCTYPE4)(String);
#elif LINUX_PLATFORM == true
typedef uint32 (*FUNCTYPE1)(void);
typedef uint32 (*FUNCTYPE2)(String);
typedef Boolean (*FUNCTYPE3)(uint32);
typedef Boolean (*FUNCTYPEINIT)(struct entry_table_struct *);
#endif
//******************************************functions
uint32 MODULE_return_module_id(String module_name);
String MODULE_return_mahdi_modules(String modname);
uint32 MODULE_return_modpack_id(String modpack_name);
Boolean MODULE_load_module(String path,StrList packs,uint32 packs_len, uint32 line, String src);
// Boolean MODULE_init_package_functions(uint32 modid,uint32 modpackid);

//******************************************call modules
/*uint32 func()*/
uint32 MODULE_call_type1(String func_name, uint32 mahdi_module_id); 
/*uint32 func(String)*/
uint32 MODULE_call_type2(String func_name, uint32 mahdi_module_id,String s);
/*Boolean func(uint32)*/
Boolean MODULE_call_type3(String func_name, uint32 mahdi_module_id,uint32 s);

#endif /* __MAHDI_MODULE_H */