#include <MAHDI/system.h>
#include <wchar.h>

//**********************************basic variables
Longint Aline = 0; //=>current line number
Boolean is_in_func = false;
Boolean is_in_pack=false;
//=>detect and get tokens like '@private_method' in package
uint8 pack_zone=PUBLIC_METHOD_FATTR;
//=>get tokens like 'override','static' in package
int32 pack_method_attrs[MAX_FUNCTION_ATTRIBUTES];
uint8 pack_method_attrs_len=0;
//=>store end parse_pars count on end
int8 end_of_pack=false;
int8 end_of_func=false;
uint16 parse_pars = 0;
uint32 Apath = 0; //=>current file path index 
uint8 block_id = 0; //=>index of is_in_stru
Longint cur_pack_id = 0;
Longint cur_func_id = 0;
Longint cur_stru_id = 0;
Boolean is_inline_stru = false;
stoi is_in_stru[MAX_INTO_IN_STRUCTURES];
//******************************************************
Boolean PARSER_analyze_source_code() {
    //=>clear all tokens of source code
    _soco_clear(TOKENS_SOURCE_CODE);
    //=>clear all nodes of struct to ther other
    _stoi_empty(is_in_stru, MAX_INTO_IN_STRUCTURES);
    //=>reset all global vars
    parse_pars = 0;
    block_id = 0;
    cur_stru_id = 0;
    cur_func_id = 0;
    cur_pack_id = 0;
    is_in_func = false;
    is_in_pack = false;
    end_of_func=-1;
    is_inline_stru = false;
    end_of_pack=-1;
    pack_zone=PUBLIC_METHOD_FATTR;
    ILIST_reset(pack_method_attrs,MAX_FUNCTION_ATTRIBUTES);
    pack_method_attrs_len=0;
    //=>get tokens of source code
    PARSER_get_tokens();
    if(is_programmer_debug>=2){
        COM_print_struct(PRINT_TOKENS_SOURCE_ST);
    }
    //=>init vars
    uint8 state = 0; //1:func , 2:stru , 3:pack
    //=>set current source path index on Apath var
    Apath=SLIST_search(entry_table.sources_list,entry_table.sources_len,entry_table.current_source_path);
    //=>start parsing codes token by token
    for (uint32 i = 0; i < entry_table.soco_tokens_count; i++) {
        //=>get token
        soco token_item = _soco_get(TOKENS_SOURCE_CODE, i);
        //=>get code token and its line
        String Acode= 0;
        STR_init(&Acode,token_item.code);
        Aline = token_item.line;
        // printf("$$$%s,%i,%i\n", Acode,i,state);
        //=>count pars
        if (STR_CH_equal(Acode, '{')) {
            parse_pars++;
            //=>if in package, then set it
            if (state == 3/*pack*/) {
                is_in_pack = true;
                end_of_pack=parse_pars-1;
                state = 0;
            }
            //=>if in function, then set it
            else if (state == 1/*func*/) {
                is_in_func = true;
                end_of_func=parse_pars-1;
                state = 0;
            }
            //=>if in structure, then set it 
            else if (state == 2/*stru*/ && block_id > 0) {
                /*is_in_stru[block_id - 1].is_active = true;
                is_in_stru[block_id - 1].stru_pars = parse_pars - 1;*/
                state = 0;
            }
        } else if (STR_CH_equal(Acode, '}')) {
            parse_pars--;
            //=>if end package
            if(end_of_pack==parse_pars){
                end_of_pack=0;
                is_in_pack=false;
                cur_pack_id = cur_func_id = cur_stru_id = 0;
            }
            //=>if end function
            if (end_of_func==parse_pars) {
                end_of_func=0;
                is_in_func = false;
                cur_func_id = cur_stru_id = 0;
            }
            //=>if end structure
            if (block_id > 0) {
                uint8 c = block_id;
                for (;;) {
                c--;
                //printf("FFFF:%i,%i\n",c,is_in_stru[0].stru_pars);
                if (is_in_stru[c].is_active && is_in_stru[c].stru_pars == parse_pars) {
                    is_in_stru[c].is_active = false;
                    is_in_stru[c].stru_pars = 0;
                    break;
                }
                if (c == 0) break;
                }
                cur_stru_id = PARSER_get_last_active_is_in_stru_id();
            }
        }
        //=>if is in a package and out of any functions
        if(cur_pack_id>0 && cur_func_id==0){
            if(pack_method_attrs_len==3) pack_method_attrs_len=0;
            //=>check for zone methods in package
            if(STR_equal(Acode,"@public_methods")){
                pack_zone=PUBLIC_METHOD_FATTR;
            }else if(STR_equal(Acode,"@private_methods")){
                pack_zone=PRIVATE_METHOD_FATTR;
            }
            //=>get method attributes
            else if(STR_equal(Acode,"override")){
                pack_method_attrs[pack_method_attrs_len++]=OVERRIDE_METHOD_FATTR;
            }
            else if(STR_equal(Acode,"static")){
                pack_method_attrs[pack_method_attrs_len++]=STATIC_METHOD_FATTR;
            }
        }
        //if token is import keyword
        if (state == 0 && STR_equal("import", Acode)) {
            i++;
            PARSER_manage_import(&i);
            continue;
        }
        //if token is pack keyword
        if (state == 0 && STR_equal(Acode, "pack") && i + 3 < entry_table.soco_tokens_count) {
            //if is_in_pack
            if (is_in_pack) {
                String name = 0;
                STR_init(&name, _soco_get(TOKENS_SOURCE_CODE, i + 1).code);
                EXP_print_error(Aline, "define_pack_in", entry_table.current_source_path, name, 0, "PARSER_analyze_source_code");
                break;
            }
            pack_zone=PUBLIC_METHOD_FATTR;
            ILIST_reset(pack_method_attrs,MAX_FUNCTION_ATTRIBUTES);
            pack_method_attrs_len=0;
            state = 3;
            i++;
            PARSER_manage_package(&i);
            continue;
        }
        //=>if token is func keyword
        if (state == 0 && STR_equal(Acode, "func") && i + 3 < entry_table.soco_tokens_count) {
            //if is_in_func
            if (is_in_func) {
                String name = 0;
                STR_init(&name, _soco_get(TOKENS_SOURCE_CODE, i + 1).code);
                EXP_print_error(Aline, "define_func_in", entry_table.current_source_path, name, 0, "PARSER_analyze_source_code");
                break;
            }
            state = 1;
            i++;
            PARSER_manage_function(&i);
            continue;
        }
        //=>if token is a if,elif,loop,... keywords 
        if (state == 0 && STR_search(block_instructions, Acode,StrArraySize(block_instructions))) {
            state = 2;
            i++;
            if (STR_equal(Acode, "else"))i--;
            is_inline_stru = false;
            PARSER_manage_structure(&i, Acode);
            //=>if stru is inline
            if (is_inline_stru) {
                is_in_stru[block_id - 1].is_inline = true;
                is_in_stru[block_id - 1].is_active = true;
                //        printf("FFDDDD:%s\n", Acode);
                state = 0;
            }
            continue;
        }
//     //-------------normal instructions
//     if (state == 0 && !str_ch_equal(Acode, '{') && !str_ch_equal(Acode, '}') && !str_ch_equal(Acode, ';')) {
//       manage_normal_instructions(&i);
//     }
//     //-------------inline stru
//     if (block_id > 0 && !str_ch_equal(Acode, ';')) {
//       uint8 c = block_id;
//       for (;;) {
//         c--;
//         if (is_in_stru[c].is_inline && is_in_stru[c].is_active) {
//           is_in_stru[c].is_active = false;
//           is_in_stru[c].is_inline = false;
//           //println("GGGG:", is_in_stru[c].id)
//           break;
//         }
//         if (c == 0) break;
//       }
//       cur_stru_id = get_last_id_active_is_in_stru();
//     }

  }
//   //-------------check for syntax error
//   //msg("&%%%", parse_pars)
//   if (parse_pars > 0) {
//     print_error(Aline, "not_end_acod", entry_table.cur_ascii_source_path, "", "", "analyze_source_code");
//     return false;
//   } else if (parse_pars < 0) {
//     print_error(Aline, "not_start_acod", entry_table.cur_ascii_source_path, "", "", "analyze_source_code");
//     return false;
//   }
  //--------------------------------
  //=>if programmer debug is enabled, then show all structs
  if(is_programmer_debug>=2){
    COM_print_struct(PRINT_IMPORT_ST);
    COM_print_struct(PRINT_PACK_ST);
    COM_print_struct(PRINT_FUNC_ST);
    COM_print_struct(PRINT_DATA_TYPES_ST);
    COM_print_struct(PRINT_STRU_ST);
  }
  //print_struct(7);*/
  return true;
}
//******************************************************
Longint PARSER_get_last_active_is_in_stru_id() {
    //=>if not in a function, then return 0
    if (block_id == 0)return 0;
    uint8 c = block_id;
    for (;;) {
        c--;
        //=>if a node is active from lastest
        if (is_in_stru[c].is_active) {
            return is_in_stru[c].id;
        }
        if (c == 0) break;
    }
    //=>if not found, return 0
    return 0;
}
//******************************************************
/**
 * get i pointer of token index in source code and detect import instruction and append it to imin struct
 * @author madkne
 * @version 1.0
 * @since 2019.12.19
 * @param i : (pointer) index of token 
 * @return void
 */ 
