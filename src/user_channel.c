/*********************************************************************
*
* Filename      :   user_channel.c
* Description   :   fundamental operation of  user channel info
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tslib.h>
#include <ts_list.h>
#include <pmt.h>
#include <pat.h>
#include <sdt.h>
#include <nit.h>
#include <user_channel.h>
#include <descriptors.h>
#include <print_debug.h>



/*  
 *  Function    : Init the global __ts_pmt_stream_list
 */
void init_ts_user_channel_list(void)
{
    INIT_LIST_HEAD(&__ts_user_channel_list.list);
}


USER_CHANNEL_INFO * alocate_and_init_user_channel_node(TS_PMT_TABLE *  pmt_table_one_program,
        unsigned short program_map_pid, unsigned int freq, unsigned short transport_stream_id)
{
    USER_CHANNEL_INFO * user_channel_node = (USER_CHANNEL_INFO *)malloc(sizeof(USER_CHANNEL_INFO));
    memset(user_channel_node, 0, sizeof(USER_CHANNEL_INFO));

    user_channel_node->program_number       = pmt_table_one_program->program_number;
    user_channel_node->program_map_PID      = program_map_pid;
    user_channel_node->freq                 = freq;
    user_channel_node->transport_stream_id  = transport_stream_id;

    P_TS_PMT_Stream tmp = NULL; 
    unsigned short program_stream_count = 0;
   
    struct list_head *pos;

    //get the stream_type and elementary_pid infomation of this program_number.
    list_for_each(pos, &__ts_pmt_stream_list.list)
    {
        tmp = list_entry(pos,TS_PMT_Stream, list);
        if(tmp->program_number == user_channel_node->program_number)
        {
            user_channel_node->program_stream_list[program_stream_count].stream_type    = tmp->stream_type;
            user_channel_node->program_stream_list[program_stream_count].elementary_PID = tmp->elementary_PID;
            program_stream_count++;
        }
    }

    user_channel_node->program_stream_count  = program_stream_count;

    return user_channel_node;
}


SDT_SERVICE * __search_sdt_service_onesection(SDT_SERVICE * Header_onesection, 
         unsigned short program_number)
{
    SDT_SERVICE *ptmp = Header_onesection;
    SDT_SERVICE *search_sdt_service = NULL;

    while(NULL != ptmp)
    {
        if(ptmp->service_id == program_number)
        {
            search_sdt_service = ptmp;
            break;
        }
        ptmp = ptmp->next_sdt_service;
    }

    return search_sdt_service;
}


SDT_SERVICE * search_given_sdt_program_num(unsigned short program_number, TS_SDT_TABLE *sdt_table_head)
{
    TS_SDT_TABLE *tmp = sdt_table_head;
    unsigned int *ptmp = (unsigned int *)tmp;
    SDT_SERVICE *result_service = NULL;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
        result_service = __search_sdt_service_onesection(tmp->first_sdt_service, program_number);
        
        if(NULL != result_service)
        {
            //uprintf("Find need sdt_service for user_channel\n");
            return result_service;
        }
        tmp++;
        //to judge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    }
    
    if(NULL == result_service)
    {
        printf("Can't find sdt_service for user_channel\n");
    }

    return NULL;
}



int init_user_channel_name_info(SDT_SERVICE *sdt_service, USER_CHANNEL_INFO *user_channel_node)
{
    DESCRIPTOR_COMMON *head = sdt_service->first_desc;
    DESCRIPTOR_COMMON *ptmp = head;
    SERVICE_DESC * tmp = NULL;

    unsigned char service_descriptor_tag = 0x48;
    int ret = -1;

    while(NULL != ptmp)
    {
        if(ptmp->descriptor_tag == service_descriptor_tag)
        {
            tmp = (SERVICE_DESC*)ptmp;         
            user_channel_node->service_name_length = tmp->service_name_length;
	        strncpy(user_channel_node->service_name, tmp->service_name, tmp->service_name_length);
            ret = 0;
            break;
        }
    
        ptmp = (DESCRIPTOR_COMMON *)ptmp->next_desc;
    }

    if(-1 == ret)
    {
        printf("Can't find the service name of user_channel\n");
    }
    
    return ret;
}


TRANSPORT_STREAM * __locate_user_channel_freq_onesection(unsigned short transport_stream_id,
        TRANSPORT_STREAM * Header_onesection)
{

    TRANSPORT_STREAM *ptmp = Header_onesection;
    TRANSPORT_STREAM *search_nit_transtream = NULL;

    while(NULL != ptmp)
    {
        if(ptmp->transport_stream_id == transport_stream_id)
        {
            search_nit_transtream = ptmp;
            break;
        }
        ptmp = ptmp->next_transport_stream;
    }

    return search_nit_transtream;
}


unsigned int __search_user_channel_freq_message(TRANSPORT_STREAM * nit_transtream_node)
{
    unsigned int result_freq = 0;

    DESCRIPTOR_COMMON *head =  nit_transtream_node->first_desc;
    DESCRIPTOR_COMMON *ptmp = head;
    CABLE_DELIVERY_SYSTEM_DESC * tmp = NULL;

    unsigned char cable_delivery_descriptor_tag = 0x44;

    while(NULL != ptmp)
    {
        if(ptmp->descriptor_tag == cable_delivery_descriptor_tag)
        {
            tmp = (CABLE_DELIVERY_SYSTEM_DESC *)ptmp;         
            result_freq = tmp->frequency;
            break;
        }
        
        ptmp = (DESCRIPTOR_COMMON *)ptmp->next_desc;
    }

    if(0 == result_freq)
    {
        printf("Can't find freq message of user_channel.\n");
    }
    return result_freq;
}




