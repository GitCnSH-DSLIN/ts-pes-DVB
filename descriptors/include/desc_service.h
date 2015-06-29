/*********************************************************************
*
* Filename      :   desc_service.h
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/

#ifndef _DESC_SERVICE_H_
#define _DESC_SERVICE_H_

#include <sdt.h>

typedef struct service_desc{
	unsigned char descriptor_tag;
	unsigned char descriptor_length;
	struct sdt_descriptor_common * next_desc;//add by myself
	unsigned char service_type;
	unsigned char service_provider_name_length;
	char* provider_name;
	unsigned char service_name_length;
	char* service_name;
}SERVICE_DESC,* P_SERVICE_DESC;

#define SERV_DESC_TAG(b)					(b[0])
#define SERV_DESC_LENGTH(b)					(b[1])
#define SERV_DESC_SERV_TYPE(b)				(b[2])
#define SERV_DESC_SERV_PRO_NAME_LENGTH(b)	(b[3])
#define SERV_DESC_SERV_NAME_LENGTH(b)		({	\
											int n = SERV_DESC_SERV_PRO_NAME_LENGTH(b); \
											int base = n + 4; \
											b[base]; \
											})

SDT_DESCRIPTOR_COMMON * decode_service_desc(unsigned char* byteptr, int this_section_length);

void free_service_desc(SDT_DESCRIPTOR_COMMON * head);

void show_service_descriptor(SDT_DESCRIPTOR_COMMON *ptmp);

#endif /* _DESC_SERVICE_H_ */