void PARSER_manage_import(uint32 *i) {
  //=>init vars
  String path = 0;
  String name=0;
  StrList packs=0;
  uint32 packs_len=0;
  StrList funcs=0;
  uint32 funcs_len=0;
  String ext=0;
  uint8 type=0;
  uint8 err_code=0;
  //=>iterate import inst
  for (; *i < entry_table.soco_tokens_count; (*i)++) {
        //=>get token after import
        soco token_item = _soco_get(TOKENS_SOURCE_CODE, *i);
        String Acode =0;
        STR_init(&Acode,token_item.code);
        Aline = token_item.line;
        // printf("gggg:%i,%i,%s\n", *i, Aline, Acode);
        //=>finish iterate when get ';'
        if(STR_CH_equal(Acode, ';')){
            break;
        }
        //=>check if type is 0, then detect it!
        if(type==0){
            if(STR_equal(Acode, "mod")){
                type=IMPORT_MODULE;
                STR_init(&ext,LIB_EXTENSION);
            }
            else if(STR_equal(Acode, "lib")){
                type=IMPORT_MAHLIB;
                STR_init(&ext,".mahlib");
            }
            else if(STR_equal(Acode, "file")){
                type=IMPORT_FILE;
                STR_init(&ext,".mah");
            }
            //=>if non of each, then error 1 for import
            else err_code=1;
            (*i)++;
            continue;
        }
        //=>check for name and path if path is null
        if(path==0){
            // printf("EEE:%s,%s\n",Acode,ext);
            path=COM_convert_mahdipath_to_abspath(Acode,ext,&name);
            continue;
        }
        //=>if continue for modules and libraries types
        if(type==IMPORT_MODULE||type==IMPORT_MAHLIB){
            if(STR_CH_equal(Acode, ':')||STR_CH_equal(Acode, ','))continue;
            //=>if all packages and functions
            if(STR_CH_equal(Acode,'*')){
                funcs_len=0;
                packs_len=0;
                funcs=0;
                packs=0;
                SLIST_append(&funcs,Acode,funcs_len++);
                SLIST_append(&packs,Acode,packs_len++);
            }
            //=>if global function name
            else if(Acode[0]=='@'){
                Acode=STR_replace(Acode,"@","",1);
                SLIST_append(&funcs,Acode,funcs_len++);
            }
            //=>if package name
            else{
                SLIST_append(&packs,Acode,packs_len++);
            }
            
        }
        //=>if contniue and type if file, then error!
        else{
            err_code=2;
        } 
    }
    //printf("DDDD:%s,%s,%i\n",path,name,type);
    //=>check errors
    if(err_code>0){
        EXP_print_error(Aline, "import_syntax_error", entry_table.current_source_path, 0, 0,"PARSER_manage_import");
    }
    //=>if no any errors, then append to imin struct
    else{
        _imin_append(type,name,path,packs,funcs,packs_len,funcs_len,Aline,Apath);
    }
}
//******************************************************
/**
 * get i pointer of token index in source code and detect package header and append it to imin struct
 * @author madkne
 * @version 1.0
 * @since 2019.12.20
 * @param i : (pointer) index of token 
 * @return void
 */
