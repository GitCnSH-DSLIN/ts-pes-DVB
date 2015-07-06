/*********************************************************************
*
* Filename      :   tot.h
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/

#ifndef  _MPEG_TS_SI_TOT_H_
#define  _MPEG_TS_SI_TOT_H_

#include <descriptor_common.h>
/*********************************************************************
 *              TOT releated struction
 *   struction : TOT table --> descriptor
 *
 *********************************************************************/
#define PID_TS_SI_TOT       (0x0014)
#define TABLE_ID_TOT        (0x73)


//TOT table
typedef struct ts_tot_table {
	unsigned table_id : 8;
	unsigned section_syntax_indicator : 1;
	unsigned reserved_future_use_1 : 1;
	unsigned reserved_1 : 2;
	unsigned section_length : 12;
    unsigned long UTC_time; //40 bit
	unsigned reserved_2 : 4;
	unsigned descriptors_loop_length : 12;
    DESCRIPTOR_COMMON * tot_first_desc;// add by myself

	unsigned CRC_32;
}TS_TOT_TABLE,*P_TS_TOT_TABLE;

//releated operation

#define	TOT_TABLE_ID(b)				                (b[0])			/*  */

#define TOT_SECTION_SYNTAX_INDICATOR(b)			    ((b[1] & 0x80) >> 7)				/*  */

#define	TOT_RESERVED_FUTURE_USE_1(b)		        ((b[1] & 0x40) >> 6)			/*  */

#define	TOT_RESERVED_1(b)			                ((b[1] & 0x30) >> 4)			/*  */

#define	TOT_SECTION_LENGTH(b)			            (((b[1] & 0x0f) << 8) | b[2])			/*  */


#define	TOT_UTC_TIME(b)				                ({ \
                                                    unsigned long utc_high,utc_low; \
                                                    utc_high = ((unsigned long)(b[3] & 0xff)) << 32; \
                                                    utc_low  = ((unsigned long)(b[4] << 24 | b[5] << 16 | \
                                                            b[6]<<8 | b[7])) & 0x00ffffffff; \
                                                    utc_high  | utc_low; \
                                                    })/*  */


#define	TOT_RESERVED_2(b)			                ((b[8] & 0xf0) >> 4)			/*  */

#define	TOT_DESC_LOOP_LENGTH(b)		                ((b[8] & 0x0f) << 8 | b[9])			/*  */
////////////////////////////////////////////////////
#define	TOT_CRC_32(b)				    ({ \
							int n = TOT_SECTION_LENGTH(b); \
							int l = n - 4; \
							int base = 3 + l; \
							((b[base] << 24) | (b[base + 1] << 16) | (b[base + 2] << 8) | b[base + 3]); \
							})

TS_TOT_TABLE * parse_tdt_table_onesection(unsigned char *byteptr, TS_TOT_TABLE * pTotTable);
TS_TOT_TABLE * parse_tot_table(FILE *pFile, unsigned int packetLength);
void show_tot_descriptors_info(DESCRIPTOR_COMMON *header);
int show_tot_table_info(TS_TOT_TABLE * ptotTable);
void free_tot_table(TS_TOT_TABLE * tot_table_header);

/***************************** End of TDT releated struction **********************/

#endif   /* ----- #ifndef _MPEG_TS_SI_H_  ----- */


