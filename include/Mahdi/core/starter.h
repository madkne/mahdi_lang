//
// Created by madkne on 2019.10.26
//

#ifndef __MAHDI_STARTER_H
#define __MAHDI_STARTER_H   1

Boolean APP_start();

int8 APP_controller();
int8 APP_executor(Longint id);
Boolean APP_init_global();
uint8 APP_labeled_instruction(String code);
void APP_exit(int32 i);
#endif //__MAHDI_STARTER_H
