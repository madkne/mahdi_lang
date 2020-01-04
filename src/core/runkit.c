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
    if (STR_search(keywords, name,StrArraySize(keywords))) {
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
/**
 * get a instruction of define vars and split it to many define vars to use in RUNKIT_defvars_analyzing
 * @author madkne
 * @version 1.1
 * @since 2019.12.29
 * @param inst : a define vars instruction code 
 * @param defvars : (pointer) list of splitted define vars
 * @param is_static : (pointer) if define vars is static or not
 * @return uint32 : count of define vars detect of StrList
 */
uint32 RUNKIT_simplify_define_vars(String inst,StrList *defvars,Boolean *is_static){
    /**
     * 1- def x1,x2,x3:string=''  ---OK---							
     * 2- static def x1,x2=(3+5)*x3 ---OK---
     * 3- def x1:number,x2,x3=0x3A4D,(5+7)/2,("we"+"He") ---OK---
     * 4- def x1,st2=[[5,7.8],[1,7]],{"q":"Amin","f":st1[0]+"!"} ---OK---
     * 5- def x1=x2[1][2]*2 ---OK---
     * 6- def g=pkg('hello',56)  ---OK---
    */
    //=>if is empty inst
    if (inst==0) return 0;
    //=>init vars
    defvar defv[MAX_VAR_ALLOC_INSTRUCTIONS];
    uint32 defvars_len=0;
    String word = 0, last_type = 0, data_type = 0;
    Boolean is_string = false, is_equal = false,verify_static=false,is_def=false,has_type=false;
    int16 vars_counter = 0, vals_counter = 0, pars = 0, bras = 0, acos = 0;
    //=>trim inst
    inst = STR_trim_space(inst);
    //=>get inst length
    uint32 len = STR_length(inst);
    //=>iterate inst chars
    for (uint32 i = 0; i < len; i++) {
        //printf("@X:%c\n",inst[i]);
        //=>check is string
        if (inst[i] == '\"' && (i == 0 || inst[i - 1] != '\\')){
            is_string = BOOL_switch(is_string);
        }
        //=>continue if ' '
        if (!is_string && i + 1 < len && inst[i] == ' ' && (inst[i + 1] == '(' || inst[i + 1] == '[')) {
            continue;
        }
        //=>count bras,pars,acos
        if (!is_string) {
        if (inst[i] == '(')pars++;
        else if (inst[i] == ')')pars--;
        else if (inst[i] == '{')acos++;
        else if (inst[i] == '}')acos--;
        else if (inst[i] == '[')bras++;
        else if (inst[i] == ']')bras--;
        else if (inst[i] == '=')is_equal = true;
        }
        //=>get 'static' attribute, if exist
        if(!is_string && !is_def && inst[i]==' ' && STR_equal(word,"static")){
            verify_static=true;
            //=>reset word and ignore
            word=0;
            continue;
        }
        //=>get 'def' keyword
        if(!is_string && !is_def && !is_equal && inst[i]==' ' && STR_equal(word,"def")){
            is_def=true;
            //=>reset word and ignore
            word=0;
            continue;
        }
        //=>get a variable
        if (!is_string && !has_type && is_def && pars == 0 && acos == 0 && bras == 0 &&((!is_equal && (inst[i] == ',' || inst[i] == ':')) || inst[i] == '=')) {
            //=>if char splitter is ':', enable has type
            if(inst[i]==':'){
                has_type=true;
            }
            //=>trim word
            word = STR_trim_space(word);
            //=>if var name not valid
            if (!RUNKIT_is_valid_name(word, false)) {
                EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
                EXP_handler("invalid_name_var", __func__, word, 0);
                return 0;
            }
            //=>if vars count is bigger than max var alloc
            if(vars_counter+1>MAX_VAR_ALLOC_INSTRUCTIONS){
                //TODO:error
                EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
                return 0;
            }
            //=>append to defvar array
            defv[vars_counter].type_var=0;
            STR_init(&defv[vars_counter++].name_var,word);
            //=>reset word and ignore
            word = 0;
            continue;
        }
        //=>get data type, if exist
        if (!is_string && has_type && vars_counter >0 && is_def && pars == 0 && acos == 0 && bras == 0 && word != 0 && ((!is_equal && inst[i] == ',') || inst[i] == '=') ) {
            //=>disable has type
            has_type=false;
            //=>trim word
            word = STR_trim_space(word);
            //=>if data type is valid
            if(STR_search(basic_types,word,StrArraySize(basic_types)) || (STR_equal(interpreter_level, "parse") && RUNKIT_is_valid_name(word,false)) || _datas_search(word, 0, true).id != 0){
                STR_init(&defv[vars_counter-1].type_var,word);
            }
            //=>if data type is not valid
            else{
                //TODO:error
                EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
                return 0;
            }
            //=>reset word and ignore
            word = 0;
            continue;
        }
        //=>allocate values to variables
        if (is_equal && !is_string &&((inst[i] == ',' && pars == 0 && acos == 0 && bras == 0) || (i + 1 == len && acos < 2) || (i + 1 == len && bras < 2) || (i + 1 == len && pars < 2))){
            //=>append last char to word
            if (i + 1 == len) {
                word = CH_append(word, inst[i]);
                //=>count acos,bras,pars is last char
                if (inst[i] == '}') acos--;
                else if (inst[i] == ']')bras--;
                else if (inst[i] == ')')pars--;
                //=>check for rest all acos,bras,pars
                if(acos > 0 || bras>0 || pars>0) continue;
            }
            //=>values is higher than vars
            if(vals_counter+1>vars_counter){
                //TODO:error
                EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
                return 0;
            }
            //=>trim word
            word = STR_trim_space(word);
            //=>append value to var by vals_counter
            STR_init(&defv[vals_counter++].value_var, word);
            //=>reset word and ignore
            word = 0;
            continue;
        }
        //=>append to word
        word = CH_append(word, inst[i]);
    }
    //=>check if vars count higher than vals count
    if(vars_counter>vals_counter){
        //=>if values count is bigger than 1 or 0
        if(vals_counter>1 || vals_counter==0){
            //TODO:error
            EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
            return 0;
        }
        //=>else all vars values is same!
        for (uint32 i = 1; i < vars_counter; i++){
            STR_init(&defv[i].value_var,defv[0].value_var);
        }
    }
    //=>convert defv array defvars list
    for (uint32 i = 0; i < vars_counter; i++){
        //=>if has data type
        if(defv[i].type_var==0){
            SLIST_append(&(*defvars),STR_multi_append(defv[i].name_var,"=",defv[i].value_var,0,0,0),defvars_len++);
        }
        //=>if not have data type
        else{
            SLIST_append(&(*defvars),STR_multi_append(defv[i].name_var,":",defv[i].type_var,"=",defv[i].value_var,0),defvars_len++);
        }
    }
    
    //=>return count
    if(is_static!=0) (*is_static)=verify_static;
    return defvars_len;

}
//******************************************************
/**
 * get a list of defined vars without 'def' and attributes and split, verify and simplify them
 * @author madkne
 * @version 1.1
 * @since 2019.12.24
 * @param code : an instruction code include string or ...
 * @return uint8 : count of vars detect or -1
 */ 
uint8 RUNKIT_defvars_analyzing(StrList defvars,uint32 defvars_len, defvar vars_store[],Boolean just_syntax,Boolean just_basic_types) {
    /**
     * 1- h:number , d:string
     * 2- d="hel"+"lo"
     * 3- x1:string="hy" , x2:number=2+(5*5)
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
        String typeval;
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
            EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
            return 0;
        }
        //=>if var has a value
        if(var.value!=0){
            //=>get value data type
            var.typeval=RUNKIT_detect_basic_type_value(var.value,0);
            //=>check for invalid value error
            if(var.typeval==0 && EXP_check_errcode(INVALID_VALUE_ERRC)){
                EXP_handler("invalid_value", __func__, var.value, "");
                EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
                return 0;
            }
            //=>if not have basic type, if not just basic types
            if(!just_basic_types && var.typeval==0){
                //TODO:search in datas
            }
            //=>comparison between var type(if exist) and val type
            if(var.type!=0 && !STR_equal(var.type,var.typeval)){
                //TODO:
                printf("ERR4454\n");
                EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
                return 0;
            }
            //=>if var type is empty, set by value type
            else if(var.type==0){
                STR_init(&var.type,var.typeval);
            }
            //=>simplify value of var
            uint8 sub_type='0';
            var.value=RUNKIT_calculate_value(var.value,var.typeval,&sub_type,true);
            if(sub_type=='0'||var.value==0){
                //TODO:error
                printf("ERR445478\n");
                EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
                return 0;
            }
            // printf("var value:%s(%s)=>%s\n",var.name,var.type,var.value);
        }
        //=>more check if just_syntax is false!
        if(!just_syntax){
            //=>a var must has a default value
            if(var.value==0){
                //TODO:err
                printf("ERR445411\n");
                EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
                return 0;
            }
            //=>verify var type (if exist)
            if(var.type!=0 && _datas_search(var.type,0,true).name==0){
                //TODO:err
                printf("ERR44500411\n");
                EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
                return 0;
            }
        }
        //=>if type var is null
        if(var.type==0){
            //TODO:err
            printf("ERR4454011\n");
            EXP_set_errcode(BAD_DEFINE_VARS_ERRC);
            return 0;
        }
        //=>fill vars store
        vars_counter++;
        STR_init(&vars_store[vars_counter].type_var,var.type);
        STR_init(&vars_store[vars_counter].name_var,var.name);
        STR_init(&vars_store[vars_counter].value_var,var.value);
        if(!just_syntax){
            // vars_store[vars_counter].fid
            // vars_store[vars_counter].pid
            // vars_store[vars_counter].sid
            //TODO:
        }
        
    }
    return vars_counter + 1;
}

//******************************************************
/**
 * get a value like [7+8,(5*5)+1] or {'q'+'u':5,'e':77} or combination of both and calculate its items and return it
 * @author madkne
 * @version 1.0
 * @since 2019.12.29
 * @param value : value of var as list or map
 * @param type : type of all items like string,number,..
 * @return String : calculated of list or map
 */ 
String RUNKIT_calculate_listormap(String value,String type){
    /**
     * 1- {"q"+"1":[5,8*2,-5^2],"q2"-"2":[7+3,7]}
     * 2- ["he"+"llo",("H"+"J")-"J"]
     * 3- {"q1":77,"q2":[4,8]} //=>error
     * 4- ["He",980] OR [6,{"q":7}] //=>error
     */ 
    //=>init vars
    String ret=0;
    uint8 subtype='0';
    //=>if value is map, then parse it!
    if(value[0]=='{'){
        StrList values=0,keys=0;
        uint32 mapl=RUNKIT_get_map_items(value,&values,&keys);
        //=>check for error
        if(mapl==0 && EXP_check_errcode(BAD_MAP_ERRC)){
            EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
            return 0;
        }
        //=>calculate map keys
        for (uint32 i = 0; i < mapl; i++){
            keys[i]=RUNKIT_calculate_value(keys[i],"string",&subtype,false);
            //=>check for error
            if(keys[i]==0 && EXP_check_errcode(INVALID_VALUE_ERRC)){
                EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
                return 0;
            }
        }
        //=>when first value is checked, the other items use this!
        Boolean value_is_maporlist=false;
        //=>calculate map values
        // printf("PPP:%s\n",SLIST_print(values,mapl));
        for (uint32 i = 0; i < mapl; i++){
            //=>if value item is a list or map
            if(value_is_maporlist || values[i][0]=='[' || values[i][0]=='{'){
                values[i]=RUNKIT_calculate_listormap(values[i],type);
                //=>if an error to calculate of list or map
                if(EXP_check_errcode(BAD_MAPORLIST_VAL_ERRC)){
                    EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
                    return 0;
                }
                value_is_maporlist=true;
            }
            //=>if value item is simple
            else{
                values[i]=RUNKIT_calculate_value(values[i],type,&subtype,false);
                //=>check for error
                if(values[i]==0 && EXP_check_errcode(INVALID_VALUE_ERRC)){
                    EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
                    return 0;
                }
            }
        }
        //=>create ret string from keys and values
        ret = CH_append(ret, '{');
        for (uint32 i = 0; i < mapl; i++) {
            ret = STR_multi_append(ret, keys[i],":",values[i],0,0);
            if (i + 1 < mapl) ret = CH_append(ret, ',');
        }
        ret = CH_append(ret, '}');
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
        //=>calculate list items
        for (uint32 i = 0; i < listl; i++){
            //=>if value item is a list or map
            if(value_is_maporlist || items[i][0]=='[' || items[i][0]=='{'){
                items[i]=RUNKIT_calculate_listormap(items[i],type);
                //=>if an error to calculate of list or map
                if(EXP_check_errcode(BAD_MAPORLIST_VAL_ERRC)){
                    EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
                    return 0;
                }
                value_is_maporlist=true;
            }
            //=>if item is simple
            else{
                items[i]=RUNKIT_calculate_value(items[i],type,&subtype,false);
                //=>check for error
                if(items[i]==0 && EXP_check_errcode(INVALID_VALUE_ERRC)){
                    EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
                    return 0;
                }
            }
            // printf("GGGG:%s\n",items[i]);
        }
        //=>create ret string from items
        ret = CH_append(ret, '[');
        for (uint32 i = 0; i < listl; i++) {
            ret = STR_append(ret, items[i]);
            if (i + 1 < listl) ret = CH_append(ret, ',');
        }
        ret = CH_append(ret, ']');
    }
    //=>if not, then error!
    else{
        EXP_set_errcode(BAD_MAPORLIST_VAL_ERRC);
        return 0;
    }
    return ret;
}
//******************************************************
/**
 * get a value string and its type like number and then if need simplify value and return sub_type of value
 * @author madkne
 * @version 1.2
 * @since 2019.12.29
 * @param value
 * @param type
 * @param ret_subtype : (pointer)
 * @param ismaporlist
 * @return String
 */
String RUNKIT_calculate_value(String value, String type, uint8 *ret_subtype,Boolean ismaporlist) {
    //=>init vars
    uint8 sub_type = '0';
    //  printf("@@@@:%s,%s\n", value, type);
    //=>if value is null
    if (type == 0) {
        EXP_set_errcode(INVALID_VALUE_ERRC);
        return 0;
    }
    //=>if value is a list or map, then simplify every items
    if(ismaporlist && (value[0]=='[' || value[0]=='{')){
        STR_init(&value,RUNKIT_calculate_listormap(value,type));
        //=>if an error to calculate of list or map
        if(EXP_check_errcode(BAD_MAPORLIST_VAL_ERRC)){
            EXP_set_errcode(INVALID_VALUE_ERRC);
            return 0;
        }
        (*ret_subtype)=type[0];
        return value;
    }
    //=>search for basic types
    uint32 basic_ind=STR_search_index(basic_types, type, StrArraySize(basic_types));
    //=>if type is a number,string,boolean
    if (basic_ind!=-1) {
        //=>if value is a variable
        if (RUNKIT_is_valid_name(value, true)) {
            //TODO:
            //     Mpoint m = return_var_memory_value(value);
            //     if (m.id > 0) {
            //         if (!str_equal(convert_sub_type_to_type(m.type_data), type)) {
            //         //TODO:error
            //         printf("#ERR3567:%s,%c,%s\n", value, m.type_data, type);
            //         return;
            //         }
            // //        printf("!!!:%s,%i;%s\n",value,m.id,m.data);
            //         (*ret_value) = str_reomve_quotations(m.data, type);
            //         (*ret_subtype) = m.type_data;
            //         return;
            //     }
        }
        if (basic_ind==1/*number*/) {
            return RUNKIT_calc_number_exp(value, '_', &(*ret_subtype));
        } else if (basic_ind==0/*string*/) {
            return STR_convert_from(RUNKIT_calc_string_exp(value,&(*ret_subtype)));
        } else if (basic_ind==2/*boolean*/) {
            (*ret_subtype)='b';
            return RUNKIT_calc_boolean_exp(value);
        }  
    }
    //=>if type is a package
    else if (_datas_search(type,0,true).id != 0) {
        //TODO:
        // (*ret_value) = calculate_struct_expression(value, type, &sub_type);
        // (*ret_subtype) = sub_type;
    } 
    else {
        EXP_set_errcode(INVALID_TYPE_ERRC);
        return 0;
    }
}
//******************************************************
/**
 * get an math expression with a target type and calculate it and return it.
 * @author madkne
 * @version 1.2
 * @since 2019.12.29
 * @order unknown
 * @param exp
 * @param target_type
 * @param rettype
 * @return String : solved value number or null if faild
 */
String RUNKIT_calc_number_exp(String exp, uint8 target_type, uint8 *rettype){
    //=>init vars
    struct math_tree{
        uint32 id; uint8 op; String num1; String num2; String res;
    } nodes[MAX_MATH_USED_OPERANDS];
    uint32 nodes_len=0;
    uint32 pars_level=0;
    uint32 max_pars_level=0;
    Boolean exist=true;
    map* queue_start=0;
    map* queue_end=0;
    uint8 type_exp=target_type;
    //=>trim exp
    exp = STR_trim_space(exp);
    //=>if exp is '0'
    if (exp==0) {
        EXP_set_errcode(INVALID_NUMBER_VAL_ERRC);
        return 0;
    }
    //=>get length of exp
    uint32 len_exp = STR_length(exp);
    //=>if exp length is 1 and not a digit!
    if (len_exp == 1 && (exp[0] < '0' || exp[0] > '9')) {
        EXP_set_errcode(INVALID_NUMBER_VAL_ERRC);
        return 0;
    }
    //=>if exp length is 1 and is a digit!
    else if(len_exp == 1){
        if(rettype!=0) (*rettype) = 'i';
        return exp;
    }
    //=>if exp just a number
    if(STR_is_num(exp)){
        if(rettype!=0){
            (*rettype)=RUNKIT_determine_type_number(exp);
            return RUNKIT_append_number_subtype(exp,(*rettype));
        }
    }
    //=>calc max pars level
    for (uint32 i = 0; i < len_exp; i++){
        //=>count pars
        if(exp[i]=='(')max_pars_level++;
    }
    //=>append exp as first item in queue
    _map_push(&queue_start,&queue_end,"0",exp);
    //=>level 1 : create math tree
    while(queue_start!=0){
        //=>reset vars
        exist=false;
        uint8 lessop='0';
        uint32 lessop_ind=0;
        uint8 pars=0;
        Boolean isexp1=false;
        map expp=_map_popleft(&queue_start,&queue_end);
        String ex=expp.value;
        uint32 len_ex=STR_length(ex);
        //=>if exp has ()
        if(ex[0]=='(' && ex[len_ex-1]==')'){
            ex = RUNKIT_remove_unused_parenthesis(ex);
        }
        //=>find less operand and lastest on pars_level!
        for (uint32 i = 0; i < len_ex; i++){
            //=>count pars
            if(ex[i]=='(')pars++;
            else if(ex[i]==')')pars--;
            //=>if find '-' or '+' as less operands
            if(pars_level==pars && (ex[i]=='-'||ex[i]=='+')){
                lessop=ex[i];
                lessop_ind=i;
                break;
            }
            //=>if find '*' or... set, nut until lastest!
            if(pars_level==pars && CH_search(single_operators, ex[i],ChArraySize(single_operators))){
                lessop=ex[i];
                lessop_ind=i;
            }
        }
        //=>if not found any operands, increase pars_level
        if(lessop=='0'){
            pars_level++;
            //=>check if pars level is bigger than max
            if(pars_level>max_pars_level){
                break;
            }
            //=>push back expp to queue
            _map_push(&queue_start,&queue_end,expp.key,ex);
        }
        //=>if found less operand
        else{
            //=>get num1 string before less operand index
            String num1=STR_substring(ex,0,lessop_ind);
            uint8 subtyp1=type_exp;
            //=>check if num1 is number or var
            String num11=RUNKIT_determine_number(num1,type_exp,&subtyp1);
            //=>if not a number (is a expression), push to stack and set num1 as next id
            if(num11==0&&EXP_check_errcode(INVALID_NUMBER_VAL_ERRC)){
                _map_push(&queue_start,&queue_end,STR_from_int32(nodes_len+1),num1);
                num1=STR_append("#",STR_from_int32(nodes_len+1));
                isexp1=true;
            }else{
                STR_init(&num1,num11);
            }
            //=>get num2 string after less operand index
            String num2=STR_substring(ex,lessop_ind+1,len_ex);
            uint8 subtyp2=type_exp;
            //=>check if num2 is number or var
            String num22=RUNKIT_determine_number(num2,type_exp,&subtyp2);
            //=>if not a number (is a expression), push to stack and set num1 as next id
            if(num22==0&&EXP_check_errcode(INVALID_NUMBER_VAL_ERRC)){
                uint32 count=(isexp1)?nodes_len+2:nodes_len+1;
                _map_push(&queue_start,&queue_end,STR_from_int32(count),num2);
                num2=STR_append("#",STR_from_int32(count));
            }else{
                STR_init(&num2,num22);
            }
            //=>detrmine type_exp, if subtyp1 or subtyp2 is not '_'
            if(subtyp1 != '_' || subtyp2 != '_') {
                type_exp=RUNKIT_priority_number_types(type_exp,subtyp1);
                type_exp = RUNKIT_priority_number_types(type_exp, subtyp2);
            }
            struct math_tree tmp1={(uint32)STR_to_int32(expp.key),lessop,num1,num2};
            nodes[nodes_len++]=tmp1;
        }
        // printf("queue is :%s[%i]\n",_map_print(queue_start),pars_level);
    }
    // printf("queue is :%s[%i]\n",_map_print(queue_start),pars_level);
    //=>level 2 : solve math tree
    //=>iterate nodes from last
    for (uint32 i = nodes_len-1; i >=0; i--){
        if(i==-1)break;
        //=>get num1,num2
        String num1=nodes[i].num1;
        String num2=nodes[i].num2;
        //=>check if num1 or num2 is id
        if(num1[0]=='#'){
            int32 ind=STR_to_int32(STR_substring(num1,1,0));
            STR_init(&num1,nodes[ind].res);
        }
        if(num2[0]=='#'){
            int32 ind=STR_to_int32(STR_substring(num2,1,0));
            STR_init(&num2,nodes[ind].res);
        }
        //=>calculate num1,num2 with op
        String result=RUNKIT_calc_two_numbers(num1,num2,nodes[i].op,type_exp);
        //=>resize result
        if (type_exp == 'i' && (STR_length(result) > MAX_INT_LEN || STR_indexof(result, ".", 0) >= 0)) {
            result = RUNKIT_resize_to_int(result);
        } else if (type_exp == 'f' && STR_length(result) > MAX_FLOAT_LEN) {
            result = RUNKIT_resize_to_float(result);
        }
        // printf("RRRR(%i):%s{%c}%s=%s[%c]\n",nodes[i].id,num1,nodes[i].op,num2,result,type_exp);
        nodes[i].res=result;
    }
    //=>print math tree
    // for (uint32 i = 0; i < nodes_len; i++){
        // printf("(%i) %c : %s , %s = %s\n",nodes[i].id,nodes[i].op,nodes[i].num1,nodes[i].num2,nodes[i].res);
    // }
    //=>set sub type on last of result
    nodes[0].res= RUNKIT_append_number_subtype(nodes[0].res,type_exp);
    //=>return sub type and value
    if(rettype!=0) (*rettype) = type_exp;
    return nodes[0].res;
}
//******************************************************
/**
 * get a string and check if is a number or a var(number type) and return its value and sub_type
 * @author madkne
 * @version 1.0
 * @since 2019.12.25
 * @param s
 * @param main_type
 * @param sub_type : (pointer)
 * @return String
 */
String RUNKIT_determine_number(String s,uint8 main_type,uint8 *sub_type){
    //=>init vars
    Boolean is_valid_val=false;
    //=>check if is number
    if (STR_is_num(s)) {
        is_valid_val = true;
        (*sub_type) = RUNKIT_determine_type_numbers(&s, 0);
        //=>check if number is hex,bin,oct
        if (STR_indexof(s, "0x", 0) >= 0 || STR_indexof(s, "0b", 0) >= 0 || STR_indexof(s, "0o", 0) >= 0) {
            s = RUNKIT_is_radix_number(s, (*sub_type));
        }
    } 
    //=>if s is a var
    else {
        //TODO:in runtime
        // Mpoint mpoint = return_var_data_from_name(buf, "num", true);
        // if (mpoint.id == 0) {
        // (*rettype) = '0';
        // return;
        // }
        // str_init(&num1, mpoint.data);
        // is_valid_val = true;
        // type_exp = RUNKIT_priority_number_types(type_exp, mpoint.type_data);
        // ty1 = type_exp;
    }
    //=>if s is not valid as a number
    if (!is_valid_val) {
        EXP_set_errcode(INVALID_NUMBER_VAL_ERRC);
        return 0;
    }
    return s;
}

//******************************************************
/**
 * get a string and check if is a string or a utf8 or a var(string type) and return its value and sub_type
 * @author madkne
 * @version 1.1
 * @since 2019.12.27
 * @param s
 * @param sub_type : (pointer)
 * @return String
 */
String RUNKIT_determine_string(String s,uint8 *sub_type){
    //=>init vars
    Boolean is_valid_val=false;
    uint32 len=STR_length(s);
    Boolean isstr=false;
    (*sub_type)='s';
    //=>check if is string like "hello"
    if (len > 1){
        for (uint32 i = 0; i < len; i++){
            //=>check if is string
            if (i < len && s[i] == '\"' && (i == 0 || s[i - 1] != '\\')){
                isstr = BOOL_switch(isstr);
                //=>if get "" and is end of string
                if(!isstr && i+1==len){
                    s = STR_substring(s, 1, len - 1);
                    is_valid_val = true;
                    break;
                }
                //=>else not a string with "", maybe ""+""
                else if(!isstr){
                    break;
                }
            }
        }
        
    }
    //=>check if is utf8 string 
    else if (STR_indexof(s, UTF8_ID_LABEL, 0) == 0) {
        (*sub_type)='u';
        is_valid_val = true;
        s = USTR_to_bytes_str(_utst_get_by_label(s).utf8_string);
    } 
    else {
           //TODO:in runtime
    //     Mpoint point = return_var_data_from_name(buf, "str", true);
    //     if (point.id == 0) {
    //       str_init(&(*value), 0);
    //       (*sub_type) = '0';
    //       return;
    //     }
    //     str_init(&str1, point.data);
    //     is_valid_val = true;
    //     if (point.type_data == 'u') {
    //       type = 'u';
    //       str1 = utf8_to_bytes_string(get_utst_by_label(str1).utf8_string);
    //     }
    }
    //=>if s is not valid as a string
    if (!is_valid_val) {
        EXP_set_errcode(INVALID_STRING_VAL_ERRC);
        return 0;
    }
    return s;
}

//******************************************************
/**
 * get a string and check if is a boolean or a var(boolean type) and return its value
 * @author madkne
 * @version 1.1
 * @since 2019.12.27
 * @param s
 * @return String
 */
String RUNKIT_determine_boolean(String s){
    //=>init vars
    Boolean is_valid_val=false;
    uint32 len=STR_length(s);
    //=>trim s
    s=STR_trim_space(s);
    //=>check if is true or false
    if(STR_is_bool(s)){
        is_valid_val = true;
    }
    //=>check if is 'not true' or 'not false'
    else if(STR_equal_indexof(s,"not",0)){
        StrList ret=0;
        uint32 ret_len=CH_split(s,' ',&ret,true);
        if(ret_len==2 && STR_equal(ret[1],"true")){
            STR_init(&s,"false");
            is_valid_val = true;
        }else if(ret_len==2 && STR_equal(ret[1],"false")){
            STR_init(&s,"true");
            is_valid_val = true;
        }
    }
    //=>check if is a var
    else {
           //TODO:in runtime
    //     Mpoint point = return_var_data_from_name(buf, "str", true);
    //     if (point.id == 0) {
    //       str_init(&(*value), 0);
    //       (*sub_type) = '0';
    //       return;
    //     }
    //     str_init(&str1, point.data);
    //     is_valid_val = true;
    //     if (point.type_data == 'u') {
    //       type = 'u';
    //       str1 = utf8_to_bytes_string(get_utst_by_label(str1).utf8_string);
    //     }
    //     //msg("&CCDDD:", str1, buf, string(tmp1))
    }
    //=>if s is not valid as a boolean
    if (!is_valid_val) {
        EXP_set_errcode(INVALID_BOOLEAN_VAL_ERRC);
        return 0;
    }
    return s;
}
//******************************************************
/**
 * determine a string is a radix number and then convert it to decimal number and return it
 * @version 1.0
 * @author madkne
 * @since 2019.12.25
 * @param buf
 * @param type_exp
 * @return String
 */
String RUNKIT_is_radix_number(String buf, uint8 type_exp) {
  Boolean is_neg = false;
  if (STR_length(buf) > 1 && (buf[0] == '-' || buf[0] == '+')) {
    if (buf[0] == '-') {
      is_neg = true;
    }
    buf = STR_substring(buf, 1, 0);
  }
  if (STR_indexof(buf, "0b", 0) == 0)
    buf = RUNKIT_radix_to_dec(buf, 2, type_exp);
  else if (STR_indexof(buf, "0o", 0) == 0)
    buf = RUNKIT_radix_to_dec(buf, 8, type_exp);
  else if (STR_indexof(buf, "0x", 0) == 0)
    buf = RUNKIT_radix_to_dec(buf, 16, type_exp);

  if (is_neg) {
    buf = STR_multi_append("-", buf, 0, 0, 0, 0);
  }
  return buf;
}
//******************************************************
/**
 * get a string base and convert it based on radix number and type char and then return a string decimal
 * @version 1.0
 * @author madkne
 * @since 2019.12.25
 * @param base
 * @param radix
 * @param type_exp
 * @return String
 */
String RUNKIT_radix_to_dec(String base, uint8 radix, uint8 type_exp) {
  //msg("&^^", base, radix)
  String final = 0, int_s = 0, float_s = 0;
  uint8 start_x, after_point = 1;
  Boolean is_point = false, is_base = false;
  double res_ii = 0, res_ff = 0;
  String nmb = 0;
  switch (radix) {
    case 2: start_x = 'b';
      STR_init(&nmb, "01");
      break;
    case 8: start_x = 'o';
      STR_init(&nmb, "01234567");
      break;
    case 16: start_x = 'x';
      STR_init(&nmb, "0123456789abcdef");
      break;
  }

  //*******************
  if (base[0] == '-') {
    final = CH_append(final, '-');
  }
  base = STR_to_lower_case(base);
  //printf("JJJJ:%s\n",base);
  //*******************split int and float of radix number to int_s,float_s
  uint32 len_base = STR_length(base);
  for (uint32 i = 0; i < len_base; i++) {
    if (base[i] == '.') {
      is_point = true;
      continue;
    }
    if (base[i] == start_x && i > 0 && base[i - 1] == '0') {
      is_base = true;
      continue;
    }
    if (!is_point && is_base && CH_search(nmb, base[i],radix)) {
      int_s = CH_append(int_s, base[i]);
    } else if (is_point && is_base && CH_search(nmb, base[i],radix)) {
      float_s = CH_append(float_s, base[i]);
    }
  }

  //*******************convert int_s to decimal
  int32 b = STR_length(int_s) - 1;
  for (uint32 i = 0; i < STR_length(int_s); i++) {
    double n3 = I32_power(radix, b);
    b--;

    int32 n4 = CH_search_index(nmb, int_s[i],radix);
    res_ii += (n4 * n3);
  }
  //******************convert float_s radix to decimal
  if (type_exp != 'i' && type_exp != '_' && type_exp != '0') {
    b = -1;
    for (uint32 i = 0; i < STR_length(float_s); i++) {
      double n3 = I32_power(radix, b);
      b--;
      int32 n4 = CH_search_index(nmb, float_s[i],radix);
      res_ff += (n4 * n3);
      //printf("GGGGG:%i,%i,%f#%i,%f=>%f\n",radix, b,n3,n4,n4 * n3,res_ff);
    }
    res_ii += res_ff;
    switch (radix) {
      case 2: after_point = (uint8) STR_length(float_s);
        break;
      case 8: after_point = (uint8) STR_length(float_s);
        break;
      case 16: after_point = (uint8) STR_length(float_s) * 2;
        break;
    }
  }
  //*****************
  String fin = STR_from_double(res_ii, after_point);
  //printf("CCCCC:%s,%s=>%f,%i,%s\n", int_s, float_s, res_ii, str_length(float_s), fin);
  final = STR_append(final, fin);
  return final;
}
//******************************************************
/**
 * get num1,num2 and determine their sub_type and if need change num1,num2
 * @version 1.0
 * @author madkne
 * @since 2019.12.25
 * @param num11
 * @param num22
 * @return uint8
 */
uint8 RUNKIT_determine_type_numbers(String *num11, String *num22) {
    //=>init vars
    String num1 = 0;
    String num2 = 0;
    STR_init(&num1, (*num11));
    if (num22 != 0)STR_init(&num2, (*num22));
    uint8 type_exp = '0';
    Boolean is_hex1 = false, is_hex2 = false;
    //=>determine length of num1,num2
    uint32 num1_len = STR_length(num1);
    uint32 num2_len = (num22 != 0) ? STR_length(num2) : 0;
    //=>check num1 and num2 is hex
    if (num1_len > 2 && num1[0] == '0' && num1[1] == 'x') {
        is_hex1 = true;
    } else if (num2_len > 2 && num2[0] == '0' && num2[1] == 'x') {
        is_hex2 = true;
    }
    //=>determine type of num1,num2
    uint8 num2_t = '0';
    uint8 num1_t = RUNKIT_determine_type_number(num1);
    if (num2 != 0) {
        num2_t = RUNKIT_determine_type_number(num2);
    }
    //=>if type of any num1,num2 is '0', return '0'
    if (num1_t == '0' || (num2_t == '0' && num2 != 0)) {
        STR_init(&(*num11), num1);
        if (num22 != 0)STR_init(&(*num22), num2);
        return '0';
    } 
    //=>if type of num1 or num2 is 'h' 
    else if (num1_t == 'h' || num2_t == 'h')
        type_exp = 'h';
    //=>if type of num1 or num2 is 'f' 
    else if (num1_t == 'f' || num2_t == 'f')
        type_exp = 'f';
    else type_exp = 'i';
    //=>if num1 not hex and last is 'i' or 'f' or 'h'
    if (num1_len > 0 && !is_hex1 &&
        ((num1[num1_len - 1] == 'i' && type_exp == 'i') || (num1[num1_len - 1] == 'f' && type_exp == 'f') ||
            (num1[num1_len - 1] == 'h' && type_exp == 'h'))) {
        num1 = STR_substring(num1, 0, num1_len - 1);
    }
    //=>if num2 not hex and last is 'i' or 'f' or 'h'
    if (num2_len > 0 && !is_hex2 &&
        ((num2[num2_len - 1] == 'i' && type_exp == 'i') || (num2[num2_len - 1] == 'f' && type_exp == 'f') ||
            (num2[num2_len - 1] == 'h' && type_exp == 'h'))) {
        num2 = STR_substring(num2, 0, num2_len - 1);
    }
    //printf("DDFFFF:%s,%i\n", num1, num2_len);
    //msg("&XXX", string(type_exp), num1, num2, string(num1_t), string(num2_t))
    STR_init(&(*num11), num1);
    if (num22 != 0)STR_init(&(*num22), num2);
    return type_exp;
}
//******************************************************
/**
 * get a num and determine its sub_type and return 'i','f','h' or '0'
 * @version 1.0
 * @author madkne
 * @since 2019.12.25
 * @param num
 * @return uint8
 */
uint8 RUNKIT_determine_type_number(String num) {
    //=>init vars
    uint8 num_type = '0';
    Boolean is_hex = false;
    //=>get length of num
    uint32 len = STR_length(num);
    //=>var is_neg=false
    if (len > 3 && (num[0] == '-' || num[0] == '+') && num[1] == '0' &&
        (num[2] == 'b' || num[2] == 'o' || num[2] == 'x')) {
        if (num[0] == '-') {
        //is_neg=true
        }
        num = STR_substring(num, 1, 0);
    }
    //=>if num is null, return '0' as type
    if (len == 0) {
        return num_type;
    }
    //=>check num is hex
    if (len > 2 && STR_indexof(num, "0x", 0) == 0) {
        is_hex = true;
    }
    //=>analyze hexdecimal
    if (is_hex) {
        int32 ind = STR_indexof(num, ".", 0);
        if (ind == -1) {
            return 'd';
        } else {
            return 'i';
        }
    }
    //=>analyze 1 : if num not hex and has 'i'or 'f' or 'h' on last
    if (len > 1 && !is_hex && (num[len - 2] >= '0' && num[len - 2] <= '9')) {
        if(num[len - 1]=='i'||num[len - 1]=='f'||num[len - 1]=='h')
            return num[len - 1];
    }
    //=>if num not number
    if (!STR_is_num(num)) {
        return '0';
    }
    //=>analyze 2 
    int32 before = 0, after = 0, max_before = 0, max_after = 0;
    Boolean is_point = false;
    //=>iterate chars on num
    for (uint32 i = 0; i < len; i++) {
        //=>check for '.'
        if (num[i] == '.') {
            is_point = true;
            continue;
        }
        if (num[i] == ',') {
            if (after > max_after) {
                max_after = after;
            }
            if (before > max_before) {
                max_before = before;
            }
            after = 0, before = 0;
            is_point = false;
            continue;
        }
        if (!is_point && num[i] >= '0' && num[i] <= '9') {
            before++;
        } else if (is_point && num[i] >= '0' && num[i] <= '9') {
            after++;
        }
    }
    if (after > max_after) {
        max_after = after;
    }
    if (before > max_before) {
        max_before = before;
    }
    //=>if num length less than MAX_INT_LEN
    if (max_after == 0 && max_before < MAX_INT_LEN)
        return 'i';
    //=>if num length less than MAX_FLOAT_LEN
    if (max_after > 0 && max_before + max_after < MAX_FLOAT_LEN)
        return 'f';
    else
        return 'h';
}
//******************************************************
/**
 * get expression string and remove its unused parenthesis
 * @version 1.1
 * @author madkne
 * @since 2019.12.25
 * @param value
 * @return String
 */
String RUNKIT_remove_unused_parenthesis(String value) {
    /**
    - (56+6) => 56+6
    - ((5+5)+6) => (5+5)+6
    - ((6-7)*(6-6)) => (6-7)*(6-6)
    - (5-7)*(6*9) => (5-7)*(6*9)
    - (((5*7)+3)) => (5*7)+3
    */
    //=>init vars
    Boolean exist=true;
    while(exist){
        exist=false;
        //=>get length of value
        uint32 len = STR_length(value);
        //=>if value has ()
        if (len > 2 && value[0] == '(' && value[len - 1] == ')') {
            Boolean is_remove = false, is_string = false, is_br = false;
            uint8 pars = 0;
            //=>check if is string
            for (uint32 i = 0; i < len; i++) {
                if (value[i] == '\"' && (i == 0 || value[i - 1] != '\\')) {
                    is_string = BOOL_switch(is_string);
                }
                //=>count pars
                if (!is_string && value[i] == '(')pars++;
                else if (!is_string && value[i] == ')') {
                    pars--;
                    //=>check if can remove ()
                    if (i + 1 == len && pars == 0) {
                        is_remove = true;
                        is_br = true;
                    } else if (i + 1 < len && pars == 0) {
                        is_br = true;
                    }
                }
                if (is_br) {
                    break;
                }
            }
            if (is_remove) {
                exist=true;
                value = STR_substring(value, 1, len - 1);
            }
        }
    }
    return value;
}
//******************************************************
/**
 * get a value like 'H'*3 , jj[4] , {'q1':true and false} , 4.5+12 and detect main data type and its sub type if possible!
 * @author madkne
 * @version 1.1
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
    String name=0,index=0;
    //=>check if value not null
    if(value==0){
        EXP_set_errcode(INVALID_VALUE_ERRC);
        return 0;
    }
    //=>if value is a list or map, then just get its first item
    if(value[0]=='[' || value[0]=='{'){
        STR_init(&value,RUNKIT_get_firstitem_listormap(value));
        //=>if an error to get first item of list or map
        if(EXP_check_errcode(BAD_FIRST_ITEM_ERRC)){
            EXP_set_errcode(INVALID_VALUE_ERRC);
            return 0;
        }
    }
    //=>if value is a var
    if(RUNKIT_is_valid_name(value,true)){
        //=>split name and index
        String index=RUNKIT_get_name_index_var(value,true,&name);
        //TODO:
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
            if(CH_search(single_operators, value[i],ChArraySize(single_operators)) && STR_is_num(word)){
                STR_init(&final_type, "number");
                final_sub='n';
            }
            //=>is boolean operators like and , or
            if (!is_string && STR_search(boolean_operators, word,StrArraySize(boolean_operators))) {
                STR_init(&final_type, "boolean");
                final_sub='b';
            }
            if (!is_string && STR_is_bool(word)) {
                STR_init(&final_type, "boolean");
                final_sub='b';
            }
            //=>append to word
            if (!is_string && (CH_search(words_splitter, value[i],ChArraySize(single_operators)) || CH_search(single_operators, value[i],ChArraySize(single_operators)))) {
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
            if(mapl==0 && EXP_check_errcode(BAD_MAP_ERRC)){
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
            if(listl==0 && EXP_check_errcode(BAD_LIST_ERRC)){
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
/**
 * get a value that is list and split its items
 * @author madkne
 * @version 1.0
 * @since 2019.12.20
 */ 
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
/**
 * get a value that is map and split its values and keys
 * @author madkne
 * @version 1.0
 * @since 2019.12.20
 */ 
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
/**
 * get two sub_types and comparison and priority for them
 * @author madkne
 * @version 1.1
 * @since 2019.12.25
 * @param type_exp
 * @param tmp1
 * @return uint8
 */
uint8 RUNKIT_priority_number_types(uint8 type_exp, uint8 tmp1) {
    if(type_exp=='_') return tmp1;
    if(tmp1=='_') return type_exp;
    if (tmp1 == 'h' || type_exp == 'h') return 'h';
    if (tmp1 == 'f' || type_exp == 'f') return 'f';
    if (tmp1 == 'i' && type_exp == 'i') return 'i';
    return 'i';
}
//******************************************************
/**
 * get two string numbers and based of operator and sub_type(i,f,h),calculate their and return result of it
 * @author madkne
 * @version 1.1
 * @since 2019.12.25
 * @param num1
 * @param num2
 * @param op
 * @param type_exp
 * @return String
 */
String RUNKIT_calc_two_numbers(String num1, String num2, uint8 op, uint8 type_exp) {
    //=>init vars
    String result = 0;
    STR_init(&result, "0");
    //=>search for errors and warnings
    if (op == '/' && STR_equal(num2, "0")) {
        EXP_handler("zero_division", __func__, STR_multi_append(num1, "/", num2, 0, 0, 0), 0);
        return "0";
    } else if (op == '%' && (type_exp == 'f' || type_exp == 'h')) {
        //com_type, _, _ := fitting_value(string(type_exp), "", "com_type")
        //exception_handler("wrong_remainder", __func__, com_type, "")
        //TODO:error
        return "0";
    }

    //=>calculate num1,num2
    //----------------int
    if (type_exp == 'i') {
        num1 = RUNKIT_resize_to_int(num1);
        num2 = RUNKIT_resize_to_int(num2);
        int32 n1 = STR_to_int32(num1);
        int32 n2 = STR_to_int32(num2);

        if (op == '/' && n2 == 0) {
            EXP_handler("zero_division", __func__, STR_multi_append(num1, "/", num2, 0, 0, 0), "");
            return "0";
        }
        //=>operands on int numbers
        switch (op) {
            case '+': result = STR_from_int32(n1 + n2);
                break;
            case '-': result = STR_from_int32(n1 - n2);
                break;
            case '*': result = STR_from_int32(n1 * n2);
                break;
            case '/':
            case '|':
            result = STR_from_int32(n1 / n2);
                break;
            case '%': result = STR_from_int32(n1 % n2);
                break;
            case '^': {
                double ret = I32_power((double) n1, n2);
                if (ret > MAX_INT_NUMBER) {
                    EXP_handler("out_of_range_integer", "calculate_two_numbers",STR_multi_append(num1, "^", num2, 0, 0, 0), "");
                    return "0";
                }
                //printf("DDDDDDD:%li\n",ret);
                result = STR_from_int32((int32) ret);
                break;
            }
        }
    }
    //----------------float
    else if (type_exp == 'f') {
        double n1 = STR_to_double(num1);
        double n2 = STR_to_double(num2);
        switch (op) {
            case '+': result = STR_from_double(n1 + n2, (uint8) STR_length(num1) / 2);
                break;
            case '-': result = STR_from_double(n1 - n2, (uint8) STR_length(num1) / 2);
                break;
            case '*': result = STR_from_double(n1 * n2, (uint8) STR_length(num1));
                break;
            case '/': result = STR_from_double(n1 / n2, (uint8) STR_length(num1));
                break;
            case '|': result = STR_from_int64((int32)n1 / (int32)n2);
                break;
            case '^': {
                double ret = I32_power(n1, (int32) n2);
                if (ret > MAX_FLOAT_NUMBER) {
                    EXP_handler("out_of_range_float", __func__, STR_multi_append(num1, "^", num2, 0, 0, 0), "");
                    return "0";
                }
                //printf("DDDDDDD:%f\n",ret);
                result = STR_from_double(ret, (uint8) (STR_length(num1) * 2));
                break;
            }
        }
    }
    //----------------huge
    else if (type_exp == 'h') {
        //TODO:
        //printf("&HHH:%s,%s,%c\n", num1, num2, op);
        // switch (op) {
        // case '+': result = sum_huge_numbers(num1, num2, false);
        //     break;
        // case '-': result = sum_huge_numbers(num1, num2, true);
        //     break;
        // case '*': result = muliply_huge_numbers(num1, num2);
        //     break;
        // case '/': result = divide_huge_numbers(num1, num2);
        //     break;
        // case '^': {
        //     str_detachment_float(num2, &num2, 0);
        //     int32 n2 = str_to_int32(num2);
        //     //printf("$$RR:%s=>%i\n",num2,n2);
        //     Boolean is_neg = false;
        //     int32 b = 0;
        //     String res = 0;
        //     str_init(&res, "1");
        //     if (n2 < 0) {
        //     is_neg = true;
        //     n2 = -n2;
        //     }
        //     for (uint32 b = 0; b < n2; b++) {
        //     res = muliply_huge_numbers(res, num1);
        //     }
        //     if (is_neg) {
        //     res = divide_huge_numbers("1", res);
        //     }
        //     str_init(&result, res);
        //     break;
        // }
        // }
    }

    return result;
}
//******************************************************
/**
 * get a string number and resize it to integer number
 * @author madkne
 * @version 1.0
 * @since 2019.12.25
 * @param str_val
 * @return String
 */
String RUNKIT_resize_to_int(String str_val) {
  String fin_res = 0;
  for (uint32 i = 0; i < STR_length(str_val); i++) {
    if (str_val[i] == '.' || STR_length(fin_res) > MAX_INT_LEN) {
      break;
    }
    fin_res = CH_append(fin_res, str_val[i]);
  }
  return fin_res;
}

//******************************************************
/**
 * get a string number and resize it to float number
 * @author madkne
 * @version 1.0
 * @since 2019.12.25
 * @param str_val
 * @return String
 */
String RUNKIT_resize_to_float(String str_val) {
  String fin_res = 0;
  uint32 count = 0;
  for (uint32 i = 0; i < STR_length(str_val); i++) {
    if (count >= MAX_FLOAT_LEN) {
      break;
    }
    if (str_val[i] != '-' && str_val[i] != '+' && str_val[i] != '.') {
      count++;
    }
    fin_res = CH_append(fin_res, str_val[i]);
  }
  if (fin_res[STR_length(fin_res) - 1] == '.') {
    fin_res = CH_append(fin_res, '0');
  }
  return fin_res;
}
//******************************************************
/**
 * get a string value or a string expression and processing it and then return final string value with sub type
 * string value can be an utf8 string like -!U8!_2
 * @author madkne
 * @version 1.1
 * @since 2019.12.29
 * @param exp
 * @param sub_type : (pointer)
 */
String RUNKIT_calc_string_exp(String exp, uint8 *sub_type){
    /**
	1- "Ind: %i[0]%\n" ---..---
	2- "Hello "+sd[0,1] ---..---
	3- ""+"Hi" ---..---
    4- ("H"+"H"+"i")-"HH" ---OK---
    5- -!U8!_2 ---..---
	*/
    //=>init vars
    struct str_exp_tree{
        uint32 id; uint8 op; String str1; String str2; String res;
    } nodes[MAX_STRING_USED_OPERANDS];
    uint32 nodes_len=0;
    uint32 pars_level=0;
    uint32 max_pars_level=0;
    Boolean exist=true;
    map* queue_start=0;
    map* queue_end=0;
    uint8 type_exp='s';
    uint8 tmp_subtype='s';
    //=>trim exp
    exp = STR_trim_space(exp);
    //=>if exp is null
    if (exp==0) {
        EXP_set_errcode(INVALID_STRING_VAL_ERRC);
        return 0;
    }
    //=>get length of exp
    uint32 len_exp = STR_length(exp);
    //=>if exp is ""
    if(STR_equal(exp, "\"\"")){
        if(sub_type!=0) (*sub_type) = 's';
        return 0;
    }
    //=>if exp is a string like "Hello" or a string var
    String tmp=RUNKIT_determine_string(exp,&tmp_subtype);
    if(tmp!=0){
        if(sub_type!=0) (*sub_type) = tmp_subtype;
        return tmp;
    }
    //=>calc max pars level
    for (uint32 i = 0; i < len_exp; i++){
        //=>count pars
        if(exp[i]=='(')max_pars_level++;
    }
    //=>append exp as first item in queue
    _map_push(&queue_start,&queue_end,"0",exp);
    //=>level 1 : create string tree
    while(queue_start!=0){
        //=>reset vars
        exist=false;
        uint8 lastop='0';
        uint32 lastop_ind=0;
        uint8 pars=0;
        Boolean isexp1=false;
        //=>pop from left of queue
        map expp=_map_popleft(&queue_start,&queue_end);
        String ex=expp.value;
        uint32 len_ex=STR_length(ex);
        //=>if exp has ()
        if(ex[0]=='(' && ex[len_ex-1]==')'){
            ex = RUNKIT_remove_unused_parenthesis(ex);
        }
        //=>find lastest operand on pars_level!
        for (uint32 i = 0; i < len_ex; i++){
            //=>count pars
            if(ex[i]=='(')pars++;
            else if(ex[i]==')')pars--;
            //=>if find '+' or '-' set, until lastest!
            if(pars_level==pars && (ex[i]=='+'||ex[i]=='-')){
                lastop=ex[i];
                lastop_ind=i;
            }
        }
        //=>if not found any operands, increase pars_level
        if(lastop=='0'){
            pars_level++;
            //=>check if pars level is bigger than max
            if(pars_level>max_pars_level){
                break;
            }
            //=>push back expp to queue
            _map_push(&queue_start,&queue_end,expp.key,ex);
        }
        //=>if found last operand
        else{
            //=>get str1 string before last operand index
            String str1=STR_substring(ex,0,lastop_ind);
            uint8 subtyp1=type_exp;
            //=>check if str1 is string or var
            String str11=RUNKIT_determine_string(str1,&subtyp1);
            //=>if not a string (is a expression), push to stack and set str1 as next id
            if(str11==0&&EXP_check_errcode(INVALID_STRING_VAL_ERRC)){
                _map_push(&queue_start,&queue_end,STR_from_int32(nodes_len+1),str1);
                str1=STR_append("#",STR_from_int32(nodes_len+1));
                isexp1=true;
            }else{
                STR_init(&str1,str11);
            }
            //=>get str2 string after last operand index
            String str2=STR_substring(ex,lastop_ind+1,len_ex);
            uint8 subtyp2=type_exp;
            //=>check if str2 is string or var
            String str22=RUNKIT_determine_string(str2,&subtyp2);
            //=>if not a string (is a expression), push to stack and set str1 as next id
            if(str22==0&&EXP_check_errcode(INVALID_STRING_VAL_ERRC)){
                uint32 count=(isexp1)?nodes_len+2:nodes_len+1;
                _map_push(&queue_start,&queue_end,STR_from_int32(count),str2);
                str2=STR_append("#",STR_from_int32(count));
            }else{
                STR_init(&str2,str22);
            }
            //=>detrmine type_exp, by subtyp1 and subtyp2
            type_exp=(subtyp1=='u'||subtyp2=='u')?'u':'s';
            struct str_exp_tree tmp1={(uint32)STR_to_int32(expp.key),lastop,str1,str2};
            nodes[nodes_len++]=tmp1;
        }
        // printf("queue is :%s[%i]\n",_map_print(queue_start),pars_level);
    }
    // printf("queue is :%s[%i]\n",_map_print(queue_start),pars_level);
    //=>level 2 : solve string tree
    //=>iterate nodes from last
    for (uint32 i = nodes_len-1; i >=0; i--){
        if(i==-1)break;
        //=>get str1,str2
        String str1=nodes[i].str1;
        String str2=nodes[i].str2;
        //=>check if str1 or str2 is id
        if(str1[0]=='#'){
            int32 ind=STR_to_int32(STR_substring(str1,1,0));
            STR_init(&str1,nodes[ind].res);
        }
        if(str2[0]=='#'){
            int32 ind=STR_to_int32(STR_substring(str2,1,0));
            STR_init(&str2,nodes[ind].res);
        }
        //=>calculate str1,str2 with op
        String result=RUNKIT_calc_two_strings(str1,str2,nodes[i].op);
        // printf("RRRR(%i):%s{%c}%s=%s[%c]\n",nodes[i].id,num1,nodes[i].op,num2,result,type_exp);
        nodes[i].res=result;
    }
    //=>print math tree
    // for (uint32 i = 0; i < nodes_len; i++){
    //     printf("(%i) %c : %s , %s = %s\n",nodes[i].id,nodes[i].op,nodes[i].str1,nodes[i].str2,nodes[i].res);
    // }
    //=>return sub type and value
    if(sub_type!=0) (*sub_type) = type_exp;
    return nodes[0].res;
}
//******************************************************
/**
 * get two strings and based of operator and sub_type(s,u),calculate their and return result of it
 * @author madkne
 * @version 1.0
 * @since 2019.12.27
 * @param str1
 * @param str2
 * @param op
 * @return String
 */
String RUNKIT_calc_two_strings(String str1, String str2, uint8 op) {
    //=>search for errors and warnings
    if (op != '-' && op !='+') {
        // TODO:error
        return 0;
    } 
    //=>format str1,str2 if have '%%'
    //TODO:runtime
    // if (str_indexof(str1, "%", 0) > -1) {
    //     str1 = format_string_expression(str1);
    //     //printf("DDDDD:%s,%s\n",str1,str2);
    //   }
    //   if (str_indexof(str2, "%", 0) > -1) {
    //     str2 = format_string_expression(str2);
    //   }
    //=>if op is '+'
    if(op=='+'){
        return STR_append(str1, str2);
    }
    //=>if op is '-'
    else if(op=='-'){
        uint32 len2=STR_length(str2);
        int32 ind=STR_last_indexof(str1,str2);
        //=>if find str2 in str1 from last
        if(ind!=-1){
            String s1=STR_substring(str1,0,ind);
            String s2=STR_substring(str1,ind+len2,0);
            return STR_append(s1, s2);
        }
        //=>if not found, return str1
        return str1;
    }

    return 0;
}
//******************************************************
/**
 * get a boolean expression and processing it and then return final boolean value
 * @author madkne
 * @version 1.0
 * @since 2019.12.27
 * @param exp
 */
String RUNKIT_calc_boolean_exp(String exp){
    /**
	1- true and (not false or (true and false)) ---OK---
	2- not(f==34) ---..---
	3- true and not (false and true) ---OK---
	4- false and not true or not false ---OK---
	5- (buf!="" not exp=="tr") or not(exp=="DF") ---..---
	*/
    //=>init vars
    struct bool_exp_tree{
        uint32 id; uint8 op; String bool1; Boolean bool1_not; String bool2; Boolean bool2_not; String res;
    } nodes[MAX_BOOLEAN_USED_OPERANDS];
    uint32 nodes_len=0;
    uint32 pars_level=0;
    uint32 max_pars_level=0;
    Boolean exist=true;
    map* queue_start=0;
    map* queue_end=0;
    //=>trim exp
    exp = STR_trim_space(exp);
    //=>if exp is null
    if (exp==0) {
        EXP_set_errcode(INVALID_BOOLEAN_VAL_ERRC);
        return 0;
    }
    //=>get length of exp
    uint32 len_exp = STR_length(exp);
    //=>if exp is true or false
    if(STR_is_bool(exp)){
        return exp;
    }
    //=>calc max pars level
    for (uint32 i = 0; i < len_exp; i++){
        //=>count pars
        if(exp[i]=='(')max_pars_level++;
    }
    //=>append exp as first item in queue
    _map_push(&queue_start,&queue_end,"0",exp);
    //=>level 1 : create string tree
    while(queue_start!=0){
        //=>reset vars
        exist=false;
        uint8 lastop='0';
        uint32 lastop_ind=0;
        uint8 pars=0;
        Boolean isexp1=false;
        Boolean bool1_not=false;
        Boolean bool2_not=false;
        //=>pop from left of queue
        map expp=_map_popleft(&queue_start,&queue_end);
        String ex=expp.value;
        //=>trim expp value
        ex=STR_trim_space(ex);
        uint32 len_ex=STR_length(ex);
        //=>if exp has ()
        if(ex[0]=='(' && ex[len_ex-1]==')'){
            ex = RUNKIT_remove_unused_parenthesis(ex);
        }
        //=>find lastest operand on pars_level!
        for (uint32 i = 0; i < len_ex; i++){
            //=>count pars
            if(ex[i]=='(')pars++;
            else if(ex[i]==')')pars--;
            //=>if find 'and' or 'or' set, until lastest!
            if (pars_level==pars && i + 3 <= len_ex && (STR_equal_indexof(ex,"or",i)||STR_equal_indexof(ex,"and",i))) {
                lastop=ex[i]; //'o' or 'a'
                lastop_ind=i;
                i+=(ex[i]=='o')?1:2;
            }
        }
        //=>if not found any operands, increase pars_level
        if(lastop=='0'){
            pars_level++;
            //=>check if pars level is bigger than max
            if(pars_level>max_pars_level){
                break;
            }
            //=>push back expp to queue
            _map_push(&queue_start,&queue_end,expp.key,ex);
        }
        //=>if found last operand
        else{
            //=>get bool1 string before last operand index
            String bool1=STR_trim_space(STR_substring(ex,0,lastop_ind));
            //=>check if bool1 is boolean or var
            String bool11=RUNKIT_determine_boolean(bool1);
            //=>if not a string (is a expression), push to stack and set bool1 as next id
            if(bool11==0&&EXP_check_errcode(INVALID_BOOLEAN_VAL_ERRC)){
                //=>check if not in bool1, remove it from experssion
                if(STR_equal_indexof(bool1,"not",0)&&(bool1[3]==' '||bool1[3]=='(')){
                    bool1=STR_substring(bool1,3,0);
                    bool1_not=true;
                }
                _map_push(&queue_start,&queue_end,STR_from_int32(nodes_len+1),bool1);
                bool1=STR_append("#",STR_from_int32(nodes_len+1));
                isexp1=true;
            }else{
                STR_init(&bool1,bool11);
            }
            //=>if 'and' indent is 3 and if 'or' is 2
            uint8 indent=(lastop=='a')?3:2;
            //=>get bool2 string after last operand index
            String bool2=STR_trim_space(STR_substring(ex,lastop_ind+indent,len_ex));
            //=>check if bool2 is string or var
            String bool22=RUNKIT_determine_boolean(bool2);
            //=>if not a string (is a expression), push to stack and set bool2 as next id
            if(bool22==0&&EXP_check_errcode(INVALID_BOOLEAN_VAL_ERRC)){
                //=>check if not in bool2, remove it from experssion
                if(STR_equal_indexof(bool2,"not",0)&&(bool2[3]==' '||bool2[3]=='(')){
                    bool2=STR_substring(bool2,3,0);
                    bool2_not=true;
                }
                uint32 count=(isexp1)?nodes_len+2:nodes_len+1;
                _map_push(&queue_start,&queue_end,STR_from_int32(count),bool2);
                bool2=STR_append("#",STR_from_int32(count));
            }else{
                STR_init(&bool2,bool22);
            }
            
            //=>append new node to tree
            struct bool_exp_tree tmp1={(uint32)STR_to_int32(expp.key),lastop,bool1,bool1_not,bool2,bool2_not,0};
            nodes[nodes_len++]=tmp1;
        }
        // printf("queue is :%s[%i]\n",_map_print(queue_start),pars_level);
    }
    // printf("queue is :%s[%i]\n",_map_print(queue_start),pars_level);
    //=>level 2 : solve string tree
    //=>iterate nodes from last
    for (uint32 i = nodes_len-1; i >=0; i--){
        if(i==-1)break;
        //=>get bool1,bool2
        String bool1=nodes[i].bool1;
        String bool2=nodes[i].bool2;
        //=>check if bool1 or bool2 is id
        if(bool1[0]=='#'){
            int32 ind=STR_to_int32(STR_substring(bool1,1,0));
            STR_init(&bool1,nodes[ind].res);
        }
        if(bool2[0]=='#'){
            int32 ind=STR_to_int32(STR_substring(bool2,1,0));
            STR_init(&bool2,nodes[ind].res);
        }
        //=>calculate bool1,bool2 with op
        Boolean b1=STR_to_bool(bool1);
        if(nodes[i].bool1_not)b1=!b1;
        Boolean b2=STR_to_bool(bool2);
        if(nodes[i].bool2_not)b2=!b2;
        if(nodes[i].op=='a'/*and*/){
            nodes[i].res=STR_from_bool(b1 && b2);
        }else if(nodes[i].op=='o'/*or*/){
            nodes[i].res=STR_from_bool(b1 || b2);
        }
    }
    //=>print math tree
    // for (uint32 i = 0; i < nodes_len; i++){
    //     printf("(%i) %c : %s(%i) , %s(%i) = %s\n",nodes[i].id,nodes[i].op,nodes[i].bool1,nodes[i].bool1_not,nodes[i].bool2,nodes[i].bool2_not,nodes[i].res);
    // }
    //=>return value
    return nodes[0].res;
}
//******************************************************
/**
 * get a a number and its sub type and check if not have last char, append sub type to it
 * @author madkne
 * @version 1.0
 * @since 2019.12.29
 * @param num
 * @param subtype
 * @return String
 */
String RUNKIT_append_number_subtype(String num,uint8 subtype){
    //=>check if num is decimal
    if(!STR_is_decimal(num)){
        return num;
    }
    //=>check if last char of num is 'i' or 'f' or 'h'
    uint32 len=STR_length(num);
    if(len>1 && (num[len-1]=='i' || num[len-1]=='f' || num[len-1]=='h')){
        //=>if last char not equal with subtype, replace it!
        if(num[len-1]!=subtype){
            num=CH_append(STR_substring(num,0,len-1),subtype);
        }else{
            return num;
        }
    }
    //=>append subtype to num
    return CH_append(num,subtype);
}
//******************************************************
/**
 * get a var name and split its name and index
 * @author madkne
 * @version 1.0
 * @since 2020.1.3
 * @param s
 * @param is_empty_index
 * @param name : (pointer)
 * @return String : index of var
 */
String RUNKIT_get_name_index_var(String s, Boolean is_empty_index, String *name) {
    //=>init vars
    String word = 0;
    uint8 bra = 0;
    STR_empty(&(*name));
    String index=0;

    //=>get length of var name
    uint32 len = STR_length(s);
    //=>finding name and index of var
    for (uint32 i = 0; i < len; i++) {
        //=>count brackets
        if (s[i] == '[')bra++;
        else if (s[i] == ']')bra--;
        //=>determine name and index of var
        if ((s[i] == '[' && bra == 1) || (s[i] == ']' && bra == 0) || i + 1 == len) {
            if (s[i] == '[' || i + 1 == len && (*name) == 0) {
                if (i + 1 == len) {
                    word = CH_append(word, s[i]);
                }
                STR_init(&(*name), word);
            } else if (name != 0 && s[i] == ']') {
                STR_init(&index, word);
                break;
            }
            word = 0;
        } 
        else {
            word = CH_append(word, s[i]);
        }
    }
    //=>return index of var
    if (!is_empty_index && index==0) {
        return "0";
    }else if(index!=0){
        return STR_trim_space(index);
    }
    return 0;
}
//******************************************************
/**
 * get a var name and return its id in Mvar by its func_index , pack_index if exist
 * @author madkne
 * @version 1.0
 * @since 2020.1.3
 * @param name
 * @param pin
 * @param fin
 * @return Longint : id of var in Mvar or 0 if not exist
 */
Longint RUNKIT_get_var_id(String name, Longint pin, Longint fin) {
    //=>iterate Mvar to find var
    for (Longint i = 0; i < entry_table.var_mem_len; i++) {
        Mvar st = _mvar_get(i);
        //=>search with cur_pin=0 : function var
        if (st.pack_index==0 && st.func_index == fin && STR_equal(st.name, name)) {
            return st.id;
        }
        //=>search with cur_pin=0 , cur_fin=0 : global var
        if (st.pack_index==0 && st.func_index == 0 && STR_equal(st.name, name)) {
            return st.id;
        }
        //=>search with cur_fin=0 : package var
        if (st.pack_index==pin && st.func_index == 0 && STR_equal(st.name, name)) {
            return st.id;
        }
        //=>search all : package method var
        if (st.pack_index==pin && st.func_index == fin && STR_equal(st.name, name)) {
            return st.id;
        }

    }
    //=>if not exist var on the scope
    return 0;
}
//******************************************************

//******************************************************
//******************************************************
//******************************************************
//******************************************************
//******************************************************
//******************************************************
