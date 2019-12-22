
#include <Mahdi/system.h>
//******************************************
String CALL_hostname() {
  #if WINDOWS_PLATFORM == true
  return getenv("COMPUTERNAME");
  #elif LINUX_PLATFORM == true
  return getenv("HOSTNAME");
  #endif
  return "<unknown>";
}
//******************************************
String CALL_homedir() {
  #if WINDOWS_PLATFORM == true
  return str_append(getenv("HOMEDRIVE"), getenv("HOMEPATH"));
  #elif LINUX_PLATFORM == true
  return getenv("HOME");
  #endif
  return "<unknown>";
}
//******************************************
Longint CALL_pid() {
  #if WINDOWS_PLATFORM == true
  return (long_int) GetCurrentProcessId();
  #elif LINUX_PLATFORM == true
  //TODO:
  #endif
  return 0;
}
//******************************************
Boolean CALL_is_directory(String path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}
//******************************************
String CALL_parent_path(String path){
  //=>init vars
  StrList entries;
  String parent=0;
  //=>split from path separator and store in entries list
  uint32 size = CH_split(path, OS_SEPARATOR, &entries, true);
  //=>join enreies without last item
  parent = CH_join(entries, OS_SEPARATOR, size - 1, true);
//  printf("GGGGG:%s,%s\n",project_root,stdin_source_path);
  #if LINUX_PLATFORM == true
  parent = STR_append("/", parent);
  #endif
  return parent;
}
//******************************************
String CALL_abspath(String path) {
  if(path==0)return 0;
  #if LINUX_PLATFORM == 1
    char abs_path[PATH_MAX+1];
    char *stat=realpath(path,abs_path);
    // printf("abspath:%s=>%s(%i,%i,%i)\n",path,abs_path,stat,errno,ENOENT);
    if(errno==0)return STR_from_const(abs_path);
    //=>has an error
    return 0;
  #elif WINDOWS_PLATFORM == 1
    char tmp[BufferSize];
    String abs_path = 0;
    GetFullPathName(path, BufferSize, tmp, 0);
    STR_init(&abs_path, tmp);
    if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(abs_path) && GetLastError() == ERROR_FILE_NOT_FOUND) return 0;
    //printf("\nSSS::%s=>%s\n",path,abs_path);
    return abs_path;
  #endif
  return 0;
}
//******************************************
Longint CALL_unix_time() {
  return (Longint) time(NULL);
}
//******************************************
String CALL_datetime(uint8 type) {
  time_t t = time(NULL);
  struct tm tim = *localtime(&t);
  //char s[64];
  String ret = 0;
  //date and time : 2018-6-23 3:45:12
  if (type == 1) {
    ret = malloc(6 * sizeof(int) + 5);
    sprintf(ret, "%i-%i-%i %i:%i:%i", tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min,
            tim.tm_sec);
  }

  //strftime(s, sizeof(s), "%c", tm);
  return ret;
}
//******************************************
String CALL_stdin() {
  String ret = "";
  uint8 c = 0;
  for (;;) {
    c = fgetc(stdin);
    if (c == EOF || c == '\n'/*ENTER*/) break;
    ret = CH_append(ret, c);
  }
  return ret;
}
//******************************************
int32 CALL_read_file(String path, StrList *lines, Boolean skip_empty_lines) {
  FILE *fp = fopen(path, "r");
  if (fp == NULL)return -1;
  //   printf ("PATH:%s\n", path);
  Boolean is_end = false;
  int32 lines_co = 0;
  for (;;) {
    String line = 0;
    for (;;) {
      int32 buf = fgetc(fp);
      if (buf == EOF) {
        is_end = true;
        break;
      }
      if (buf == 0)continue;
      if ('\n' == buf || '\r' == buf)break;
      line = CH_append(line, buf);
  //      printf("-----%s\n", line);
    }
  //    printf("CO:%i\n",++co);
    if (skip_empty_lines) {
      line = STR_trim_space(line);
      if (line == 0 && !is_end)continue;
      else if (line == 0 && is_end)break;
    }
  //    printf ("LI:%s\n", line);
    SLIST_append(&(*lines), line, lines_co++);
    if (is_end)break;
  }
  fclose(fp);
  return lines_co;

}
//******************************************
Boolean CALL_write_file(String path, String s) {
  FILE *fp = fopen(path, "w");
  if (fp == NULL)return false;
  //=>write to file
  fwrite(s, sizeof(uint8), STR_length(s), fp);
  fclose(fp);
  return true;
}
//******************************************
Boolean CALL_exist_file(String path) {
  FILE *fp = fopen(path, "r");
  if (fp == NULL)return false;
  fclose(fp);
  return true;
}

