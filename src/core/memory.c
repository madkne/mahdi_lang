

#include <Mahdi/system.h>

void VM_init()
{
    //=>init global memory vars states
    entry_table.var_memory_start = 0;
    //entry_table.pointer_memory_start = 0;
    entry_table.var_mem_id = 1;
    entry_table.pointer_mem_id = 5;
    entry_table.var_mem_len = 0;
    entry_table.pointer_mem_len = 0;
    //=>append first entry to mvar as null
    _mvar_append(0, 0, 0, 0, 0, 0, 0, 0, false, false, 0);
    //=>append first entry to mvar as null
    _mpoint_append('0', 0);
    //tmp return pointer vars
    //   Mpoint tmp3 = {RETURN_TMP_POINTER_ID, 0, 'v'};
    //   append_Mpoint(tmp3);
}

//****************************************************
/**
 * display Mvar,Mpoint or both in console by details based on wh
 * @param wh {uint8}
 * @author madkne
 * @version 1.0
 */
void VM_show(uint8 wh)
{
    // if (wh == 40){
    //     wh = 0;
    // }
    //=>if not is programmer debug mode, then return
    if (is_programmer_debug == 0)
    {
        return;
    }
    //*************show var_memory
    printf("-------------------------\n");
    if (wh == 0 || wh == 1 || wh == 3)
    {
        printf("*****var_memory*****\n");
        for (Longint i = 0; i < entry_table.var_mem_len; i++)
        {
            Mvar st = _mvar_get(i);
            if (wh != 3 && st.name == 0)
            {
                continue;
            }
            printf("%i::VAR(id:%i,[fid:%i,fin:%i],[pid:%i,pin:%i],sid:%i,pointer:%i,type:%i)\n\t%s,{flag:%c,static:%i,private:%i}\n=====================\n", i, st.id, st.func_id, st.func_index, st.pack_id, st.pack_index, st.stru_id, st.pointer_id, st.type_id, st.name, st.flag, st.is_static, st.is_private);
        }
    }
    //*************show pointer_memory
    if (wh == 0 || wh == 2)
    {
        printf("*****pointer_memory*****\n");
        for (uint32 i = 0; i < HASH_MEM_SIZE; i++)
        {
            if (hash_pointers[i] == 0)
                continue;
            Mpoint *tmp1 = hash_pointers[i];
            for (;;)
            {
                printf("%i::POINTER(id:%i,Type:%c)%.50s;\n", i, (*tmp1).id, (*tmp1).type, (*tmp1).data);
                tmp1 = tmp1->next;
                if (tmp1 == 0)
                    break;
            }
        }
    }
    printf("-------------------------\n");
}
//*************************************************************
//***********************mvar functions************************
//*************************************************************

/**
 * get all attributes of mvar except id, and create an entry in mvar linkedlist
 * @return success var_id {Longint} last entry id that added to mvar
 * @return faild 0
 * @author madkne
 * @version 1.0
 */
Longint _mvar_append(Longint pointer_id, Longint stru_id, Longint func_id, Longint func_index, Longint pack_id, Longint pack_index, Longint type_id, Boolean is_static, Boolean is_private, uint8 flag, String name)
{
    Mvar *q;
    q = (Mvar *)malloc(sizeof(Mvar));
    if (q == 0)
        return 0;
    q->id = ++entry_table.var_mem_id;
    q->pointer_id = pointer_id;
    q->stru_id = stru_id;
    q->func_id = func_id;
    q->func_index = func_index;
    q->pack_id = pack_id;
    q->pack_index = pack_index;
    q->type_id = type_id;
    q->is_static = is_static;
    q->is_private = is_private;
    q->flag = flag;
    STR_init(&q->name, name);
    q->next = 0;
    entry_table.var_mem_len++;
    if (entry_table.var_memory_start == 0)
        entry_table.var_memory_start = entry_table.var_memory_end = q;
    else
    {
        entry_table.var_memory_end->next = q;
        entry_table.var_memory_end = q;
    }
    return entry_table.var_mem_id;
}
//****************************************************
Mvar _mvar_get(Longint i)
{
    Mvar null = {0, 0, 0, 0, 0, 0, 0, 0, false, false, 0, 0, 0};
    Longint counter = 0;
    Mvar *tmp1 = entry_table.var_memory_start;
    for (;;)
    {
        if (i == counter)
            return (*tmp1);
        tmp1 = tmp1->next;
        counter++;
        if (tmp1 == 0)
            break;
    }
    return null;
}
//****************************************************
/**
 * get an index of mvar linkedlist and remove indexed node of mvar and decrease mvar length
 * @param i {Longint} : index of mvar linkedlist
 * @author madkne
 * @version 1.0
 */
