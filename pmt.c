/*********************************************************************
*
* Filename      :   pmt.c
* Description   :   fundamental operation of psi protocol of DVB
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
void init_ts_pmt_stream_list(void)
{
    INIT_LIST_HEAD(&__ts_pmt_stream_list.list);
}



/*  
 *  Function    : Parse the pmt table and add the pmt stream info into
 *                global list------ts_pmt_stream_list
 */
int parse_pmt_table_one_program(FILE *pFile, unsigned int packetLength,
        unsigned int programNumber)  
{   

    TABLE_SECTION_LIST * pmt_section_one_program_head = store_psi_si_table(pFile, packetLength,
            programNumber, TABLE_ID_TS_PMT);

    TABLE_SECTION_LIST * pmt_section_one_program_index = pmt_section_one_program_head;

    //the last 8 byte is the end flag
    TS_PMT_TABLE * pmt_table_one_program_head = (TS_PMT_TABLE *)malloc(sizeof(TS_PMT_TABLE)*(pmt_section_one_program_head->last_section_number + 1) + 2*sizeof(int));
    memset(pmt_table_one_program_head, 0, sizeof(TS_PMT_TABLE)*(pmt_section_one_program_head->last_section_number + 1) + 2*sizeof(int));

    TS_PMT_TABLE * pmt_table_one_program_iloop = pmt_table_one_program_head;
    unsigned int i=0;

    while(NULL != pmt_section_one_program_index)
    {
        parse_pmt_table_onesection(pmt_section_one_program_index->pbuffer, pmt_table_one_program_iloop + i, programNumber);
        i++;
        pmt_section_one_program_index = pmt_section_one_program_index->next_section;
    }
    

    //free TABLE_SECTION_LIST
    pmt_section_one_program_index = pmt_section_one_program_head;

    TABLE_SECTION_LIST * next_pmt_section_one_program = NULL;

    while(NULL != pmt_section_one_program_index)
    {
        next_pmt_section_one_program = pmt_section_one_program_index->next_section;

        //free pbuffer
        free(pmt_section_one_program_index->pbuffer);
        pmt_section_one_program_index->pbuffer = NULL;

        //free table_section
        free(pmt_section_one_program_index);
        pmt_section_one_program_index = NULL;
    
        pmt_section_one_program_index = next_pmt_section_one_program;
    }
    
    //return the pmt table head of one program
    return pmt_table_one_program_head;
}



    
    