//******************************************
String CALL_get_line(FILE *fp) {
  char *line = malloc(100), *linep = line;
  size_t lenmax = 100, len = lenmax;
  int c;

  if (line == NULL)
    return NULL;

  for (;;) {
    c = fgetc(fp);
    if (c == EOF)
      break;

    if (--len == 0) {
      len = lenmax;
      char *linen = realloc(linep, lenmax *= 2);

      if (linen == NULL) {
        free(linep);
        return NULL;
      }
      line = linen + (line - linep);
      linep = linen;
    }

    if ((*line++ = c) == '\n')
      break;
  }
  *line = '\0';
  return linep;
}
//******************************************
Boolean CALL_mkdir(String path, Boolean is_make_parents) {
  if (is_make_parents) {
    #if WINDOWS_PLATFORM == true
    //TODO:
    #elif LINUX_PLATFORM == true
    int8 status = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    return(status==0)? true:false;
    #endif
  } else {
    #if WINDOWS_PLATFORM == true
    int err = CreateDirectoryA(path, NULL);
//    printf("mkdir:%s=>%i\n",path,err);
    if (err == 0)return false;
    else return true;
    #elif LINUX_PLATFORM == true
    int8 status =mkdir(path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    return(status==0)? true:false;
    #endif
  }
  return false;
}
//******************************************
uint32 CALL_rand(uint32 min, uint32 max) {
  //=>Use current time as seed for random generator
  srand(time(0));
  //max >= min and 1+max-min < RAND_MAX
  if (min > max) {
    uint32 tmp = max + 1; // include max in output
    max = min;
    min = tmp;
  }
  if (max > RAND_MAX)max = RAND_MAX;
  uint32 result = (rand() % (max - min)) + min;
  return result;
}
//******************************************
Boolean CALL_binary_copy(String src, String dst) {
  char buff[BUFSIZ];
  FILE *in, *out;
  size_t n;
  in = fopen(src, "rb");
  if (in == NULL)return false;
  out = fopen(dst, "wb");
  if (out == NULL)return false;
  while ((n = fread(buff, 1, BUFSIZ, in)) != 0) {
    fwrite(buff, 1, n, out);
  }
  fclose(in);
  fclose(out);
  return true;
}
//******************************************
String CALL_shell(String command) {
  //=>INIT VARS
  String ret = 0;
  FILE *fp;
  uint8 line[1035];
  StrList lines = 0;
  //=>Open the command for reading.
  fp = popen(command, "r");
  if (fp == NULL) {
    EXP_set_errcode(BAD_SHELL_ERRC);
    return 0;
  }
  //=>Read the output a line at a time - output it.
  while (fgets(line, sizeof(line) - 1, fp) != NULL) {
    ret = STR_append(ret, line);
  }
  //=>close file
  pclose(fp);
  return ret;
}
//******************************************
#if WINDOWS_PLATFORM == true
String CALL_reg_value(HKEY hkey, String path, String key) {
  String ret = 0;
  uint8 value[8192];
  DWORD BufferSize = 8192;
  LSTATUS status = RegGetValueA(hkey, path, key, RRF_RT_ANY, NULL, (PVOID) &value, &BufferSize);
  if (status == ERROR_SUCCESS) {
    for (uint32 i = 0; i < (uint32) BufferSize; i++) {
      ret = CH_append(ret, value[i]);
    }
    return ret;
  }
  return 0;
}

#endif