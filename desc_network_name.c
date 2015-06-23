/*
 * desc_network_name.c
 *
 *  Created on: Jun 24, 2011
 *      Author: orion
 */
#include <stdio.h>
#include <desc_network_name.h>
#include <print_debug.h>

int decode_network_name_desc(unsigned char * byteptr, int this_section_length,
        NETWORK_NAME_DESC* desc_network_name){
	unsigned char* b = byteptr;

	desc_network_name->descriptor_tag = NETWORKNAME_DESC_TAG(b);
	desc_network_name->descriptor_length = NETWORKNAME_DESC_LENGTH(b);

	desc_network_name->network_name = (char*)calloc(desc_network_name->descriptor_length + 1, sizeof(char));
	strncpy(desc_network_name->network_name, (char*)&b[2], desc_network_name->descriptor_length);

	uprintf("descriptor_tag:0x%x:\n",desc_network_name->descriptor_tag);
	uprintf("descriptor_length:0x%x\n",desc_network_name->descriptor_length);
	uprintf("network_name:%s\n",desc_network_name->network_name);

	return (desc_network_name->descriptor_length + 2);
}

void free_network_name_desc(NETWORK_NAME_DESC* head){
	free(head->network_name);
	head->network_name = NULL;
	free_desc(head->next_desc);
	head->next_desc = NULL;
	free(head);
	head = NULL;
}