int parse_pmt_table_onesection(unsigned char * pBuffer, TS_PMT_TABLE * psiPMT, unsigned int programNumber) 
{    
    
    int pmt_len;
    int pos_offset;
    P_TS_PMT_Stream tmp,freetmp;
    TS_PACKET_HEADER tsPacketHeader;
    P_TS_PACKET_HEADER ptsPacketHeader = &tsPacketHeader;
    parse_ts_packet_header(ptsPacketHeader, pBuffer);

    unsigned int offset = locate_offset(ptsPacketHeader, pBuffer, PSI_SI_PACKET_FLAG, 0);
    unsigned char * buffer = pBuffer;
    struct list_head *pos;

    psiPMT->table_id                            = PMT_TABLE_ID(buffer, offset);  
    psiPMT->section_syntax_indicator            = PMT_SECTION_SYNTAX_INDICATOR(buffer, offset);  
    //psiPMT->zero                                = buffer[1] >> 6 & 0x01;   
    //psiPMT->reserved_1                          = buffer[1] >> 4 & 0x03;  
    psiPMT->section_length                      = PMT_SECTION_LENGTH(buffer,offset);      
    psiPMT->program_number                      = PMT_PROGRAM_NUM(buffer, offset);  
    //psiPMT->reserved_2                          = buffer[5] >> 6;  
    psiPMT->version_number                      = PMT_VERSION_NUM(buffer, offset);  
    psiPMT->current_next_indicator              = PMT_CUR_NEXT_INDICATOR(buffer, offset);  
    psiPMT->section_number                      = PMT_SECTION_NUM(buffer, offset);  
    psiPMT->last_section_number                 = PMT_LAST_SECTION_NUM(buffer, offset);  
    //psiPMT->reserved_3                          = buffer[8] >> 5;  
    psiPMT->PCR_PID                             = PMT_PCR_PID(buffer,offset);  
    //psiPMT->reserved_4                          = buffer[10] >> 4;  
    psiPMT->program_info_length                 = PMT_PROGRAM_INFO_LENGTH(buffer,offset);   
    // Get CRC_32    
    psiPMT->CRC_32  = PMT_CAC32(buffer,offset);
    
    INIT_LIST_HEAD(&(psiPMT->this_section_pmt_stream_head.program_list));

    buffer += offset;
   
    pos_offset = 12;  
    // program info descriptor  
    if ( psiPMT->program_info_length != 0 )  
        pos_offset += psiPMT->program_info_length;      
        // Get stream type and PID

    //judge to add to __ts_pmt_stream_list or not.
    tmp =  (P_TS_PMT_Stream)malloc(sizeof(TS_PMT_Stream));
    freetmp = tmp;
    list_for_each(pos,&__ts_pmt_stream_list.list)
    {
        tmp = list_entry(pos,TS_PMT_Stream, list);

        if(tmp->program_number == programNumber)
        {
            free(freetmp);
            return 0;
        }
    }

    //section_length + 2 --> pmt start.  -4 --> CRC
    for ( ; pos_offset <= (psiPMT->section_length + 2 ) - 4; pos_offset += 5 )  
    {  
        tmp = (P_TS_PMT_Stream)malloc(sizeof(TS_PMT_Stream));
        tmp->stream_type    =  buffer[pos_offset];  
        tmp->reserved_5     =   buffer[pos_offset+1] >> 5;  
        tmp->elementary_PID =  ((buffer[pos_offset+1] << 8) | buffer[pos_offset+2]) & 0x1FFF;  
        tmp->reserved_6     =   buffer[pos_offset+3] >> 4;  
        tmp->ES_info_length =   (buffer[pos_offset+3] & 0x0F) << 8 | buffer[pos_offset+4];  
        tmp->program_number = programNumber;

        list_add(&(tmp->list), &(__ts_pmt_stream_list.list));
        list_add(&(tmp->program_list), &(psiPMT->this_section_pmt_stream_head.program_list));

        if (tmp->ES_info_length != 0)  
            pos_offset += tmp->ES_info_length;    
    }  
    
    #ifdef DEBUG
//        show_pmt_stream_info();
    #endif
    
    return 0;  
}  

//very simple  unknown 0 Video =1  Audio 2
int judge_media_type(P_TS_PMT_Stream ptsPmtStream)
{   
    int mediaType = 0;
    switch (ptsPmtStream->stream_type)
    {
        case 0x01:
        case 0x02:
        case 0x1b:
            //Video
            mediaType = 1;
            break;
        case 0x03:
        case 0x04:
        case 0x11:
        case 0x0f:
            //Audio
            mediaType = 2;
            break;
        default:
            break;
    }

    return mediaType;
}



int show_pmt_stream_info_onesection(TS_PMT_TABLE * pmtTable)
{
    struct list_head *pos;
    P_TS_PMT_Stream tmp = (P_TS_PMT_Stream)malloc(sizeof(TS_PMT_Stream));
    P_TS_PMT_Stream pFreetmp = tmp;
    int mediaType = -1;
    char * mediaTypeString[3] = {"Unknown","Video","Audio"};
    
    list_for_each(pos, &(pmtTable->this_section_pmt_stream_head.program_list))
    {
        tmp = list_entry(pos,TS_PMT_Stream, program_list);
        mediaType = judge_media_type(tmp);
        uprintf("-------------------------------------------\n");
        uprintf("the program_number is 0x%X(%d)\n",tmp->program_number,tmp->program_number);
        uprintf("the stream_type is    0x%X(%d)(%s)\n",tmp->stream_type,tmp->stream_type, mediaTypeString[mediaType]);
        uprintf("the elementary_PID is 0x%X,(%d)\n",tmp->elementary_PID, tmp->elementary_PID);
        uprintf("-------------------------------------------\n");
    }

    free(pFreetmp);

    return 0;
}


