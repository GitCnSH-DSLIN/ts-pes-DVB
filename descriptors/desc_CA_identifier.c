#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <print_debug.h>
#include <desc_CA_identifier.h>

int decode_CA_identifier_desc(unsigned char* byteptr, int this_section_length,
        CA_IDENTIFIER_DESC * caIdentifierDesc)
{
	unsigned char* b = byteptr;

	caIdentifierDesc->descriptor_tag = byteptr[0];
	caIdentifierDesc->descriptor_length = byteptr[1];

	caIdentifierDesc->CA_system_id = (unsigned short*)malloc(caIdentifierDesc->descriptor_length);
	strncpy(caIdentifierDesc->CA_system_id, (unsigned short*)&b[2], caIdentifierDesc->descriptor_length);

	return (caIdentifierDesc->descriptor_length + 2);
}

void free_CA_identifier_desc(CA_IDENTIFIER_DESC * pCaIdentifierDesc)
{
    free(pCaIdentifierDesc->CA_system_id); 
    pCaIdentifierDesc->CA_system_id = NULL;

    //recursion itself
    if (NULL != pCaIdentifierDesc->next_desc)
    {
        free_desc(pCaIdentifierDesc->next_desc);
        pCaIdentifierDesc->next_desc = NULL;
    }

    free(pCaIdentifierDesc);
    pCaIdentifierDesc = NULL;
}

