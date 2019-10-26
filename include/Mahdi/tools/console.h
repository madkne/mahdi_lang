//
// Created by madkne on 2019.10.26
//

#ifndef __MAHDI_CONSOLE_H
#define __MAHDI_CONSOLE_H

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

Boolean CMD_enable_color_mode();
void CMD_color_red();
void CMD_color_yellow();
void CMD_color_bold();
void CMD_color_underline();
void CMD_color_reverse();
void CMD_color_reset();
#endif //__MAHDI_CONSOLE_H
