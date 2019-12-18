//
// Created by madkne on 2019.12.18
//


#ifndef __MAHDI_PARSER_H
#define __MAHDI_PARSER_H  1

#define GET_ORDER_ID(x1, x2, x3) ((x1*max_size_id*max_size_id)+(x2*max_size_id)+x3)

//********************************
Boolean PARSER_analyze_source_code();
void PARSER_get_tokens();



#endif /*__MAHDI_PARSER_H*/


