/*********************************************************************
*
* Filename      :   desc_service_list.c
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#ifndef _DESC_SERVICE_LIST_H_
#define _DESC_SERVICE_LIST_H_
#include <sdt.h>

typedef struct service_list_desc_item{
	unsigned short service_id;
	unsigned char service_type;
}SERIVCE_LIST_DESC_ITEM, *P_SERIVCE_LIST_DESC_ITEM;


typedef struct service_list_desc{
	unsigned char descriptor_tag;
	unsigned char descriptor_length;
	void* next_desc;
}SERVICE_LIST_DESC,*P_SERVICE_LIST_DESC;


#define SLD_DESC_TAG(b)				(b[0])
#define SLD_DESC_LEN(b)				(b[1])

#define SLD_DESC_ITEM_ID(b)			((b[0] << 8) | b[1])
#define SLD_DESC_ITEM_TYPE(b)		(b[2])

SDT_DESCRIPTOR_COMMON * decode_service_list_desc(unsigned char* byteptr, int this_section_length);

int decode_servicelist_item(unsigned char* byteptr, int this_section_length,
        SERIVCE_LIST_DESC_ITEM* item_servicelist);

void show_service_list_descriptor(SDT_DESCRIPTOR_COMMON *ptmp);

void free_service_list_desc(SDT_DESCRIPTOR_COMMON *head);
#endif /* _DESC_SERVICE_LIST_H_ */
