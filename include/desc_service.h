/*
 * desc_service.h
 *
 *  Created on: Jun 24, 2011
 *      Author: orion
 */

#ifndef _DESC_SERVICE_H_
#define _DESC_SERVICE_H_

typedef struct service_desc{
	unsigned char descriptor_tag;
	unsigned char descriptor_length;
	unsigned char service_type;
	unsigned char service_provider_name_length;
	char* provider_name;
	unsigned char service_name_length;
	char* service_name;
	void* next_desc;
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

int decode_service_desc(unsigned char* byteptr, int this_section_length,
        SERVICE_DESC * desc_service);
void free_service_desc(SERVICE_DESC * head);

#endif /* _DESC_SERVICE_H_ */
