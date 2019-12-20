#include <MAHDI/system.h>
#include <wchar.h>

//**********************************basic variables
Longint Aline = 0; //=>current line number
Boolean is_in_func = false;
Boolean is_in_pack=false;
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
    //=>get tokens of source code
    PARSER_get_tokens();
    COM_print_struct(PRINT_TOKENS_SOURCE_ST);
    //=>init vars
    uint8 state = 0; //1:func , 2:stru , 3:pack
    //=>set current source path index on Apath var
    Apath=SLIST_search(entry_table.sources_list,entry_table.sources_len,entry_table.current_source_path);
    //printf("GGGGG:%s,%i\n", print_str_list(source_paths, entry_table.source_counter), entry_table.source_counter);
    //=>start parsing codes token by token
    for (uint32 i = 0; i < entry_table.soco_tokens_count; i++) {
        //=>get token
        soco token_item = _soco_get(TOKENS_SOURCE_CODE, i);
        //=>get code token and its line
        String Acode= 0;
        STR_init(&Acode,token_item.code);
        Aline = token_item.line;
        //printf("$%s,%i,%i\n", Acode,i,is_in_func);
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
//     //-------------structure headers
//     if (state == 0 && str_search(block_instructions, Acode, StrArraySize(block_instructions))) {
//       state = 2;
//       i++;
//       if (str_equal(Acode, "else"))i--;
//       is_inline_stru = false;
//       manage_structures(&i, Acode);
//       //if stru is inline
//       if (is_inline_stru) {
//         is_in_stru[block_id - 1].is_inline = true;
//         is_in_stru[block_id - 1].is_active = true;
// //        printf("FFDDDD:%s\n", Acode);
//         state = 0;
//       }
//       continue;
//     }
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
    COM_print_struct(PRINT_DATA_TYPES_ST);
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
    //=>append to blst pack struct
    blst tmp1 = {0, 0, 0,0, PACK_BLOCK_ID, name, inherit, 0,0,true, Aline, Apath, 0};
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
 * @version 1.0
 * @since 2019.12.20
 * @param i : (pointer) index of token 
 * @return void
 */
void PARSER_manage_function(uint32 *i){
    //-----------------init vars
    Boolean is_par = false;
    String name = 0,param_buf=0;
    StrList parameters = 0;
    uint32 params_len = 0;
    uint8 pars = 0;
    uint8 param_bra=0,param_acol=0;
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
    printf("func_header:%s;%s;%i\n",name,SLIST_print(parameters,params_len),params_len);
    //=>append to blst_func
    blst tmp1 = {0, cur_pack_id, 0,0, FUNC_BLOCK_ID, name,0, parameters, params_len,false, Aline, Apath, 0};
    _blst_append(tmp1);
    //=>set global vars
    cur_func_id = entry_table.func_id;
    cur_stru_id = 0;
}
//******************************************************
/**
 * split every code line of main source code and append these as tokens, these tokens can parsed later!
 * @author madkne
 * @version 1.0
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
            if (!is_string && (CH_search(token_splitters, inst[i]) || i + 1 == STR_length(inst))) {
                uint32 instlen=STR_length(inst);
                //=>simicolon is end of a word of inst
                Boolean is_simi = false;
                //=>complete word at end of inst
                if (i + 1 == instlen && !CH_search(token_splitters, inst[i])) {
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
                    (i + 1 == instlen && CH_search(token_splitters, inst[i]))) {
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