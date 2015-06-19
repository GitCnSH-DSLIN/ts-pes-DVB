/**************************************************************************
*
* Filename      :   print_debug.h
* Description   :   for debug
* Edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
**************************************************************************/
#define DEBUG

#ifdef DEBUG
#   define uprintf printf
#else
#   define uprintf(fmt, args...)
#endif