void _mvar_delete(Longint i)
{
    Longint counter = 0;
    Mvar *first = 0;
    Mvar *last = entry_table.var_memory_start;
    for (;;)
    {
        if (counter == i)
        {
            Mvar *next = last->next;
            free(last);
            if (i == 0)
                entry_table.var_memory_start = next; //if was first node
            else
                first->next = next;
            if (next == 0)
                entry_table.var_memory_end = first; //if was last node
            break;
        }
        first = last;
        last = last->next;
        if (last == 0)
            break;
        counter++;
    }
    entry_table.var_mem_len--;
}
//****************************************************
void _mvar_set_pointer_id(Longint i, Longint new_po)
{
    Longint counter = 0;
    Mvar *tmp1 = entry_table.var_memory_start;
    for (;;)
    {
        if (i == counter)
        {
            (*tmp1).pointer_id = new_po;
            return;
        }
        tmp1 = tmp1->next;
        counter++;
        if (tmp1 == 0)
            break;
    }
}
//**************************************************************
/**
 * delete a Mvar node by its id
 * @param id {Longint}
 * @return Boolean
 * @author madkne
 * @version 1.0
 */
Boolean _mvar_delete_by_id(Longint id)
{
    for (Longint i = 0; i < entry_table.var_mem_len; i++)
    {
        if (_mvar_get(i).id == id)
        {
            _mvar_delete(i);
        }
    }
    return true;
}

//**************************************************************
/**
 * get an id and search for index it in Mvar
 * @param id {Longint}
 * @return success index {Longint}
 * @return faild 0 
 * @author madkne
 * @version 1.0
 */
Longint _mvar_find_index_by_id(Longint id)
{
    if (id == 0)
        return 0;
    for (Longint i = 0; i < entry_table.var_mem_len; i++)
    {
        if (_mvar_get(i).id == id)
        {
            return i;
        }
    }
    return 0;
}

//****************************************************
/**
 * get a pointer_id of var and return an index in Mvar
 * @param pointer_id {Longint}
 * @return success index {Longint}
 * @return faild 0 
 * @author madkne
 * @version 1.0
 */
Longint _mvar_find_index_by_pointer_id(Longint pointer_id)
{
    for (Longint i = 0; i < entry_table.var_mem_len; i++)
    {
        if (_mvar_get(i).pointer_id == pointer_id)
        {
            return i;
        }
    }
    return 0;
}
//*************************************************************
//**********************mpoint functions***********************
//*************************************************************
uint32 _mpoint_hash_index(Longint id)
{
    return (uint32)(id % HASH_MEM_SIZE);
}
//****************************************************
Longint _mpoint_append(uint8 type, String data)
{
    //create Mpoint struct
    Mpoint *q;
    q = (Mpoint *)malloc(sizeof(Mpoint));
    if (q == 0)
        return 0;
    q->id = ++entry_table.pointer_mem_id;
    q->type = type;
    STR_init(&q->data, data);
    q->next = 0;
    //get hash_pointers index
    uint32 hash_index = _mpoint_hash_index(q->id);
    //append to hash_pointers entry
    entry_table.pointer_mem_len++;
    if (hash_pointers[hash_index] == 0)
        hash_pointers[hash_index] = hash_pointers_end[hash_index] = q;
    else
    {
        hash_pointers_end[hash_index]->next = q;
        hash_pointers_end[hash_index] = q;
    }
    return entry_table.pointer_mem_id;
}

//****************************************************
void _mpoint_delete(Longint id)
{
    Mpoint *first = 0;
    uint32 hash_index = _mpoint_hash_index(id);
    Mpoint *last = hash_pointers[hash_index];
    uint32 counter = 0;
    for (;;)
    {
        if (last->id == id)
        {
            Mpoint *next = last->next;
            free(last);
            if (counter == 0 || first == 0)
                hash_pointers[hash_index] = next; //if was first node
            else
                first->next = next;
            if (next == 0)
                hash_pointers_end[hash_index] = first; //if was last node
            break;
        }
        first = last;
        last = last->next;
        if (last == 0)
            break;
        counter++;
    }
    entry_table.pointer_mem_len--;
}

