

#include <Mahdi/system.h>

Boolean CMD_enable_color_mode() {
  #if WINDOWS_PLATFORM == true
  //=>Set output mode to handle virtual terminal sequences
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE) return false;
  DWORD dwMode = 0;
  if (!GetConsoleMode(hOut, &dwMode))return false;
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  if (!SetConsoleMode(hOut, dwMode))return false;
  #elif LINUX_PLATFORM == true
  //=>check command output
  String result=CALL_shell("echo $TERM");
  if(STR_indexof(result,"xterm",0)==-1){
    return false;
  }
  #endif
  return true;
}
void CMD_color_red(FILE *__restrict __stream) {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[1;31m");
  #elif LINUX_PLATFORM == true
  fprintf(__stream,"\033[1;31m");
  #endif
}
//********************************
void CMD_color_yellow(FILE *__restrict __stream) {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[1;33m");
  #elif LINUX_PLATFORM == true
  fprintf(__stream,"\033[1;33m");
  #endif
}
//********************************
void CMD_color_bold(FILE *__restrict __stream) {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[1m");
  #elif LINUX_PLATFORM == true
  fprintf(__stream,"\033[1m");
  #endif
}
//********************************
void CMD_color_underline(FILE *__restrict __stream) {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[4m");
  #elif LINUX_PLATFORM == true
  fprintf(__stream,"\033[4m");
  #endif
}
//********************************
/**
 * Swaps foreground and background colors
 */
void CMD_color_reverse(FILE *__restrict __stream) {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[7m");
  #elif LINUX_PLATFORM == true
  fprintf(__stream,"\033[7m");
  #endif
}
//********************************
void CMD_color_reset(FILE *__restrict __stream) {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[0m");
  #elif LINUX_PLATFORM == true
  fprintf(__stream,"\033[0m");
  #endif
}

