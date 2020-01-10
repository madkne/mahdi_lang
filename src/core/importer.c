#include <Mahdi/system.h>
//******************************************************
void IMPORT_init(){
    //=>add stdin source to import struct
    _imin_append(IMPORT_FILE,"stdin",stdin_source_path,0,0,0,0,0,0);
}
//******************************************************
Boolean IMPORT_run(){
    //=>loop until not exist any import instruction
    while (entry_table.import_active_count > 0){
        Boolean ret1 = false;
        //=>clear source code struct for new data
        _soco_clear(MAIN_SOURCE_CODE);
        //=>get first active import node from imin struct
        imin ret = _imin_get_first_active(); 
        //=>if import type is file, the open it
        if (ret.type == IMPORT_FILE){
          //=>append source path to sources list
          SLIST_append(&entry_table.sources_list, ret.path, entry_table.sources_len++);
          //=>open file and fill soco struct
          ret1 = IMPORT_open_file(ret);
        }
        //=>if import type is module, the open it
        else if (ret.type == IMPORT_MODULE){
          //=>resolve mahdi module path
          ret.path=MODULE_return_mahdi_modules(ret.path);
          //=>load module by its path
          ret1 =MODULE_load_module(ret.path,ret.packages,ret.pack_len,ret.line,entry_table.sources_list[ret.source_index]);
        }
        //TODO:the others
        //=>set current source path
        STR_init(&entry_table.current_source_path, ret.path);
        //=>if success to import file or module or..., then analyze it!
        if (ret1){
            PARSER_analyze_source_code();
        }
        _imin_set_inactive(ret.id);
    }
    return true;
}

//******************************************************
/**
 * get an import node struct by file type and fill source code and utf8 strings struct
 * @author madkne
 * @version 1.0
 * @since 2019.12.18
 * @param s : import struct node
 * @return Boolean : if failed or success to complete read file
 */ 
Boolean IMPORT_open_file(imin s)
{
    //=>check if file exist and open it
    FILE *fp = UTF_file_open(s.path, "r");
    if (fp == NULL){
        EXP_print_error(s.line, "not_open_file", s.name, s.path,0, "IMPORT_open_file");
        return false;
    }
    //printf("WWWWWWWW:%s\n",ascii_path);
    //=>init vars
    int32 status = 1;
    Longint line_number = 0;
    Boolean is_multiple_comment = false;
    Boolean is_multiple_str = false;
    //=>used for multiline strings
    UString glob_buffer = 0, glob_collect = 0; 
    while (status >= 0){
        //=>init vars
        UString chars;
        uint32 size = 0;
        String optimize = 0;
        Boolean is_str = false;
        UString buffer = 0;
        UString collect = 0;
        uint8 max_bytes = 1;
        //----------------restore buffer,collect,is_str
        //=>if multiple string, then set before collected strings to buffer and set is_str
        if (is_multiple_str){
          is_multiple_str = false;
          is_str = true;
          USTR_init(&buffer, glob_buffer);
          USTR_init(&collect, glob_collect);
        }
        //=>reading line by line from file in utf8
        status = UTF_read_line(fp, &chars, &size);
        // USTR_print("RRTT",chars,true);
        //=>if status code is -10 (can not read an utf8 char)
        if (status == -10){
          EXP_print_error(s.line, "file_not_support_utf8", s.name, s.path, "","IMPORT_open_file");
          return false;
        }
        //=>increase line number
        line_number++;
        //=>trim space code line
        UString code_line = USTR_trim_space(chars);
        //=>if line length after is null,then ignore it!
        if (code_line == 0){
            continue;
        }
        //=>get line length
        uint32 line_size = USTR_length(code_line);
        //=>iterate all line chars, detect utf8 strings
        for (uint32 i = 0; i < line_size; i++){
          //=>check if this line is can multi line string, if last line has '\'
          if (is_str && i + 1 == line_size && code_line[i] == '\\'){
            is_multiple_str = true;
            USTR_init(&glob_buffer,buffer);
            USTR_init(&glob_collect,collect);
            break;
          }
          //=>check is line comment,then ignore it
          if (!is_str && i + 1 < line_size && code_line[i] == '/' && code_line[i + 1] == '/'){
            break;
          }
          //=>check is multi line comments
          if (!is_str && i + 1 < line_size){
            //=>if start multi comment with '/*'
            if (code_line[i] == '/' && code_line[i + 1] == '*')
              is_multiple_comment = true;
            //=>if end multi comment with '*/'
            else if (code_line[i] == '*' && code_line[i + 1] == '/'){
              is_multiple_comment = false;
              i += 1;
              continue;
            }
          }
          //=>if line is multi comment, then ignore
          if (is_multiple_comment){
            continue;
          }
          //=>check line has string
          if ((code_line[i] == '\"' || code_line[i] == '\'') && (i == 0 || code_line[i - 1] != '\\')){
            //=>convert single quotation to double quotation
            if (code_line[i] == '\''){
              code_line[i] = '\"';
            }
            is_str = BOOL_switch(is_str);
            //=>if is string(start of string)
            if (is_str){
              max_bytes = 1;
              collect = 0;
              buffer = UCH_append(buffer, (uint32)'\"');
            }
            //=>if not string(end of string)
            else{
              //=>get max bytes needs per char of string
              max_bytes = USTR_max_bytes(collect, true);
              //printf("XXXX:%i,%s\n",max_bytes,utf8_to_bytes_string(collect));
              //=>if string is unicode and not english!
              if (max_bytes > 1){
                //=>add utf8 string to utst linkedlist and get its id
                Longint utf8_id = _utst_add(line_number, collect, max_bytes);
                collect = 0;
                UString ss;
                //=>generate a label replacement utf8 string by its id
                STR_to_utf8(&ss, STR_append(UTF8_ID_LABEL, STR_from_Longint(utf8_id)));
                //=>remove first quotation of string (just a utf8 label)
                buffer = USTR_substring(buffer, 0, USTR_length(buffer) - 1);
                buffer = USTR_append(buffer, ss);
                // printf("OOOO:%s\n",str_from_long_int(entry_table.utf8_strings_id));
              }
              //=>if string is english
              else{
                buffer = USTR_append(buffer, collect);
                collect = 0;
              }
            }
          }
          //      printf("WW@@@:%c\n",code_line[i]);
          //=>if char of line is in string, append it! 
          if (is_str && (collect != 0 || code_line[i] != '\"')){
            collect = UCH_append(collect, code_line[i]);
            uint8 tmp12 = UTF_need_bytes((uint8)code_line[i]);
          }
          //=>if char of line is space and not in string, then ignore it
          if (!is_str && i > 0 && code_line[i - 1] == ' ' && code_line[i] == ' '){
            continue;
          }
          //=>if char is '"', then igonre it
          else if (code_line[i] == '\"' && max_bytes > 1){
            continue;
          }
          //=>if char of line on in string, append it to buffer
          else if (!is_str){
            //printf("WW@@@:%c\n",code_line[i]);
            buffer = UCH_append(buffer, code_line[i]);
          }
        }
        //=>if not multiple string
        if (!is_multiple_str){
          //=>trim again buffer
          buffer = USTR_trim_space(buffer);
          //=>if buffer is null, then ignore
          if (buffer == 0){
              continue;
          }
          //=>convert buffer to String
          for (uint32 i = 0; i < USTR_length(buffer); i++){
            //=>if still rest any utf8 char, then ERROR!
            if (!IS_ONE_BYTE(buffer[i])){
              // printf("ERR:%i\n", buffer[i]);
              EXP_print_error(line_number, "bad_place_using_utf8", s.name, USTR_to_bytes_str(buffer), 0,"IMPORT_open_file");
              //printf("CXXXX:%s\n",);
              //=>if programmer debug is enabled, then display main source and utf8 structures
              if (is_programmer_debug >= 3){
                COM_print_struct(PRINT_MAIN_SOURCE_ST);
                COM_print_struct(PRINT_UTF8_ST);
              }
              return false;
            }
            //=>convert utf8 chars to ascii chars
            optimize = CH_append(optimize, (uint8)buffer[i]);
          }
          // optimize=utf8_to_str(buffer);
          //=>store code line to source_code struct
          //utf8_str_print("Optimize",buffer,true);
          //printf("optimize:%s\n",optimize);
          _soco_append(MAIN_SOURCE_CODE,(uint32)line_number, optimize);
        }
        //=>free memory every read line from file
        free(chars);
    }
    //=>free memory related reading source file
    free(glob_buffer);
    free(glob_collect);
    //=>if programmer debug is enabled, then display main source and utf8 structures
    if (is_programmer_debug >= 3){
      COM_print_struct(PRINT_MAIN_SOURCE_ST);
      COM_print_struct(PRINT_UTF8_ST);
    }
    fclose(fp);
    //printf("ENDDDDD:%s\n",ascii_path);
    return true;
}