//****************************************************
Mpoint _mpoint_get(Longint id)
{
    Mpoint null = {0, 0, 0, 0};
    uint32 hash_index = _mpoint_hash_index(id);
    Mpoint *tmp1 = hash_pointers[hash_index];
    //printf("!!!!!!:%i,%i,%i\n",id,hash_index,hash_pointers[hash_index]);
    if (tmp1 == 0)
        return null;
    for (;;)
    {
        if (id == tmp1->id)
            return (*tmp1);
        tmp1 = tmp1->next;
        if (tmp1 == 0)
            break;
    }
    return null;
}
//****************************************************
void _mpoint_edit(Longint id, String data, uint8 subtype, Boolean set_data, Boolean set_type)
{
    uint32 hash_index = _mpoint_hash_index(id);
    Mpoint *tmp1 = hash_pointers[hash_index];
    for (;;)
    {
        if (id == tmp1->id)
        {
            if (set_data)
                STR_init(&(*tmp1).data, data);
            if (set_type)
                (*tmp1).type = subtype;
            return;
        }
        tmp1 = tmp1->next;
        if (tmp1 == 0)
            break;
    }
}
//**************************************************************
/**
 * delete a Mpoint node by its id
 * @param id {Longint}
 * @return Boolean
 * @author madkne
 * @version 1.0
 */
Boolean _mpoint_delete_by_id(Longint id)
{
    for (Longint i = 0; i < entry_table.pointer_mem_len; i++)
    {
        if (_mpoint_get(i).id == id)
        {
            _mpoint_delete(i);
        }
    }
    return true;
}
//****************************************************
// Boolean append_Mpoint_pointer(Longint id, Longint new_pointer) {
    //TODO:
//   Mpoint pointers = get_Mpoint(id);
//   if (pointers.type_data != 'p' && pointers.type_data != 'l')return false;
//   String new_po = str_from_long_int(new_pointer);
//   if (pointers.data == 0)pointers.data = new_po;
//   else {
//     pointers.data = char_append(pointers.data, ';');
//     pointers.data = str_append(pointers.data, new_po);
//   }
//   edit_Mpoint(id, pointers.data, pointers.type_data, true, false);
//   return true;
// }

//****************************************************
// Boolean change_Mvar_flag(Longint i, uint8 flag) {
    //TODO:
//   Longint counter = 0;
//   Mvar *tmp1 = entry_table.var_memory_start;
//   for (;;) {
//     if (i == counter) {
//       (*tmp1).flag = flag;
//       return true;
//     }
//     tmp1 = tmp1->next;
//     counter++;
//     if (tmp1 == 0) break;
//   }
//   return false;
// }
//****************************************************
/**
 * get a name(and index) of var and return a Mpoint struct of var by its index
 * defualt of index is 0
 * @param var_name
 * @return Mpoint
 */
// Mpoint return_var_memory_value(String var_name) {
    //TODO:
//   /**
//   1- s[1,9]
//   2- st1
//   Samples:
//   1- num nu=d1[0,1]+d1[1,0]
//   2- str st2={"Amin",st1[0]*2+"!"}
//   */
//   //msg("&VVVVV:", var_name)
//   //**************************define variables
//   String var_index = "0";
//   uint8 type_var = '0';
//   Longint pointer_id = 0;
//   Mpoint failed = {0, 0, '0', 0};
//   //var tmp_name = var_name
//   //**************************start analyzing
//   //**************************get name and index
//   return_name_index_var(var_name, false, &var_name, &var_index);
//   //**************************get value of var
//   //----------------search for variable
//   Longint tmpx = return_var_id(var_name, 0);
//   Longint real_id = find_index_var_memory(tmpx);
//   //msg("&XXXXX:", var_name, var_index, real_id)
//   //show_memory(0)
//   if (real_id == 0) {
//     //msg("CXCXC:", var_name, "=>", value_var)
//     return failed;
//   }

//   pointer_id = get_Mvar(real_id).pointer_id;
//   //msg("III:", pointer_id)
//   //----------------simplification var_index
//   var_index = simplification_var_index(var_index);
//   //----------------search for data in index
//   Longint data_id = get_data_memory_id(pointer_id, var_index);
//   //**************************return
// //  printf("CXCXC:%s[%s]=>(%i)%s\n", var_name,var_index,data_ind, get_Mpoint(data_ind).data);
//   return get_Mpoint(data_id);
// }
//****************************************************
/**
 * get a name(and index) of var and return a Mpoint struct of var by its index
 * defualt of index is empty
 * @param var_name
 * @return Mpoint
 */
// Mpoint __return_var_memory_value(String var_name) {
    //TODO:
