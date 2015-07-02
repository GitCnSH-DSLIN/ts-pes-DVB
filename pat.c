/*********************************************************************
*
* Filename      :   pat.c
* Description   :   fundamental operation of psi protocol of DVB
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tslib.h>
#include <pat.h>
#include <ts_list.h>
#include <print_debug.h>


/*  
 *  Function    : Init the global pat_program_list and pmt_stream list
 */
void init_ts_pat_program_list(void)
{
    INIT_LIST_HEAD(&__ts_pat_program_list.list);
}

TS_PAT_TABLE * parse_pat_table(FILE *pFile, unsigned int packetLength)
{
    TABLE_SECTION_LIST * pat_section = store_psi_si_table(pFile, packetLength, PID_TS_PAT, TABLE_ID_TS_PAT);
    TABLE_SECTION_LIST * pat_section_index = pat_section;
    //the last 8 byte is the end flag
    TS_PAT_TABLE * pat_table_head = (TS_PAT_TABLE *)malloc(sizeof(TS_PAT_TABLE)*(pat_section->last_section_number + 1) + 2*sizeof(int));
    memset(pat_table_head, 0, sizeof(TS_PAT_TABLE)*(pat_section->last_section_number + 1) + 2*sizeof(int));

    TS_PAT_TABLE * pat_table_index = pat_table_head;
    unsigned int i=0;

    while(NULL != pat_section_index)
    {
        parse_pat_table_onesection(pat_section_index->pbuffer, pat_table_index + i);
        i++;
        pat_section_index = pat_section_index->next_section;
    }
    
    //free TABLE_SECTION_LIST
    pat_section_index = pat_section;
    TABLE_SECTION_LIST * next_pat_section = NULL;
    while(NULL != pat_section_index)
    {
        next_pat_section = pat_section_index->next_section;

        //free pbuffer
        free(pat_section_index->pbuffer);
        pat_section_index->pbuffer = NULL;

        //free table_section
        free(pat_section_index);
        pat_section_index = NULL;
    
        pat_section_index = next_pat_section;
    }
    
    //return the pat list head
    return pat_table_head;
}




/*  
 *  Function    : Parse the pat table and add the pat_program info into
 *                global list------ts_pat_program_list
 */
int parse_pat_table_onesection(unsigned char * pBuffer,TS_PAT_TABLE * psiPAT)
{

    unsigned short program_num;
    int ret=0, n = 0;
    P_TS_PAT_Program tmp = NULL;
    
    TS_PACKET_HEADER tsPacketHeader;
    P_TS_PACKET_HEADER ptsPacketHeader = &tsPacketHeader;

    parse_ts_packet_header(ptsPacketHeader, pBuffer);

    unsigned int offset = locate_offset(ptsPacketHeader, pBuffer, PSI_SI_PACKET_FLAG, 0);
    unsigned char *buffer = pBuffer;

    psiPAT->table_id                    = PAT_TABLE_ID(buffer,offset);  
    psiPAT->section_syntax_indicator    = PAT_SECTION_SYNTAX_INDICATOR(buffer, offset);  
    //psiPAT->zero                        = buffer[1] >> 6 & 0x1;  
    //psiPAT->reserved_1                  = buffer[1] >> 4 & 0x3;  
    psiPAT->section_length              = PAT_SECTION_LENGTH(buffer,offset);   
    psiPAT->transport_stream_id         = PAT_TRANSPORT_STREAM_ID(buffer,offset);
    //psiPAT->reserved_2                  = buffer[5] >> 6;  
    psiPAT->version_number              = PAT_VERSION_NUM(buffer,offset);  
    psiPAT->current_next_indicator      = PAT_CUR_NEXT_INDICATOR(buffer,offset);  
    psiPAT->section_number              = PAT_SECTION_NUM(buffer,offset);  
    psiPAT->last_section_number         = PAT_LAST_SECTION_NUM(buffer,offset);   
  
    // the 3 meaning that the section_length offset of buffer head is 3 byte.
    // and the len length is the total length of buffer. 
    psiPAT->CRC_32                      = PAT_CAC32(buffer,offset); 
    
    INIT_LIST_HEAD(&(psiPAT->this_section_program_head.section_list));
    //psiPAT->pat_next_section = NULL;

    //Parse the PAT_Program_
    // 12 = 8 pat relate table + 4 CRC
    
    //uprintf("the psiPAT->section_length value is %d\n",psiPAT->section_length);
    buffer += offset;

    for ( n = 0; n < psiPAT->section_length - 12; n += 4 )  
    {  
        program_num   =   buffer[8 + n ] << 8 | buffer[9 + n ];    
        psiPAT->reserved_3      =   buffer[10 + n ] >> 5;   
        
        psiPAT->network_pid     =   0x00;  
        if ( 0x00 == program_num)  
        {    
            psiPAT->network_pid = (buffer[10 + n ] & 0x1F) << 8 | buffer[11 + n ];  
            //TS_network_Pid = psiPAT->network_PID; //记录该TS流的网络PID  
        }  
        else  
        {  
            tmp = (P_TS_PAT_Program)malloc(sizeof(TS_PAT_Program));
            tmp->program_number = program_num;
            tmp->program_map_pid = (buffer[10 + n] & 0x1F) << 8| buffer[11 + n];
            list_add(&(tmp->list), &(__ts_pat_program_list.list));
            list_add(&(tmp->section_list), &(psiPAT->this_section_program_head.section_list));
        }
    }

    #ifdef DEBUG
        //show_pat_program_info();
    #endif

    return ret;
}