void PARSER_manage_package(uint32 *i) {
    //=>init vars
    Boolean is_extend = false;
    String params = 0, name = 0, inherit=0;
    uint8 pars = 0;
    //=>iterate tokens from pack inst
    for (; *i < entry_table.soco_tokens_count; (*i)++) {
        //=>get token after pack
        soco token_item = _soco_get(TOKENS_SOURCE_CODE, *i);
        String Acode =0;
        STR_init(&Acode,token_item.code);
        Aline = token_item.line;
        //=>ignore if semicolon
        if(STR_CH_equal(Acode, ';'))continue;
        //=>count pars
        if (STR_CH_equal(Acode, '('))pars++;
        else if (STR_CH_equal(Acode, ')')) pars--;
        //=>get pack name
        if (name == 0) {
            STR_init(&name, Acode);
            continue;
        }
        //=>detect inherit(extend) pack, if exist!
        if (name != 0 && pars == 1 && STR_CH_equal(Acode, '(')) {
            is_extend = true;
            continue;
        }
        //=>get extend pack
        if (is_extend && pars > 0) {
            STR_init(&inherit,Acode);
        } else if (is_extend && pars == 0 && STR_CH_equal(Acode, ')')) {
            is_extend = false;
            break;
        }
    }
    //=>verify package name
    if(!RUNKIT_is_valid_name(name,false)){
        EXP_print_error(Aline,"invalid_name_block",entry_table.current_source_path,name,0,"PARSER_manage_package");
        return;
    }
    //=>append to blst pack struct
    blst tmp1 = {0, 0, 0,0, PACK_BLOCK_ID, name, inherit, 0,0,true,0, Aline, Apath, 0};
    _blst_append(tmp1);
    //=>append to datas struct
    _datas_append(entry_table.pack_id,PACKAGE_DATA_TYPE,name);
    //=>set global vars
    cur_pack_id=entry_table.pack_id;
    cur_func_id = 0;
    cur_stru_id = 0;
}
//******************************************************
/**
 * get i pointer of token index in source code and detect function header and append it to imin struct
 * @author madkne
 * @version 1.3
 * @since 2019.12.20
 * @param i : (pointer) index of token 
 * @return void
 */