//   /**
//   1- s[1,9]
//   2- st1
//   Samples:
//   1- num nu=d1[0,1]+d1[1,0]
//   2- str st2={"Amin",st1[0]*2+"!"}
//   */
//   //**************************define variables
//   String var_index = 0;
//   uint8 type_var = '0';
//   Longint pointer_id = 0;
//   Mpoint failed = {0, 0, '0', 0};
//   //**************************start analyzing
//   //**************************get name and index
//   return_name_index_var(var_name, true, &var_name, &var_index);
//   //**************************get value of var
//   //----------------search for variable
//   Longint tmpx = return_var_id(var_name, 0);
//   Longint real_id = find_index_var_memory(tmpx);
//   if (real_id == 0) {
//     return failed;
//   }
//   pointer_id = get_Mvar(real_id).pointer_id;
//   //----------------simplification var_index
//   var_index = simplification_var_index(var_index);
//   //----------------search for data in index
//   Longint data_id = get_data_memory_id(pointer_id, var_index);
//   //**************************return
// //  printf("CXCXC:%s[%s]=>(%i)%s\n", var_name,var_index,data_ind, get_Mpoint(data_ind).data);
//   return get_Mpoint(data_id);
// }
//****************************************************

/**
 * get a pointer_id of var and index of var and return an id of Mpoint
 * @param pointer_id
 * @param index_var (atomic index numbers)
 * @return Longint
 */
// Longint get_data_memory_id(Longint pointer_id, String index_var) {
    //TODO:
//   //**************************define variables
//   Longint ret_index = 0;
//   Longint basic_pointer = pointer_id;
//   //**************************start analyzing
//   //msg("&III:", pointer_id, index_var)
//   //show_memory(40)
//   //NOTE:do not simplification index here!!!
//   //----------------if not even index
//   if (index_var == 0) return pointer_id;
//   //----------------if pointer_id is zero
//   if (pointer_id == 0)return 0;
//   //----------------search for data in index
//   str_list tmp1 = 0;
//   uint32 tmp1_len = char_split(index_var, ',', &tmp1, true);
//   for (uint32 i = 0; i < tmp1_len + 1; i++) {
//     if (pointer_id == 0) {
//       return ret_index;
//     }
//     //msg("RTE:", pointer_memory[po_ind], po_ind)
//     uint32 user_ind = 0;
//     if (i < tmp1_len) {
//       user_ind = str_to_int32(tmp1[i]);
//     }
//     Mpoint mpoint = get_Mpoint(pointer_id);
//     if (mpoint.type_data == 'p') {
//       str_list tmp2 = 0;
//       uint32 tmp2_len = char_split(mpoint.data, ';', &tmp2, true);
//       if (user_ind >= tmp2_len || user_ind < 0) {
//         String var_name = get_Mvar(return_var_ind_pointer_id(basic_pointer)).name;
//         if (var_name == 0)STR_init(&var_name, "undefined");
//         exception_handler("out_of_range_index", __func__, index_var, var_name);
// //        printf("##EEEE:%i,%s,%s\n",basic_pointer,get_Mvar(return_var_ind_pointer_id(basic_pointer)).name,index_var);
//         return 0;
//       }
//       pointer_id = str_to_long_int(tmp2[user_ind]);
//     } else if (mpoint.type_data != 'v' && mpoint.type_data != 'p' || mpoint.type_data != '0') {
//       //msg("&YY", i, tmp1, len(tmp1), index_var)
//       if (i < tmp1_len) {
//         String var_name = 0;
//         STR_init(&var_name, get_Mvar(return_var_ind_pointer_id(basic_pointer)).name);
//         exception_handler("out_of_range_index", __func__, index_var, var_name);
//         return 0;
//       }
//       return pointer_id;
//       //msg("CCXXXX:", user_ind, pointer_memory[po_ind].data_ids, data_ind)
//     }
//   }
//   return 0;
// }

//****************************************************
/**
 * get name of new var and its value , type and its func_index,stru_index. finally create new var on VM and return id of Mvar
 * @param fin
 * @param sid
 * @param name
 * @param value_var
 * @param type_var
 * @param is_create_var
 * @return Longint
 */
// Longint
// set_memory_var(Longint fin, Longint sid, String name, String value_var, String type_var, Boolean is_create_var) {
    //TODO:
