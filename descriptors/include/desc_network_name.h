/*********************************************************************
*
* Filename      :   desc_network_name.h
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/

#ifndef _DESC_NETWORK_NAME_H_
#define _DESC_NETWORK_NAME_H_
#include <sdt.h>

typedef struct network_name_desc{
		unsigned char descriptor_tag;
		unsigned char descriptor_length;
		struct sdt_descriptor_common * next_desc;
		char * network_name;
}NETWORK_NAME_DESC, *P_NETWORK_NAME_DESC;


#define NETWORKNAME_DESC_TAG(b)			(b[0])
#define NETWORKNAME_DESC_LENGTH(b)		(b[1])

SDT_DESCRIPTOR_COMMON * decode_network_name_desc(unsigned char * byteptr, int this_section_length);
void free_network_name_desc(SDT_DESCRIPTOR_COMMON* head);
void show_network_name_descriptor(SDT_DESCRIPTOR_COMMON *ptmp);

#endif /* _DESC_NETWORK_NAME_H_ */
