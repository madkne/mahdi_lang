

#include <Mahdi/system.h>

Boolean CMD_enable_color_mode() {
  #if WINDOWS_PLATFORM == true
  // Set output mode to handle virtual terminal sequences
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE) return false;
  DWORD dwMode = 0;
  if (!GetConsoleMode(hOut, &dwMode))return false;
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  if (!SetConsoleMode(hOut, dwMode))return false;
  #endif
}
void CMD_color_red() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[1;31m");
  #endif
}
//********************************
void CMD_color_yellow() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[1;33m");
  #endif
}
//********************************
void CMD_color_bold() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[1m");
  #endif
}
//********************************
void CMD_color_underline() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[4m");
  #endif
}
//********************************
/**
 * Swaps foreground and background colors
 */
void CMD_color_reverse() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[7m");
  #endif
}
//********************************
void CMD_color_reset() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[0m");
  #endif
}

