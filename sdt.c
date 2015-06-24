/*
 * sdt.c
 *
 *  Created on: Jun 21, 2011
 *      Author: orion
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <print_debug.h>
#include <sdt.h>
#include <desc_short_event.h>
#include <desc_network_name.h>
#include <desc_service.h>
#include <desc_service_list.h>
#include <ts_psi.h>

P_SDT_SERVICE insert_sdt_service_node(SDT_SERVICE * Header, SDT_SERVICE * node)
{
    SDT_SERVICE * ptmp = Header;
    SDT_SERVICE * pcur = NULL;

    while (ptmp != NULL)
    {
        pcur = ptmp;
        ptmp = ptmp->next_sdt_service;
    }

    pcur->next_sdt_service = node;

    return Header;
}


void* decode_desc(unsigned char* byteptr, int this_section_length)
{
	unsigned char* b = byteptr;
	int desc_len;
	int l = this_section_length;

	void* this_descriptor = NULL;

	switch (b[0]) 
    {
		//maybe NETWORK_NAME_DESC will be globle var
	    case 0x40: // network_name_descriptor
	    {
		    this_descriptor = (void*)malloc(sizeof(NETWORK_NAME_DESC));
		    memset(this_descriptor, 0, sizeof(NETWORK_NAME_DESC));
		    //NETWORK_NAME_DESC desc_network_name;
		    desc_len = decode_network_name_desc(b, this_section_length,
				    this_descriptor);
		    b += desc_len;
		    l -= desc_len;

		    if(b < b + l)
            {
			    ((NETWORK_NAME_DESC*)this_descriptor)->next_desc = decode_desc(b, l);
		    }
	    }
		break;
	    
        case 0x41: 
        {
		    this_descriptor = (void*)malloc(sizeof(SERVICE_LIST_DESC));
		    memset(this_descriptor, 0, sizeof(SERVICE_LIST_DESC));
		    desc_len = decode_servicelist_desc(b, this_section_length,
				    this_descriptor);
		    b += desc_len;
		    l -= desc_len;

		    if(b < b + l)
            {
			    ((SERVICE_LIST_DESC*)this_descriptor)->next_desc = decode_desc(b, l);
		    }
	    }
		break;

	    case 0x48: 
        {
		    this_descriptor = (void*)malloc(sizeof(SERVICE_DESC));
		    memset(this_descriptor, 0, sizeof(SERVICE_DESC));
		    desc_len = decode_service_desc(b, this_section_length,
				    this_descriptor);
		    b += desc_len;
		    l -= desc_len;

		    if(b < b + l){
			    ((SERVICE_DESC*)this_descriptor)->next_desc = decode_desc(b, l);
		    }
	    }
		break;
	
        case 0x4d: 
        {
		    this_descriptor = (void*)malloc(sizeof(SHORT_EVENT_DESC));
		    memset(this_descriptor, 0 ,sizeof(SHORT_EVENT_DESC));
		    desc_len = decode_short_evt_desc(b, this_section_length,
				    this_descriptor);
		    b += desc_len;
		    l -= desc_len;

		    if(b < b + l)
            {
			    ((SHORT_EVENT_DESC*)this_descriptor)->next_desc = decode_desc(b, l);
		    }
	    }
		break;

	    default:
        {
		    // other descriptors we don't handle them,but we need their length,so we get it.
		    desc_len = b[1] + 2 ;
		    b += desc_len;
		    l -= desc_len;

		    if(b < b + l)
            {
			    this_descriptor = decode_desc(b, l);
		    }
	    }
    }

	return this_descriptor;
}


void free_desc(void* phead)
{
	//void* this_descriptor = phead;
	unsigned char* b = (unsigned char *)phead;

	if(b == NULL){
		return;
	}

    switch(b[0]){
	case 0x40:  // NETWORK_NAME_DESC
		free_network_name_desc((NETWORK_NAME_DESC*)phead);
		break;
	case 0x41: //SERVICE_LIST_DESC
		free_servicelist_desc((SERVICE_LIST_DESC*)phead);
		break;
	case 0x48: //SERVICE_DESC
		free_service_desc((SERVICE_DESC*)phead);
		break;
	case 0x4d: //SHORT_EVENT_DESC
		free_short_evt_desc((SHORT_EVENT_DESC*)phead);
		break;
	default:
		//free_desc();
		return;
	}
}





int decode_sdt_service(unsigned char * byteptr, int this_section_length, SDT_SERVICE* psdtService)
{
	unsigned char * b = byteptr;
//	int l = this_section_length;

	psdtService->service_id = SDT_SERVICE_ID(b);
	psdtService->reserved_future_use = SDT_SERVICE_RESERVED_FUTURE_USD(b);
	psdtService->EIT_schedule_flag = SDT_SERVICE_EIT_SCHEDULE_FLAG(b);
	psdtService->EIT_present_following_flag = SDT_SERVICE_EIT_PRESENT_FOLLOWING_FLAG(b);
	psdtService->running_status = SDT_SERVICE_RUNNING_STATUS(b);
	psdtService->free_CA_mode = SDT_SERVICE_FREE_CA_MODE(b);
	psdtService->descriptors_loop_length = SDT_SERVICE_DESCRIPTORS_LOOP_LENGTH(b);

	int len = psdtService->descriptors_loop_length;

	//5byte = the size from service_id to descriptors_loop_length;
    unsigned char * serv_start = &b[5];

	if(len > 0)
    {
		psdtService->first_desc = decode_desc(serv_start,len);
	}

	return (len + 5);
}

int parse_sdt_table(unsigned char * byteptr, int this_section_length, TS_SDT_TABLE * pSdtTable)
{
    TS_PACKET_HEADER mtsPacketHeader;
    unsigned int pes_start;
    parse_ts_packet_header(&mtsPacketHeader,byteptr);

    unsigned int offset = locate_offset(&mtsPacketHeader, byteptr, 0, &pes_start);
    unsigned char * b = byteptr + offset;

	
    pSdtTable->table_id = SDT_TABLE_ID(b);
	pSdtTable->section_syntax_indicator = SDT_SECTION_SYNTAX_INDICATOR(b);
	pSdtTable->reserved_future_use_1 = SDT_RESERVED_FUTURE_USD_1(b);
	pSdtTable->reserved_1 = SDT_RESERVED_1(b);
	pSdtTable->section_length = SDT_SECTION_LENGTH(b);
	pSdtTable->transport_stream_id = SDT_TRANSPORT_STREAM_ID(b);
	pSdtTable->reserved_2 = SDT_RESERVED_2(b);
	pSdtTable->version_number = SDT_VERSION_NUMBER(b);
	pSdtTable->current_next_indicator = SDT_CURRENT_NEXT_INDICATOR(b);
	pSdtTable->section_number = SDT_SECTION_NUMBER(b);
	pSdtTable->last_section_number = SDT_LAST_SECTION_NUMBER(b);
	pSdtTable->original_network_id = SDT_ORIGINAL_NETWORK_ID(b);
	pSdtTable->reserved_future_use_2 = SDT_RESERVED_FUTURE_USD_2(b);
	pSdtTable->CRC_32 = SDT_CRC(b);


    //12 = CRC4 + 8byte after section_length
    int len = pSdtTable->section_length - 12;
    unsigned char * serv_start = &b[11];
    int sdtServiceLength = 0;
    SDT_SERVICE  * psdtServiceNode = NULL;
    int firstFlag = 1;

	while(len > 0)
    {
        psdtServiceNode = (SDT_SERVICE *)malloc(sizeof(SDT_SERVICE));
		memset(psdtServiceNode, 0, sizeof(SDT_SERVICE));
        sdtServiceLength = decode_sdt_service(serv_start, len, psdtServiceNode);
        if (1 == firstFlag)
        {
            pSdtTable->first_sdt_service = psdtServiceNode;
            firstFlag = 0;
        }
        else
        {
            insert_sdt_service_node(pSdtTable->first_sdt_service, psdtServiceNode);
        }

        len -= sdtServiceLength;
        serv_start += sdtServiceLength;
   
    }

    show_sdt_table_info(pSdtTable);

    return 0;
}

#if 0
void show_sdt_service_descriptors_info(SDT_SERVICE * sdtService)
{
	void * HeaderDesc = sdtService->first_desc;
    void * ptmp = HeaderDesc;

    while (NULL != ptmp)
    {
        uprintf("\t\tSDT_Service_Desc->descriptor_tag       :   0x%x\n",ptmp->descriptor_tag);
	    uprintf("\t\tSDT_Service_Desc->descriptor_length    :   0x%x\n",ptmp->descriptor_length);
        ptmp = ptmp->next_desc;
    }
}
#endif


void show_sdt_service_info(SDT_SERVICE * Header)
{
	SDT_SERVICE *ptmp = Header;
    while (NULL != ptmp)
    {
        uprintf("\tSDT_Service->service_id                   :   0x%x\n",ptmp->service_id);
	    uprintf("\tSDT_Service->EIT_schedule_flag            :   0x%x\n",ptmp->EIT_schedule_flag);
	    uprintf("\tSDT_Service->EIT_present_following_flag   :   0x%x\n",ptmp->EIT_present_following_flag);
	    uprintf("\tSDT_Service->running_status               :   0x%x\n",ptmp->running_status);
	    uprintf("\tSDT_Service->descriptors_loop_length      :   0x%x\n\n",ptmp->descriptors_loop_length);
        ptmp = ptmp->next_sdt_service;
    }
}


void show_sdt_table_info(TS_SDT_TABLE * pSdtTable)
{
	uprintf("SDT->table_id                :   0x%x\n",pSdtTable->table_id);
	uprintf("SDT->section_syntax_indicator:   0x%x\n",pSdtTable->section_syntax_indicator);
	uprintf("SDT->section_length          :   0x%x\n",pSdtTable->section_length);
	uprintf("SDT->transport_stream_id     :   0x%x\n",pSdtTable->transport_stream_id);
	uprintf("SDT->version_number          :   0x%x\n",pSdtTable->version_number);
	uprintf("SDT->current_next_indicator  :   0x%x\n",pSdtTable->current_next_indicator);
	uprintf("SDT->section_number          :   0x%x\n",pSdtTable->section_number);
	uprintf("SDT->last_section_number     :   0x%x\n",pSdtTable->last_section_number);
	uprintf("SDT->original_network_id     :   0x%x\n",pSdtTable->original_network_id);
	uprintf("SDT->CRC_32                  :   0x%x\n",pSdtTable->CRC_32);

    uprintf("\nFollowing information is the SDT Service Information.\n\n");
    show_sdt_service_info(pSdtTable->first_sdt_service);
}





void free_sdt_service(TS_SDT_TABLE * sdt)
{
	SDT_SERVICE *head = sdt->first_sdt_service;
	SDT_SERVICE *temp;

	while(head != NULL){
        //free all of the descriptors of this sdtService.
		free_desc(head->first_desc);

		temp = head;
		head = temp->next_sdt_service;

        //free this sdtService struction.
		temp->next_sdt_service = NULL;
		free(temp);
	}

	sdt->first_sdt_service = NULL;
}

void free_sdt(TS_SDT_TABLE * sdt)
{
	free_sdt_service(sdt);
}

