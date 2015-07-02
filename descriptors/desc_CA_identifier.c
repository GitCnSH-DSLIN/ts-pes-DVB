/*********************************************************************
*
* Filename      :   desc_CA_identifier.c
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sdt.h>
#include <sdt_descriptor.h>
#include <print_debug.h>
#include <desc_CA_identifier.h>

SDT_DESCRIPTOR_COMMON * decode_CA_identifier_desc(unsigned char* byteptr, int this_section_length)
{
	unsigned char* b = byteptr;
	
    CA_IDENTIFIER_DESC *caIdentifierDesc = (CA_IDENTIFIER_DESC *)malloc(sizeof(CA_IDENTIFIER_DESC));
	memset(caIdentifierDesc, 0, sizeof(CA_IDENTIFIER_DESC));

	caIdentifierDesc->descriptor_tag = byteptr[0];
	caIdentifierDesc->descriptor_length = byteptr[1];

	caIdentifierDesc->CA_system_id = (unsigned short*)calloc((caIdentifierDesc->descriptor_length + 2)/2, sizeof(unsigned short));
    unsigned char *pCA = (unsigned char *)caIdentifierDesc->CA_system_id;

	memcpy(pCA, &b[2], caIdentifierDesc->descriptor_length);

	return  (SDT_DESCRIPTOR_COMMON *)caIdentifierDesc;
}


void free_CA_identifier_desc(SDT_DESCRIPTOR_COMMON *head)
{
    CA_IDENTIFIER_DESC * phead = (CA_IDENTIFIER_DESC *)head;

    free(phead->CA_system_id); 
    phead->CA_system_id = NULL;

    //recursion itself
    free_desc(phead->next_desc);
    phead->next_desc = NULL;

    free(phead);
    phead = NULL;
    head  = NULL;
}


void show_CA_identifier_descriptor(SDT_DESCRIPTOR_COMMON *ptmp)
{
    CA_IDENTIFIER_DESC * tmp = (CA_IDENTIFIER_DESC *)ptmp;
	uprintf("\t\tdescriptor_tag     :   0x%x\n",tmp->descriptor_tag);
	uprintf("\t\tdescriptor_name    :   CA_identifier_descriptor\n");
	uprintf("\t\tdescriptor_length  :   0x%x\n",tmp->descriptor_length);

    int index = 0;
    int length = (tmp->descriptor_length + 1)/2;

    unsigned short * p_index = tmp->CA_system_id;

    for(; index < length; index++)
    {
        uprintf("%04X ",p_index[index]);
        if((index + 1)%16 == 0)
            uprintf("\n");
    }
    uprintf("\n");
}