int show_pmt_table_info_one_program(TS_PMT_TABLE * pmtTable)
{
    TS_PMT_TABLE *tmp = pmtTable;
    unsigned int *ptmp = (unsigned int *)tmp;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
    
        uprintf("-------------------------------------------\n");
        uprintf("table_id                   :   0x%X(%d)\n",tmp->table_id, tmp->table_id);
        uprintf("section_syntax_indicator   :   0x%X(%d)\n",tmp->section_syntax_indicator, tmp->section_syntax_indicator);
        uprintf("section_length             :   0x%X(%d)\n",tmp->section_length,tmp->section_length);
        uprintf("program_number             :   0x%X(%d)\n",tmp->program_number,tmp->program_number);
        uprintf("version_number             :   0x%X(%d)\n",tmp->version_number,tmp->version_number);
        uprintf("current_next_indicator     :   0x%X(%d)\n",tmp->current_next_indicator,tmp->current_next_indicator);
        uprintf("section_number             :   0x%X(%d)\n",tmp->section_number,tmp->section_number);
        uprintf("last_section_number        :   0x%X(%d)\n",tmp->last_section_number,tmp->last_section_number);
        uprintf("PCR_PID                    :   0x%X(%d)\n",tmp->PCR_PID,tmp->PCR_PID);
        uprintf("program_info_length        :   0x%X(%d)\n",tmp->program_info_length,tmp->program_info_length);
        uprintf("CRC_32                     :   0x%X(%d)\n",tmp->CRC_32,tmp->CRC_32);
        show_pmt_stream_info_onesection(tmp);
        uprintf("-------------------------------------------\n");
        tmp++;
        //to jedge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    }
}




int show_pmt_stream_info(void)
{
    struct list_head *pos;
    P_TS_PMT_Stream tmp = (P_TS_PMT_Stream)malloc(sizeof(TS_PMT_Stream));
    P_TS_PMT_Stream pFreetmp = tmp;
    int mediaType = -1;
    char * mediaTypeString[3] = {"Unknown","Video","Audio"};
    
    list_for_each(pos, &__ts_pmt_stream_list.list)
    {
        tmp = list_entry(pos,TS_PMT_Stream, list);
        mediaType = judge_media_type(tmp);

        uprintf("-------------------------------------------\n");
        uprintf("the program_number is 0x%X(%d)\n",tmp->program_number,tmp->program_number);
        uprintf("the stream_type is    0x%X(%d)(%s)\n",tmp->stream_type,tmp->stream_type, mediaTypeString[mediaType]);
        uprintf("the elementary_PID is 0x%X,(%d)\n",tmp->elementary_PID, tmp->elementary_PID);
        uprintf("-------------------------------------------\n");
    }

    free(pFreetmp);

    return 0;
}















#if 0
int setup_pmt_stream_list(FILE *pFile, unsigned int packetLength)
{
    struct list_head *pos;
    P_TS_PAT_Program tmp_pat_program = (P_TS_PAT_Program)malloc(sizeof(TS_PAT_Program));
    P_TS_PAT_Program pFreetmp = tmp_pat_program;
    TS_PMT_TABLE mtsPmtTable;

    list_for_each(pos, &__ts_pat_program_list.list)
    {
        tmp_pat_program = list_entry(pos,TS_PAT_Program, list);
        TABLE_SECTION_LIST *pmt_table = store_psi_si_table(pFile, packetLength, tmp_pat_program->program_map_pid, TABLE_ID_TS_PMT);
        TABLE_SECTION_LIST *pmt_index = pmt_table;
        while()
        
        parse_pmt_table(pPacketBuffer, tmp_pat_program->program_number, &mtsPmtTable); 
    }

    show_pmt_stream_info();

    free(pFreetmp);
    
    fseek(pFile, 0, SEEK_SET);
    
    return 0;
}
#endif
