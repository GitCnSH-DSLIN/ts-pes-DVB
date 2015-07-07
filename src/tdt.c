/*********************************************************************
*
* Filename      :   tdt.c
* Description   :   fundamental operation for TDT table.
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <print_debug.h>
#include <tdt.h>
#include <tslib.h>
#include <descriptor_common.h>

TS_TDT_TABLE * parse_tdt_table_onesection(unsigned char *byteptr, TS_TDT_TABLE * pTdtTable)
{
    TS_PACKET_HEADER mtsPacketHeader;
    parse_ts_packet_header(&mtsPacketHeader,byteptr);

    unsigned int offset = locate_offset(&mtsPacketHeader, byteptr, PSI_SI_PACKET_FLAG, 0);
    uprintf("the value of offset is %d\n",offset);
    unsigned char * b = byteptr + offset;

	
    pTdtTable->table_id = TDT_TABLE_ID(b);
	pTdtTable->section_syntax_indicator = TDT_SECTION_SYNTAX_INDICATOR(b);
	pTdtTable->reserved_future_use_1 = TDT_RESERVED_FUTURE_USE_1(b);
	pTdtTable->reserved_1 = TDT_RESERVED_1(b);
	pTdtTable->section_length = TDT_SECTION_LENGTH(b);
	pTdtTable->UTC_time = TDT_UTC_TIME(b);

    //uprintf("UTC_time : 0x%lx\n",pTdtTable->UTC_time);
      
    return pTdtTable;
}

//tdt only have one section
TS_TDT_TABLE * parse_tdt_table(FILE *pFile, unsigned int packetLength)
{
    TABLE_SECTION_LIST * tdt_section = store_psi_si_table_reserved(pFile, packetLength, PID_TS_SI_TDT, TABLE_ID_TDT);
    //the last 8 byte is the end flag
    TS_TDT_TABLE * tdt_table_head = (TS_TDT_TABLE *)malloc(sizeof(TS_TDT_TABLE));
    memset(tdt_table_head, 0, sizeof(TS_TDT_TABLE));

    parse_tdt_table_onesection(tdt_section->pbuffer, tdt_table_head);

    //free TABLE_SECTION_LIST
    //free pbuffer
    free(tdt_section->pbuffer);
    tdt_section->pbuffer = NULL;

    //free table_section
    free(tdt_section);
    tdt_section = NULL;
    
    //return the tdt list head
    return tdt_table_head;
}






int show_tdt_table_info(TS_TDT_TABLE * ptdtTable)
{
    TS_TDT_TABLE *tmp = ptdtTable;
    
    uprintf("-------------------------------------------\n");
	uprintf("TDT->table_id                :   %#x(%d)\n",tmp->table_id,tmp->table_id);
	uprintf("TDT->section_syntax_indicator:   %#x(%d)\n",tmp->section_syntax_indicator,tmp->section_syntax_indicator);
	uprintf("TDT->section_length          :   %#x(%d)\n",tmp->section_length,tmp->section_length);
	uprintf("TDT->UTC_time                :   %#8lx(%lu)\n",tmp->UTC_time,tmp->UTC_time);
    uprintf("-------------------------------------------\n\n");
    return 0;
}


void free_tdt_table(TS_TDT_TABLE * tdt_table_header)
{

    free(tdt_table_header);
    tdt_table_header = NULL;
}


