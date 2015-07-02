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
#define PSI_SI_PACKET_FLAG  (0)
#define PES_PACKET_FLAG  (1)
#define SECTION_INFO_HEADER_SIZE (3)



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

#define TS_PACKET_HEADER_SYNC_BYTE(b)                       (b[0])
#define TS_PACKET_HEADER_TRANS_ERR_INDICATOR(b)             (b[1] >> 7)
#define TS_PACKET_HEADER_PAYLOAD_UNIT_START_INDICATOR(b)    (b[1] >> 6 & 0x01)
#define TS_PACKET_HEADER_TRANS_PRIO(b)                      (b[1] >> 5 & 0x01)
#define TS_PACKET_HEADER_PID(b)                             (((b[1] & 0x1F) << 8) | b[2])
#define TS_PACKET_HEADER_TRANS_SCRAM_CONTROL(b)             (b[3] >> 6)
#define TS_PACKET_HEADER_ADAPT_FIELD_CONTROL(b)             (b[3] >> 4) & 0x03)
#define TS_PACKET_HEADER_CONTINUITY_COUNT(b)                (b[3] & 0x0f)



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




typedef struct table_section_list
{
    unsigned int pid;
    unsigned char table_id;
    unsigned char section_number;
    unsigned char last_section_number;
    unsigned int buffer_size;// the buffe_size = section_length + 3 + offset_length
    unsigned char * pbuffer;
    struct table_section_list * next_section;
}TABLE_SECTION_LIST, *P_TABLE_SECTION_LIST;

#define TS_PSI_SI_TABLE_ID(b,offset)                    (b[offset])
#define TS_PSI_SI_TABLE_SECTION_LENGTH(b,offset)        ((b[offset + 1] & 0x0F) << 8 | (b[offset + 2]))
#define TS_PSI_SI_TABLE_SECTION_NUM(b,offset)           (b[offset + 6])
#define TS_PSI_SI_TABLE_LAST_SECTION_NUM(b,offset)      (b[offset + 7])

int parse_ts_packet_header(TS_PACKET_HEADER *packet_head, unsigned char *buffer);

int show_ts_packet_header(TS_PACKET_HEADER *packet_head);

unsigned int locate_offset(TS_PACKET_HEADER *packet_head, unsigned char *buffer,
        unsigned int psi_flag, unsigned int * pes_start);

int store_one_section_data(FILE *pFile, unsigned int mPacketLength, 
        TABLE_SECTION_LIST * p_table_section);

TABLE_SECTION_LIST * alloc_and_init_table_section(unsigned char * tmp_ts_buffer, 
        unsigned int section_num, unsigned int offset_length);

TABLE_SECTION_LIST * table_section_list_add(TABLE_SECTION_LIST *head, TABLE_SECTION_LIST *node);
 
TABLE_SECTION_LIST * store_psi_si_table(FILE *pFile, unsigned int mPacketLength, 
        unsigned int mUserPid,unsigned int tableId);

void show_packet_memory(unsigned char * buffer, unsigned int packetLength);

#endif	//_MPEG_TS_H_

