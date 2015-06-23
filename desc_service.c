/*
 * desc_service.c
 *
 *  Created on: Jun 24, 2011
 *      Author: orion
 */
#include <stdio.h>
#include <desc_service.h>
#include <print_debug.h>


int decode_service_desc(unsigned char* byteptr, int this_section_length,
        SERVICE_DESC* desc_service){
	unsigned char* b = byteptr;

	desc_service->descriptor_tag = SERV_DESC_TAG(b);
	desc_service->descriptor_length = SERV_DESC_LENGTH(b);
	desc_service->service_type = SERV_DESC_SERV_TYPE(b);
	desc_service->service_provider_name_length = SERV_DESC_SERV_PRO_NAME_LENGTH(b);
	desc_service->provider_name = (char *)calloc(desc_service->service_provider_name_length + 1,sizeof(char));
	strncpy(desc_service->provider_name,(char *)&b[4],desc_service->service_provider_name_length);
	desc_service->service_name_length = SERV_DESC_SERV_NAME_LENGTH(b);
	desc_service->service_name = (char*)calloc(desc_service->service_name_length + 1, sizeof(char));
	strncpy(desc_service->service_name, (char*)&b[desc_service->service_provider_name_length + 5], desc_service->service_name_length);


 	uprintf("desc_service->descriptor_tag:0x%x\n",desc_service->descriptor_tag);
 	uprintf("desc_service->descriptor_length:0x%x\n",desc_service->descriptor_length);
 	uprintf("desc_service->service_type:0x%x\n",desc_service->service_type);
 	uprintf("desc_service->service_provider_name_length:0x%x\n",desc_service->service_provider_name_length);
 	uprintf("desc_service->provider_name:%s\n",desc_service->provider_name);
 	uprintf("desc_service->service_name_length:0x%x\n",desc_service->service_name_length);
 	uprintf("desc_service->service_name:%s\n",desc_service->service_name);

	return (desc_service->descriptor_length + 2);

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

