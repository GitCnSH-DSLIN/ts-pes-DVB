/*
 * desc_network_name.h
 *
 *  Created on: Jun 23, 2011
 *      Author: orion
 */

#ifndef _DESC_NETWORK_NAME_H_
#define _DESC_NETWORK_NAME_H_

typedef struct network_name_desc{
		unsigned char descriptor_tag;
		unsigned char descriptor_length;
		char * network_name;
		void* next_desc;
}NETWORK_NAME_DESC, *P_NETWORK_NAME_DESC;


#define NETWORKNAME_DESC_TAG(b)			(b[0])
#define NETWORKNAME_DESC_LENGTH(b)		(b[1])

int decode_network_name_desc(unsigned char* byteptr, int this_section_length,
        NETWORK_NAME_DESC * desc_network_name);
void free_network_name_desc(NETWORK_NAME_DESC * head);

#endif /* _DESC_NETWORK_NAME_H_ */