int show_pat_program_info_onesection(TS_PAT_TABLE * patTable)
{
    struct list_head *pos;
    P_TS_PAT_Program tmp = NULL;

    list_for_each(pos, &(patTable->this_section_program_head.section_list))
    {
        tmp = list_entry(pos,TS_PAT_Program, section_list);
        uprintf("-------------------------------------------\n");
        uprintf("the program_num is  0x%X(%d)\n",tmp->program_number, tmp->program_number);
        uprintf("the program_map_pid 0x%X(%d)\n",tmp->program_map_pid, tmp->program_map_pid);
        uprintf("-------------------------------------------\n");
    }

    
    return 0;
}

int show_pat_table_info(TS_PAT_TABLE * patTable)
{
    TS_PAT_TABLE *tmp = patTable;
    unsigned int *ptmp = (unsigned int *)tmp;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
    
        uprintf("-------------------------------------------\n");
        uprintf("table_id                   :   0x%X(%d)\n",tmp->table_id, tmp->table_id);
        uprintf("section_syntax_indicator   :   0x%X(%d)\n",tmp->section_syntax_indicator, tmp->section_syntax_indicator);
        uprintf("section_length             :   0x%X(%d)\n",tmp->section_length,tmp->section_length);
        uprintf("transport_stream_id        :   0x%X(%d)\n",tmp->transport_stream_id,tmp->transport_stream_id);
        uprintf("version_number             :   0x%X(%d)\n",tmp->version_number,tmp->version_number);
        uprintf("current_next_indicator     :   0x%X(%d)\n",tmp->current_next_indicator,tmp->current_next_indicator);
        uprintf("section_number             :   0x%X(%d)\n",tmp->section_number,tmp->section_number);
        uprintf("last_section_number        :   0x%X(%d)\n",tmp->last_section_number,tmp->last_section_number);
        uprintf("CRC_32                     :   0x%X(%d)\n",tmp->CRC_32,tmp->CRC_32);
        show_pat_program_info_onesection(tmp);
        uprintf("-------------------------------------------\n");
        tmp++;
        //to jedge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    }
}



void __remove_ts_pat_program_list_node(unsigned int program_number)
{

    struct list_head *pos, *n;
    P_TS_PAT_Program tmp = NULL;
    
    list_for_each_safe(pos, n, &__ts_pat_program_list.list)
    {
        //del node from __ts_pat_program_list
        tmp = list_entry(pos,TS_PAT_Program, list);
        if(tmp->program_number == program_number)
        {
            list_del_init(pos);
            break;
        }

    }
}

void free_pat_program_info_onesection(TS_PAT_TABLE * pat_table)
{
    struct list_head *pos, *n;
    P_TS_PAT_Program tmp = NULL;

    list_for_each_safe(pos, n, &(pat_table->this_section_program_head.section_list))
    {
        //del node from this section_program_head list
        tmp = list_entry(pos,TS_PAT_Program, section_list);

        list_del_init(pos);
        
        //del node from __ts_pat_program_list
        __remove_ts_pat_program_list_node(tmp->program_number);
        free(tmp);
        tmp = NULL;
    }
    
}


void free_pat_table(TS_PAT_TABLE * pat_table_header)
{
    TS_PAT_TABLE *tmp = pat_table_header;
    unsigned int *ptmp = (unsigned int *)tmp;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
        free_pat_program_info_onesection(tmp);
        tmp++;
        //to jedge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    } 

    free(pat_table_header);
    pat_table_header = NULL;
}




int show_pat_program_info(void)
{
    struct list_head *pos;
    P_TS_PAT_Program tmp = NULL;

    if(list_empty(&__ts_pat_program_list.list))
    {
        uprintf("Empty list \n");
        return 0;
    }

    list_for_each(pos, &__ts_pat_program_list.list)
    {
        tmp = list_entry(pos,TS_PAT_Program, list);
        uprintf("-------------------------------------------\n");
        uprintf("the program_num is  0x%X(%d)\n",tmp->program_number, tmp->program_number);
        uprintf("the program_map_pid 0x%X(%d)\n",tmp->program_map_pid, tmp->program_map_pid);
        uprintf("-------------------------------------------\n");
    }
    
    return 0;
}

int search_given_program_info(unsigned int searchProgramId)
{
    int programPid = -1;
    
    struct list_head *pos;
    
    P_TS_PAT_Program tmp = NULL;

    list_for_each(pos, &__ts_pat_program_list.list)
    {
        tmp = list_entry(pos,TS_PAT_Program, list);
        if (tmp->program_number == searchProgramId)
        {
            programPid = tmp->program_map_pid;
            break;
        }
    }

    if(-1 == programPid)
        uprintf("Can't find the program info pid\n");
    else
        uprintf("We find the given program info pid\n");

    return programPid;    
}

