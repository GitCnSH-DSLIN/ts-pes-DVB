/*********************************************************************
*
* Filename      :   ca_pid_emm.h
* Description   :   only used for ca_pid_emm
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#ifndef  _MPEG_TS_SI_CA_PID_EMM_H_
#define  _MPEG_TS_SI_CA_PID_EMM_H_


#include <tslib.h>


#define DATA_END_VAL (0xFFFFFFFF)
#define TS_PACKET_LENGTH    (188)

#define SECTION_INFO_SIZE   (11)
#define SECTION_INFO_AFTER_SIZE (8)
#define SECTION_DATA_LESS       (8) /*section_length = data_length + 8 byte.*/
#define SECTION_INFO_BEFORE_SIZE (3)

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
    unsigned char *pdata;//the pdata size should section_length - 8
    unsigned int cur_pdata_length;
    struct ca_pid_emm * next_section;
} CA_PID_EMM, *P_CA_PID_EMM;


#define CA_PID_EMM_TABLE_ID(b)          (b[0])

#define	CA_PID_EMM_SECTION_LENGTH(b)    (((b[1] & 0x0f) << 8) | b[2])

#define CA_PID_EMM_DATA_LENGTH(b)       ((((b[1] & 0x0f) << 8) | b[2]) - 8)

#define CA_PID_EMM_IS_END(b)                ({\
                                ((b[0] <<24 ) | (b[1]<<16) | (b[2]<<8) | (b[3])) == DATA_END_VAL;\
                                })


CA_PID_EMM * alloc_init_capid_emm(unsigned char * buffer);

CA_PID_EMM * ca_pid_list_lastnode(CA_PID_EMM * Header);

CA_PID_EMM *ca_pid_add_list(CA_PID_EMM *Header, CA_PID_EMM *newNode);

void show_section_info(CA_PID_EMM * pcaPidEmm);

void show_ca_pid_emm_list_info(CA_PID_EMM *pcaPidEmmHeader);

void free_ca_pid_emm_list(CA_PID_EMM *pcaPidEmmHeader);

int store_one_packet_data(CA_PID_EMM * caPidEmmHeader, unsigned char *tmpbuffer,
        P_TS_PACKET_HEADER ptsPacketHeader);

int load_ca_pid_emm(FILE *pFile, unsigned int CA_PID,
        CA_PID_EMM * caPidEmmHeader, unsigned int packetLength);

#endif