unsigned int locate_user_channel_freq(unsigned short transport_stream_id, 
        TS_NIT_TABLE * nit_table_head)
{
    
    TRANSPORT_STREAM * nit_transtream_node = NULL;
   
    TS_NIT_TABLE *tmp = nit_table_head;
    unsigned int *ptmp = (unsigned int *)tmp;
    unsigned int result_freq = 0;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
        if(NULL != tmp->first_transport_stream)
        {
            nit_transtream_node = __locate_user_channel_freq_onesection(transport_stream_id,tmp->first_transport_stream);
        
            if(NULL != nit_transtream_node)
                break;
        }
        tmp++;
        //to judge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    }

    result_freq = __search_user_channel_freq_message(nit_transtream_node);    
    
    return result_freq;
}


TS_NIT_TABLE * setup_user_channel_list(FILE *pFile, unsigned int packetLength, int reserved_nit_flag)
{
    
    init_ts_user_channel_list();
   
    //1.PAT table parse
    init_ts_pat_program_list();
    TS_PAT_TABLE *pat_table_head = parse_pat_table(pFile, packetLength);
        
    //2. SDT table parse.
    TS_SDT_TABLE * sdt_table_head = parse_sdt_table(pFile, packetLength,
            TABLE_ID_SDT_ACTUAL);
    SDT_SERVICE * sdt_service_node = NULL;
    
    //3. NIT table parse
    TS_NIT_TABLE * nit_table_head = parse_nit_table(pFile, packetLength,
            TABLE_ID_NIT_ACTUAL);

    unsigned short transport_stream_id = sdt_table_head->transport_stream_id;
    unsigned int frequent = locate_user_channel_freq(transport_stream_id, nit_table_head);

    //4.PMT table parse. And setup the user_channel_info
    struct list_head *pos;
    P_TS_PAT_Program tmp_pat_program = NULL;
    USER_CHANNEL_INFO * user_channel_node = NULL;
    init_ts_pmt_stream_list();

    list_for_each(pos, &__ts_pat_program_list.list)
    {
        tmp_pat_program = list_entry(pos,TS_PAT_Program, list);
        
        TS_PMT_TABLE *pmt_table_one_program_head = parse_pmt_table_one_program(pFile, packetLength,
                tmp_pat_program->program_number, tmp_pat_program->program_map_pid);
        
        //allocate and init some infor for user_channel_info
        user_channel_node = alocate_and_init_user_channel_node(pmt_table_one_program_head,
                tmp_pat_program->program_map_pid, frequent, transport_stream_id);
        
        //parse the sdt and get the service_name_length, service_name, transport_stream_id
        //the transport_stream_id is used to NIT to get the freq message.
        sdt_service_node = search_given_sdt_program_num(tmp_pat_program->program_number, sdt_table_head);
        init_user_channel_name_info(sdt_service_node, user_channel_node);


        list_add(&(user_channel_node->list), &(__ts_user_channel_list.list)); 

        //free pmt table info
        free_pmt_table_one_program(pmt_table_one_program_head, 0);
    }

    
    
    fseek(pFile, 0, SEEK_SET);

    //free_pat_table releated memory
    free_pat_table(pat_table_head);
    pat_table_head = NULL;

    //free_sdt_table releated memory
    free_sdt_table(sdt_table_head);
    sdt_table_head = NULL;
    

    //free_nit_table releated memory
    //reserved_nit_flag : 1 reserved,  0 free;
    if(0 == reserved_nit_flag)
    {
        free_nit_table(nit_table_head);
        nit_table_head = NULL;
        return NULL;
    }
    else
        return nit_table_head;
}


int show_user_channel_info_list(void)
{
    struct list_head *pos;
    USER_CHANNEL_INFO * tmp = NULL;

    if(list_empty(&(__ts_user_channel_list.list)))
    {
        uprintf("Empty list \n");
        return 0;
    }

    unsigned short channel_id = 0;
    list_for_each(pos, &(__ts_user_channel_list.list))
    {
        tmp = list_entry(pos, USER_CHANNEL_INFO, list);
        uprintf("-------------------------------------------\n");
        uprintf("ChannelId          :   %d\n",channel_id);
        uprintf("Program_number     :   0x%X(%d)\n",tmp->program_number, tmp->program_number);
        uprintf("Program_map_PID    :   0x%X(%d)\n",tmp->program_map_PID, tmp->program_map_PID);
        uprintf("Transport_stream_id:   %d\n",tmp->transport_stream_id);
        uprintf("Service_name       :   %s\n",tmp->service_name);
        uprintf("Frequence          :   %d(0x%X)\n",tmp->freq, tmp->freq);
        uprintf("Program_streamcount:   %d\n",tmp->program_stream_count);
        int i=0;
        for(i=0; i<tmp->program_stream_count;i++)
        {
            uprintf("\tstream_type      :   %d\n",tmp->program_stream_list[i].stream_type);
            uprintf("\telementary_PID   :   %d(0x%X)\n",tmp->program_stream_list[i].elementary_PID,tmp->program_stream_list[i].elementary_PID);
        }
        uprintf("-------------------------------------------\n");
        channel_id++;
    }
    
    return 0;
}




