
#include <Mahdi/system.h>

//******************************************
String CALL__hostname() {
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
String CALL_abspath(String path) {
  //uint8 resolved_path[PATH_MAX+1];
//   #if LINUX_PLATFORM == 1
//   String abs_path=0;
//   abs_path=realpath(path,0);
//   //printf("\n%s=>%s\n",path,abs_path);
//   return abs_path;
//   #elif WINDOWS_PLATFORM == 1
//   char tmp[BufferSize];
//   String abs_path = 0;
//   GetFullPathName(path, BufferSize, tmp, 0);
//   STR_init(&abs_path, tmp);
//   if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(abs_path) && GetLastError() == ERROR_FILE_NOT_FOUND) return 0;
// //  printf("\nSSS::%s=>%s\n",path,abs_path);
//   return abs_path;
//   #endif
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
Boolean CALL__write_file(String path, String s) {
  FILE *fp = fopen(path, "w");
  if (fp == NULL)return false;
  //=>write to file
  fwrite(s, sizeof(uint8), STR_length(s), fp);
  fclose(fp);
  return true;
}
//******************************************
Boolean CALL__exist_file(String path) {
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
    //TODO
  } else {
    #if WINDOWS_PLATFORM == true
    int err = CreateDirectoryA(path, NULL);
//    printf("mkdir:%s=>%i\n",path,err);
    if (err == 0)return false;
    else return true;
    #elif LINUX_PLATFORM == true
    mkdir(path,777);
    //TODO
    #endif

  }

  return true;
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