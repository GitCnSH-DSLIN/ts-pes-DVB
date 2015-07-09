/*********************************************************************
*
* Filename      :   user_channel.h
* Description   :   fundamental struction of PAT table
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#ifndef  _MPEG_TS_SI_USER_CHANNEL_H_
#define  _MPEG_TS_SI_USER_CHANNEL_H_

#include <pat.h>
#include <pmt.h>
#include <sdt.h>
#include <nit.h>

#define MAX_SERVICE_NAME_SIZE   (32)
#define MAX_PROGRAM_STREAM_SIZE     (16)


typedef struct program_stream_info{

    unsigned char stream_type;
    unsigned short elementary_PID;

}PROGRAM_STREAM_INFO,P_PROGRAM_STREAM_INFO;


typedef struct user_channel_info{

    unsigned short program_number;
    unsigned short program_map_PID;
    unsigned short transport_stream_id ; //SDT->transport_stream_id   === NIT->TRANSPORT_STREAM->transport_stream_id
    unsigned int freq;
                        
    unsigned char service_name_length;
    unsigned char service_name[MAX_SERVICE_NAME_SIZE];
    
    unsigned char program_stream_count;
    PROGRAM_STREAM_INFO program_stream_list[MAX_PROGRAM_STREAM_SIZE];
    
    //struct user_channel_info * next_channel;
    struct list_head list;// userd for freq global list.

}USER_CHANNEL_INFO, *P_USER_CHANNEL_INFO;

void init_ts_user_channel_list(void);

USER_CHANNEL_INFO * alocate_and_init_user_channel_node(TS_PMT_TABLE *  pmt_table_one_program,
        unsigned short program_map_pid, unsigned int freq, unsigned short transport_stream_id);

SDT_SERVICE * search_given_sdt_program_num(unsigned short program_number, TS_SDT_TABLE *sdt_table_head);

int init_user_channel_name_info(SDT_SERVICE *sdt_service, USER_CHANNEL_INFO *user_channel_node);

unsigned int locate_user_channel_freq(unsigned short transport_stream_id, 
        TS_NIT_TABLE * nit_table_head);

int setup_user_channel_list(FILE *pFile, unsigned int packetLength);

int show_user_channel_info_list(void);

#endif

