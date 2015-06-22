/*********************************************************************
*
* Filename      :   ts_psi.h
* Description   :   given the fundamental struction of psi protocol of DVB
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <ts_list.h>

#ifndef	_MPEG_TS_H_
#define	_MPEG_TS_H_

#define	TS_PACKET_SIZE					(188)
#define	TS_BASE_PACKET_HEADER_SIZE		(4)

#define	TS_SYNC_BYTE_VAL		(0x47)

#define	PID_TS_PAT				(0x00)
#define	PID_TS_CAT				(0x01)	//Conditional Accept Table
//TSDT -- Transport Stream Description Table
#define	PID_TS_SDT 			    (0x02)
#define	PID_TS_NULL			    (0x1FFF)

#define PACKET_START_CODE_PREFIX        (0x000001)

//adaption_field_control, 11，标识既有adaptation_field又有payload
typedef struct ts_package_header
{
	unsigned int sync_byte :8;
	unsigned int transport_error_indicator  :1;	//high bit
	unsigned int payload_unit_start_indicator :1;	//有效载荷单元起始指示符
	unsigned int transport_priority :1;	//优先传输?
	unsigned int pid :13;
	unsigned int transport_scrambling_control :2;	//传输加扰控制类型
	unsigned int adaptation_field_control :2;	// 1 -- only include additional info but not include payload
	unsigned int continuity_counter :4;	//连续计数器(针对每个pid的计数0~15)
}TS_PACKET_HEADER, *P_TS_PACKET_HEADER;


//自适应区
typedef struct ts_adaptive_info{
	unsigned char break_indicator:1;	//间断指示符
	unsigned char random_access_indicator:1;	//随机指示符
	unsigned char base_bitrate_optimize_indicator:1;	//基本码流优化指示符
	unsigned char PCR_flag:1;	//PCR flag
	unsigned char Joint_flag:1;	//接点标志
	unsigned char TransformSpecifiedData_flag:1;	//传输专用标志
	unsigned char original_PCR_flag:1;	//原始PCR flag
	unsigned char AdaptiveExtension_flag:1;	//自适应区扩展标志
}TS_ADAPTIVE_INFO, *P_TS_ADAPTIVE_INFO;



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
	unsigned program_number : 16;
	unsigned reserved_3 : 3;
	unsigned network_pid : 13;
	unsigned program_map_pid : 13; 	//Pid of PMT

    unsigned CRC_32 : 32; 
}TS_PAT_TABLE,*P_TS_PAT_TABLE;


typedef struct ts_pmt_stream
{
	unsigned stream_type : 8;
	unsigned reserved_5 : 3;
	unsigned elementary_PID : 13;
	unsigned reserved_6 : 4;
	unsigned ES_info_length : 12; 
    
    struct list_head list;
    unsigned int program_number;
}TS_PMT_Stream,*P_TS_PMT_Stream;

typedef struct ts_pat_program
{
    unsigned program_number : 16;
    unsigned reseved_1 : 3;
    unsigned program_map_pid : 13;
    //TS_PMT_Stream pmt_stream_info;
    struct list_head list;
}TS_PAT_Program, *P_TS_PAT_Program;

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

	unsigned CRC_32 : 32; 
}TS_PMT_TABLE,*P_TS_PMT_TABLE; 


TS_PAT_Program __ts_pat_program_list;
TS_PMT_Stream  __ts_pmt_stream_list;



void init_ts_psi_list(void);


int parse_ts_packet_header(TS_PACKET_HEADER *packet_head, unsigned char *buffer);

int find_given_table(FILE *pFile, unsigned char *storeBuffer, 
        unsigned int mPacketLength, unsigned int mUserPid);

int parse_pat_table(unsigned char * pBuffer,TS_PAT_TABLE * psiPAT);

int parse_pmt_table (unsigned char * pBuffer,unsigned int programNumber,
        TS_PMT_TABLE * psiPMT);  

unsigned char * deal_with_given_table(TS_PACKET_HEADER *packet_head, unsigned char *buffer, unsigned int packetLength);

int show_pat_program_info(void);
int show_pmt_stream_info(void);
int show_ts_packet_header(TS_PACKET_HEADER *packet_head);

int search_given_program_info(unsigned int serachProgramId);

unsigned int locate_offset(TS_PACKET_HEADER *packet_head, unsigned char *buffer,
        unsigned int psiFlag, unsigned int * pes_start);

int store_pes_stream(FILE *pFile, FILE *storeFile,
        unsigned int packetLength, unsigned int mElementaryPid);

void show_packet_memory(unsigned char * buffer, unsigned int packetLength);

int setup_pmt_stream_list(FILE *pFile, unsigned int packetLength);

#endif	//_MPEG_TS_H_
