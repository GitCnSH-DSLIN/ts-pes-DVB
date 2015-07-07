/*********************************************************************
*
* Filename      :   pmt.h
* Description   :   fundamental struction of PMT table
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/

#ifndef  _MPEG_TS_SI_PMT_H_
#define  _MPEG_TS_SI_PMT_H_

#include <ts_list.h>
#include <descriptor_common.h>


#define TABLE_ID_TS_PMT         (0x02)

typedef struct ts_pmt_stream
{
	unsigned stream_type : 8;
	unsigned reserved_5 : 3;
	unsigned elementary_PID : 13;
	unsigned reserved_6 : 4;
	unsigned ES_info_length : 12; 
    
    struct list_head program_list;//list_head for one section.
    struct list_head list;//mount on global ts_pmt_stream_list.
    unsigned int program_number;
}TS_PMT_Stream,* P_TS_PMT_Stream;


//PMT -- Program Map Table
typedef struct ts_pmt_table
{
	unsigned table_id : 8;
	unsigned section_syntax_indicator : 1;
	unsigned zero : 1;
	unsigned reserved_1 : 2;
	unsigned section_length : 12;
	unsigned program_number : 16;
	unsigned reserved_2 : 2;
	unsigned version_number : 5;
	unsigned current_next_indicator : 1;
	unsigned section_number : 8;
	unsigned last_section_number : 8;
	unsigned reserved_3 : 3;
	unsigned PCR_PID : 13;
	unsigned reserved_4 : 4;
	unsigned program_info_length : 12;	//bytes number

    DESCRIPTOR_COMMON * first_desc;

    struct ts_pmt_stream this_section_pmt_stream_head;
    struct ts_pmt_table * pmt_next_section;

	unsigned CRC_32 : 32; 
}TS_PMT_TABLE,*P_TS_PMT_TABLE; 

#define PMT_TABLE_ID(b, offset)                     (b[offset])
#define PMT_SECTION_SYNTAX_INDICATOR(b, offset)     (b[offset + 1] >> 7)
#define PMT_SECTION_LENGTH(b,offset)                (((b[offset + 1] & 0x0F) << 8) | b[offset + 2])
#define PMT_PROGRAM_NUM(b, offset)                  ((b[offset + 3] << 8) | buffer[offset + 4]) 
#define PMT_VERSION_NUM(b, offset)                  ((b[offset + 5] >> 1) & 0x1F)
#define PMT_CUR_NEXT_INDICATOR(b, offset)           ((b[offset + 5] << 7) >>7) 
#define PMT_SECTION_NUM(b, offset)                  (b[offset + 6])
#define PMT_LAST_SECTION_NUM(b, offset)             (b[offset + 7])
#define PMT_PCR_PID(b,offset)                       (((b[offset + 8] << 8) | buffer[9]) & 0x1FFF);
#define PMT_PROGRAM_INFO_LENGTH(b,offset)           (((b[offset + 10] & 0x0F) << 8) | b[offset + 11])
#define PMT_CAC32(b,offset)                         ({ int len = PMT_SECTION_LENGTH(b,offset) + 3; \
                                                        ((b[offset + len - 4] & 0xFF) << 24) | \
                                                        ((b[offset + len - 3] & 0xFF) << 16) | \
                                                        ((b[offset + len - 2] & 0xFF) << 8) | \
                                                        ((b[offset + len - 1] & 0xFF)); \
                                                    })

TS_PMT_Stream  __ts_pmt_stream_list;
void init_ts_pmt_stream_list(void);


TS_PMT_TABLE * parse_pmt_table_one_program(FILE *pFile, unsigned int packetLength,
        unsigned int programNumber, unsigned int programPid);  
int parse_pmt_table_onesection(unsigned char * pBuffer, TS_PMT_TABLE * psiPMT, 
        unsigned int programNumber, unsigned int porgramPid); 
void free_pmt_table_onesection(TS_PMT_TABLE * pmt_table, int reserve_pmt_list_flag);
void free_pmt_table_one_program(TS_PMT_TABLE * pmt_table_header, int reserve_pmt_list_flag);

int judge_media_type(P_TS_PMT_Stream ptsPmtStream);
int show_pmt_stream_info_onesection(TS_PMT_TABLE * pmtTable);
int show_pmt_table_info_one_program(TS_PMT_TABLE * pmtTable);

int setup_pmt_stream_list(FILE *pFile, unsigned int packetLength);


#endif
