

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

//*************************************************************
/**
 * display Mvar,Mpoint or both in console by details based on wh
 * @param wh {uint8}
 * @author madkne
 * @version 1.0
 */
void VM_show(uint8 wh){
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

//**************************************************************
/**
 * get name of new var and its value , type and its pack_index,func_index,stru_index. finally create new var on VM and return id of Mvar
 * @version 1.0
 * @author madkne
 * @since 2020.1.3
 * @param current
 * @param name
 * @param value_var
 * @param type_var
 * @param is_static
 * @param is_private
 * @param is_create_var
 * @return Longint
 */
Longint VM_set_var(rrss current, String name_var, String value_var, String type_var,Boolean is_static,Boolean is_private, Boolean is_create_var) {
    /**
        1- def g=785i                ---..---
        2- def li:string=["sd","ER"] ---..---
        3- def r=li                  ---..---
        4- def u={"RT":5.7,"F":-13}  ---..---
        5- def ddd=[{'q1':3,'q2':4},{'f':'fg','g':'gf'}]  ---..---
    */
    //  printf("&@@@:%s,%i,%s(%i)\n", name, return_var_id(name, "0"), value_var,str_length(value_var));
    //=>if value is a variable
    if (RUNKIT_is_valid_name(value_var, true)) {
        String al_name = 0;
        String al_index=RUNKIT_get_name_index_var(value_var, true, &al_name);
        //=>if is a variable
        if (al_index == 0) {
            Longint varid = RUNKIT_get_var_id(value_var,current.pin,current.fin);
            //=>if not exist var
            if (varid == 0) {
                EXP_handler("not_exist_var", __func__, value_var, 0);
                EXP_set_errcode(NOT_SET_VAR_MEMORY_ERRC);
                return 0;
            }
            //=>if var is exist, get its value
            //TODO:
            // value_var = return_value_var_complete(find_index_var_memory(ret0));
        }
        //=>if is a data variable (has index)
        else {
            //TODO:
            // uint8 ty = '0';
            // Mpoint point = return_var_memory_value(value_var);
            // value_var = point.data;
            // if (point.type_data == 's') {
            //     value_var = convert_to_string(value_var);
            // } else if (point.type_data == 'p' || point.type_data == '0') {
            //     //TODO:error
            //     printf("#ERR9877\n");
            //     return 0;
            // }
        }
    }
    //=>check if value is not null
    value_var = STR_trim_space(value_var);
    if(value_var==0){
        //TODO:error
        EXP_set_errcode(NOT_SET_VAR_MEMORY_ERRC);
        return 0;
    }
    //=>init vars
    datas type_props;
    Boolean is_package_var = false;
    Longint main_pointer_id =0;
    //=>if var type is a package
    type_props=_datas_search(type_var,0,true);
    if (type_props.type==PACKAGE_DATA_TYPE) {
        is_package_var = true;
    }
    //=>if redeclared var name
    if (RUNKIT_get_var_id(name_var,current.pin,current.fin) > 0) {
        EXP_handler("redeclared_var", __func__, name_var, 0);
        EXP_set_errcode(NOT_SET_VAR_MEMORY_ERRC);
        return 0;
    }
    //=>if type var is package
    if(is_package_var){
        //TODO:reading package var pointers from value_var
    }
    //=>if type var is basic(string,number,boolean)
    else{
        //=>if value is a list or map
        if(value_var[0]=='[' || value_var[0]=='{'){
            main_pointer_id= VM_set_maporlist_mpoint(value_var,type_var);
            //=>if an error to set list or map in mpoint
            if(EXP_check_errcode(BAD_MAPORLIST_VAL_ERRC)){
                EXP_set_errcode(NOT_SET_VAR_MEMORY_ERRC);
                return 0;
            }
        }
        //=>if value is simple
        else{
            main_pointer_id=VM_set_basictype_mpoint(value_var,type_var);
        }
    }
    //=>add to var_memory
    if (is_create_var) {
        return _mvar_append(main_pointer_id,current.sid,current.fid, current.fin,current.pid,current.pin, type_props.pack_id,is_static,is_private, 0,name_var);
    } else {
        return main_pointer_id;
    }
}
//**************************************************************
/**
 * get a value and its type(string,boolean,number) and set it to mpoint and return its id
 * @version 1.0
 * @author madkne
 * @since 2020.1.4
 * @param value
 * @param type
 * @return Longint : id of node that created in mpoint
 */
Longint VM_set_basictype_mpoint(String value,String type){
    //=>if value is boolean
    if(STR_equal(type,"boolean")){
        return _mpoint_append('b',value);
    }
    //=>if value is string
    else if(STR_equal(type,"string")){
        //=>if value is utf8
        uint8 sub='s';
        if(STR_indexof(value,UTF8_ID_LABEL,0)==0){
            sub='u';
        }else{
            value=STR_reomve_quotations(value, "s");
        }
        return _mpoint_append(sub,value);
    }
    //=>if value is number
    else{
        uint8 sub=RUNKIT_determine_type_number(value);
        return _mpoint_append(sub,value);
    }
}
//**************************************************************
/**
 * get a value and its basic type(string,number,boolean) and if is a map or list or combination of them and set them into mpoint and return parent (last) node id
 * @version 1.0
 * @author madkne
 * @since 2020.1.4
 * @param value
 * @param type
 * @return Longint : id of last node that created in mpoint
 */
Longint VM_set_maporlist_mpoint(String value,String type){
    //=>init vars
    Longint parentid=0;
    //=>if value is map, then parse it!
    if(value[0]=='{'){
        StrList values=0,keys=0;
        uint32 mapl=RUNKIT_get_map_items(value,&values,&keys);
        //=>check for error
        if(mapl==0 && EXP_check_errcode(BAD_MAP_ERRC)){
            EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
            return 0;
        }
        //=>set keys on mpoint
        for (uint32 i = 0; i < mapl; i++){
            keys[i]= STR_from_Longint(VM_set_basictype_mpoint(keys[i],"string"));
        }
        //=>when first value is checked, the other items use this!
        Boolean value_is_maporlist=false;
        //=>set values on mpoint
        for (uint32 i = 0; i < mapl; i++){
            //=>if value item is a list or map
            if(value_is_maporlist || values[i][0]=='[' || values[i][0]=='{'){
                values[i]=STR_from_Longint(VM_set_maporlist_mpoint(values[i],type));
                //=>if an error to calculate of list or map
                if(EXP_check_errcode(BAD_MAPORLIST_VAL_ERRC)){
                    EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
                    return 0;
                }
                value_is_maporlist=true;
            }
            //=>if value item is a simple basic type
            else{
                values[i]= STR_from_Longint(VM_set_basictype_mpoint(values[i],type));
            }
        }
        //=>create mpoint entry for map items (type='m')
        String data=0;
        for (uint32 i = 0; i < mapl; i++) {
            data=STR_multi_append(data,keys[i],";",values[i],0,0);
            if(i+1<mapl)data=CH_append(data,';');
        }
        //=>append to mpoint
        parentid=_mpoint_append('m',data);
    }
    //=>if value is list, then parse it!
    else if(value[0]=='['){
        StrList items=0;
        uint32 listl=RUNKIT_get_list_items(value,&items);
        //=>check for error
        if(EXP_check_errcode(BAD_LIST_ERRC)){
            EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
            return 0;
        }
        //=>when first value is checked, the other items use this!
        Boolean value_is_maporlist=false;
        //=>set items on mpoint
        for (uint32 i = 0; i < listl; i++){
            //=>if item is a list or map
            if(value_is_maporlist || items[i][0]=='[' || items[i][0]=='{'){
                items[i]=STR_from_Longint(VM_set_maporlist_mpoint(items[i],type));
                //=>if an error to calculate of list or map
                if(EXP_check_errcode(BAD_MAPORLIST_VAL_ERRC)){
                    EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
                    return 0;
                }
                value_is_maporlist=true;
            }
            //=>if item is a simple basic type
            else{
                items[i]= STR_from_Longint(VM_set_basictype_mpoint(items[i],type));
            }
        }
        //=>create mpoint entry for list items (type='l')
        String data=0;
        for (uint32 i = 0; i < listl; i++) {
            data=STR_append(data,items[i]);
            if(i+1<listl)data=CH_append(data,';');
        }
        //=>append to mpoint
        parentid=_mpoint_append('l',data);
    }
    //=>if not, then error!
    else{
        EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
        return 0;
    }
    return parentid;
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
Longint _mvar_append(Longint pointer_id, Longint stru_id, Longint func_id, Longint func_index, Longint pack_id, Longint pack_index, Longint type_id, Boolean is_static, Boolean is_private, uint8 flag, String name){
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
//*************************************************************
Mvar _mvar_get(Longint i){
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
//*************************************************************
/**
 * get an index of mvar linkedlist and remove indexed node of mvar and decrease mvar length
 * @param i {Longint} : index of mvar linkedlist
 * @author madkne
 * @version 1.0
 */
void _mvar_delete(Longint i){
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
//*************************************************************
void _mvar_set_pointer_id(Longint i, Longint new_po){
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
//*************************************************************
/**
 * delete a Mvar node by its id
 * @param id {Longint}
 * @return Boolean
 * @author madkne
 * @version 1.0
 */
Boolean _mvar_delete_by_id(Longint id){
    for (Longint i = 0; i < entry_table.var_mem_len; i++)
    {
        if (_mvar_get(i).id == id)
        {
            _mvar_delete(i);
        }
    }
    return true;
}

//*************************************************************
/**
 * get an id and search for index it in Mvar
 * @param id {Longint}
 * @return success index {Longint}
 * @return faild 0 
 * @author madkne
 * @version 1.0
 */
Longint _mvar_find_index_by_id(Longint id){
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

//*************************************************************
/**
 * get a pointer_id of var and return an index in Mvar
 * @param pointer_id {Longint}
 * @return success index {Longint}
 * @return faild 0 
 * @author madkne
 * @version 1.0
 */
Longint _mvar_find_index_by_pointer_id(Longint pointer_id){
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
//*************************************************************
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

//*************************************************************
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

//*************************************************************
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
//*************************************************************
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
Boolean _mpoint_delete_by_id(Longint id){
    for (Longint i = 0; i < entry_table.pointer_mem_len; i++)
    {
        if (_mpoint_get(i).id == id)
        {
            _mpoint_delete(i);
        }
    }
    return true;
}


//*************************************************************


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

//*************************************************************
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
//*************************************************************
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
//*************************************************************
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
//*************************************************************

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