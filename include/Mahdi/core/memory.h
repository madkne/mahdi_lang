//
// Created by madkne on 2019.10.27
//

#ifndef __MAHDI_MEMORY_H
#define __MAHDI_MEMORY_H  1

void VM_init();
void VM_show(uint8 wh);
Longint VM_set_var(rrss current, String name_var, String value_var, String type_var,Boolean is_static,Boolean is_private, Boolean is_create_var);
Longint VM_set_basictype_mpoint(String value,String type);
Longint VM_set_maporlist_mpoint(String value,String type);
//=>mvar functions
Longint _mvar_append(Longint pointer_id,Longint stru_id,Longint func_id,Longint func_index,Longint pack_id,Longint pack_index,Longint type_id,Boolean is_static,Boolean is_private,uint8 flag,String  name);
Mvar _mvar_get(Longint i);
void _mvar_delete(Longint i);
void _mvar_set_pointer_id(Longint i, Longint new_po);
Boolean _mvar_delete_by_id(Longint id);
Longint _mvar_find_index_by_id(Longint id);
Longint _mvar_find_index_by_pointer_id(Longint pointer_id);

//=>mpoint functions
uint32 _mpoint_hash_index(Longint id);
Longint _mpoint_append(uint8 type,String data);
void _mpoint_delete(Longint id);
Mpoint _mpoint_get(Longint id);
void _mpoint_edit(Longint id, String data, uint8 subtype, Boolean set_data, Boolean set_type);
Boolean _mpoint_delete_by_id(Longint id);

#endif //__MAHDI_MEMORY_H