//
// Created by amindelavar on 1/17/2019.
//

#include <MPL/system.h>

String MPLV1_encode(String s) {
  //init vars
  String result = 0, lvl1 = 0, lvl2 = 0;
  uint32 len = str_length(s);
  //-------------------------------
  //=>level1: convert chars between 100..255
  /**
   * special chars: (max=131)
   * \n : 127
   * \t : 128
   * \b : 129
   * \a : 130
   */
  for (uint32 i = 0; i < len; i++) {
    if (s[i] == '\n')s[i] = 127;
    else if (s[i] == '\t')s[i] = 128;
    else if (s[i] == '\b')s[i] = 129;
    else if (s[i] == '\a')s[i] = 130;
    uint8 ch_code = s[i] + 68;
    lvl1 = char_append(lvl1, ch_code);
//    printf("%i ", ch_code);
  }
//  printf("\n");
//  printf("\nlevel1:%s=>%s$\n", s, lvl1);
  //=>level2: optimize every two chars
  /**
   * (A) 192 192 (||) => 10  3  (1..10 is for same two chars)
   * (B) 177 173 (mi) => 18 73 (11..20 is for first two same digits for two chars)
   * (C) 177 133 (mA) => 25 73 (25 is for last two same digits for two chars)
   */
  for (uint32 i = 0; i < len; i++) {
    //(A)
    if (i + 1 < len && lvl1[i] == lvl1[i + 1]) {
      lvl2 = char_append(lvl2, (lvl1[i] % 100) / 10 + 1);
      lvl2 = char_append(lvl2, (lvl1[i] % 10) + 1);
      i++;
      continue;
    }
      //(B)
    else if (i + 1 < len && lvl1[i] / 10 == lvl1[i + 1] / 10) {
      lvl2 = char_append(lvl2, lvl1[i] / 10 + 1);
      lvl2 = char_append(lvl2, (lvl1[i] % 10) * 10 + (lvl1[i + 1] % 10));
      i++;
      continue;
    }
      //(C)
    else if (i + 1 < len && (lvl1[i] % 100) % 11 == 0 && (lvl1[i + 1] % 100) % 11 == 0) {
      lvl2 = char_append(lvl2, 25);
      lvl2 = char_append(lvl2, (lvl1[i] % 10) * 10 + (lvl1[i + 1] % 10));
      i++;
      continue;
    }
      //ignore
    else lvl2 = char_append(lvl2, lvl1[i]);
  }
//  for (uint32 i = 0; i < len; i++)printf("%i ", lvl2[i]);
//  printf("\nlevel2:%s=>%s$\n", s, lvl2);
//=>return final result
  result = lvl2;
  return result;
}
//****************************************
String MPLV1_decode(String s) {
  //init vars
  String result = 0, lvl1 = 0, lvl2 = 0;
  uint32 len = str_length(s);
//-------------------------------
//=>level2: ignore charcodes over 100 and change charcodes less 100
/**
   * (A) 192 192 (||) => 10  3  (1..10 is for same two chars)
   * (B) 177 173 (mi) => 18 73 (11..20 is for first two same digits for two chars)
   * (C) 177 133 (mA) => 25 73 (25 is for last two same digits for two chars)
   */
  for (uint32 i = 0; i < len; i++) {
    //over 100
    if (s[i] >= 100) lvl2 = char_append(lvl2, s[i]);
      //(A)
    else if (i + 1 < len && s[i] <= 10) {
      lvl2 = char_append(lvl2, 100 + (s[i] - 1) * 10 + (s[i + 1] - 1));
      lvl2 = char_append(lvl2, 100 + (s[i] - 1) * 10 + (s[i + 1] - 1));
      i++;
      continue;
    }
      //(B)
    else if (i + 1 < len && s[i] <= 20) {
      lvl2 = char_append(lvl2, (s[i] - 1) * 10 + (s[i + 1] / 10));
      lvl2 = char_append(lvl2, (s[i] - 1) * 10 + (s[i + 1] % 10));
      i++;
      continue;
    }
      //(C)
    else if (i + 1 < len && s[i] == 25) {
      lvl2 = char_append(lvl2, 100 + (s[i + 1] / 10) * 11);
      lvl2 = char_append(lvl2, 100 + (s[i + 1] % 10) * 11);
      i++;
      continue;
    }
      //error
    else return 0;
  }
//  for (uint32 i = 0; i < len; i++)
//    printf("%i ", s[i]);
//  printf("\n");
//  for (uint32 i = 0; i < len; i++)
//    printf("%i ", lvl2[i]);
//  printf("\n");
  //=>level1: convert chars between 32..126
  len = str_length(lvl2);
  for (uint32 i = 0; i < len; i++) {
    uint8 ch_code = lvl2[i] - 68;
    if (ch_code == 127)ch_code = '\n';
    else if (ch_code == 128)ch_code = '\t';
    else if (ch_code == 129)ch_code = '\b';
    else if (ch_code == 130)ch_code = '\a';
    lvl1 = char_append(lvl1, ch_code);
//    printf("%i ", ch_code);
  }
//  printf("\nDECODE:%s=>%s=>%s$\n", s, lvl2, lvl1);

  result = lvl1;
  return result;

}

