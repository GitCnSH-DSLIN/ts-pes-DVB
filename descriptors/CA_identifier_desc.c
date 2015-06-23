
int decode_CA_identifier_desc(byte* byteptr, int this_section_length,
        CA_identifier_descriptor * caIdentifierDesc)
{
	byte* b = byteptr;

	caIdentifierDesc->descriptor_tag = byteptr[0];
	caIdentifierDesc->descriptor_length = byteptr[1];

	caIdentifierDesc->CA_system_id = (unsigned short*)malloc(caIdentifierDesc->descriptor_length);
	strncpy(caIdentifierDesc->CA_system_id, (unsigned short*)&b[2], caIdentifierDesc->descriptor_length);

	return (caIdentifierDesc->descriptor_length + 2);
}

void free_CA_identifier_desc(caIdentifierDesc * pCaIdentifierDesc)
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

