/*********************************************************************
*
* Filename      :   sdt.c
* Description   :   fundamental operation for SDT table and SDT service.
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <print_debug.h>
#include <sdt.h>
#include <tslib.h>
#include <descriptor_common.h>
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
    else
    {
		psdtService->first_desc = NULL;
    }

	return (len + 5);
}



TS_SDT_TABLE * parse_sdt_table(FILE *pFile, unsigned int packetLength)
{
    TABLE_SECTION_LIST * sdt_section = store_psi_si_table(pFile, packetLength, PID_TS_SI_SDT, TABLE_ID_SDT_OTHER);
    TABLE_SECTION_LIST * sdt_section_index = sdt_section;
    //the last 8 byte is the end flag
    TS_SDT_TABLE * sdt_table_head = (TS_SDT_TABLE *)malloc(sizeof(TS_SDT_TABLE)*(sdt_section->last_section_number + 1) + 2*sizeof(int));
    memset(sdt_table_head, 0, sizeof(TS_SDT_TABLE)*(sdt_section->last_section_number + 1) + 2*sizeof(int));

    TS_SDT_TABLE * sdt_table_index = sdt_table_head;
    unsigned int i=0;

    while(NULL != sdt_section_index)
    {
        parse_sdt_table_onesection(sdt_section_index->pbuffer, sdt_table_index + i);
        i++;
        sdt_section_index = sdt_section_index->next_section;
    }
    
    //free TABLE_SECTION_LIST
    sdt_section_index = sdt_section;
    TABLE_SECTION_LIST * next_sdt_section = NULL;
    while(NULL != sdt_section_index)
    {
        next_sdt_section = sdt_section_index->next_section;

        //free pbuffer
        free(sdt_section_index->pbuffer);
        sdt_section_index->pbuffer = NULL;

        //free table_section
        free(sdt_section_index);
        sdt_section_index = NULL;
    
        sdt_section_index = next_sdt_section;
    }
    
    //return the sdt list head
    return sdt_table_head;
}



TS_SDT_TABLE * parse_sdt_table_onesection(unsigned char *byteptr, TS_SDT_TABLE * pSdtTable)
{
    TS_PACKET_HEADER mtsPacketHeader;
    parse_ts_packet_header(&mtsPacketHeader,byteptr);

    unsigned int offset = locate_offset(&mtsPacketHeader, byteptr, PSI_SI_PACKET_FLAG, 0);
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

    if(len <= 0)
    {
        pSdtTable->first_sdt_service = NULL;
        return 0;
    }

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

    //show_sdt_table_info(pSdtTable);

    return pSdtTable;
}



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
        
        show_sdt_service_descriptors_info(ptmp);
        ptmp = ptmp->next_sdt_service;
    }
}

int show_sdt_table_info(TS_SDT_TABLE * pSdtTable)
{
    TS_SDT_TABLE *tmp = pSdtTable;
    unsigned int *ptmp = (unsigned int *)tmp;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
    
        uprintf("-------------------------------------------\n");
	    uprintf("SDT->table_id                :   0x%x(%d)\n",tmp->table_id,tmp->table_id);
	    uprintf("SDT->section_syntax_indicator:   0x%x(%d)\n",tmp->section_syntax_indicator,tmp->section_syntax_indicator);
	    uprintf("SDT->section_length          :   0x%x(%d)\n",tmp->section_length,tmp->section_length);
	    uprintf("SDT->transport_stream_id     :   0x%x(%d)\n",tmp->transport_stream_id,tmp->transport_stream_id);
	    uprintf("SDT->version_number          :   0x%x(%d)\n",tmp->version_number,tmp->version_number);
	    uprintf("SDT->current_next_indicator  :   0x%x(%d)\n",tmp->current_next_indicator,tmp->current_next_indicator);
	    uprintf("SDT->section_number          :   0x%x(%d)\n",tmp->section_number,tmp->section_number);
	    uprintf("SDT->last_section_number     :   0x%x(%d)\n",tmp->last_section_number,tmp->last_section_number);
	    uprintf("SDT->original_network_id     :   0x%x(%d)\n",tmp->original_network_id,tmp->original_network_id);
	    uprintf("SDT->CRC_32                  :   0x%x(%d)\n",tmp->CRC_32,tmp->CRC_32);
        uprintf("-------------------------------------------\n");
        uprintf("\nFollowing information is the SDT Service Information.\n\n");
        show_sdt_service_info(tmp->first_sdt_service);
        tmp++;
        //to judge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    }
    return 0;
}



void show_sdt_service_descriptors_info(SDT_SERVICE * sdtService)
{
    DESCRIPTOR_COMMON *head = sdtService->first_desc;
    
    show_desc(head); 
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


void free_sdt_table(TS_SDT_TABLE * sdt_table_header)
{
    TS_SDT_TABLE *tmp = sdt_table_header;
    
    unsigned int *ptmp = (unsigned int *)tmp;

    while(NULL != tmp && (ptmp[0] | ptmp[1]) != 0)
    {
        free_sdt_service(tmp);
        tmp++;
        //to jedge if goto the end. last_8 byte.
        ptmp = (unsigned int *)tmp;
    }

    free(sdt_table_header);
    sdt_table_header = NULL;
    
}

