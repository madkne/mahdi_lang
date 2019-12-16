//
// Created by madkne on 2019.10.24
//

#ifndef MAHDI_SETTINGS_H
#define MAHDI_SETTINGS_H

//------------------------------------------basic settings
#define  true                           1
#define  false                          0
//------------------------------------------main settings
#define  OS_BUILD                       "linux-debian"
#define  LINUX_PLATFORM                  true
#define  WINDOWS_PLATFORM                false
#define  x86_OS                          true
#define  x64_OS                          false
#define  C_COMPILER_STD                  "C99"
#define  C_COMPILER_NAME                 "GCC"
#define  BUILD_DATE                      __DATE__
#define  BUILD_TIME                      __TIME__
//------------------------------------------define __func__ constant
#if __STDC_VERSION__ < 199901L
#if __GNUC__ >= 2
#define __func__ __FUNCTION__
#else
#define __func__ "<unknown>"
#endif
#endif


#endif //MAHDI_SETTINGS_H
