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
 * get a list of defined vars without 'def' and attributes and split, verify and simplify them
 * @author madkne
 * @version 1.0
 * @since 2019.12.24
 * @param code : an instruction code include string or ...
 * @return uint8 : count of vars detect or -1
 */ 
uint8 RUNKIT_defvars_analyzing(StrList defvars,uint32 defvars_len, defvar vars_store[],Boolean just_syntax,Boolean just_basic_types) {
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
            return 0;
        }
        //=>if var has a value
        if(var.value!=0){
            //=>get value data type
            var.typeval=RUNKIT_detect_basic_type_value(var.value,0);
            //=>check for invalid value error
            if(EXP_check_errcode(INVALID_VALUE_ERRC)){
                EXP_handler("invalid_value", __func__, var.value, "");
                return 0;
            }
            //=>if not have basic type, if not just basic types
            if(!just_basic_types && var.typeval==0){
                //TODO:
            }
            //=>comparison between var type(if exist) and val type
            if(var.type!=0 && !STR_equal(var.type,var.typeval)){
                //TODO:
            }
            //=>simplify value of var
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
 * get a value string and its type like number and then if need simplify value and return sub_type of value
 * @author madkne
 * @version 1.0
 * @since 2019.12.24
 * @param value
 * @param type
 * @param ret_value : (pointer)
 * @param ret_subtype : (pointer)
 */
void RUNKIT_calculate_value(String value, String type, String *ret_value, uint8 *ret_subtype) {
    //=>init vars
    uint8 sub_type = '0';
    //  printf("@@@@:%s,%s\n", value, type);
    //=>if value is null
    if (type == 0) {
        EXP_set_errcode(INVALID_VALUE_ERRC);
        return;
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
            (*ret_value)=RUNKIT_calc_number_exp(value, '_', &(*ret_subtype));
        } else if (basic_ind==0/*string*/) {
            //TODO:
            // calculate_string_expression(value, &(*ret_value), &(*ret_subtype));
            //printf("STRING:%s=>%s(%c)\n", value, *ret_value, *ret_subtype);
        } else if (basic_ind==2/*boolean*/) {
            //TODO:
            // (*ret_value) = calculate_boolean_expression(value, &sub_type);
            // (*ret_subtype) = sub_type;
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
        return;
    }
}
//******************************************************
/**
 * get an math expression with a target type and calculate it and return it.
 * @author madkne
 * @version 1.0
 * @since 2019.12.25
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
    //=>if exp has ()
    if(exp[0]=='('){
        exp = RUNKIT_remove_unused_parenthesis(exp);
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
        printf("RRRR(%i):%s{%c}%s=%s[%c]\n",nodes[i].id,num1,nodes[i].op,num2,result,type_exp);
        nodes[i].res=result;
    }
    //=>print math tree
    for (uint32 i = 0; i < nodes_len; i++){
        printf("(%i) %c : %s , %s = %s\n",nodes[i].id,nodes[i].op,nodes[i].num1,nodes[i].num2,nodes[i].res);
    }
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
 * get a value like 'H'*3 , {'q1':true and false} , 4.5+12 and detect main data type and its sub type if possible!
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

//******************************************************
//******************************************************

