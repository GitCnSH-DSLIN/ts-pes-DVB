/*********************************************************************
*
* Filename      :   desc_service_list.c
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <descriptor_common.h>
#include <desc_service_list.h>
#include <print_debug.h>

DESCRIPTOR_COMMON * decode_service_list_desc(unsigned char* byteptr, int this_section_length)
{
	unsigned char* b = byteptr;

	SERVICE_LIST_DESC * desc_servicelist = (SERVICE_LIST_DESC *)malloc(sizeof(SERVICE_LIST_DESC));
	memset(desc_servicelist, 0, sizeof(SERVICE_LIST_DESC));
	
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

	return (DESCRIPTOR_COMMON *)desc_servicelist;
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

void free_service_list_desc(DESCRIPTOR_COMMON *head)
{
	SERVICE_LIST_DESC *phead = (SERVICE_LIST_DESC *)head;
        
    free_desc(phead->next_desc);
	phead->next_desc = NULL;
	
    free(phead);
    phead = NULL;
	head = NULL;
}



void show_service_list_descriptor(DESCRIPTOR_COMMON *ptmp)
{
    SERVICE_LIST_DESC * tmp = (SERVICE_LIST_DESC*)ptmp;
	uprintf("\t\tdescriptor_tag     :   0x%x\n",tmp->descriptor_tag);
	uprintf("\t\tdescriptor_name    :   service_list_descriptor\n");
	uprintf("\t\tdescriptor_length  :   0x%x\n\n",tmp->descriptor_length);
}
