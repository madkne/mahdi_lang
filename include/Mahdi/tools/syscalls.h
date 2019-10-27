//
// Created by madkne on 2019.10.26
//

#ifndef __MAHDI_SYSCALLS_H
#define __MAHDI_SYSCALLS_H  1
String CALL_hostname();
String CALL_homedir();
Longint CALL_pid();
Boolean CALL_is_directory(String path);
String CALL_parent_path(String path);
String CALL_abspath(String path);
Longint CALL_unix_time();
String CALL_datetime(uint8 type);
String CALL_stdin();
int32 CALL_read_file(String path, StrList *lines, Boolean skip_empty_lines);
Boolean CALL_write_file(String path, String s);
Boolean CALL__exist_file(String path);
String CALL_get_line(FILE *fp);
Boolean CALL_mkdir(String path, Boolean is_make_parents);
uint32 CALL_rand(uint32 min, uint32 max);
Boolean CALL_binary_copy(String src, String dst);
#if WINDOWS_PLATFORM == true
String CALL_reg_value(HKEY hkey, String path, String key);
#endif
#endif //__MAHDI_SYSCALLS_H