void PARSER_manage_function(uint32 *i){
    //=>init vars
    Boolean is_par = false;
    String name = 0,param_buf=0;
    StrList parameters = 0;
    uint32 params_len = 0;
    uint8 pars = 0;
    uint8 param_bra=0,param_acol=0;
    int32 func_attrs[MAX_FUNCTION_ATTRIBUTES];
    ILIST_reset(func_attrs,MAX_FUNCTION_ATTRIBUTES);
    //=>iterate tokens of function header
    for (; *i < entry_table.soco_tokens_count; (*i)++) {
        //=>get token after pack
        soco token_item = _soco_get(TOKENS_SOURCE_CODE, *i);
        String Acode =0;
        STR_init(&Acode,token_item.code);
        Aline = token_item.line;
        uint32 next = (*i) + 1;
        //printf("SSSS:%s\n", buf);
        if (STR_CH_equal(Acode, ';'))continue;
        //=>count pars
        if (STR_CH_equal(Acode, '('))pars++;
        else if (STR_CH_equal(Acode, ')')) pars--;
        //=>get name
        if (name == 0) {
            STR_init(&name, Acode);
            continue;
        }
        //=>check for parameters
        if (name != 0 && pars == 1 && STR_CH_equal(Acode, '(')) {
            is_par = true;
            continue;
        }
        //=>get parameters one by one!
        if (is_par && pars > 0) {
            //=>if token is ',' between two parameters, then ignore it! 
            if(STR_CH_equal(Acode, ',')&&param_acol==0&&param_bra==0)continue;
            //=>count param bra and acol
            if(STR_CH_equal(Acode, '['))param_bra++;
            else if(STR_CH_equal(Acode, ']'))param_bra--;
            if(STR_CH_equal(Acode, '{'))param_acol++;
            else if(STR_CH_equal(Acode, '}'))param_acol--;
            //=>buffer tokens to get a complete parameter like x:string='hello'
            param_buf = STR_append(param_buf, Acode);
            //=>if can get next token
            if (next < entry_table.soco_tokens_count){
                //=>get next token
                soco next_token=_soco_get(TOKENS_SOURCE_CODE, next);
                //=>check if next token is ',' or ')'
                if((STR_CH_equal(next_token.code,',')||STR_CH_equal(next_token.code,')'))&&param_acol==0&&param_bra==0){
                    SLIST_append(&parameters,param_buf,params_len++);
                    param_buf=0;
                    param_acol=param_bra=0;
                }
            }
        }
        //=>end of function parameters 
        else if (is_par && pars == 0 && STR_CH_equal(Acode, ')')) {
            is_par = false;
            break;
        }
    }
    //=>verify function name
    for (uint32 i = 0; i < STR_length(name); i++) {
        //=>if chars of name is valid, ignore it like _var,var,@var,var34
        if ((name[i] >= '0' && name[i] <= '9' && i>0) || (name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z') || name[i] == '_' ) {
            continue;
        } else {
            EXP_print_error(Aline,"invalid_name_block",entry_table.current_source_path,name,0,"PARSER_manage_function");
            return;
        }
    }
    //=>check if function is global, not in predefined package function
    if(cur_pack_id==0 && _prme_search(name)!=-1){
        EXP_print_error(Aline,"invalid_predefined_method",entry_table.current_source_path,name,0,"PARSER_manage_function");
        return;
    }
    //=>set function attributes if in a package
    if(cur_pack_id>0){
        for (uint8 i = 0; i < pack_method_attrs_len; i++){
            func_attrs[i]=pack_method_attrs[i]; 
        }
        func_attrs[pack_method_attrs_len]=pack_zone;
        //=>add predefined attribute, if is a predefined method
        if(_prme_search(name)>-1){
            func_attrs[pack_method_attrs_len+1]=PREDEFINED_METHOD_FATTR;  
            //=>if predefined has 'private' or 'static', set error!
            if(ILIST_search(func_attrs,PRIVATE_METHOD_FATTR,MAX_FUNCTION_ATTRIBUTES)||ILIST_search(func_attrs,STATIC_METHOD_FATTR,MAX_FUNCTION_ATTRIBUTES)){
                EXP_print_error(Aline,"invalid_predefined_attr_method",entry_table.current_source_path,name,0,"PARSER_manage_function");
                return;
            }
        }
        //TODO:
    }
    //=>if function is _bootup_
    else if(STR_equal(name,"_bootup_")){
        func_attrs[0]=BOOTUP_FUNCTION_FATTR;
    }
    //=>empty method attributes array 
    ILIST_reset(pack_method_attrs,MAX_FUNCTION_ATTRIBUTES);
    pack_method_attrs_len=0;
    // printf("func_header:%s;%s;%i\n",name,SLIST_print(parameters,params_len),params_len);
    //=>append to blst_func
    blst tmp1 = {0, cur_pack_id, 0,0, FUNC_BLOCK_ID, name,0, parameters, params_len,false,(IntList)func_attrs, Aline, Apath, 0};
    _blst_append(tmp1);
    //=>set global vars
    cur_func_id = entry_table.func_id;
    cur_stru_id = 0;
}
//******************************************************
/**
 * get i pointer of token index in source code and structure name like 'if','loop' and detect structure header and append it to blst struct
 * @author madkne
 * @version 1.1
 * @since 2019.12.21
 * @param i : (pointer) index of token 
 * @param lbl : label of structure like 'if','elif','loop'
 * @return void
 */
void PARSER_manage_structure(uint32 *i, String lbl) {
    //=>init vars
    int32 is_par = 0;
    uint8 type = LOOP_STRU_ID;
    String buf = 0;
    uint32 params_len = 0;
    StrList params = 0;
    if (STR_equal(lbl, "if")) type = IF_STRU_ID;
    else if (STR_equal(lbl, "elif")) type = ELIF_STRU_ID;
    else if (STR_equal(lbl, "else")) type = ELSE_STRU_ID;
    else if (STR_equal(lbl, "manage")) type = MANAGE_STRU_ID;
    else if (STR_equal(lbl, "choose")) type = CHOOSE_STRU_ID;

    //=>analyze all structure except 'else'
    if(type != ELSE_STRU_ID){
        for (; *i < entry_table.soco_tokens_count; (*i)++) {
            //=>get token after structure label
            soco token_item = _soco_get(TOKENS_SOURCE_CODE, *i);
            String Acode =0;
            STR_init(&Acode,token_item.code);
            Aline = token_item.line;
            //  printf("IIO:%s\n", Acode);
            if (STR_CH_equal(Acode, ';') && is_par==0) {
                break;
            }
            //=>count pars
            if (STR_CH_equal(Acode, '(')) {
                is_par++;
                if (is_par == 1)continue;
            }else if (STR_CH_equal(Acode, ')')) {
                is_par--;
                //break all
                if (is_par == 0) break;
            }
            /*if (str_ch_equal(Acode, '{'))parse_pars++;
            else if (str_ch_equal(Acode, '}'))parse_pars--;*/
            //=>append to buf
            buf = STR_append(buf, Acode);
            if ((*i) + 1 < entry_table.soco_tokens_count && !CH_search(words_splitter, Acode[0],ChArraySize(words_splitter)) &&
                !CH_search(words_splitter, _soco_get(TOKENS_SOURCE_CODE, (*i) + 1).code[0],ChArraySize(words_splitter))) {
                buf = CH_append(buf, ' ');
            }
        }
    }
    //=>if structure is 'else' type
    if (type == ELSE_STRU_ID) {
        SLIST_append(&params, "null", params_len++);
    }
    //=>if structure is other types
    else {
        buf = PARSER_trim_inst_code(buf); //trim code
        SLIST_append(&params, buf, params_len++);
    }
    // printf("@@@DDDD:%s,%s\n", lbl, buf);
    //=>check if stru is inline or not!
    uint32 cur_ind = *i;
    if (cur_ind + 2 <= entry_table.soco_tokens_count && !STR_CH_equal(_soco_get(TOKENS_SOURCE_CODE, cur_ind + 1).code, '{') &&
        !STR_CH_equal(_soco_get(TOKENS_SOURCE_CODE, cur_ind + 2).code, '{')) {
        is_inline_stru = true;
    }

    //=>analyze loop stru, split its parameters
    if (type == LOOP_STRU_ID) {
        String buffer = 0, tmp = 0;
        //=>get parameters of loop
        STR_init(&buffer, params[0]);
        params_len = 0;
        params = 0;
        Boolean is_str = false;
        uint32 blen = STR_length(buffer);
        //=>iterate loop parameters
        for (uint32 j = 0; j <= blen; j++) {
            //=>check if is string
            if (j < blen && buffer[j] == '\"' && (j == 0 || buffer[j - 1] != '\\')){
                is_str = BOOL_switch(is_str);
            }
            //=>if get ';' or end
            if (!is_str && (buffer[j] == ';' || j == blen)) {
                SLIST_append(&params, tmp, params_len++);
                tmp = 0;
                continue;
            }
            //=>append to tmp
            tmp = CH_append(tmp, buffer[j]);
        }
        //=>if parameters count is less that 3
        if (params_len < 3)
            while (params_len < 3)SLIST_append(&params, 0, params_len++);
        //=>if parameters count is more than 3, then error!
        else if (params_len > 3) {
            //TODO:error
            return;
        }
        //=>if condition segment of loop is null,then error!
        if (params[1] == 0) {
            //TODO:error
            return;
        }
    //    printf("#WW:%i;;;%s;;;%s\n", params_len, print_str_list(params, params_len), buf);
    }
    //=>get current stru id
    Longint sid_t = cur_stru_id;
    //=>increase block id and check if not bigger than max!
    block_id++;
    if (block_id >= MAX_INTO_IN_STRUCTURES) {
      //TODO:error
      //block_id = 1;
      return;
    }
    //printf("CFFFF:%s,%s,%i\n",lbl, print_str_list(params, params_len), params_len);

    //=>append to blst
    String inst = STR_multi_append(STRUCTURES_LABEL, STR_from_Longint(entry_table.stru_id + 1), 0, 0, 0, 0);
    blst tmp1 = {0,cur_pack_id, cur_func_id, cur_stru_id, type, inst,0, params, params_len,false,0, Aline, Apath, 0};
    _blst_append(tmp1);
    //=>update current stru id and is_in_stru array
    cur_stru_id = entry_table.stru_id;
    is_in_stru[block_id - 1].is_active = true;
    is_in_stru[block_id - 1].is_inline = false;
    is_in_stru[block_id - 1].id = cur_stru_id;
    is_in_stru[block_id - 1].stru_pars = parse_pars;
    //printf("PPP:%i,%s\n",block_id,lbl);
    //=>
    // Longint order = get_order(cur_func_id, sid_t);
    // set_order(cur_func_id, sid_t, ++order);
    // //=>append to instru
    // instru tmp2 = {0, cur_func_id, sid_t, order, inst, STRUCTURE_LBL_INST, Aline, Apath, 0};
    // append_instru(tmp2);
    //printf("GGGGGGGGGGGG:%s\n",inst);
}
//******************************************************
/**
 * get a code and trim it by ineffective spaces
 * @author madkne
 * @version 1.2
 * @since 2019.12.21
 * @param code : an instruction code include string or ...
 * @return String : a valid trim code
 */ 
String PARSER_trim_inst_code(String code) {
    //=>init vars
    Boolean is_string = false;
    String ret = 0;
    uint32 len = STR_length(code);
    //=>start analyzing
    for (uint32 i = 0; i < len; i++) {
        //=>check is string
        if (code[i] == '\"' && (i == 0 || code[i - 1] != '\\')) {
            is_string = BOOL_switch(is_string);
        }
        //=>continue if ' ' is before
        if (!is_string && i + 1 < len && code[i] == ' '
            && (CH_search(single_operators, code[i + 1],ChArraySize(single_operators)) || CH_search(words_splitter, code[i + 1],ChArraySize(words_splitter)))) {
            continue;
        }
        //=>continue if ' ' is after
        if (!is_string && i - 1 >= 0 && code[i] == ' '
            && (CH_search(single_operators, code[i - 1],ChArraySize(single_operators)) || CH_search(words_splitter, code[i - 1],ChArraySize(words_splitter)))) {
            continue;
        }
        printf("@@@continue\n");
        //=>append to ret
        ret = CH_append(ret, code[i]);
    }
    //=>return
    //  printf("@Code:%s=>%s$\n", code, ret);
    return ret;
}
//******************************************************
/**
 * split every code line of main source code and append these as tokens, these tokens can parsed later!
 * @author madkne
 * @version 1.1
 * @since 2019.12.18
 * @return void
 */ 
void PARSER_get_tokens() {
    //=>init vars
    Boolean is_string = false, is_valid_func = false;
    String word = 0;
    int8 pars = 0, acol = 0, bras = 0; //pars:(),acol:{},bras:[]
    //=>iterate nodes of source code
    for (uint32 ii = 0; ii < entry_table.soco_main_count; ii++) {
        //=>get code line of source code
        soco source_code = _soco_get(MAIN_SOURCE_CODE, ii);
        //=>get code and line number
        String inst = source_code.code;
        uint32 line = source_code.line;
        //printf("line:%i,code:%s,path:%s\n",line,inst,(String)cur_ascii_source_path);
        acol = 0;
        //=>iterate code line chars
        for (uint32 i = 0; i < STR_length(inst); i++) {
            //=>check is str
            if (inst[i] == '\"' && (i == 0 || inst[i - 1] != '\\')) {
                is_string = BOOL_switch(is_string);
            }
            //=>count pars
            if (!is_string && inst[i] == '(') {
                pars++;
                if (pars == 1)
                    is_valid_func = false;
                //=>if last word, is valid name, can be a function name
                if (pars == 1 && entry_table.soco_tokens_count > 0 && RUNKIT_is_valid_name(word, false)) {
                    is_valid_func = true;
                }
            } else if (!is_string && inst[i] == ')') {
                pars--;
            }
            //=>count acol
            else if (!is_string && inst[i] == '{') {
                acol++;
            } else if (!is_string && inst[i] == '}') {
                acol--;
            }
            //=>count bras
            else if (!is_string && inst[i] == '[') {
                bras++;
            } else if (!is_string && inst[i] == ']') {
                bras--;
            }
            //=>append to buffer and word
            if (!is_string && (CH_search(token_splitters, inst[i],ChArraySize(token_splitters)) || i + 1 == STR_length(inst))) {
                uint32 instlen=STR_length(inst);
                //=>simicolon is end of a word of inst
                Boolean is_simi = false;
                //=>complete word at end of inst
                if (i + 1 == instlen && !CH_search(token_splitters, inst[i],ChArraySize(token_splitters))) {
                    word = CH_append(word, inst[i]);
                    is_simi = true;
                }
                //=>check if at end of a valid function name
                if (pars == 0 && is_valid_func && (inst[i] == ')')) {
                    is_simi = true;
                    is_valid_func = false;
                    //msg("SSSSS:", word)
                }
                //=>check if last char of inst is ),},]
                if (i + 1 == instlen && (inst[i] == ')' || inst[i] == '}' || inst[i] == ']') && pars == 0 &&
                    acol == 0) {
                    is_simi = true;
                }
                //=>trim word and if not null, append it as a token
                word = STR_trim_space(word);
                if (word != 0) {
                    _soco_append(TOKENS_SOURCE_CODE, line,word);
                }
                //=>append any separator char as a token
                if ((inst[i] != ' ' && i + 1 != instlen) ||
                    (i + 1 == instlen && CH_search(token_splitters, inst[i],ChArraySize(token_splitters)))) {
                    _soco_append(TOKENS_SOURCE_CODE, line,CH_to_string(inst[i]));
                }
                //=>if end of inst, then append a ';' as a token
                if (is_simi) {
                    soco tmp = {line, ";"};
                    _soco_append(TOKENS_SOURCE_CODE, line,";");
                }
                //**********************
                word = 0;
                continue;
            }
            //=>create a word until next char be a separator char like space,;,... 
            else {
                word = CH_append(word, inst[i]);
            }
        }
    }
    //*******************fix else inst
    /*for i:=0;i<len(token_words);i++{
        if i+1<len(token_words)&&token_words[i].token=="else"&&token_words[i+1].token=="{"{
            var tmp1 []token_words_struct
            }
    }*/
    //*******************print all
    /*for i := 0; i < len(token_words); i++ {
        msg("&CCCC", token_words[i].token, token_words[i].line)
    }*/
    //msg("&\n\n")
}