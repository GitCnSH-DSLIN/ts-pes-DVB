/*********************************************************************
*
* Filename      :   tot.c
* Description   :   fundamental operation for TOT table.
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <print_debug.h>
#include <tot.h>
#include <tslib.h>
#include <descriptor_common.h>

TS_TOT_TABLE * parse_tot_table_onesection(unsigned char *byteptr, TS_TOT_TABLE * pTotTable)
{
    TS_PACKET_HEADER mtsPacketHeader;
    parse_ts_packet_header(&mtsPacketHeader,byteptr);

    unsigned int offset = locate_offset(&mtsPacketHeader, byteptr, PSI_SI_PACKET_FLAG, 0);
    unsigned char * b = byteptr + offset;

	
    pTotTable->table_id = TOT_TABLE_ID(b);
	pTotTable->section_syntax_indicator = TOT_SECTION_SYNTAX_INDICATOR(b);
	pTotTable->reserved_future_use_1 = TOT_RESERVED_FUTURE_USE_1(b);
	pTotTable->reserved_1 = TOT_RESERVED_1(b);
	pTotTable->section_length = TOT_SECTION_LENGTH(b);
	pTotTable->UTC_time = TOT_UTC_TIME(b);
	pTotTable->descriptors_loop_length = TOT_DESC_LOOP_LENGTH(b);
	pTotTable->CRC_32 = TOT_CRC_32(b);
      
    int len = pTotTable->descriptors_loop_length;
    unsigned char * serv_start = &b[10];
    
    if(len > 0)
    {
        pTotTable->tot_first_desc = decode_desc(serv_start, len);
    }
    else
    {
        pTotTable->tot_first_desc = NULL;
    }

    return pTotTable;
}

//tot only have one section
TS_TOT_TABLE * parse_tot_table(FILE *pFile, unsigned int packetLength)
{
    TABLE_SECTION_LIST * tot_section = store_psi_si_table_reserved(pFile, packetLength, PID_TS_SI_TOT, TABLE_ID_TOT);
    //the last 8 byte is the end flag
    TS_TOT_TABLE * tot_table_head = (TS_TOT_TABLE *)malloc(sizeof(TS_TOT_TABLE));
    memset(tot_table_head, 0, sizeof(TS_TOT_TABLE));

    parse_tot_table_onesection(tot_section->pbuffer, tot_table_head);

    //free TABLE_SECTION_LIST
    //free pbuffer
    free(tot_section->pbuffer);
    tot_section->pbuffer = NULL;

    //free table_section
    free(tot_section);
    tot_section = NULL;
    
    //return the tot list head
    return tot_table_head;
}

void show_tot_descriptors_info(DESCRIPTOR_COMMON *header)
{
    show_desc(header);
}


int show_tot_table_info(TS_TOT_TABLE * ptotTable)
{
    TS_TOT_TABLE *tmp = ptotTable;
    
    uprintf("-------------------------------------------\n");
	uprintf("TOT->table_id                :   %#x(%d)\n",tmp->table_id,tmp->table_id);
	uprintf("TOT->section_syntax_indicator:   %#x(%d)\n",tmp->section_syntax_indicator,tmp->section_syntax_indicator);
	uprintf("TOT->section_length          :   %#x(%d)\n",tmp->section_length,tmp->section_length);
	uprintf("TOT->UTC_time                :   %#8lx(%lu)\n",tmp->UTC_time,tmp->UTC_time);
	uprintf("TOT->descriptors_loop_length :   %#x(%lu)\n",tmp->descriptors_loop_length,tmp->descriptors_loop_length);
    uprintf("-------------------------------------------\n\n");
    uprintf("\nFollowing information is the TOT descriptors information.\n\n");
    show_tot_descriptors_info(tmp->tot_first_desc);
    uprintf("-------------------------------------------\n");
    return 0;
}


void free_tot_table(TS_TOT_TABLE * tot_table_header)
{
    TS_TOT_TABLE *tmp = tot_table_header;
    if(NULL != tmp->tot_first_desc);
        free_desc(tmp->tot_first_desc);
    
    free(tot_table_header);
}