//   /**
//   1- str st1=fi.name
//   2- str st2=fg //fg={"sd","ER"} ---OK---
//   3- str r=er ---OK---
//   4- str u="RT"*2 ---OK---
//   5- num j=j1 //num j1=56d => j=56d
//   */
// //  printf("&@@@:%s,%i,%s(%i)\n", name, return_var_id(name, "0"), value_var,str_length(value_var));
// //  *******************************************if value is a variable
//   if (is_valid_name(value_var, true)) {
//     String al_name = 0, al_index = 0;
//     return_name_index_var(value_var, true, &al_name, &al_index);
// //    printf("&!!!:%s,%s\n", value_var, al_index);
//     //----------if is a variable
//     if (al_index == 0) {
//       //msg("&NNNN", value_var, return_var_id(value_var, "0"), cur_fin)
//       Longint ret0 = return_var_id(value_var, "0");
//       if (ret0 == 0) {
//         exception_handler("not_exist_var", __func__, value_var, "");
//         return 0;
//       }
//       //show_memory(40)
//       value_var = return_value_var_complete(find_index_var_memory(ret0));
//       //msg("&NNN+++",value_var,find_index_var_memory(return_var_id(value_var, "0")))
//     }
//     //----------if is a data variable
//     if (al_index != 0) {
//       //msg("&UUU")
//       uint8 ty = '0';
//       Mpoint point = return_var_memory_value(value_var);
//       value_var = point.data;
//       if (point.type_data == 's') {
//         value_var = convert_to_string(value_var);
//       } else if (point.type_data == 'p' || point.type_data == '0') {
//         //TODO:error
//         printf("#ERR9877\n");
//         return 0;
//       }
//     }
//   }
//   //msg("&IIIII", value_var)
//   //*******************************************define variables
//   value_var = str_trim_space(value_var);
//   vaar_en vals_array = {0, 0, 0};
//   //int32 set_indexes[MAX_ARRAY_DIMENSIONS + 1];
//   int32 max_indexes[MAX_ARRAY_DIMENSIONS];//get user indexes
//   max_indexes[0] = 0;
//   Boolean is_multi_array = false, is_struct = false;
//   uint8 indexes_len = 0;//count of user indexes dimensions
//   //*******************************************if type is a struct
//   if (search_datas(type_var, entry_table.cur_fid, false).id != 0 &&
//       !str_search(basic_types, type_var, StrArraySize(basic_types)) && !str_equal(type_var, "vars")) {
//     is_struct = true;
//   }
//   //*******************************************analyzing name of variable
//   String tmp_name = 0, tmp_ind = 0;
//   return_name_index_var(name, true, &tmp_name, &tmp_ind);
// //  printf("CCCCCCC:%s=>%s,%s\n",name,tmp_name, tmp_ind);
//   //is_multi array
//   if (tmp_ind != 0 && !str_ch_equal(tmp_ind, '0')) {
//     Boolean has_unknown_index = false;
//     is_multi_array = true;
//     str_list indexes;
//     indexes_len = (uint8) char_split(tmp_ind, ',', &indexes, true);
//     if (indexes_len > MAX_ARRAY_DIMENSIONS) {
//       //TODO:error
//       printf("#ERR32\n");
//       return 0;
//     }
//     //******************add indexes to max_indexes
//     for (uint32 i = 0; i < indexes_len; i++) {
//       if (str_equal(indexes[i], "?")) {
//         has_unknown_index = true;
//         max_indexes[i] = -10;
//       } else max_indexes[i] = str_to_int32(indexes[i]);
//     }
//     //******************if has ? index
//     if (has_unknown_index) {
//       if (value_var == 0) {
//         //TODO:error
//         printf("VM#ERR52\n");
//       }

//       if (str_equal(value_var, "null")) {
//         for (uint32 i = 0; i < indexes_len; i++)
//           if (max_indexes[i] == -10)max_indexes[i] = 1;
//       } else {
//         int32 tmp_indexes[MAX_ARRAY_DIMENSIONS];
//         uint8 tmp_len = return_size_value_dimensions(value_var, tmp_indexes, 0);
//         if (tmp_len != indexes_len) {
//           //TODO:error
//           printf("VM#ERR53\n");
//         }
//         for (uint32 i = 0; i < indexes_len; i++)
//           if (max_indexes[i] == -10)max_indexes[i] = tmp_indexes[i];
//       }
//     }
//   }
//   STR_init(&name, tmp_name);
// //  printf("CCCCCCCC:%s,%i,%s\n",name,return_var_id(name, 0),value_var);
//   //show_memory(1);
//   if (__return_var_id(name, fin) > 0) {
//     //printf("ERR:%s(fin:%i,Rfin:%i,sid:%i)\n",name,fin,entry_table.cur_fin,sid);
//     exception_handler("redeclared_var", __func__, name, "");
//     return 0;
//   }
//   //*******************************************analyzing values of variable
//   //if value is null
//   if (value_var == 0 || str_equal(value_var, "null")) {
// //    printf("fgjfjff:%s,%i[%i]\n",type_var,max_indexes[0],indexes_len);
//     if (!is_multi_array && str_search(basic_types, type_var, StrArraySize (basic_types)))
//       value_var = return_default_value(type_var);
//     else if (!is_multi_array && str_equal(type_var, "null") && str_indexof(name, RETURN_TMP_NAME, 0) == 0) {
//       STR_init(&type_var, "num");
//       value_var = return_default_value(type_var);
//     }
//     else
//       value_var = create_null_array(type_var, max_indexes, indexes_len);
// //    printf ("####EmptyVal:%s,%s\n", type_var, value_var);
//   }
//   //is multi array
//   if (is_multi_array) {
// //    printf("fffffffff....:%s=>%s\n",type_var,value_var);
//     vals_array = return_value_dimensions(value_var, type_var, max_indexes, indexes_len);

