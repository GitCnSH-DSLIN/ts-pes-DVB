/*
 * desc_service.c
 *
 *  Created on: Jun 24, 2011
 *      Author: orion
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <desc_service.h>
#include <print_debug.h>


SDT_DESCRIPTOR_COMMON * decode_service_desc(unsigned char* byteptr, int this_section_length)
{
	unsigned char* b = byteptr;

	SERVICE_DESC * desc_service = (SERVICE_DESC*)malloc(sizeof(SERVICE_DESC));
	memset(desc_service, 0, sizeof(SERVICE_DESC));
	
    desc_service->descriptor_tag = SERV_DESC_TAG(b);
	desc_service->descriptor_length = SERV_DESC_LENGTH(b);
	desc_service->service_type = SERV_DESC_SERV_TYPE(b);
	desc_service->service_provider_name_length = SERV_DESC_SERV_PRO_NAME_LENGTH(b);
	desc_service->provider_name = (char *)calloc(desc_service->service_provider_name_length + 1,sizeof(char));
	strncpy(desc_service->provider_name,(char *)&b[4],desc_service->service_provider_name_length);
	desc_service->service_name_length = SERV_DESC_SERV_NAME_LENGTH(b);
	desc_service->service_name = (char*)calloc(desc_service->service_name_length + 1, sizeof(char));
	strncpy(desc_service->service_name, (char*)&b[desc_service->service_provider_name_length + 5], desc_service->service_name_length);

#if 0
 	uprintf("desc_service->descriptor_tag:0x%x\n",desc_service->descriptor_tag);
 	uprintf("desc_service->descriptor_length:0x%x\n",desc_service->descriptor_length);
 	uprintf("desc_service->service_type:0x%x\n",desc_service->service_type);
 	uprintf("desc_service->service_provider_name_length:0x%x\n",desc_service->service_provider_name_length);
 	uprintf("desc_service->provider_name:%s\n",desc_service->provider_name);
 	uprintf("desc_service->service_name_length:0x%x\n",desc_service->service_name_length);
 	uprintf("desc_service->service_name:%s\n",desc_service->service_name);
#endif
	return (SDT_DESCRIPTOR_COMMON *)desc_service;
}
void free_service_desc(SERVICE_DESC* head){
	free(head->provider_name);
	free(head->service_name);
	head->provider_name = NULL;
	head->service_name = NULL;
	free_desc(head->next_desc);
	head->next_desc = NULL;
	free(head);
	head = NULL;
}

void show_service_descriptor(SDT_DESCRIPTOR_COMMON *ptmp)
{
    SERVICE_DESC * tmp = (SERVICE_DESC*)ptmp;
	uprintf("\t\tdescriptor_tag     :   0x%x\n",tmp->descriptor_tag);
	uprintf("\t\tdescriptor_name    :   service_descriptor\n");
	uprintf("\t\tdescriptor_length  :   0x%x\n",tmp->descriptor_length);
 	uprintf("\t\tservice_type       :   0x%x\n",tmp->service_type);
 	uprintf("\t\tservice_provider_name_length   :   0x%x\n",tmp->service_provider_name_length);
 	uprintf("\t\tprovider_name      :   %s\n",tmp->provider_name);
 	uprintf("\t\tservice_name_length:   0x%x\n",tmp->service_name_length);
 	uprintf("\t\tservice_name       :   %s\n\n",tmp->service_name);
}
