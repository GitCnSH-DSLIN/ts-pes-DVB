/*********************************************************************
*
* Filename      :   pat.h
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#define PID_TS_PAT          (0x00)
#define TABLE_ID_TS_PAT     (0x00)

typedef struct ts_pat_program
{
    unsigned program_number : 16;
    unsigned reseved_1 : 3;
    unsigned program_map_pid : 13;
    //TS_PMT_Stream pmt_stream_info;
    struct list_head section_list;//list for one section
    struct list_head list;//global list __ts_pat_program_list
}TS_PAT_Program, *P_TS_PAT_Program;


TS_PAT_Program __ts_pat_program_list;

//PAT --  Program Association Table
typedef struct ts_pat_table
{
	unsigned table_id : 8;
	unsigned section_syntax_indicator : 1;
	unsigned zero : 1;
	unsigned reserved_1 : 2;
	unsigned section_length : 12;
	unsigned transport_stream_id : 16;
	unsigned reserved_2 : 2;
	unsigned version_number : 5;
	unsigned current_next_indicator : 1;	//if 1, PAT is available currently
	unsigned section_number : 8;
	unsigned last_section_number : 8; 
//	unsigned program_number : 16;
	unsigned reserved_3 : 3;
	unsigned network_pid : 13;
//	unsigned program_map_pid : 13; 	//Pid of PMT
   
    struct ts_pat_program this_section_program_head;

    struct ts_pat_table * pat_next_section;
    unsigned CRC_32 : 32; 
}TS_PAT_TABLE,*P_TS_PAT_TABLE;

#define PAT_TABLE_ID(b, offset)                     (b[offset])
#define PAT_SECTION_SYNTAX_INDICATOR(b, offset)     (b[offset + 1] >> 7)
#define PAT_SECTION_LENGTH(b,offset)                (((b[offset + 1] & 0x0F) << 8) | b[offset + 2])
#define PAT_TRANSPORT_STREAM_ID(b, offset)          ((b[offset + 3] << 8) | buffer[offset + 4]) 
#define PAT_VERSION_NUM(b, offset)                  ((b[offset + 5] >> 1) & 0x1F)
#define PAT_CUR_NEXT_INDICATOR(b, offset)           ((b[offset + 5] << 7) >>7) 
#define PAT_SECTION_NUM(b, offset)                  (b[offset + 6])
#define PAT_LAST_SECTION_NUM(b, offset)             (b[offset + 7])
#define PAT_CAC32(b,offset)                         ({ int len = PAT_SECTION_LENGTH(b,offset) + 3; \
                                                        ((b[offset + len - 4] & 0xFF) << 24) | \
                                                        ((b[offset + len - 3] & 0xFF) << 16) | \
                                                        ((b[offset + len - 2] & 0xFF) << 8) | \
                                                        ((b[offset + len - 1] & 0xFF)); \
                                                    })




TS_PAT_TABLE * parse_pat_table(FILE *pFile, unsigned int packetLength);
int parse_pat_table_onesection(unsigned char * pBuffer,TS_PAT_TABLE * psiPAT);
int show_pat_program_info_onesection(TS_PAT_TABLE * patTable);
int show_pat_table_info(TS_PAT_TABLE * patTable);
void free_pat_table(TS_PAT_TABLE * pat_table_header);

void init_ts_pat_program_list(void);
int show_pat_program_info(void);