// //    print_vaar (vals_array);
//     if (vals_array.start == 0) {
//       //TODO:error
//       printf("#ERR54\n");
//       return 0;
//     }
//   }
//     //is single data
//   else {
//     uint32 val_len = str_length(value_var);
//     if (val_len > 2 && value_var[0] == '{' && value_var[val_len - 1] == '}') {
//       exception_handler("not_defined_array", __func__, name, "");
//       return 0;
//     }
//     //printf ("DDDDDDDDD:%s,%s\n", value_var, type_var);
//     String main_value = 0;
//     uint8 sub_type = '0';
//     calculate_value_of_var(value_var, type_var, &main_value, &sub_type);
//     if (sub_type == '0' && !str_equal(type_var, "vars")) {
//       //printf("###########failed2\n");
//       //TODO:error
//       printf("VM#ERR55:%s;%s;%s(%c)\n", name, value_var, type_var, sub_type);
//       return 0;
//     }
//     //printf("----SSSSW:%s[%s]=>%s[%c]\n", value_var, type_var, main_value, sub_type);
//     indexes_len = 1;
//     max_indexes[0] = 1;
//     //-----
//     vaar s = {1, sub_type, main_value, "0", 0};
//     append_vaar(s, &vals_array);
//     //TODO:
//   }
//   //  print_vaar(vals_array);
//   //*******************************************determine type of var
//   datas data_type = search_datas(type_var, entry_table.cur_fid, false);
// //  printf("#$EEEE:%s,%i,%i\n",type_var,data_type.id,is_struct);
//   //*******************************************add to memory
//   //******************add to pointer_memory
//   longint_list pointers_id = 0;
//   uint32 pointers_id_len = 0;
//   //-------------create data pointers (last nodes)
//   vaar *st = vals_array.start;
//   for (;;) {
//     //printf("@WWWW:%i,%s,%c\n",st->data_id,st->value,st->sub_type);
//     //is a struct node
//     if (is_struct) {
//       Longint stde_id = str_to_long_int(st->value);
//       stde s = get_stde(stde_id);
//       Longint po = set_struct_node_Mpoint(s.st);
//       if (po == 0) continue;
//       longint_list_append(&pointers_id, pointers_id_len++, po);
//     }
//       //is a normal node
//     else {
//       if (st->sub_type == 's')st->value = str_reomve_quotations(st->value, "s");
//       Longint po = add_to_pointer_memory(st->value, st->sub_type);
//       longint_list_append(&pointers_id, pointers_id_len++, po);
//     }
//     st = st->next;
//     if (st == 0) break;
//   }
//   //-------------create other pointers (parent nodes)
//   Longint main_pointer_id = set_parent_nodes_Mpoint(max_indexes, indexes_len, pointers_id, pointers_id_len);
//   //******************add to var_memory
//   if (is_create_var) {
//     return add_to_var_memory(main_pointer_id, fin, sid, data_type.id, name, 0);
//   } else {
//     return main_pointer_id;
//   }
//   //*******************************************
// }

//****************************************************
/**
 * get collection of vaar items and create its pointers and finally return id of last pointer that has struct node
 * @param st
 * @return Longint
 */
