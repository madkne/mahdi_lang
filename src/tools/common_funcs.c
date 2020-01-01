#include <Mahdi/system.h>

// common functions
//******************************************
/**
 * get a mahdi path like '..main' or '..libs.mylib' and convert it to a absolute path of filesystem
 * @author madkne
 * @version 1.0
 * @since 2019.12.18
 * @param mahdipath : a mahdi custom path
 * @param extension : extension with dot that attach to last path like '.mah'
 * @param mahdiname : (pointer) if non-zero then set it filename
 * @return String : absolute path of mahdi path
 */ 
String COM_convert_mahdipath_to_abspath(String mahdipath,String extension,String *mahdiname) {
  //=>init vars
  // String StrSep = CH_to_string(OS_SEPARATOR);
  String ret = STR_trim_space(mahdipath);
  StrList segs=0;
  uint32 segslen=0;
  //=>split by '.' as slash
  segslen=CH_split(ret,'.',&segs,false);
  //=>set last segment as name
  if(mahdiname!=0){
    STR_init(&(*mahdiname),segs[segslen-1]);
  }
    //=>if segments length is 0, then return null
  if(segslen==1){
    return segs[0]; 
  }
  //=>if first segment is empty, set app directory path to it!
  if(segs[0]==0){
    STR_init(&segs[0],project_root);
  }
  //=>append extension in last segment
  if(extension!=0){
    segs[segslen-1]=STR_append(segs[segslen-1],extension);
  }
    
  //=>join all segments with '/'
  ret=CH_join(segs,OS_SEPARATOR,segslen,true);
// printf("dffff:%s,%s\n",SLIST_print(segs,segslen),ret);
  return ret;

}
//******************************************
// String COM_get_Mahdi_dir_path() {
//   String out = 0;
//   #if LINUX_PLATFORM == 1
//   //TODO:
//   #elif WINDOWS_PLATFORM == 1
//   uint8 ownPth[MAX_PATH];
//   HMODULE hModule = GetModuleHandle(NULL);
//   if (hModule != NULL) {
//     // Use GetModuleFileName() with module handle to get the path
//     GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
//     StrList entries = 0;
//     uint32 size = CH_split(ownPth, OS_SEPARATOR, &entries, true);
//     for (uint32 i = 0; i < size - 1; i++) {
//       out = STR_append(out, entries[i]);
//       if (i + 1 < size - 1)out = CH_append(out, OS_SEPARATOR);
//     }
//   }
//   #endif
//   return out;
// }

