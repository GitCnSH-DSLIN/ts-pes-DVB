/*********************************************************************
*
* Filename      :   user_channel.h
* Description   :   fundamental struction of PAT table
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#ifndef  _MPEG_TS_SI_USER_CHANNEL_H_
#define  _MPEG_TS_SI_USER_CHANNEL_H_


#define MAX_SERVICE_NAME_SIZE   (32)
#define MAX_PMT_STREAM_SIZE     (16)


typedef program_stream_info{

    unsigned char stream_type;
    unsigned short elementary_PID;

}PROGRAM_STREAM_INFO,P_PROGRAM_STREAM_INFO;


typedef user_channel_info{

    unsigned short program_number;
    unsigned short program_map_PID;
    unsigned short transport_stream_id ; //SDT->transport_stream_id   === NIT->TRANSPORT_STREAM->transport_stream_id
    unsigned short freq;
                        
    unsigned char service_name_length;
    unsigned char service_name[MAX_SERVICE_NAME_SIZE];
    
    unsigned char pmt_stream_count;
    TS_PMT_STREAM pmt_stream_list[MAX_PMT_STREAM_SIZE];
    
    //struct user_channel_info * next_channel;
    struct list_head list;// userd for freq global list.

}USER_CHANNEL_INFO, *P_USER_CHANNEL_INFO;

#endif

