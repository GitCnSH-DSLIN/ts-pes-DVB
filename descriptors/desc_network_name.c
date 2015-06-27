/*
 * desc_network_name.c
 *
 *  Created on: Jun 24, 2011
 *      Author: orion
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <desc_network_name.h>
#include <print_debug.h>

SDT_DESCRIPTOR_COMMON * decode_network_name_desc(unsigned char * byteptr, int this_section_length)
{
	unsigned char* b = byteptr;

	NETWORK_NAME_DESC * desc_network_name = (NETWORK_NAME_DESC *)malloc(sizeof(NETWORK_NAME_DESC));
	memset(desc_network_name, 0, sizeof(NETWORK_NAME_DESC));
    
	desc_network_name->descriptor_tag = NETWORKNAME_DESC_TAG(b);
	desc_network_name->descriptor_length = NETWORKNAME_DESC_LENGTH(b);

	desc_network_name->network_name = (char*)calloc(desc_network_name->descriptor_length + 1, sizeof(char));
	strncpy(desc_network_name->network_name, (char*)&b[2], desc_network_name->descriptor_length);
	
    return (SDT_DESCRIPTOR_COMMON *)desc_network_name; 
}

void free_network_name_desc(NETWORK_NAME_DESC* head){
	free(head->network_name);
	head->network_name = NULL;
	free_desc(head->next_desc);
	head->next_desc = NULL;
	free(head);
	head = NULL;
}

void show_network_name_descriptor(SDT_DESCRIPTOR_COMMON *ptmp)
{
    NETWORK_NAME_DESC * tmp = (NETWORK_NAME_DESC*)ptmp;
	uprintf("\t\tdescriptor_tag     :   0x%x\n",tmp->descriptor_tag);
	uprintf("\t\tdescriptor_name    :   network_name_descriptor\n");
	uprintf("\t\tdescriptor_length  :   0x%x\n",tmp->descriptor_length);
	uprintf("\t\tnetwork_name       :   %s\n",tmp->network_name);
}