//*************************************************************
double COM_calculate_period_time(Longint start_time, String *unit) {
  Longint during = (Longint) clock() - start_time;
  double time_taken = ((double) during) / CLOCKS_PER_SEC; // in seconds
  if (time_taken == 0) time_taken += 0.000001;
  if (time_taken >= 60) {
    time_taken /= 60;
    STR_init(&(*unit), "minutes");
  } else STR_init(&(*unit), "seconds");
  return time_taken;
}
//******************************************
void COM_print_struct(uint8 which) {
  //=>print all nodes of imin struct (import instruction)
  if (which == 0 || which == PRINT_IMPORT_ST) {
    imin *tmp1 = entry_table.import_start;
    printf("=====Print import_inst_struct :\n");
    for (;;) {
      printf("Active:%i,id:%li,type:%i,name:%s,packs:%s,funcs:%s,path:%s,line:%i,source:[%i][ERR%i]\n", tmp1->is_active, tmp1->id, tmp1->type,tmp1->name,SLIST_print(tmp1->packages,tmp1->pack_len),SLIST_print(tmp1->functions,tmp1->func_len),tmp1->path, tmp1->line,tmp1->source_index,tmp1->err_code);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } 
  //=>print all nodes of datas struct (data types)
  else if (which == 0 || which == PRINT_DATA_TYPES_ST) {
    datas *tmp1 = entry_table.datas_start;
    printf("=====Print data_types_struct :\n");
    for (;;) {
      printf("[id:%li,type:%i,pid:%li]:%s\n", tmp1->id,tmp1->type,tmp1->pack_id,tmp1->name);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } 
// else if (which == 0 || which == PRINT_EXCEPTIONS_ST) {
//     exli *tmp1 = entry_table.exli_start;
//     printf("=====Print exceptions_list_struct :\n");
//     for (;;) {
//       printf("id:%i,type:%i,group:%s,lbl:%s,text:%s\n", tmp1->id, tmp1->type, exceptions_group[tmp1->group],
//              tmp1->lbl, tmp1->text);
//       tmp1 = tmp1->next;
//       if (tmp1 == 0) break;
//     }
//     printf("=====End printed\n");
  // } 
  //=>print all nodes of utst struct (utf8 strings)
  if (which == 0 || which == PRINT_UTF8_ST) {
    utst *tmp1 = entry_table.utst_start;
    if (tmp1 == 0) return;
    printf("=====Print utf8_strings_struct :\n");
    for (;;) {
      printf("id:%li,line:%i,max_bytes:%i, ", tmp1->id, tmp1->line, tmp1->max_bytes_per_char);
      USTR_print("string", tmp1->utf8_string, true);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } 
  //=>print all nodes of soco struct (source code)
  else if (which == 0 || which == PRINT_MAIN_SOURCE_ST) {
    soco *tmp1 = entry_table.soco_main_start;
    if (tmp1 == 0) return;
    printf("=====Print source_code_struct :\n");
    for (;;) {
      printf("line:%i,code:%s\n", tmp1->line, tmp1->code);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } 
  //=>print all nodes of soco struct (tokens)
  else if (which == 0 || which == PRINT_TOKENS_SOURCE_ST) {
    soco *tmp1 = entry_table.soco_tokens_start;
    if (tmp1 == 0) return;
    printf("=====Print source_code_tokens_struct :\n");
    for (;;) {
      printf("line:%i,code:%s\n", tmp1->line, tmp1->code);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } 
  //=>print all nodes of blst funcs struct (functions)
  else if (which == 0 || which == PRINT_FUNC_ST) {
    blst *tmp1 = entry_table.blst_func_start;
    if (tmp1 == 0) return;
    printf("=====Print func_block_struct :\n");
    for (;;) {
      printf("[id:%li,pid:%li],%s,name:%s,params:%s\n", tmp1->id,tmp1->pack_id,ILIST_print(tmp1->func_attrs,MAX_FUNCTION_ATTRIBUTES), tmp1->label, SLIST_print(tmp1->params, tmp1->params_len));
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } 
  //=>print all nodes of fpp struct (function params and package attrs)
  else if (which == 0 || which == PRINT_FUNC_PACK_PARAMS_ST) {
    fpp *tmp1 = entry_table.fpp_start;
    if (tmp1 == 0) return;
    printf("=====Print func_pack_params_struct :\n");
    for (;;) {
      printf("[id:%li,typ:%i,refid:%li,ord:%i]:%s|%s|%s\t[priv:%i,stat:%i,ref:%i,line:%i]\n",tmp1->id, tmp1->type,tmp1->refid,tmp1->porder, tmp1->pname,tmp1->ptype,tmp1->pvalue,tmp1->is_private,tmp1->is_static,tmp1->is_reference,tmp1->line);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } 
  //=>print all nodes of instru struct (instructions)
  else if (which == 0 || which == PRINT_INSTRU_ST) {
      instru *tmp1 = entry_table.instru_start;
      if (tmp1 == 0) return;
      printf("=====Print instructions_struct :\n");
      for (;;) {
        printf("(id:%li,pid:%li,fid:%li,sid:%li,order:%li,type:%i,line:%i),code:%s\n",
              tmp1->id,tmp1->pack_id,tmp1->func_id,tmp1->stru_id,tmp1->order,tmp1->type,tmp1->line,tmp1->code
              // entry_table.sources_list[tmp1->source_id], tmp1->line
              );
        tmp1 = tmp1->next;
        if (tmp1 == 0) break;
      }
      printf("=====End printed\n");
    } 
  //=>print all nodes of blst packs struct (packages)
  else if (which == 0 || which == PRINT_PACK_ST) {
    blst *tmp1 = entry_table.blst_pack_start;
    if (tmp1 == 0) return;
    printf("=====Print pack_block_struct :\n");
    for (;;) {
      if(tmp1->type==PACK_BLOCK_ID){
        printf("id:%li,pid:%li,name:%s,inherit:%s,params:%s\n", tmp1->id, tmp1->pack_id, tmp1->label,tmp1->inherit, SLIST_print(tmp1->params, tmp1->params_len));
      }
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } 
  //=>print all nodes of inpk struct (inherit packages)
  else if (which == 0 || which == PRINT_INHERIT_ST) {
    inpk *tmp1 = entry_table.inpk_start;
    if (tmp1 == 0) return;
    printf("=====Print inherit_package_struct :\n");
    for (;;) {
      printf("pid:%li,name:%s[iid:%li,inherit:%s,done:%i]\n", tmp1->parent_id, tmp1->parent_name, tmp1->inherit_id,tmp1->inherit_name,tmp1->is_done);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } 
  //=>print all nodes of blst stru struct (structures)
  else if (which == 0 || which == PRINT_STRU_ST) {
    blst *tmp1 = entry_table.blst_stru_start;
    if (tmp1 == 0) return;
    printf("=====Print block_structure_struct :\n");
    for (;;) {
      printf("id:%li,lbl[%i]:%s,(pid:%li,fid:%li,sid:%li),argvs:%s\n", tmp1->id, tmp1->type, tmp1->label, tmp1->pack_id,tmp1->func_id,tmp1->stru_id, SLIST_print(tmp1->params, tmp1->params_len));
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } 
// else if (which == 0 || which == PRINT_STRUCT_DES_ST) {
//     stde *tmp1 = entry_table.stde_start;
//     if (tmp1 == 0) return;
//     printf("=====Print struct descriptor :\n");
//     for (;;) {
//       printf("%i(%s):\n", tmp1->id, tmp1->type);
//       print_vaar(tmp1->st);
//       tmp1 = tmp1->next;
//       if (tmp1 == 0) break;
//     }
//     printf("=====End printed\n");
//   } else if (which == 0 || which == PRINT_MAGIC_MACROS_ST) {
//     mama *tmp1 = entry_table.mama_start;
//     if (tmp1 == 0) return;
//     printf("=====Print magic macros :\n");
//     for (;;) {
//       printf("[id:%i,type:%i]%s;%s:%s\n", tmp1->id, tmp1->type, tmp1->value_type, tmp1->key, tmp1->value);
//       tmp1 = tmp1->next;
//       if (tmp1 == 0) break;
//     }
//     printf("=====End printed\n");
//   } else if (which == 0 || which == PRINT_FUNCTIONS_STACK_ST) {
//     fust *tmp1 = entry_table.fust_start;
//     if (tmp1 == 0) return;
//     printf("=====Print functions stack :\n");
//     for (;;) {
//       printf("fid:%i,fin:%i,Pfin:%i,sid:%i,order:%i\n", tmp1->fid, tmp1->fin, tmp1->parent_fin, tmp1->sid, tmp1
//           ->order);
//       tmp1 = tmp1->next;
//       if (tmp1 == 0) break;
//     }
//     printf("=====End printed\n");
//   } else if (which == 0 || which == PRINT_STRUCTURES_STACK_ST) {
//     stst *tmp1 = entry_table.stst_start;
//     if (tmp1 == 0) return;
//     printf("=====Print structures stack :\n");
//     for (;;) {
//       printf("type:%i,fid:%i,fin:%li,sid:%li,Psid:%li,order:%li,extra:%s\n",
//              tmp1->type,
//              tmp1->fid,
//              tmp1->fin,
//              tmp1->sid,
//              tmp1->parent_sid,
//              tmp1
//                  ->order,
//              tmp1->extra);
//       tmp1 = tmp1->next;
//       if (tmp1 == 0) break;
//     }
//     printf("=====End printed\n");
//   } else if (which == 0 || which == PRINT_CONDITION_LEVEL_ST) {
//     cole *tmp1 = entry_table.cole_start;
//     if (tmp1 == 0) {
//       printf("(null) condition level\n");
//       return;
//     }
//     printf("=====Print condition level :\n");
//     for (;;) {
//       printf("[id:%i,fin:%i,sid:%i]:%i\n", tmp1->id, tmp1->fin, tmp1->sid, tmp1->is_complete);
//       tmp1 = tmp1->next;
//       if (tmp1 == 0) break;
//     }
//     printf("=====End printed\n");
//   } else if (which == 0 || which == PRINT_LOOP_LEVEL_ST) {
//     lole *tmp1 = entry_table.lole_start;
//     if (tmp1 == 0) {
//       printf("(null) loop level\n");
//       return;
//     }
//     printf("=====Print loop level :\n");
//     for (;;) {
//       printf("id:%i,fin:%i,sid:%i\n", tmp1->id, tmp1->fin, tmp1->sid);
//       tmp1 = tmp1->next;
//       if (tmp1 == 0) break;
//     }
//     printf("=====End printed\n");
//   }
}



// Boolean functions
//******************************************
Boolean BOOL_switch(Boolean b) {
  if (b) return false;
  return true;
}
//******************************************
Boolean BOOL_switch_str(String b) {
  if (STR_to_bool(b)) return false;
  return true;
}

// int32 functions
//******************************************
double I32_power(double base, int32 power) {
  double ret = 1;
  Boolean is_neg = false;
  if (power < 0) {
    is_neg = true;
    power *= -1;
  }
  for (uint32 b = 0; b < power; b++) {
    ret *= base;
  }
  if (is_neg) {
    ret = (double) 1 / ret;
  }
  return ret;
}
//******************************************


//******************************************

//******************************************



//*************************************************************
// void print_vaar(vaar_en s) {
//   vaar *tmp1 = s.start;
//   if (tmp1 == 0) {
//     printf("--NULL-- Vars_Array_struct\n");
//     return;
//   }
//   printf("=====Print Vars_Array_struct :\n");
//   if (tmp1 != 0) {
//     for (;;) {
//       printf("id:%i,%type:%c,index:%s,value:%s\n", tmp1->data_id, tmp1->sub_type, tmp1->index, tmp1->value);
//       tmp1 = tmp1->next;
//       if (tmp1 == 0) break;
//     }
//   }
//   printf("=====End printed\n");
// }
//*************************************************************
// uint32 source_paths_search(str_utf8 path) {
//   for (uint32 i = 0; i < entry_table.source_counter; i++) {
//     if (utf8_str_equal(path, source_paths[i]))
//       return i;
//   }
//   return 0;
// }
// //*************************************************************
// String validate_path(String s) {
//   #if WINDOWS_PLATFORM == true
//   String ret = 0;
//   for (uint32 i = 0; i < str_length(s); i++) {
//     if (s[i] == '/') {
//       ret = char_append(ret, '\\');
//       continue;
//     }
//     ret = char_append(ret, s[i]);
//   }
//   return ret;
//   #elif LINUX_PLATFORM == true
//   String ret = 0;
//   for (uint32 i = 0; i < str_length(s); i++) {
//     if (s[i] == '\\') {
//       ret = char_append(ret, '/');
//       continue;
//     }
//     ret = char_append(ret, s[i]);
//   }
//   return ret;
//   #endif
//   return s;
// }




// //*************************************************************
// String remove_incorrect_pars(String str, int32 *invalid_pars) {
//   String final = 0;
//   str_init(&final, str);
//   Boolean is_string = false;
//   int32 pars = 0;
//   for (uint32 i = 0; i < str_length(str); i++) {
//     if (str[i] == '\"' && (i == 0 || str[i - 1] != '\\')) {
//       is_string = switch_bool(is_string);
//     }
//     if (!is_string) {
//       switch (str[i]) {
//         case '(': pars++;
//           break;
//         case ')': pars--;
//           break;
//       }
//     }
//   }
//   //*********************
//   uint32 final_len = str_length(final);
//   if (pars > 0 && pars < final_len) {
//     final = str_substring(final, pars, 0);
//   } else if (pars < 0 && (-pars) < final_len) {
//     final = str_substring(final, 0, final_len - (-pars));
//   }
//   //msg("&***", pars)
//   (*invalid_pars) = pars;
//   return final;

// }



// //*************************************************************
// String replace_in_expression(String exp, String rep, int32 start, int32 end, Boolean remove_pars, Boolean is_trim) {
//   //msg("&@@@", exp, rep, start, end)
//   String s1 = 0, s2 = 0, expression = 0;
//   if (is_trim) {
//     s1 = str_trim_space(str_substring(exp, 0, start));
//     s2 = str_trim_space(str_substring(exp, end, 0));
//   } else {
//     s1 = str_substring(exp, 0, start);
//     s2 = str_substring(exp, end, 0);
//   }
//   uint32 s1_len = str_length(s1);
//   if (remove_pars && s1_len > 0 && s1[s1_len - 1] == '(' && s2[0] == ')') {
//     s1 = str_substring(s1, 0, s1_len - 1);
//     s2 = str_substring(s2, 1, 0);
//   }
//   expression = str_multi_append(s1, rep, s2, 0, 0, 0);
//   return expression;
// }

// //*************************************************************
// String make_valid_double(String s) {
//   uint32 s_len = str_length(s);
//   if (s_len == 0 || (s_len == 1 && s[0] == '.'))return "0";
//   if (s_len > 1 && s[0] == '.')return str_multi_append("0", s, 0, 0, 0, 0);
//   if (s_len > 1 && s[s_len - 1] == '.')return str_substring(s, 0, s_len - 1);
//   return s;
// }

// //*************************************************************
// String limit_decimal_huge_numbers(String s) {
//   uint32 s_len = str_length(s);
//   uint32 ind = 0, limit = 0;
//   Boolean is_point = false;
//   for (uint32 i = 0; i < s_len; i++) {
//     if (s[i] == '.') {
//       is_point = true;
//       continue;
//     }
//     if (is_point) {
//       limit++;
//       if (limit == max_decimal_has_huge) {
//         ind = i + 1;
//         break;
//       }
//     }
//   }
//   if (ind == 0)ind = s_len;
//   return str_substring(s, 0, ind);
// }

// //*************************************************************
// Boolean is_equal_arrays_indexes(String s1, String s2) {
//   //printf("#WWWWW:%s@@%s\n",s1,s2);
//   if ((s1 == 0 && str_ch_equal(s2, '0')) || (s2 == 0 && str_ch_equal(s1, '0')))return true;
//   if ((str_ch_equal(s1, '_') && str_ch_equal(s2, '0')) || (str_ch_equal(s2, '_') && str_ch_equal(s1, '0')))return true;
//   str_list sl1 = 0, sl2 = 0;
//   uint32 sl1_len = char_split(s1, ',', &sl1, true);
//   uint32 sl2_len = char_split(s2, ',', &sl2, true);
//   if (sl1_len != sl2_len)return false;
//   for (uint32 i = 0; i < sl1_len; ++i) {
//     if (str_ch_equal(sl1[i], '?') || str_ch_equal(sl2[i], '?'))continue;
//     if (!str_equal(sl1[i], sl2[i]))return false;
//   }
//   return true;
// }
// //*************************************************************
// Boolean has_two_limiting(String s, uint8 l1, uint8 l2, Boolean ignore_space) {
//   if (ignore_space)s = str_trim_space(s);
//   uint32 len = str_length(s);
//   if (len < 2)return false;
//   if (s[0] == l1 && s[len - 1] == l2)return true;
//   return false;
// }

// //*************************************************************
// String replace_control_chars(String val) {
//   uint32 len = str_length(val);
//   if (len < 2)return val;
//   String ret = 0;
//   for (uint32 i = 0; i < len; i++) {
//     if (val[i] == '\\' && i + 1 < len) {
//       if (val[i + 1] == 'n')ret = char_append(ret, '\n');
//       else if (val[i + 1] == 't') ret = char_append(ret, '\t');
//       else if (val[i + 1] == '\"') ret = char_append(ret, '\"');
//       else if (val[i + 1] == '\'') ret = char_append(ret, '\'');
//       else if (val[i + 1] == '\\') ret = char_append(ret, '\\');
//         //backspace
//       else if (val[i + 1] == 'b') ret = char_append(ret, '\b');//ret = char_backspace (ret);
//         //alert
//       else if (val[i + 1] == 'a') ret = char_append(ret, '\a');
//       i++;
//     } else ret = char_append(ret, val[i]);
//   }
//   return ret;

// }
// //*************************************************************
// String set_valid_control_chars(String val) {
//   uint32 len = str_length(val);
//   if (len < 1)return val;
//   String ret = 0;
//   for (uint32 i = 0; i < len; i++) {
// //    printf("Dsfsdgdrgh:%c\n",val[i]);
//     if ((i == 0 || val[i - 1] != '\\')
//         && (val[i] == '\n' || val[i] == '\t' || val[i] == '\"' || val[i] == '\'' || val[i] == '\\' || val[i] == '\b'
//             || val[i] == '\a')) {

//       if (val[i] == '\n')ret = str_append(ret, "\\n");
//       else if (val[i] == 't') ret = str_append(ret, "\\t");
//       else if (val[i] == '\"') ret = str_append(ret, "\\\"");
//       else if (val[i] == '\'') ret = str_append(ret, "\\'");
//       else if (val[i] == '\\') ret = str_append(ret, "\\\\");
//         //backspace
//       else if (val[i] == 'b') ret = str_append(ret, "\\b");
//         //alert
//       else if (val[i] == 'a') ret = str_append(ret, "\\a");
//     } else ret = char_append(ret, val[i]);
//   }
//   return ret;

// }
// //*************************************************************
// Boolean is_equal_data_types(uint8 t1, uint8 t2) {
//   if (t1 == 0 || t2 == 0 || t1 == '0' || t2 == '0')return false;
//   if (t1 == t2)return true;
//   if ((t1 == 'i' || t1 == 'f' || t1 == 'h') && (t2 == 'i' || t2 == 'f' || t2 == 'h'))return true;
//   return false;

// }
// //*************************************************************
// int32 search_int32_array(int32 a[], uint32 al, int32 n) {
//   if (al < 1)return -1;
//   for (int32 i = 0; i < al; ++i) {
//     if (a[i] == n)return i;
//   }
//   return -1;
// }
// //*************************************************************
// Boolean delete_int32_element_array(int32 a[], uint32 al, int32 n, Boolean delete_last) {
//   int32 tmp[al - 1];
//   uint32 co = 0;
//   Boolean is_find = false;
//   if (delete_last) {
//     co = al - 2;
//     for (int32 i = al - 1; i >= 0; i--) {
//       if (i < 0)break;
//       if (a[i] == n && !is_find) is_find = true;
//       else tmp[co--] = a[i];
//     }
//   } else
//     for (int32 i = 0; i < al; ++i) {
//       if (a[i] == n && !is_find) is_find = true;
//       else tmp[co++] = a[i];
//     }
//   for (int32 i = 0; i < al - 1; ++i) {
//     a[i] = tmp[i];
//   }
//   a[al - 1] = -1;
//   return is_find;
// }
// //*************************************************************
// uint8 convert_index_to_int32(String ind, int32 ret[], Boolean manage_ques) {
//   str_list indexes = 0;
//   uint8 indexes_len = 0;
//   uint8 ret_len = 0;
//   indexes_len = (uint8) char_split(ind, ',', &indexes, true);
//   if (indexes_len > MAX_ARRAY_DIMENSIONS) {
//     //TODO:error
//     return 0;
//   }
//   for (uint32 i = 0; i < indexes_len; i++) {
//     if (manage_ques && str_ch_equal(indexes[i], '?'))ret[ret_len++] = 1;
//     else if (str_ch_equal(indexes[i], '?'))ret[ret_len++] = -1;
//     else ret[ret_len++] = str_to_int32(indexes[i]);
//   }
//   return ret_len;
// }
// //*************************************************************

// //*************************************************************
// String find_first_var_name(String exp, uint32 start, Boolean is_inverse, uint32 *endpoint) {
//   String ret = 0;
//   String buf = 0;
//   int32 bra = 0;
//   //is inverse----------------------------------
//   if (is_inverse) {
//     for (uint32 i = start; i >= 0; i--) {
//       if (i == 0) {
//         buf = char_append(buf, exp[i]);
//         (*endpoint) = 0;
//         break;
//       }
//       if ((exp[i] == ']') && bra == 0) {
//         bra--;
//         buf = char_append(buf, exp[i]);
//       } else if ((bra == 0) && exp[i] != '.' &&
//           (char_search(single_operators, exp[i]) ||
//               char_search(words_splitter, exp[i]) ||
//               exp[i] == '\"')) {
//         (*endpoint) = i;
//         break;
//       } else {
//         if (exp[i] == '[')bra++;
//         else if (exp[i] == ']')bra--;
//         buf = char_append(buf, exp[i]);
//       }
//       //printf ("$$$$:%s=>%s(%i,%c)\n", buf, str_reverse (buf), i, exp[i]);
//     }
//     buf = str_reverse(buf);
//     str_init(&ret, buf);
//   }
//     //not inverse----------------------------------
//   else {
//     uint32 len = str_length(exp);
//     for (uint32 i = start; i < len; i++) {
//       if ((exp[i] == '[') && bra == 0) {
//         bra++;
//         buf = char_append(buf, exp[i]);
//       } else if ((bra == 0) && exp[i] != '.' &&
//           (char_search(single_operators, exp[i]) ||
//               char_search(words_splitter, exp[i]) ||
//               exp[i] == '\"')) {
//         (*endpoint) = i;
//         break;
//       } else {
//         if (exp[i] == '[')bra++;
//         else if (exp[i] == ']')bra--;
//         buf = char_append(buf, exp[i]);
//       }
//     }
//     str_init(&ret, buf);
//   }
//   return ret;
// }
// //*************************************************************
// String format_int32_array(int32 s[], uint32 start, uint32 end) {
//   String ret = "{";
//   for (uint32 i = start; i < end; i++) {
//     ret = str_append(ret, str_from_int32(s[i]));
//     if (i + 1 < end)ret = char_append(ret, ',');
//   }
//   ret = char_append(ret, '}');
//   return ret;
// }

// //*************************************************************
// String return_type_structure(uint8 t) {
//   if (t == IF_STRU_ID)return "if";
//   if (t == ELIF_STRU_ID)return "elif";
//   if (t == ELSE_STRU_ID)return "else";
//   if (t == LOOP_STRU_ID)return "loop";
//   if (t == MANAGE_STRU_ID)return "manage";
//   if (t == SWITCH_STRU_ID)return "switch";
// }
// //*************************************************************
// /**
//  * get a valid path (relative or absolute) and return filename if exist and if ext not zero return file extension
//  * @param path
//  * @param ext
//  * @return String
//  */
// String return_file_name_extension_path(String path, String *ext, Boolean must_ext) {
//   str_list ret = 0;
//   uint32 len = 0;
//   int32 pos = char_last_indexof(path, OS_SEPARATOR);
//   if (pos == -1) {
//     len = char_split(path, '.', &ret, true);
//     if (must_ext && len < 2)return 0;
//   } else {
//     String sub = str_substring(path, pos + 1, 0);
//     len = char_split(sub, '.', &ret, true);
//     if (must_ext && len < 2)return 0;
//   }
//   if (ext != 0 && len > 1) {
//     str_list_delete_first(&ret, len--);
//     (*ext) = char_join(ret, '.', len, true);
//   }
// //  printf("Filename:%s=>%s\n", path, ret[0]);
//   return ret[0];
// }
// //*************************************************************
// String convert_sub_type_to_type(uint8 sub_type) {
//   if (sub_type == 0)return 0;
//   if (sub_type == 'i' || sub_type == 'f' || sub_type == 'h')return "num";
//   if (sub_type == 's' || sub_type == 'u')return "str";
//   if (sub_type == 'b')return "bool";
//   return 0;
// }
// //*************************************************************
// Boolean has_suffix_for_array(String s, str_list ar, uint32 ar_len) {
//   for (uint32 i = 0; i < ar_len; i++) {
//     if (str_has_suffix(s, ar[i]))return true;
//   }
//   return false;
// }

// //*************************************************************
