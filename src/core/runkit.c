#include <Mahdi/system.h>
//******************************************************
/**
 * get a name of var,func,pack and detect if is valid or not!
 * @author madkne
 * @version 1.0
 * @since 2019.12.18
 * @param name : name of var can bundled with index like var,var[0]
 * @param is_array : split name and index or not!
 * @return Boolean : if valid or not valid name
 */ 
Boolean RUNKIT_is_valid_name(String name, Boolean is_array){
    //=>init vars
    String name_v = 0, index = 0, buf = 0, extra = 0;
    Boolean is_string = false, is_end = false;
    int32 bras = 0;
    //=>get length of name    
    uint32 name_len = STR_length(name);
    //=>if name is null, not valid!
    if (name_len < 1) {
        return false;
    }
    //=>check name is mahdi keyword, not valid
    if (STR_search(keywords, name, StrArraySize(keywords))) {
        return false;
    }
    //=>if must check array and splite name and index
    if (is_array) {
        //=>split index , name like n[2]=>n,2
        for (uint32 i = 0; i < name_len; i++) {
            //=>check is string
            if (name[i] == '\"' && (i == 0 || name[i - 1] != '\\')) {
                is_string = BOOL_switch(is_string);
            }
            //=>count pars
            if (!is_string) {
                if (name[i] == '[') bras++;
                else if (name[i] == ']') bras--;
            }
            //=>set name of var like ar[2]=>name_v='ar'
            if (!is_string && name[i] == '[' && name_v == 0 && bras == 1) {
                name_v = STR_trim_space(buf);
                buf = 0;
                continue;
            }
            //=>set index of var
            if (!is_string && index == 0 && name[i] == ']' && name_v != 0 && bras == 0){
                index = STR_trim_space(buf);
                buf = 0;
                is_end = true;
                continue;
                //break
            }
            //=>append to buf
            buf = CH_append(buf, name[i]);
        }
        //=>set extra like k1[0]+3=>k1,0,+3
        if (!is_string && is_end && buf != 0) {
            STR_init(&extra, buf);
        }
        //=>if no detect name, then set buffer as name
        if (name_v == 0) {
            STR_init(&name_v, buf);
        }

    } 
    //=>if not array
    else {
        STR_init(&name_v, name);
    }
    //  printf("&%%%:%s;%s;%s;%s\n", name, name_v, index, extra);
    //=>check for extra, if not null, then not valid!
    if (extra != 0) {
        return false;
    }
    //=>check for name_v, if has pattern like _var (just in parser level) or 133var, not valid
    if (STR_length(name_v) > 0 &&
        ((name_v[0] >= '0' && name_v[0] <= '9') || (name_v[0] == '_' && STR_equal(interpreter_level, "parse")))) {
        return false;
    }
    //=>iterate name var chars
    for (uint32 i = 0; i < STR_length(name_v); i++) {
        //=>if chars of name is valid, ignore it like _var,var,@var,var34
        if ((name_v[i] >= '0' && name_v[i] <= '9') || (name_v[i] >= 'A' && name_v[i] <= 'Z') ||
            (name_v[i] >= 'a' && name_v[i] <= 'z') || name_v[i] == '_' || (name_v[i] == '@' && i == 0)) {
            continue;
        } else {
            return false;
        }
    }
    //=>check for index, if is_array
    if (is_array && index == 0) {
    //return false
    }
    //=>if name is valid, then return True!
    return true;
}
//******************************************************
uint8 RUNKIT_defvars_analyzing(StrList defvars,uint32 defvars_len, defvar vars_store[],Boolean just_syntax) {
    /**
        1- x1,x2,x3:string 								        ---..---
        2- x1,x2=(3+5)*x3 								        ---..---
        3- x1:number,x2,x3=0x3A4D,(5+7)/2,("we"*10)  	        ---..---
        4- x1=45 										        ---..---
        5- x1=true&&(f==45) 							        ---..---
        6- x1=[[5,7.8],[1,7]]	 					            ---..---
        7- x1=x2[1][2]*2 								        ---..---
        8- st2={"q":"Amin","f":st1[0]*2+"!"} 			        ---..---
        9- q1:number=[{'q1':67,'q2':-78},{'q1':0,'q2':45.4}] 	---..---
        10- g=pkg('hello',56)                                   ---..---
        */
    //=>if is empty list
    if (defvars_len==0) return 0;
    //=>init vars
    String word = 0, last_type = 0, data_type = 0;
    Boolean is_string = false, is_equal = false,is_colon=false;
    int16 vars_counter = -1, vals_counter = -1, pars = 0, bras = 0, acos = 0;
    struct inst_var{
        String name;
        String type;
        String value;
    };
    //=>iterate define vars list
    for(uint32 k = 0; k < defvars_len; k++){
        //=>init vars for define var item
        String inst=defvars[k];
        uint32 len=STR_length(inst);
        struct inst_var var={0,0,0};
        word=0;
        is_string = is_equal = is_colon = false;
        pars = bras = acos = 0;
        //=>analyzing inst to fill inst_var struct
        for (uint32 i = 0; i < len; i++){
            //=>check if string
            if (inst[i] == '\"' && (i == 0 || inst[i - 1] != '\\')){
                is_string = BOOL_switch(is_string);
            }
            //=>count bras,pars,acos,=,:
            if (!is_string) {
                if (inst[i] == '(')pars++;
                else if (inst[i] == ')')pars--;
                else if (inst[i] == '{')acos++;
                else if (inst[i] == '}')acos--;
                else if (inst[i] == '[')bras++;
                else if (inst[i] == ']')bras--;
                else if (inst[i] == '=')is_equal = true;
                else if (inst[i] == ':')is_colon = true;
            }
            //=>get var name
            if(!is_string && var.name==0 && ((is_equal && inst[i] == '=') || (is_colon && inst[i] == ':')|| i+1==len)){
                if(i+1==len)word = CH_append(word, inst[i]);
                STR_init(&var.name,word);
            }
            //=>get var type (if exist)
            else if(!is_string && var.name!=0 && is_colon && ((is_equal && inst[i] == '=') || (!is_equal && i+1==len))){
                if(i+1==len)word = CH_append(word, inst[i]);
                STR_init(&var.type,word);
            }
            //=>get var value (if exist)
            else if(!is_string && var.name!=0 && is_equal && i+1==len){
                if(i+1==len)word = CH_append(word, inst[i]);
                STR_init(&var.value,word);
            }
            //=>append to word
            if (!is_string && pars == 0 && acos == 0 && bras == 0 &&
            (inst[i] == ':' || inst[i] == '=')) {
                word=0;
            }else{
                word = CH_append(word, inst[i]);
            }
        }
        // printf("vardef:%s,%s,%s,%i\n",var.name,var.type,var.value,RUNKIT_is_valid_name(var.name,false));
        //=>check for var name is valid
        if(!RUNKIT_is_valid_name(var.name,false)){
            EXP_handler("invalid_name_var", __func__, var.name, "");
            return 0;
        }
        //=>get value data type
        String valtype=RUNKIT_detect_basic_type_value(var.value,0);
        //=>check for invalid value error
        if(EXP_check_errcode(INVALID_VALUE_ERRC)){
            EXP_handler("invalid_value", __func__, var.value, "");
            return 0;
        }
        //=>if not have basic type
        if(valtype==0){
            //TODO:
        }
        //=>comparison between var type(if exist) and val type
        if(var.type!=0 && !STR_equal(var.type,valtype)){
            //TODO:
        }
        //=>more check if just_syntax is false!
        if(!just_syntax){
            //=>a var must has a default value
            if(var.value==0){
                //TODO:err
            }
            //=>verify var type (if exist)
            if(var.type!=0 && _datas_search(var.type,0,true).name==0){
                //TODO:err
            }
        }
        //=>fill vars store
        vars_counter++;
        STR_init(&vars_store[vars_counter].type_var,var.type);
        // STR_init(&vars_store[vars_counter].type_var,var.type);
        //TODO:
    }
    return vars_counter + 1;
}

