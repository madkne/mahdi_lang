//
// Created by madkne on 2019.10.26
//

#ifndef __MAHDI_HELP_H
#define __MAHDI_HELP_H  1

//*******************************defines
#define header_full   "________________________"
#define header_sp     "___|____________________"
#define DRAW_TABLE_ROW(ID, ROW) printf("   |\n%3i| %s\n%s\n", ID, ROW,header_sp);
#define DRAW_TABLE_FOOTER(ROW) printf("%s\n   |\n:::| %s\n%s\n",header_full, ROW,header_sp);
#define MAHDI_LICENCE_VIEW_COMMAND  "mahdi -h about license"
//*******************************arrays
String MHELP_groups[10];
String MHELP_get_started_subjects[3];
String MHELP_about_subjects[4];

//*******************************functions
Boolean MHELP_starter(StrList argvs, uint32 argvs_len);
void MHELP_usage();
void MHELP_version();
void MHELP_info();
void MHELP_main();
int8 MHELP_print_article(String path, String part,Boolean generate_list);
void MHELP_article(String params[3], String *not_exist);
void MHELP_list(uint8 type, String params[3], String *not_exist);
#endif //__MAHDI_HELP_H
