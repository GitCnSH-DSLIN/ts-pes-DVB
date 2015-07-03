/*********************************************************************
*
* Filename      :   desc_local_time_offset.c
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sdt.h>
#include <descriptor_common.h>
#include <desc_local_time_offset.h>
#include <print_debug.h>

LOCAL_TIME_OFFSET_ITEM * insert_localtime_offset_item_node(LOCAL_TIME_OFFSET_ITEM * Header, LOCAL_TIME_OFFSET_ITEM * node)
{
    LOCAL_TIME_OFFSET_ITEM *ptmp = Header;
    LOCAL_TIME_OFFSET_ITEM *pcur = NULL;
    while (NULL != ptmp)
    {
        pcur = ptmp;
        ptmp = ptmp->next_item;
    }

    pcur->next_item = node;
    
    return Header;
}

void decode_localtime_offset_item(unsigned char* byteptr, LOCAL_TIME_OFFSET_ITEM * item_localtime)
{
	unsigned char* b = byteptr;

	item_localtime->country_code                =   LOCAL_TIME_OFFSET_ITEM_COUNTRY_CODE(b);
	item_localtime->country_regin_id            =   LOCAL_TIME_OFFSET_ITEM_COUNTRY_REGIN_ID(b);
	item_localtime->local_time_offset_polarity  =   LOCAL_TIME_OFFSET_ITEM_POLARITY(b);
	item_localtime->local_time_offset           =   LOCAL_TIME_OFFSET_ITEM_TIME_OFFSET(b);
	item_localtime->time_of_change              =   LOCAL_TIME_OFFSET_ITEM_TIME_OF_CHANGE(b);
	item_localtime->next_time_offset            =   LOCAL_TIME_OFFSET_ITEM_NEXT_TIME_OFFSET(b);
	
}


DESCRIPTOR_COMMON * decode_local_time_offset_desc(unsigned char* byteptr, int this_section_length)
{
	unsigned char* b = byteptr;

	LOCAL_TIME_OFFSET_DESC * desc_localtime_offset = (LOCAL_TIME_OFFSET_DESC *)malloc(sizeof(LOCAL_TIME_OFFSET_DESC));
	memset(desc_localtime_offset, 0, sizeof(LOCAL_TIME_OFFSET_DESC));
	
    desc_localtime_offset->descriptor_tag = LOCAL_TIME_OFFSET_DESC_TAG(b);
	desc_localtime_offset->descriptor_length = LOCAL_TIME_OFFSET_DESC_LENGTH(b);
	
    int len = desc_localtime_offset->descriptor_length;
	unsigned char* item_start = &b[2];

    int firstFlag = 1;

	while(len > 0)
    {
		LOCAL_TIME_OFFSET_ITEM * localtime_item_node = (LOCAL_TIME_OFFSET_ITEM *)malloc(sizeof(LOCAL_TIME_OFFSET_ITEM));
		memset(localtime_item_node, 0, sizeof(LOCAL_TIME_OFFSET_ITEM));

        decode_localtime_offset_item(item_start, localtime_item_node);
		if(1 == firstFlag)
        {
            desc_localtime_offset->first_item = localtime_item_node;
            firstFlag = 0;
        }
        else
        {
            insert_localtime_offset_item_node(desc_localtime_offset->first_item, localtime_item_node);
        }

        item_start += 13;
		len -= 13;
	}

	return (DESCRIPTOR_COMMON *)desc_localtime_offset;
}



void free_localtime_offset_item(LOCAL_TIME_OFFSET_ITEM *phead)
{

    LOCAL_TIME_OFFSET_ITEM *p_item_header = phead;
    LOCAL_TIME_OFFSET_ITEM *p_tmp;

    while(NULL != p_item_header)
    {
        p_tmp           = p_item_header;
        p_item_header   = p_item_header->next_item;

        free(p_tmp);
    }
}


void free_local_time_offset_desc(DESCRIPTOR_COMMON *head)
{
	LOCAL_TIME_OFFSET_DESC *phead = (LOCAL_TIME_OFFSET_DESC *)head;
    
    free_localtime_offset_item(phead->first_item);
    phead->first_item = NULL;

    free_desc(phead->next_desc);
	phead->next_desc = NULL;
	
    free(phead);
    phead = NULL;
	head = NULL;
}


void show_localtime_offset_item(LOCAL_TIME_OFFSET_ITEM *head)
{
    LOCAL_TIME_OFFSET_ITEM *ptmp = head;

    while(NULL != ptmp)
    {
        uprintf("\t\t\tcountry_code                 :   0x%x\n",ptmp->country_code);
        uprintf("\t\t\tcountry_regin_id             :   0x%x\n",ptmp->country_regin_id);
        uprintf("\t\t\tlocal_time_offset_polarity   :   %d\n",ptmp->local_time_offset_polarity);
        uprintf("\t\t\tlocal_time_offset            :   0x%x\n",ptmp->local_time_offset);
        uprintf("\t\t\ttime_of_change               :   0x%lx\n",ptmp->time_of_change);
        uprintf("\t\t\tnext_time_offset             :   0x%x\n\n",ptmp->next_time_offset);
    }
}


void show_local_time_offset_descriptor(DESCRIPTOR_COMMON *ptmp)
{
    LOCAL_TIME_OFFSET_DESC * tmp = (LOCAL_TIME_OFFSET_DESC *)ptmp;
	uprintf("\t\tdescriptor_tag     :   0x%x\n",tmp->descriptor_tag);
	uprintf("\t\tdescriptor_name    :   local time offset descriptor\n");
	uprintf("\t\tdescriptor_length  :   0x%x\n",tmp->descriptor_length);

    show_localtime_offset_item(tmp->first_item);
}
