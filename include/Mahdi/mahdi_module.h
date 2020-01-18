//
// Created by madkne on 2020.1.5
//

#ifndef __MAHDI_MODULE_H
#define __MAHDI_MODULE_H  1
//******************************************constants
#define EXPORT                          __declspec(dllexport)
//******************************************typedef
/**
 * boolean=numberi
 * numberi func(string)                               : STR2NUMI [OK]
 * string func()                                      : NULL2STR
 * numberi func(numberi)                              : NUMI2NUMI [OK]
 * string func(numberi)                               : NUMI2STR
 * string func(numberi,numberi)                       : NUMI_NUMI2STR
 * string func(string)                                : STR2STR
 * numberi func()                                     : NULL2NUMI [OK]
 */ 
#if WINDOWS_PLATFORM == true
//TODO:
typedef uint32 (WINAPI *FUNCTYPE1)(void);
// typedef String (WINAPI *FUNCTYPE2)(String);
// typedef String (WINAPI *FUNCTYPE3)(int32);
// typedef Boolean (WINAPI *FUNCTYPE4)(String);
#elif LINUX_PLATFORM == true
typedef int32 (*NULL2NUMI)(void);
typedef int32 (*STR2NUMI)(String);
typedef int32 (*NUMI2NUMI)(int32);
typedef Boolean (*FUNCTYPEINIT)(struct entry_table_struct *);
#endif
//******************************************functions
int32 MODULE_call_function(uint32 mod_id,uint32 pack_id,String func_name,map *params_type,StrList params,uint32 params_len,StrList *rets_mod);
uint32 MODULE_call_by_adapter(String fname,uint32 mid,StrList params,mpfu func_struct);
String MODULE_generate_func_name(uint32 modid,uint32 packid,String funcname);
uint32 MODULE_return_module_id(String module_name);
String MODULE_return_mahdi_modules(String modname);
uint32 MODULE_return_modpack_id(String modpack_name,uint32 *modid);
Boolean MODULE_load_module(String path,StrList packs,uint32 packs_len, uint32 line, String src);
//******************************************call modules
/*numberi func()*/
 int32 MODULE_adapt_NULL2NUMI(String func_name, uint32 mahdi_module_id); 
/*numberi func(String)*/
int32 MODULE_adapt_STR2NUMI(String func_name, uint32 mahdi_module_id,String s);
/*numberi func(numberi)*/
int32 MODULE_adapt_NUMI2NUMI(String func_name, uint32 mahdi_module_id,int32 s);

#endif /* __MAHDI_MODULE_H */