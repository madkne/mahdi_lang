//
// Created by madkne on 2019.10.29
//

#ifndef __MAHDI_IMPORTER_H
#define __MAHDI_IMPORTER_H  1

void IMPORT_init();
Boolean IMPORT_run();
Boolean IMPORT_open_file(imin s);

//=>imin functions
Longint _imin_append(uint8 type,String name,String path,StrList packages,StrList functions,uint32 pack_len,uint32 func_len,uint32 line,uint32 source_index);
imin _imin_get(Longint id);
imin _imin_get_first_active();
void _imin_set_inactive(Longint id);

#endif /*__MAHDI_IMPORTER_H*/