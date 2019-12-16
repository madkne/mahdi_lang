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
        if (ret.type == IMPORT_FILE)
        {
            //=>append source path to sources list
            SLIST_append(&entry_table.sources_list, ret.path, entry_table.sources_len++);
            //=>open file and fill soco struct
            ret1 = IMPORT_open_file(ret);
        }
        //TODO:the others
        // else if (ret.type == IMPORT_MODULE)
        // {
        //     ret1 =
        //         load_module_file(utf8_to_bytes_string(ret.path), ret.line, utf8_to_bytes_string(source_paths[ret.source_id]));
        // }
        //=>set current source path
        STR_init(&entry_table.current_source_path, ret.path);
        //----------
        //TODO:
        // if (ret1)
        //     analyze_source_code();
        _imin_set_inactive(ret.id);
    }
    return true;
}

//******************************************************
Boolean IMPORT_open_file(imin s)
{
    // //  printf("DONE!:%s\n",ascii_path);
    //=>check if file exist and open it
    FILE *fp = UTF_file_open(s.path, "r");
    if (fp == NULL){
        EXP_print_error(s.line, "not_open_file", s.name, s.path,0, "IMPORT_open_file");
        return false;
    }
    // //printf("WWWWWWWW:%s\n",ascii_path);
    // int32 status = 1;
    // long_int line_number = 0;
    // Boolean is_multiple_comment = false;
    // Boolean is_multiple_str = false;
    // str_utf8 glob_buffer = 0, glob_collect = 0;
    // while (status >= 0)
    // {
    //     //----------------init vars
    //     str_utf8 chars;
    //     uint32 size = 0;
    //     String optimize = 0;
    //     Boolean is_str = false;
    //     str_utf8 buffer = 0;
    //     str_utf8 collect = 0;
    //     uint8 max_bytes = 1;
    //     //----------------restore buffer,collect,is_str
    //     if (is_multiple_str)
    //     {
    //         is_multiple_str = false;
    //         is_str = true;
    //         utf8_str_init(&buffer, glob_buffer);
    //         utf8_str_init(&collect, glob_collect);
    //     }
    //     //----------------reading line by line from file in utf8
    //     status = utf8_read_line(fp, &chars, &size);

    //     //utf8_str_print("RRTT",chars,true);
    //     if (status == -10)
    //     {
    //         print_error(s.line, "file_not_support_utf8", (String)source_paths[s.source_id], ascii_path, "",
    //                     "open_mpl_file");
    //         return false;
    //     }

    //     //----------------trim line and check is empty
    //     line_number++;
    //     str_utf8 code_line = utf8_str_trim_space(chars);
    //     if (code_line == 0)
    //     {
    //         continue;
    //     }
    //     uint32 line_size = utf8_str_length(code_line);
    //     //----------------basic analyzing every line
    //     for (uint32 i = 0; i < line_size; i++)
    //     {
    //         //----------------check is multi line str
    //         if (is_str && i + 1 == line_size && code_line[i] == '\\')
    //         {
    //             is_multiple_str = true;
    //             glob_buffer = buffer;
    //             glob_collect = collect;
    //             break;
    //         }
    //         //----------------check is line comment
    //         if (!is_str && i + 1 < line_size && code_line[i] == '/' && code_line[i + 1] == '/')
    //         {
    //             break;
    //         }
    //         //----------------check is multi line comments
    //         if (!is_str && i + 1 < line_size)
    //         {
    //             if (code_line[i] == '/' && code_line[i + 1] == '*')
    //                 is_multiple_comment = true;
    //             else if (code_line[i] == '*' && code_line[i + 1] == '/')
    //             {
    //                 is_multiple_comment = false;
    //                 i += 1;
    //                 continue;
    //             }
    //         }
    //         //----------------
    //         if (is_multiple_comment)
    //         {
    //             continue;
    //         }
    //         //----------------check is str
    //         if ((code_line[i] == '\"' || code_line[i] == '\'') && (i == 0 || code_line[i - 1] != '\\'))
    //         {
    //             //=>convert single quotation to double quotation
    //             if (code_line[i] == '\'')
    //                 code_line[i] = '\"';
    //             is_str = switch_bool(is_str);
    //             if (is_str)
    //             {
    //                 max_bytes = 1;
    //                 collect = 0;
    //                 buffer = utf8_char_append(buffer, (uint32)'\"');
    //             }
    //             else
    //             {
    //                 //          utf8_str_print("gggg",collect,true);
    //                 max_bytes = utf8_str_max_bytes(collect, true);
    //                 //          printf("XXXX:%i,%s\n",max_bytes,utf8_to_bytes_string(collect));
    //                 if (max_bytes > 1)
    //                 {
    //                     long_int utf8_id = add_to_utst(line_number, collect, max_bytes);
    //                     collect = 0;
    //                     str_utf8 ss;
    //                     str_to_utf8(&ss, str_append(UTF8_ID_LABEL, str_from_long_int(utf8_id)));
    //                     //=>remove first quotation of string
    //                     buffer = utf8_str_substring(buffer, 0, utf8_str_length(buffer) - 1);
    //                     buffer = utf8_str_append(buffer, ss);
    //                     // printf("OOOO:%s\n",str_from_long_int(entry_table.utf8_strings_id));
    //                 }
    //                 else
    //                 {
    //                     buffer = utf8_str_append(buffer, collect);
    //                     collect = 0;
    //                 }
    //             }
    //         }
    //         //      printf("WW@@@:%c\n",code_line[i]);
    //         //----------------add char to buffer
    //         if (is_str && (collect != 0 || code_line[i] != '\"'))
    //         {
    //             collect = utf8_char_append(collect, code_line[i]);
    //             uint8 tmp12 = utf8_need_bytes((uint8)code_line[i]);
    //         }
    //         if (!is_str && i > 0 && code_line[i - 1] == ' ' && code_line[i] == ' ')
    //             continue;
    //         else if (code_line[i] == '\"' && max_bytes > 1)
    //             continue;
    //         else if (!is_str)
    //         {
    //             //        printf("WW@@@:%c\n",code_line[i]);
    //             buffer = utf8_char_append(buffer, code_line[i]);
    //         }
    //     }
    //     if (!is_multiple_str)
    //     {
    //         //trim again buffer
    //         buffer = utf8_str_trim_space(buffer);
    //         if (buffer == 0)
    //         {
    //             continue;
    //         }
    //         //----------------convert buffer to String
    //         for (uint32 i = 0; i < utf8_str_length(buffer); i++)
    //         {
    //             if (!IS_ONE_BYTE(buffer[i]))
    //             {
    //                 // printf("ERR:%i\n", buffer[i]);
    //                 print_error(line_number, "bad_place_using_utf8", ascii_path, utf8_to_bytes_string(buffer), "",
    //                             "open_mpl_file");
    //                 //printf("CXXXX:%s\n",);
    //                 if (is_programmer_debug >= 2)
    //                 {
    //                     print_struct(PRINT_MAIN_SOURCE_ST);
    //                     print_struct(PRINT_UTF8_ST);
    //                 }
    //                 return false;
    //             }
    //             optimize = char_append(optimize, (uint8)buffer[i]);
    //         }
    //         // optimize=utf8_to_str(buffer);
    //         //----------------store code line to source_code struct
    //         //utf8_str_print("Optimize",buffer,true);
    //         soco tmp1 = {(uint32)line_number, optimize, 0};
    //         //printf("optimize:%s\n",optimize);
    //         append_soco(1, tmp1);
    //     }
    //     //----------------free memory
    //     free(chars);
    // }
    // free(glob_buffer);
    // free(glob_collect);
    // //print_struct(3);
    // //print_struct(4);
    // fclose(fp);
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