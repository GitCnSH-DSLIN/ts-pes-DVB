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
//#include <ts_psi.h>
#include <tslib.h>
#include <pat.h>
#include <ts_list.h>
#include <print_debug.h>


TS_PAT_Program __ts_pat_program_list;

/*  
 *  Function    : Init the global pat_program_list and pmt_stream list
 */
void init_ts_pat_program_list(void)
{
    INIT_LIST_HEAD(&__ts_pat_program_list.list);
}



/*  
 *  Function    : Parse the pat table and add the pat_program info into
 *                global list------ts_pat_program_list
 */
int parse_pat_table(unsigned char * pBuffer,TS_PAT_TABLE * psiPAT)
{

    int pat_len = 0;
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
        }
    }

    #ifdef DEBUG
        show_pat_program_info();
    #endif

    return ret;
}

int show_pat_program_info(void)
{
    struct list_head *pos;
    P_TS_PAT_Program tmp = (P_TS_PAT_Program)malloc(sizeof(TS_PAT_Program));
    P_TS_PAT_Program pFreetmp = tmp;

    list_for_each(pos, &__ts_pat_program_list.list)
    {
        tmp = list_entry(pos,TS_PAT_Program, list);
        uprintf("-------------------------------------------\n");
        uprintf("the program_num is  0x%X(%d)\n",tmp->program_number, tmp->program_number);
        uprintf("the program_map_pid 0x%X(%d)\n",tmp->program_map_pid, tmp->program_map_pid);
        uprintf("-------------------------------------------\n");
    }

    free(pFreetmp);
    
    return 0;
}