//******************************************************
/**
 * get a value like 'H'*3 , {'q1':true and false} , 4.5+12 and detect main data type and its sub type if possible!
 * @author madkne
 * @version 1.0
 * @since 2019.12.20
 * @param value : value of var
 * @param sub_type : (pointer) set sub type of value
 * @return String : data type of value (number,string,boolean)
 */ 
String RUNKIT_detect_basic_type_value(String value,uint8 *sub_type){
    //=>init vars
    uint32 len=STR_length(value);
    Boolean is_string=false;
    String final_type=0,word=0;
    uint8 final_sub=0;
    //=>if value is a list or map, then just get its first item
    if(value[0]=='[' || value[0]=='{'){
        STR_init(&value,RUNKIT_get_firstitem_listormap(value));
        //=>if an error to get first item of list or map
        if(EXP_check_errcode(BAD_FIRST_ITEM_ERRC)){
            EXP_set_errcode(INVALID_VALUE_ERRC);
            return 0;
        }
    }
    //=>if value is simple (number:+56.5,string:"hello",boolean:true)
    if (STR_is_bool(value)) {
        final_sub='b';
        STR_init(&final_type,"boolean");
    } else if (STR_is_num(value)){ 
        final_sub='n';
        STR_init(&final_type,"number");
    }else if(len>1&&value[0]=='\"'&&value[len-1]=='\"'){
        final_sub='s';
        STR_init(&final_type,"string");
    }else if (STR_indexof(value, UTF8_ID_LABEL, 0) == 0){
        final_sub='u';
        STR_init(&final_type,"string");
    }
    //=>if value is complex (number,string,boolean)
    else{
        for (uint32 i = 0; i < len; i++) {
            //=>check is string
            if (value[i] == '\"' && (i == 0 || value[i - 1] != '\\')) {
                is_string = BOOL_switch(is_string);
                STR_init(&final_type, "string");
                final_sub='s';
            }
            //=>check is utf8 string
            if (!is_string && STR_equal(word, UTF8_ID_LABEL)) {
                STR_init(&final_type, "string");
                final_sub='u';
                break;
            }
            //=>is number operators like +,-,*,..
            if(CH_search(single_operators, value[i]) && STR_is_num(word)){
                STR_init(&final_type, "number");
                final_sub='n';
            }
            //=>is boolean operators like and , or
            if (!is_string && STR_search(boolean_operators, word, StrArraySize(boolean_operators))) {
                STR_init(&final_type, "boolean");
                final_sub='b';
            }
            if (!is_string && STR_is_bool(word)) {
                STR_init(&final_type, "boolean");
                final_sub='b';
            }
            //=>append to word
            if (!is_string && (CH_search(words_splitter, value[i]) || CH_search(single_operators, value[i]))) {
                word = 0;
            } else {
                word = CH_append(word, value[i]);
            }
        }
    }
  //=>return data type and sub type
    if(sub_type!=0){
        (*sub_type)=final_sub;
    }
    return final_type;
}
//******************************************************
/**
 * get a value like [78,55] or {'q':5,'e':77} or combination of both and return its firstitem 
 * @author madkne
 * @version 1.0
 * @since 2019.12.20
 * @param value : value of var as list or map
 * @return String : first item of list or map
 */ 
