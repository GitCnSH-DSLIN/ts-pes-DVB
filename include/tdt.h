/*********************************************************************
*
* Filename      :   tdt.h
* Description   :   given the fundamental struction of TDT table.
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/

#ifndef  _MPEG_TS_SI_TDT_H_
#define  _MPEG_TS_SI_TDT_H_

#include <descriptor_common.h>

/*********************************************************************
 *              TDT releated struction
 *   struction : TDT table  --> descriptor
 *   
 *********************************************************************/
#define PID_TS_SI_TDT       (0x0014)
#define TABLE_ID_TDT        (0x70)

//TDT table
typedef struct ts_tdt_table {
	unsigned table_id : 8;
	unsigned section_syntax_indicator : 1;
	unsigned reserved_future_use_1 : 1;
	unsigned reserved_1 : 2;
	unsigned section_length : 12;
    unsigned long UTC_time; //40 bit
}TS_TDT_TABLE,*P_TS_TDT_TABLE;

//releated operation

#define	TDT_TABLE_ID(b)				                (b[0])			/*  */

#define TDT_SECTION_SYNTAX_INDICATOR(b)			    ((b[1] & 0x80) >> 7)				/*  */

#define	TDT_RESERVED_FUTURE_USE_1(b)		        ((b[1] & 0x40) >> 6)			/*  */

#define	TDT_RESERVED_1(b)			                ((b[1] & 0x30) >> 4)			/*  */

#define	TDT_SECTION_LENGTH(b)			            (((b[1] & 0x0f) << 8) | b[2])			/*  */

#define	TDT_UTC_TIME(b)				                ({ \
                                                    unsigned long utc_high,utc_low; \
                                                    utc_high = ((unsigned long)(b[3] & 0xff)) << 32; \
                                                    utc_low  = ((unsigned long)(b[4] << 24 | b[5] << 16 | \
                                                            b[6]<<8 | b[7])) & 0x00ffffffff; \
                                                    utc_high  | utc_low; \
                                                    })/*  */


TS_TDT_TABLE * parse_tdt_table(FILE *pFile, unsigned int packetLength);
TS_TDT_TABLE * parse_tdt_table_onesection(unsigned char *byteptr, TS_TDT_TABLE * pTdtTable);
int show_tdt_table_info(TS_TDT_TABLE * ptdtTable);
void free_tdt_table(TS_TDT_TABLE * tdt_table_header);
/***************************** End of TDT releated struction **********************/

#endif   /* ----- #ifndef _MPEG_TS_SI_TDT_H_  ----- */


