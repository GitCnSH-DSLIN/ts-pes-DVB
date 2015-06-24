/*********************************************************************
*
* Filename      :   ca_pid_emm.h
* Description   :   only used for ca_pid_emm
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#define DATA_END_VAL (0xFFFFFFFF)
#define TS_PACKET_LENGTH    (188)


typedef struct ca_pid_emm
{
	unsigned table_id : 8;//used
	unsigned section_syntax_indicator : 1;
	unsigned zero : 1;
	unsigned reserved_1 : 2;
	unsigned section_length : 12;//used
	unsigned transport_stream_id : 16;
	unsigned reserved_2 : 2;
	unsigned version_number : 5;
	unsigned current_next_indicator : 1;
	unsigned section_number : 8;
	unsigned last_section_number : 8; 
	unsigned program_number : 16;
	unsigned reserved_3 : 3;
	unsigned program_map_pid : 13;
    unsigned char *pdata;
    struct ca_pid_emm * prev_section;
    struct ca_pid_emm * next_section;
} CA_PID_EMM, *P_CA_PID_EMM;


#define CA_PID_EMM_TABLE_ID(b)          (b[0])

#define	CA_PID_EMM_SECTION_LENGTH(b)    (((b[1] & 0x0f) << 8) | b[2])

#define CA_PID_IS_END(b)                ({\
                            int length = CA_PID_EMM_SECTION_LENGTH(b);  \
                            int base = 3 + length;\
                            (b[base + 1] <<24 ) | (b[base + 2]<<16) | (b[base + 3]<<8) | (b[base + 4]) == DATA_END_VAL;\
                        })