String RUNKIT_get_firstitem_listormap(String value){
    //=>init vars
    uint8 mode='0';
    String firstitem=0;
    STR_init(&firstitem,value);
    //=>search for get first item
    while(true){
        //=>check if is map or list or not!
        if(firstitem[0]=='{'){
            mode='m';
        }else if(firstitem[0]=='['){
            mode='l';
        }else{
            return firstitem;
        }
        //=>if firstitem is map, then parse it!
        if(mode=='m'){
            StrList values=0,keys=0;
            uint32 mapl=RUNKIT_get_map_items(firstitem,&values,&keys);
            //=>check for error
            if(EXP_check_errcode(BAD_MAP_ERRC)){
                EXP_set_errcode(BAD_FIRST_ITEM_ERRC);
                return 0;
            }
            //=>if have not any items
            if(mapl==0) return 0;
            STR_init(&firstitem,values[0]);
        }
        //=>if firstitem is list, then parse it!
        else if(mode=='l'){
            StrList items=0;
            uint32 listl=RUNKIT_get_list_items(firstitem,&items);
            //=>check for error
            if(EXP_check_errcode(BAD_LIST_ERRC)){
                EXP_set_errcode(BAD_FIRST_ITEM_ERRC);
                return 0;
            }
            //=>if have not any items
            if(listl==0) return 0;
            STR_init(&firstitem,items[0]);
        }
    }
    return 0;
}
//******************************************************
uint32 RUNKIT_get_list_items(String list,StrList *items){
    //=>init vars
    uint32 len=STR_length(list);
    uint32 items_len=0;
    String word = 0;
    Boolean is_string = false;
    uint8 pars = 0;
    uint8 bras = 0;
    uint8 acos = 0;
    //=>check if really value is list!
    if(len>1 && list[0]=='[' && list[len-1]==']'){
        list=STR_substring(list,1,len-1);
    }else{
        EXP_set_errcode(BAD_LIST_ERRC);
        return 0;
    }
    //=>analyzing list to get its items
    for (uint32 i = 0; i < len; i++){
        //=>check if string
        if (list[i] == '\"' && (i == 0 || list[i - 1] != '\\')){
            is_string = BOOL_switch(is_string);
        }
        //=>count bras,pars,acos
        if (!is_string) {
            if (list[i] == '(')pars++;
            else if (list[i] == ')')pars--;
            else if (list[i] == '{')acos++;
            else if (list[i] == '}')acos--;
            else if (list[i] == '[')bras++;
            else if (list[i] == ']')bras--;
        }
        //=>append to word
        if (!is_string && pars == 0 && acos == 0 && bras == 0 &&(list[i] == ','||i+1==len)) {
            SLIST_append(&(*items),word,items_len++);
            word=0;
        }else{
            word = CH_append(word, list[i]);
        }
    }
    return items_len;
}
//******************************************************
uint32 RUNKIT_get_map_items(String map,StrList *values,StrList *keys){
    //=>init vars
    uint32 len=STR_length(map);
    uint32 items_len=0;
    String word = 0;
    Boolean is_string = false;
    Boolean is_colon=false;
    uint8 pars = 0;
    uint8 bras = 0;
    uint8 acos = 0;
    //=>check if really value is map!
    if(len>1 && map[0]=='{' && map[len-1]=='}'){
        map=STR_substring(map,1,len-1);
    }else{
        EXP_set_errcode(BAD_MAP_ERRC);
        return 0;
    }
    //=>analyzing map to get its items
    for (uint32 i = 0; i < len; i++){
        //=>check if string
        if (map[i] == '\"' && (i == 0 || map[i - 1] != '\\')){
            is_string = BOOL_switch(is_string);
        }
        //=>count bras,pars,acos
        if (!is_string) {
            if (map[i] == '(')pars++;
            else if (map[i] == ')')pars--;
            else if (map[i] == '{')acos++;
            else if (map[i] == '}')acos--;
            else if (map[i] == '[')bras++;
            else if (map[i] == ']')bras--;
        }
        //=>get an item key 
        if(!is_string && pars == 0 && acos == 0 && bras == 0 && !is_colon && map[i]==':' && word!=0){
            SLIST_append(&(*keys),word,items_len++);
            is_colon=true;
            word=0;
            continue;
        }
        //=>append to word, get item value
        if (!is_string && pars == 0 && acos == 0 && bras == 0 && word!=0 &&(map[i] == ','||i+1==len)) {
            //=>check for exist a key
            if(is_colon){
                SLIST_append(&(*values),word,items_len-1);
                word=0;
                is_colon=false;
            }else{
                EXP_set_errcode(BAD_MAP_ERRC);
                return 0;
            }
        }else{
            word = CH_append(word, map[i]);
        }
    }
    return items_len;
}
//******************************************************
//******************************************************
//******************************************************
//******************************************************

//******************************************************
//******************************************************

