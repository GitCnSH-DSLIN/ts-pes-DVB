/*********************************************************************
*
* Filename      :   sdt.c
* Description   :   given the fundamental struction of SDT table and SDT service.
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/

#ifndef  _MPEG_TS_SI_H_
#define  _MPEG_TS_SI_H_
/*********************************************************************
 *              SDT releated struction
 *   struction : sdt table --> sdt_service --> descriptor
 *   
 *********************************************************************/
#define PID_TS_SI_SDT   (0x0011)
#define TABLE_ID_SDT_ACTUAL     (0x42)
#define TABLE_ID_SDT_OTHER     (0x46)

typedef struct sdt_descriptor_common{
    unsigned char descriptor_tag;
    unsigned char descriptor_length;
    struct sdt_descriptor_common * next_desc;
}SDT_DESCRIPTOR_COMMON, *P_SDT_DESCRIPTOR_COMMON;

//SDT service
typedef struct sdt_service {
	unsigned service_id : 16;
	unsigned reserved_future_use : 6;
	unsigned EIT_schedule_flag : 1;
	unsigned EIT_present_following_flag : 1;
	unsigned running_status : 3;
	unsigned free_CA_mode : 1;
	unsigned descriptors_loop_length : 12;
	
    SDT_DESCRIPTOR_COMMON * first_desc; //point to descriptor list.

	struct sdt_service* next_sdt_service;
}SDT_SERVICE, *P_SDT_SERVICE;



//SDT table
typedef struct ts_sdt_table {
	unsigned table_id : 8;
	unsigned section_syntax_indicator : 1;
	unsigned reserved_future_use_1 : 1;
	unsigned reserved_1 : 2;
	unsigned section_length : 12;
	unsigned transport_stream_id : 16;
	unsigned reserved_2 : 2;
	unsigned version_number : 5;
	unsigned current_next_indicator : 1;
	unsigned section_number : 8;
	unsigned last_section_number : 8;
	unsigned original_network_id : 16;
	unsigned reserved_future_use_2 : 8;
	
    SDT_SERVICE * first_sdt_service;
    struct ts_sdt_table * sdt_next_section;
	unsigned CRC_32;
}TS_SDT_TABLE,*P_TS_SDT_TABLE;

//releated operation

#define	SDT_TABLE_ID(b)				    (b[0])			/*  */

#define SDT_SECTION_SYNTAX_INDICATOR(b)			    ((b[1] & 0x80) >> 7)				/*  */

#define	SDT_RESERVED_FUTURE_USD_1(b)		    ((b[1] & 0x40) >> 6)			/*  */

#define	SDT_RESERVED_1(b)			    ((b[1] & 0x30) >> 4)			/*  */

#define	SDT_SECTION_LENGTH(b)			    (((b[1] & 0x0f) << 8) | b[2])			/*  */

#define	SDT_TRANSPORT_STREAM_ID(b)				    ((b[3] << 8) | b[4])			/*  */

#define	SDT_RESERVED_2(b)			    ((b[5] & 0xc0) >> 6)			/*  */

#define	SDT_VERSION_NUMBER(b)			    ((b[5] & 0x3e) >> 1)			/*  */

#define	SDT_CURRENT_NEXT_INDICATOR(b)			    (b[5] & 0x01)			/*  */

#define	SDT_SECTION_NUMBER(b)			    (b[6])		/*  */

#define	SDT_LAST_SECTION_NUMBER(b)			    (b[7])			/*  */

#define	SDT_ORIGINAL_NETWORK_ID(b)		    ((b[8] << 8) | b[9])			/*  */

#define	SDT_RESERVED_FUTURE_USD_2(b)		    (b[10])			/*  */
////////////////////////////////////////////////////

#define	SDT_SERVICE_ID(b)			    ((b[0] << 8) | b[1])			/*  */

#define	SDT_SERVICE_RESERVED_FUTURE_USD(b)		    ((b[2] & 0xfc) >> 2)			/*  */

#define	SDT_SERVICE_EIT_SCHEDULE_FLAG(b)		    ((b[2] & 0x02) >> 1)			/*  */

#define	SDT_SERVICE_EIT_PRESENT_FOLLOWING_FLAG(b)	    (b[2] & 0x01)			/*  */

#define	SDT_SERVICE_RUNNING_STATUS(b)			    ((b[3] & 0xe0) >> 5)			/*  */

#define	SDT_SERVICE_FREE_CA_MODE(b)		    ((b[3] & 0x10) >> 4)			/*  */

#define	SDT_SERVICE_DESCRIPTORS_LOOP_LENGTH(b)			    (((b[3] & 0x0f) << 8) | b[4])			/*  */
/////////////////////////////////////////////////
#define	SDT_CRC(b)				    ({ \
							int n = SDT_SECTION_LENGTH(b); \
							int l = n - 4; \
							int base = 3 + l; \
							((b[base] << 24) | (b[base + 1] << 16) | (b[base + 2] << 8) | b[base + 3]); \
							})


P_SDT_SERVICE insert_sdt_service_node(SDT_SERVICE * Header, SDT_SERVICE * node);
int decode_sdt_service(unsigned char * byteptr, int this_section_length, SDT_SERVICE* psdtService);
TS_SDT_TABLE * parse_sdt_table(FILE *pFile, unsigned int packetLength);
TS_SDT_TABLE * parse_sdt_table_onesection(unsigned char *byteptr, TS_SDT_TABLE * pSdtTable);
void show_sdt_service_info(SDT_SERVICE * Header);
int show_sdt_table_info(TS_SDT_TABLE * pSdtTable);
void free_sdt_service(TS_SDT_TABLE * sdt);
void free_sdt_table(TS_SDT_TABLE * sdt_table_header);

/***************************** End of SDT releated struction **********************/

#endif   /* ----- #ifndef _MPEG_TS_SI_H_  ----- */