// Longint set_struct_node_Mpoint(vaar_en st) {
//   longint_list tmp_po_id = 0, struct_po_id = 0;
//   uint32 tmp_po_id_len = 0, struct_po_id_len = 0;
//   vaar *tmp1 = st.start;
//   uint32 co = 0;
//   int32 tmp_max_indexes[MAX_ARRAY_DIMENSIONS];
//   uint8 tmp_indexes_len = 0;
//   if (tmp1 == 0)return 0;
//   for (;;) {
//     //when switch to next item:create last node and create parent pointers
//     if (tmp1 == 0 || tmp1->data_id == co + 1) {
//       co++;
//       Longint po_id = set_parent_nodes_Mpoint(tmp_max_indexes, tmp_indexes_len, tmp_po_id, tmp_po_id_len);
//       //printf("@WWW:%i,%i,%i\n", po_id, tmp_max_indexes[0], tmp_indexes_len);
//       longint_list_append(&struct_po_id, struct_po_id_len++, po_id);
//       tmp_po_id = 0;
//       tmp_po_id_len = 0;
//       tmp_indexes_len = 0;
//       if (tmp1 == 0) break;
//     }
//     //first time for any items:get max_indexes
//     if (tmp_indexes_len == 0) {
//       str_list ret1 = 0;
//       tmp_indexes_len = (uint8) char_split(tmp1->index, ';', &ret1, true);
//       for (uint8 b = 0; b < tmp_indexes_len; b++) {
//         tmp_max_indexes[b] = str_to_int32(ret1[b]);
//       }
//     }
//     //when select an item:create last node for item and save its id
//     if (tmp1->data_id == co) {
//       Longint po = 0;
//       if (tmp1->sub_type == 'l') {
//         Longint stde_id = str_to_long_int(tmp1->value);
//         stde s = get_stde(stde_id);
//         po = set_struct_node_Mpoint(s.st);
//       } else {
//         if (tmp1->sub_type == 's')tmp1->value = str_reomve_quotations(tmp1->value, "s");
//         po = add_to_pointer_memory(tmp1->value, tmp1->sub_type);
//       }
//       longint_list_append(&tmp_po_id, tmp_po_id_len++, po);
//     }
//     //go to next item
//     tmp1 = tmp1->next;
//   }
//   //create struct node as a single room
//   String struct_val = 0;
//   for (uint32 i = 0; i < struct_po_id_len; ++i) {
//     //printf("RRRE:%i\n", struct_po_id[i]);
//     struct_val = str_append(struct_val, str_from_long_int(struct_po_id[i]));
//     if (i + 1 < struct_po_id_len)struct_val = char_append(struct_val, ';');
//   }
//   Longint po = add_to_pointer_memory(struct_val, 'l');
//   return po;
// }

//****************************************************
// Longint
// set_parent_nodes_Mpoint(int32 max_indexes[], uint8 indexes_len, longint_list pointers_id, uint32 pointers_id_len) {
//   Longint main_pointer_id = 0;
//   for (int32 j = indexes_len - 1; j >= 0; --j) {
//     //calculate how many pointers_id set in each next pointers
//     uint32 room_members = max_indexes[j];
//     uint32 rooms_count = pointers_id_len / room_members;
//     longint_list tmp_pointers_id = 0;
//     uint32 tmp_pointers_id_len = 0;
//     //=======
//     for (uint32 i = 0; i < rooms_count; ++i) {
//       String pointers_list = 0;
//       for (uint32 k = 0; k < room_members; ++k) {
//         pointers_list = str_append(pointers_list, str_from_long_int(pointers_id[0]));
//         longint_list_delete_first(&pointers_id, pointers_id_len--);
//         if (k + 1 < room_members)pointers_list = char_append(pointers_list, ';');
//       }
//       Longint po = add_to_pointer_memory(pointers_list, 'p');
//       longint_list_append(&tmp_pointers_id, tmp_pointers_id_len++, po);
//     }
//     //=======
//     if (rooms_count == 1) {
//       main_pointer_id = tmp_pointers_id[0];
//       break;
//     }
//     longint_list_init(&pointers_id, tmp_pointers_id, tmp_pointers_id_len);
//     pointers_id_len = tmp_pointers_id_len;
//   }
//   return main_pointer_id;
// }

//****************************************************
/**
 * get index of a Mvar node and delete it (if you permission it) and all its pointers
 * @param var_ind
 * @param is_del_var
 * @return Boolean
 */
// Boolean delete_full_memory_var(Longint var_ind, Boolean is_del_var) {
//   //******************init vars
//   longint_list del_ids = 0;
//   uint32 del_ids_len = 0;
//   //******************analyzing
//   longint_list_append(&del_ids, del_ids_len++, get_Mvar(var_ind).pointer_id);
//   //msg("&DEL:", var_memory[var_id].name, var_memory[var_id].id)
//   for (;;) {
//     if (del_ids_len == 0) {
//       break;
//     }
//     //-----------------
//     Mpoint st = get_Mpoint(del_ids[0]);
//     if (st.type_data == 'p') {
//       str_list tmp1 = 0;
//       uint32 tmp1_len = char_split(st.data, ';', &tmp1, true);
//       for (uint32 i = 0; i < tmp1_len; i++) {
//         longint_list_append(&del_ids, del_ids_len++, str_to_long_int(tmp1[i]));
//       }
//     }
//     //-----------------
//     delete_pointer_memory(del_ids[0]);
//     longint_list_delete_first(&del_ids, del_ids_len--);

