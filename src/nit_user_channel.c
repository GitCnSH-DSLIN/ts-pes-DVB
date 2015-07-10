/*********************************************************************
*
* Filename      :   nit_user_channel.c
* Description   :   from nit table to get all of user channel info from different frequency
* Edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
* Note          :   when we search from NIT Table to get all of the freqence
*                   of user_channel, Firstly we must set a default frequence
*                   to get a NIT table.
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
#include <nit_user_channel.h>
#include <descriptors.h>
#include <print_debug.h>



/*  
 *  Function    : Init the global __ts_nit_freq_list
 */
void init_ts_nit_freq_list(void)
{
    INIT_LIST_HEAD(&__ts_nit_freq_list.list);
}


char *__itoa(int num, char *str, int radix)   
{
    char string[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* ptr = str;
    int i;
    int j;
    while (num)
    {
        *ptr++ = string[num % radix];
        num /= radix;

        if (num < radix)
        {
            *ptr++ = string[num];
            *ptr = '\0';
            break;
        }
    }
    
    j = ptr - str - 1;
    for (i = 0; i < (ptr - str) / 2; i++)
    {
        int temp = str[i];
        str[i] = str[j];
        str[j--] = temp;
    }
    return str;
}

/*Sub core function*/
void find_user_channel_from_given_freq(FILE *pFile, unsigned int packetLength, unsigned int given_freq)
{
    init_ts_pat_program_list();
    TS_PAT_TABLE *pat_table_head = parse_pat_table(pFile, packetLength);

    //2. SDT table parse.
    TS_SDT_TABLE * sdt_table_head = parse_sdt_table(pFile, packetLength,
            TABLE_ID_SDT_ACTUAL);
    TS_SDT_TABLE *sdt_service_node = NULL;
    unsigned short transport_stream_id = sdt_table_head->transport_stream_id;
    unsigned int frequent = given_freq;

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

    //free_pat_table releated memory
    free_pat_table(pat_table_head);
    pat_table_head = NULL;

    //free_sdt_table releated memory
    free_sdt_table(sdt_table_head);
    sdt_table_head = NULL;

    fclose(pFile);
}






void __search_freq_nit_transtream_descriptors(TRANSPORT_STREAM * nitTransStream)
{
    DESCRIPTOR_COMMON *head = nitTransStream->first_desc;

    DESCRIPTOR_COMMON *ptmp = head;

    NIT_FREQ_INFO *tmp_nit_freq_node = NULL;

    unsigned char cable_delivery_descriptor_tag = 0x44;
    CABLE_DELIVERY_SYSTEM_DESC *tmp = NULL;

    while (NULL != ptmp)
    {
        
        if(ptmp->descriptor_tag == cable_delivery_descriptor_tag)
        {
            tmp = (CABLE_DELIVERY_SYSTEM_DESC *)ptmp;         
            tmp_nit_freq_node  = (NIT_FREQ_INFO *)malloc(sizeof(NIT_FREQ_INFO));
            tmp_nit_freq_node->frequency = tmp->frequency;
            
            list_add(&(tmp_nit_freq_node->list), &(__ts_nit_freq_list.list));
        }
        ptmp = (DESCRIPTOR_COMMON *)ptmp->next_desc;
    }
}


void __search_freq_nit_transtream(TRANSPORT_STREAM *Header_onesection)
{
    
	TRANSPORT_STREAM *ptmp = Header_onesection;
    while (NULL != ptmp)
    {
        __search_freq_nit_transtream_descriptors(ptmp);
        ptmp = ptmp->next_transport_stream;
    }

}


void search_freq_list_from_nit(TS_NIT_TABLE * nit_table_head)
{

    TS_NIT_TABLE *tmp = nit_table_head;
    unsigned int *ptmp = (unsigned int *)tmp;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
        if(NULL != tmp->first_transport_stream)
            __search_freq_nit_transtream(tmp->first_transport_stream);
        tmp++;
        //to judge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    }
}


FILE * __adjust_specify_freq(unsigned int frequency)
{
    unsigned int freq = frequency/(16*16*16*16);
    char freq_name[10]={0};
    char *p_freq_name = freq_name;
    p_freq_name = __itoa(freq, freq_name,16);
//    uprintf("p_freq_name : %s\n",p_freq_name);
    p_freq_name = strcat(p_freq_name,".ts");
//    uprintf("p_freq_name.ts : %s\n",p_freq_name);

    char file_name[100] = {0};
    char *p_file_name = file_name;
    p_file_name = strcpy(p_file_name, "/home/jensen/workspace/TsFile/ocn_");
    p_file_name = strcat(p_file_name, p_freq_name);
//    uprintf("p_file_name : %s\n",p_file_name);

    FILE * pCurFile;
    if((pCurFile = fopen(p_file_name,"rb")) == NULL)
    {
        uprintf("Open : %s file failed\n",p_file_name);
        return NULL;
    }
    
    return pCurFile;
}


int write_file_user_channel_info()
{
    FILE * pSaveFile;

    if((pSaveFile = fopen("/home/jensen/workspace/TsFile/user_channel_info","wb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }
    
    struct list_head *pos;
    USER_CHANNEL_INFO * tmp = NULL;

    if(list_empty(&(__ts_user_channel_list.list)))
    {
        uprintf("Empty list \n");
        return;
    }

    unsigned short channel_id = 0;
    list_for_each(pos, &(__ts_user_channel_list.list))
    {
        tmp = list_entry(pos, USER_CHANNEL_INFO, list);
        if(1 != fwrite(tmp,sizeof(USER_CHANNEL_INFO),1,pSaveFile))
        {
            uprintf("fwrite failed \n");
            return -1;
        }
    }
    
    //after write the user_channel_list into file, free user_channel_list.
    free_user_channel_list_info();

    fclose(pSaveFile);
    
    return 0;
}

int read_user_channel_from_storefile()
{
    
    FILE * pSaveFile;

    if((pSaveFile = fopen("/home/jensen/workspace/TsFile/user_channel_info","rb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }
    unsigned char * buffer = (unsigned char *)malloc(sizeof(USER_CHANNEL_INFO));
    memset(buffer, 0, sizeof(USER_CHANNEL_INFO));

    unsigned step_length = sizeof(USER_CHANNEL_INFO);
    unsigned char * b = NULL;
    USER_CHANNEL_INFO * tmp = NULL;

    init_ts_user_channel_list();

    //write low first, then high.
    while(fread(buffer,step_length , 1, pSaveFile) == 1)
    {   
        tmp = (USER_CHANNEL_INFO *)malloc(sizeof(USER_CHANNEL_INFO));
        memset(tmp, 0, sizeof(USER_CHANNEL_INFO));
        memcpy((unsigned char *)tmp,buffer, sizeof(USER_CHANNEL_INFO));
        list_add(&(tmp->list), &(__ts_user_channel_list.list)); 
    }

    free(buffer);
    fclose(pSaveFile);
    return 0;
}



/*Core function*/
int setup_user_channel_list_from_nit(FILE *pFile, unsigned int packetLength, unsigned int default_freq)
{
    TS_NIT_TABLE * nit_table_head = setup_user_channel_list(pFile, packetLength, 1);
    //after call setup_user_channel_list, we have already put the default_freq into global
    //__ts_user_channel_list
    
    init_ts_nit_freq_list();
    search_freq_list_from_nit(nit_table_head);
//    show_nit_freq_list_info();
    // after setup the freq list. we loop the freq list to search all of the user_channel from
    // different freq.

    NIT_FREQ_INFO * tmp = NULL;
    struct list_head * pos;
    list_for_each(pos, &__ts_nit_freq_list.list)
    {
        tmp = list_entry(pos, NIT_FREQ_INFO, list);
       
        //when run setup_user_channel_list, we already put the default_freq user_chanenel info into
        //list
        if(default_freq == tmp->frequency)
            continue;

        //adjust to given freq to dump or just open file
        FILE *curFile = __adjust_specify_freq(tmp->frequency); 

        if(NULL == curFile)
            continue;
        else
        {
            find_user_channel_from_given_freq(curFile, packetLength,tmp->frequency);
        }
    }
    
    
    //free nit_table memory
    free_nit_table(nit_table_head);
    nit_table_head = NULL;

    //free nit_freq_list
    free_nit_freq_list_info();
}


void free_nit_freq_list_info()
{
    struct list_head *pos, *n;
    NIT_FREQ_INFO * tmp = NULL;

    list_for_each_safe(pos, n, &(__ts_nit_freq_list.list))
    {
        tmp = list_entry(pos, NIT_FREQ_INFO, list);
        list_del_init(pos);
        free(tmp);
        tmp = NULL;
    }
}


int show_nit_freq_list_info(void)
{
    struct list_head *pos;
    NIT_FREQ_INFO * tmp = NULL;

    if(list_empty(&__ts_nit_freq_list.list))
    {
        uprintf("Empty list \n");
        return 0;
    }

    list_for_each(pos, &__ts_nit_freq_list.list)
    {
        tmp = list_entry(pos,NIT_FREQ_INFO, list);
        uprintf("-------------------------------------------\n");
        uprintf("frequency  :    0x%X\n",tmp->frequency);
        uprintf("-------------------------------------------\n");
    }
    
    return 0;
}








