/*********************************************************************
*
* Filename      :   nit.c
* Description   :   fundamental operation for SDT table and SDT service.
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <print_debug.h>
#include <nit.h>
#include <tslib.h>
#include <descriptor_common.h>


P_TRANSPORT_STREAM insert_nit_transport_stream_node(TRANSPORT_STREAM * Header, TRANSPORT_STREAM * node)
{
    TRANSPORT_STREAM * ptmp = Header;
    TRANSPORT_STREAM * pcur = NULL;

    while (ptmp != NULL)
    {
        pcur = ptmp;
        ptmp = ptmp->next_transport_stream;
    }

    pcur->next_transport_stream = node;

    return Header;
}



int decode_nit_transport_stream(unsigned char * byteptr, int this_section_length, TRANSPORT_STREAM * pNitTranstream)
{
	unsigned char * b = byteptr;

	pNitTranstream->transport_stream_id = NIT_TRANSPORT_STREAM_ID(b);
	pNitTranstream->original_network_id = NIT_TRANSPORT_ORIGINAL_NETWORK_ID(b);
	pNitTranstream->transport_descriptors_length = NIT_TRANSPORT_DESC_LENGTH(b);

	int len = pNitTranstream->transport_descriptors_length;

	//6byte = the size from transport_stream_id to transport_descriptors_length;
    unsigned char * serv_start = &b[6];

	if(len > 0)
    {
		pNitTranstream->first_desc = decode_desc(serv_start,len);
	}
    else
    {
		pNitTranstream->first_desc = NULL;
    }

	return (len + 6);
}



TS_NIT_TABLE * parse_nit_table(FILE *pFile, unsigned int packetLength)
{
    TABLE_SECTION_LIST * nit_section = store_psi_si_table(pFile, packetLength, PID_TS_SI_NIT, TABLE_ID_NIT_ACTUAL);
    TABLE_SECTION_LIST * nit_section_index = nit_section;
    //the last 8 byte is the end flag
    TS_NIT_TABLE * nit_table_head = (TS_NIT_TABLE *)malloc(sizeof(TS_NIT_TABLE)*(nit_section->last_section_number + 1) + 2*sizeof(int));
    memset(nit_table_head, 0, sizeof(TS_NIT_TABLE)*(nit_section->last_section_number + 1) + 2*sizeof(int));

    TS_NIT_TABLE * nit_table_index = nit_table_head;
    unsigned int i=0;

    while(NULL != nit_section_index)
    {
        parse_nit_table_onesection(nit_section_index->pbuffer, nit_table_index + i);
        i++;
        nit_section_index = nit_section_index->next_section;
    }
    
    //free TABLE_SECTION_LIST
    nit_section_index = nit_section;
    TABLE_SECTION_LIST * next_nit_section = NULL;
    while(NULL != nit_section_index)
    {
        next_nit_section = nit_section_index->next_section;

        //free pbuffer
        free(nit_section_index->pbuffer);
        nit_section_index->pbuffer = NULL;

        //free table_section
        free(nit_section_index);
        nit_section_index = NULL;
    
        nit_section_index = next_nit_section;
    }
    
    //return the sdt list head
    return nit_table_head;
}



TS_NIT_TABLE * parse_nit_table_onesection(unsigned char *byteptr, TS_NIT_TABLE * pNitTable)
{
    TS_PACKET_HEADER mtsPacketHeader;
    parse_ts_packet_header(&mtsPacketHeader,byteptr);

    unsigned int offset = locate_offset(&mtsPacketHeader, byteptr, PSI_SI_PACKET_FLAG, 0);
    unsigned char * b = byteptr + offset;

	
    pNitTable->table_id                 =   NIT_TABLE_ID(b);
	pNitTable->section_syntax_indicator =   NIT_SECTION_SYNTAX_INDICATOR(b);
	pNitTable->reserved_future_use_1    =   NIT_RESERVED_FUTURE_USE_1(b);
	pNitTable->reserved_1               =   NIT_RESERVED_1(b);
	pNitTable->section_length           =   NIT_SECTION_LENGTH(b);
	pNitTable->network_id                =   NIT_NETWORK_ID(b);
	pNitTable->reserved_2               =   NIT_RESERVED_2(b);
	pNitTable->version_number           =   NIT_VERSION_NUMBER(b);
	pNitTable->current_next_indicator   =   NIT_CURRENT_NEXT_INDICATOR(b);
	pNitTable->section_number           =   NIT_SECTION_NUMBER(b);
	pNitTable->last_section_number      =   NIT_LAST_SECTION_NUMBER(b);
	pNitTable->reserved_future_use_2    =   NIT_RESERVED_FUTURE_USE_2(b);
	pNitTable->network_descriptors_length = NIT_NETWORK_DESC_LENGTH(b);
	pNitTable->CRC_32 = NIT_CRC_32(b);
        
    int len = pNitTable->network_descriptors_length;
    unsigned char * serv_start = &b[10];
    
    if(len > 0)
    {
        pNitTable->nit_first_desc = decode_desc(serv_start, len);
    }
    else
    {
        pNitTable->nit_first_desc = NULL;
    }

    //10byte is from table_id to network_desc_length;
    b +=  10 + pNitTable->network_descriptors_length;

    pNitTable->transport_stream_loop_length = (((b[0] & 0x0f) << 8) | b[1]);
    b += 2;
    len =  pNitTable->transport_stream_loop_length;
    serv_start = &b[0];

    int transportStreamLength = 0;
    TRANSPORT_STREAM * pNitTransNode = NULL;
    int firstFlag = 1;
    if(0 == len)
    {
        pNitTable->first_transport_stream = NULL;
        return 0;
    }

	while(len > 0)
    {
        pNitTransNode = (TRANSPORT_STREAM *)malloc(sizeof(TRANSPORT_STREAM));
		memset(pNitTransNode, 0, sizeof(TRANSPORT_STREAM));
        transportStreamLength = decode_nit_transport_stream(serv_start, len, pNitTransNode);
        if (1 == firstFlag)
        {
            pNitTable->first_transport_stream = pNitTransNode;
            firstFlag = 0;
        }
        else
        {
            insert_nit_transport_stream_node(pNitTable->first_transport_stream, pNitTransNode);
        }

        len -= transportStreamLength;
        serv_start += transportStreamLength;
    }

    return 0;
}


void show_nit_descriptors_info(DESCRIPTOR_COMMON *header)
{
    show_desc(header);
}

void show_nit_transport_stream_descriptors_info(TRANSPORT_STREAM * nitTransStream)
{
    DESCRIPTOR_COMMON *head = nitTransStream->first_desc;
    show_desc(head); 
}


void show_nit_transport_stream_info(TRANSPORT_STREAM * Header)
{
	TRANSPORT_STREAM *ptmp = Header;
    while (NULL != ptmp)
    {
        uprintf("\tTRANSPORT_STREAM->transport_stream_id            :   0x%x\n",ptmp->transport_stream_id);
	    uprintf("\tTRANSPORT_STREAM->original_network_id            :   0x%x\n",ptmp->original_network_id);
	    uprintf("\tTRANSPORT_STREAM->transport_descriptors_length   :   0x%x\n\n",ptmp->transport_descriptors_length);
        
        show_nit_transport_stream_descriptors_info(ptmp);
        ptmp = ptmp->next_transport_stream;
    }
}

int show_nit_table_info(TS_NIT_TABLE * pNitTable)
{
    TS_NIT_TABLE *tmp = pNitTable;
    unsigned int *ptmp = (unsigned int *)tmp;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
    
	    uprintf("SECTION : %d,  LAST_SECTION :  %d\n",tmp->section_number,tmp->last_section_number);
        uprintf("-------------------------------------------\n");
	    uprintf("NIT->table_id                  :   0x%x(%d)\n",tmp->table_id,tmp->table_id);
	    uprintf("NIT->section_syntax_indicator  :   0x%x(%d)\n",tmp->section_syntax_indicator,tmp->section_syntax_indicator);
	    uprintf("NIT->section_length            :   0x%x(%d)\n",tmp->section_length,tmp->section_length);
	    uprintf("NIT->network_id                :   0x%x(%d)\n",tmp->network_id,tmp->network_id);
	    uprintf("NIT->version_number            :   0x%x(%d)\n",tmp->version_number,tmp->version_number);
	    uprintf("NIT->current_next_indicator    :   0x%x(%d)\n",tmp->current_next_indicator,tmp->current_next_indicator);
	    uprintf("NIT->section_number            :   0x%x(%d)\n",tmp->section_number,tmp->section_number);
	    uprintf("NIT->last_section_number       :   0x%x(%d)\n",tmp->last_section_number,tmp->last_section_number);
	    uprintf("NIT->network_descriptors_length:   0x%x(%d)\n",tmp->network_descriptors_length,tmp->network_descriptors_length);
	    uprintf("NIT->trans_stream_loop_length  :   0x%x(%d)\n",tmp->transport_stream_loop_length,tmp->transport_stream_loop_length);
	    uprintf("NIT->CRC_32                    :   0x%x(%d)\n",tmp->CRC_32,tmp->CRC_32);
        uprintf("-------------------------------------------\n");
        uprintf("\nFollowing information is the NIT descriptors information.\n\n");
        show_nit_descriptors_info(tmp->nit_first_desc);
        uprintf("-------------------------------------------\n");
        uprintf("\nFollowing information is the NIT Transport Stream Information.\n\n");
        uprintf("-------------------------------------------\n\n\n");
        if(NULL != tmp->first_transport_stream)
            show_nit_transport_stream_info(tmp->first_transport_stream);
        tmp++;
        //to judge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    }
    return 0;
}





void free_transport_stream(TS_NIT_TABLE * nit)
{
	TRANSPORT_STREAM *head = nit->first_transport_stream;
	TRANSPORT_STREAM *temp;

	while(head != NULL){
        //free all of the descriptors of this transport_stream.
		free_desc(head->first_desc);

		temp = head;
		head = temp->next_transport_stream;

        //free this nit_transport_stream struction.
		temp->next_transport_stream = NULL;
		free(temp);
	}

	nit->first_transport_stream = NULL;
}


void free_nit_table(TS_NIT_TABLE * nit_table_header)
{
    TS_NIT_TABLE *tmp = nit_table_header;
    
    unsigned int *ptmp = (unsigned int *)tmp;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
        free_desc(tmp->nit_first_desc);

        free_transport_stream(tmp);
        tmp++;
        //to jedge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    }

    free(nit_table_header);
}

