//
// Created by madkne on 2019.10.26
//

#ifndef __MAHDI_CONSOLE_H
#define __MAHDI_CONSOLE_H

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

Boolean CMD_enable_color_mode();
void CMD_color_red(FILE *__restrict __stream);
void CMD_color_yellow(FILE *__restrict __stream);
void CMD_color_bold(FILE *__restrict __stream);
void CMD_color_underline(FILE *__restrict __stream);
void CMD_color_reverse(FILE *__restrict __stream);
void CMD_color_reset(FILE *__restrict __stream);
#endif //__MAHDI_CONSOLE_H
