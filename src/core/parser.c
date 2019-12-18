#include <MAHDI/system.h>
#include <wchar.h>

//**********************************basic variables
Longint Aline = 0;
Boolean is_in_func = false;
uint16 parse_pars = 0;
uint32 Apath = 0;
uint8 block_id = 0; //index of is_in_stru
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
    is_in_func = false;
    is_inline_stru = false;
    //=>get tokens of source code
    PARSER_get_tokens();
    COM_print_struct(PRINT_TOKENS_SOURCE_ST);
//   //-------------------init vars
//   uint8 state = 0; //1:func , 2:stru , 3:struct
//   uint32 ret = source_paths_search(entry_table.cur_source_path);
//   entry_table.cur_ascii_source_path = utf8_to_bytes_string(entry_table.cur_source_path);
//   //printf("CALL:%s\n",entry_table.cur_ascii_source_path);
//   if (ret > 0)Apath = ret;
//   else {
//     utf8_str_list_append(&source_paths, entry_table.cur_source_path, entry_table.source_counter++);
//     Apath = entry_table.source_counter - 1;
//   }
//   //printf("GGGGG:%s,%i\n", print_str_list(source_paths, entry_table.source_counter), entry_table.source_counter);
//   //-------------------start parsing codes line by line
//   for (uint32 i = 0; i < entry_table.soco_tokens_count; i++) {
//     soco token_item = get_soco(2, i);
//     String Acode = token_item.code;
//     Aline = token_item.line;
//     //printf("$%s,%i,%i\n", Acode,i,is_in_func);
//     //-------------count pars
//     if (str_ch_equal(Acode, '{')) {
//       parse_pars++;
//       if (state == 1/*func*/) {
//         is_in_func = true;
//         state = 0;
//       } else if (state == 2/*stru*/ && block_id > 0) {
//         /*is_in_stru[block_id - 1].is_active = true;
//         is_in_stru[block_id - 1].stru_pars = parse_pars - 1;*/
//         state = 0;
//       }
//       //msg("&OP", parse_pars)
//     } else if (str_ch_equal(Acode, '}')) {
//       parse_pars--;
//       //is func
//       if (parse_pars == 0) {
//         is_in_func = false;
//         cur_func_id = cur_stru_id = 0;
//       }
//       //is stru
//       if (block_id > 0) {
//         uint8 c = block_id;
//         for (;;) {
//           c--;
//           //printf("FFFF:%i,%i\n",c,is_in_stru[0].stru_pars);
//           if (is_in_stru[c].is_active && is_in_stru[c].stru_pars == parse_pars) {
//             is_in_stru[c].is_active = false;
//             is_in_stru[c].stru_pars = 0;
//             break;
//           }
//           if (c == 0) break;
//         }
//         cur_stru_id = get_last_id_active_is_in_stru();
//       }
//     }
//     //-------------import keyword
//     if (state == 0 && str_equal("import", Acode)) {
//       i++;
//       //printf("SSSSSS:%s,%i\n",Acode,StrArraySize(sharp_instructions));
//       manage_import_keywords(&i);
//       continue;
//     }
//     //-------------structs
//     if (state == 0 && str_equal(Acode, "struct") && i + 3 < entry_table.soco_tokens_count) {
//       //state = 3;
//       i++;
//       manage_structs(&i);
//       continue;
//     }
//     //-------------function headers
//     if (state == 0 && str_equal(Acode, "func") && i + 3 < entry_table.soco_tokens_count) {
//       //if is_in_func
//       if (is_in_func) {
//         String name = 0;
//         str_init(&name, get_soco(2, i + 1).code);
//         print_error(Aline, "define_func_in", entry_table.cur_ascii_source_path, name, "", "analyze_source_code");
//         break;
//       }
//       state = 1;
//       i++;
//       manage_functions(&i);
//       continue;
//     }
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

//   }
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
  //print_struct(PRINT_IMPORT_ST);
  //print_struct(PRINT_PACK_ST);
  //print_struct(7);*/
  return true;
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
    //=>iterate by count of source code files
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
                    //msg("~~!!~~CCCC:", word)
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
            if (!is_string && (CH_search(splitters, inst[i]) || i + 1 == STR_length(inst))) {
                uint32 instlen=STR_length(inst);
                //=>simicolon is end of a word of inst
                Boolean is_simi = false;
                //=>complete word at end of inst
                if (i + 1 == instlen && !CH_search(splitters, inst[i])) {
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
                    (i + 1 == instlen && CH_search(splitters, inst[i]))) {
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