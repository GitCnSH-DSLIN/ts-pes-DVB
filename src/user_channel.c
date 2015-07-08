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
#include <pmt.h>
#include <pat.h>
#include <ts_list.h>
#include <print_debug.h>

/*  
 *  Function    : Init the global __ts_pmt_stream_list
 */
void init_ts_user_channel_list(void)
{
    INIT_LIST_HEAD(&__ts_user_channel_list.list);
}




USER_CHANNEL_INFO * alocate_and_init_user_channel_node(TS_PMT_TABLE *  pmt_table_one_program,
        unsigned short program_map_pid, unsigned short freq, unsigned short transport_stream_id)
{
    USER_CHANNEL_INFO * user_channel_node = (USER_CHANNEL_INFO *)malloc(sizeof(USER_CHANNEL_INFO));
    memset(user_channel_node, 0, sizeof(USER_CHANNEL_INFO));

    user_channel_node->program_number       = pmt_table_one_program->program_number;
    user_channel_node->program_map_PID      = program_map_pid;
    user_channel_node->freq                 = freq;
    user_channel_node->transport_stream_id  = transport_stream_id;

    P_TS_PMT_Stream tmp = NULL; 
    unsigned pmt_stream_count = 0;
    
    //get the stream_type and elementary_pid infomation of this program_number.
    list_for_each(pos, &__ts_pmt_stream_list.list)
    {
        tmp = list_entry(pos,TS_PMT_Stream, list);
        if(tmp->program_number == user_channel_node->program_number)
        {
            user_channel_node->pmt_stream_list[pmt_stream_count].stream_type    = tmp->stream_type;
            user_channel_node->pmt_stream_list[pmt_stream_count].elementary_PID = tmp->elementary_PID;
            pmt_stream_count++;
        }
    }

    user_channel_node->pmt_stream_count  = pmt_stream_count + 1;

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
    TS_SDT_TABLE *tmp = pSdtTable;
    unsigned int *ptmp = (unsigned int *)tmp;
    SDT_SERVICE *result_service = NULL;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
        result_service = __search_sdt_service_onesection(tmp->first_sdt_service, program_number);
        
        if(NULL != result_service)
        {
            uprintf("Find need sdt_service\n");
            
            return result_service;
        }
        tmp++;
        //to judge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    }

    return NULL;


}


int init_user_channel_name_info(SDT_SERVICE *sdt_service, USER_CHANNEL_INFO *user_channel_node)
{
    DESCRIPTOR_COMMON *head = sdtService->first_desc;
    DESCRIPTOR_COMMON *ptmp = head;
    SERVICE_DESC * tmp = NULL;

    unsigned char service_descriptor_tag = 0x48;_

    while(NULL != ptmp)
    {
        if(ptmp->descriptor_tag == service_descriptor_tag)
        {
            tmp = (SERVICE_DESC*)ptmp;         
            user_channel_node->service_name_length = tmp->service_name_length;
	        strncpy(user_channel_node->service_name, tmp->service_name, tmp->service_name_length);
            break;
        }
    
    
    }



}




unsigned short locate_user_channel_freq(unsigned short transport_stream_id, 
        TS_NIT_TABLE * nit_table_head)
{
    
    TRANSPORT_STREAM * nit_transport_stream_node = NULL;
   
    TS_NIT_TABLE *tmp = nit_table_head;
    unsigned int *ptmp = (unsigned int *)tmp;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
        if(NULL != tmp->first_transport_stream)
        {
            nit_trans_stream_node = __locate_user_channel_freq_onesection(transport_stream_id,tmp->first_transport_stream);
        
            if(NULL != nit_transport_stream_node)
                break;
        }
        tmp++;
        //to judge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    }

    result_freq = __search_user_channel_freq_message(nit_transport_stream_node);    
    
    return result_freq;
}


int setup_user_channel_list(FILE *pFile, unsigned int packetLength)
{
    
    //1.PAT table parse
    init_ts_pat_program_list();
    TS_PAT_TABLE *pat_table_head = parse_pat_table(pFile, packetLength);
    show_pat_program_info();
        
    //2. SDT table parse.
    TS_SDT_TABLE * sdt_table_head = parse_sdt_table(pFile, packetLength,
            TABLE_ID_SDT_ACTUAL);
    SDT_SERVICE * sdt_service_node = NULL;
    
    //3. NIT table parse
    TS_NIT_TABLE * nit_table_head = parse_nit_table(pFile, packetLength,
            TABLE_ID_NIT_ACTUAL);

    unsigned short transport_stream_id = sdt_table_head->transport_stream_id;
    unsigned short frequent = locate_user_channel_freq(transport_stream_id, nit_table_head);

    //4.PMT table parse. And setup the user_channel_info
    struct list_head *pos;
    P_TS_PAT_Program tmp_pat_program = NULL;
    USER_CHANNEL_INFO * user_channel_node = NULL;

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
        init_user_channel_name_info(sdt_service_node，user_channel_node);


        list_add(&(user_channel_node->list),&(__ts_user_channel_info_list.list)); 

        //free pmt table info
        free_pmt_table_one_program(pmt_table_one_program_head, 1);
    }

    
    
    fseek(pFile, 0, SEEK_SET);

    //free_pat_table releated memory
    free_pat_table(pat_table_head);
    
    //free_sdt_table releated memory
    free_sdt_table(sdt_table_head);
    
    //free_nit_table releated memory
    free_nit_table(nit_table_head);
    
    return 0;
}







