/*
 * desc_service_list.c
 *
 *  Created on: Jun 24, 2011
 *      Author: orion
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <desc_service_list.h>
#include <print_debug.h>

int decode_servicelist_desc(unsigned char* byteptr, int this_section_length,
        SERVICE_LIST_DESC* desc_servicelist){
	unsigned char* b = byteptr;

	desc_servicelist->descriptor_tag = SLD_DESC_TAG(b);
	desc_servicelist->descriptor_length = SLD_DESC_LEN(b);
#if 0
	uprintf("servicelist->descriptor_tag:0x%x\n",desc_servicelist->descriptor_tag);
	uprintf("desc_servicelist->descriptor_length:0x%x\n",desc_servicelist->descriptor_length);
#endif
	int len = desc_servicelist->descriptor_length;
	unsigned char* item_start = &b[2];
	while(len > 0){
		SERIVCE_LIST_DESC_ITEM sld_item;
		decode_servicelist_item(item_start,len,&sld_item);
		item_start += 3;
		len -= 3;
	}

	return (desc_servicelist->descriptor_length + 2);
}

int decode_servicelist_item(unsigned char* byteptr, int this_section_length,SERIVCE_LIST_DESC_ITEM* item_servicelist){
	unsigned char* b = byteptr;
	int l = this_section_length;

	item_servicelist->service_id = SLD_DESC_ITEM_ID(b);
	item_servicelist->service_type = SLD_DESC_ITEM_TYPE(b);
#if 0
	uprintf("item_servicelist->service_id:0x%x\n",item_servicelist->service_id);
	uprintf("item_servicelist->service_type:0x%x\n",item_servicelist->service_type);
#endif
	return l;
}
void free_servicelist_desc(SERVICE_LIST_DESC* head){
	free_desc(head->next_desc);
	head->next_desc = NULL;
	free(head);
	head = NULL;
}