//   }
//   if (is_del_var) {
//     delete_Mvar(var_ind);
//   }
//   //******************return
//   return true;
// }

//****************************************************
/**
 * get a pointer id of an array and return count of its dimensions like: f[1,2];=>1,2
 * @param var_ind
 * @param ret
 * @return uint32
 */
// uint32 return_array_dimensions(Longint pointer_id, str_list *ret) {
//   //-------------init vars
//   uint32 ret_len = 0;
//   //-------------get pointer_id
//   Longint po_id = pointer_id;
//   //-------------start analyzing
//   for (;;) {
//     Mpoint mpoint = get_Mpoint(po_id);
//     if (mpoint.type_data != 'p') {
//       break;
//     }
//     str_list pointers = 0;
//     uint32 po_len = char_split(mpoint.data, ';', &pointers, true);
//     //printf("QQQ:%s,%i\n", mpoint.data, po_len);
//     str_list_append(&(*ret), str_from_long_int((Longint) po_len), ret_len++);
//     if (po_len > 0) po_id = str_to_long_int(pointers[0]);
//     else break;
//   }

//   return ret_len;
// }

// //****************************************************
/**
 * get var_index of a var and copy it to new var
 * @param var_index
 * @param new_name
 * @param fin
 * @return Longint
 */
// Longint copy_memory_var(Longint var_index, String new_name, Longint fin) {
//   if (var_index == 0) return 0;
//   Mvar var = get_Mvar(var_index);
//   if (var.id == 0)return 0;
// //  show_memory(0);
//   String final_value = return_value_var_complete(var_index);
//   //printf("#WWWWW:%s,%s\n",var.name,final_value);
//   //******************set var memory with new value
//   String type_var = 0;
//   STR_init(&type_var, get_datas(var.type_var).name);
//   //printf("###COPY:%s=>%s,%s,%s\n", var.name, new_name, type_var, final_value);
//   //******************return
//   return set_memory_var(fin, 0, new_name, final_value, type_var, true);
// }
// //****************************************************
// uint32 recursive_list_pointer_ids(Longint pointer_id, longint_list *ret) {
//   uint32 len = 0, tmp_len = 0;
//   longint_list tmp = 0;
//   longint_list_append(&tmp, tmp_len++, pointer_id);
//   for (;;) {
//     if (tmp_len == 0)break;
//     Longint po_id = longint_list_delete_first(&tmp, tmp_len--);
//     Mpoint p = get_Mpoint(po_id);
//     if (p.type_data == 'p') {
//       str_list tmp1 = 0;
//       uint32 tmp1_len = char_split(p.data, ';', &tmp1, true);
//       for (uint32 i = 0; i < tmp1_len; i++) {
//         longint_list_append(&tmp, tmp_len++, str_to_long_int(tmp1[i]));
//       }
//     } else {
//       longint_list_append(&(*ret), len++, po_id);
//     }
//   }

//   return len;
// }
// //****************************************************
// uint32 delete_array_Mpoints(Longint pointer_id, Boolean is_delete_root) {
//   uint32 co = 0;
//   uint32 len = 0, tmp_len = 0;
//   longint_list tmp = 0;
//   longint_list_append(&tmp, tmp_len++, pointer_id);
//   for (;;) {
//     if (tmp_len == 0)break;
//     Longint po_id = longint_list_delete_first(&tmp, tmp_len--);
//     Mpoint p = get_Mpoint(po_id);
//     if (p.type_data == 'p' || p.type_data == 'l') {
//       str_list tmp1 = 0;
//       uint32 tmp1_len = char_split(p.data, ';', &tmp1, true);
//       for (uint32 i = 0; i < tmp1_len; i++) {
//         longint_list_append(&tmp, tmp_len++, str_to_long_int(tmp1[i]));
//       }
//     }
//     if (po_id == pointer_id)continue;
//     delete_Mpoint(po_id);
//     co++;
//   }
//   if (is_delete_root) {
//     //printf ("Root:%i,%i\n", pointer_id,  pointer_id);
//     delete_Mpoint(pointer_id);
//     co++;
//   }

//   return co;
// }