//*************************************************************
//*******************import_inst functions*********************
//*************************************************************
Longint _imin_append(uint8 type,String name,String path,StrList packages,StrList functions,uint32 pack_len,uint32 func_len,uint32 line,uint32 source_index) {
  imin *q;
  q = (imin *) malloc(sizeof(imin));
  if (q == 0) return 0;
  q->id = ++entry_table.import_id;
  q->is_active = true;
  q->type = type;
  STR_init(&q->name,name);
  STR_init(&q->path,path);
  SLIST_init(&q->packages,packages,pack_len);
  q->pack_len=pack_len;
  SLIST_init(&q->functions,functions,func_len);
  q->func_len=func_len;
  q->line = line;
  q->err_code = 0;
  q->source_index = source_index;
  q->next = 0;
  if (entry_table.import_start == 0) {
    entry_table.import_start = entry_table.import_end = q;
  } else {
    entry_table.import_end->next = q;
    entry_table.import_end = q;
  }
  //=>increase import_active_count var state
  entry_table.import_active_count++;
  return entry_table.import_id;
}

//*************************************************************
imin _imin_get(Longint id) {
  imin ret = {0,0,0,0,0,0,0,0,0,0,0,0,0};
  imin *tmp1 = entry_table.import_start;
  for (;;) {
    if (tmp1->id == id) {
      ret = *tmp1;
      break;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return ret;
}

//*************************************************************
imin _imin_get_first_active() {
  imin ret = {0,0,0,0,0,0,0,0,0,0,0,0,0};
  imin *tmp1 = entry_table.import_start;
  for (;;) {
    if (tmp1->is_active) {
      ret = *tmp1;
      break;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return ret;
}

//*************************************************************
void _imin_set_inactive(Longint id) {
  imin *tmp1 = entry_table.import_start;
  for (;;) {
    if (tmp1->is_active && tmp1->id == id) {
      entry_table.import_active_count--;
        //=>inactive imin node
        tmp1->is_active = false;
      break;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
}