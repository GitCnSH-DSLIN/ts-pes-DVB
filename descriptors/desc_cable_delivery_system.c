/*********************************************************************
*
* Filename      :   desc_cable_delivery_system.c
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sdt.h>
#include <sdt_descriptor.h>
#include <desc_cable_delivery_system.h>
#include <print_debug.h>

SDT_DESCRIPTOR_COMMON * decode_cable_delivery_system_desc(unsigned char* byteptr, int this_section_length)
{
	unsigned char* b = byteptr;

	CABLE_DELIVERY_SYSTEM_DESC * desc_cable_delivery_system = (CABLE_DELIVERY_SYSTEM_DESC *)malloc(sizeof(CABLE_DELIVERY_SYSTEM_DESC ));
	memset(desc_cable_delivery_system, 0, sizeof(CABLE_DELIVERY_SYSTEM_DESC ));
	
    desc_cable_delivery_system->descriptor_tag          = CABLE_DELIVERY_SYSTEM_DESC_TAG(b);
	desc_cable_delivery_system->descriptor_length       = CABLE_DELIVERY_SYSTEM_DESC_LENGTH(b);
	desc_cable_delivery_system->frequency               = CABLE_DELIVERY_SYSTEM_DESC_FREQUENCY(b);
	desc_cable_delivery_system->FEC_outer               = CABLE_DELIVERY_SYSTEM_DESC_FEC_OUTER(b);
	desc_cable_delivery_system->modulation              = CABLE_DELIVERY_SYSTEM_DESC_MODULATION(b);
	desc_cable_delivery_system->symbol_rate             = CABLE_DELIVERY_SYSTEM_DESC_SYMBOL_RATE(b);
	desc_cable_delivery_system->FEC_inner               = CABLE_DELIVERY_SYSTEM_DESC_FEC_INNER(b);

#if 0
 	uprintf("desc_cable_delivery_system->descriptor_tag     :   0x%x\n",desc_cable_delivery_system->descriptor_tag);
 	uprintf("desc_cable_delivery_system->descriptor_length  :   0x%x\n",desc_cable_delivery_system->descriptor_length);
 	uprintf("desc_cable_delivery_system->frquency           :   0x%x\n",desc_cable_delivery_system->frequency);
 	uprintf("desc_cable_delivery_system->FEC_outer          :   0x%x\n",desc_cable_delivery_system->FEC_outer);
 	uprintf("desc_cable_delivery_system->modulation         :   0x%x\n",desc_cable_delivery_system->modulation);
 	uprintf("desc_cable_delivery_system->symbol_rate        :   0x%x\n",desc_cable_delivery_system->symbol_rate);
 	uprintf("desc_cable_delivery_system->FEC_inner          :   0x%x\n",desc_cable_delivery_system->FEC_inner);
#endif
	return (SDT_DESCRIPTOR_COMMON *)desc_cable_delivery_system;
}


void free_cable_delivery_system_desc(SDT_DESCRIPTOR_COMMON * head)
{
//    uprintf("prepare to free : 0x%x\n",head->descriptor_tag);
	CABLE_DELIVERY_SYSTEM_DESC * phead = (CABLE_DELIVERY_SYSTEM_DESC *)head;
	
    free_desc(phead->next_desc);
	phead->next_desc = NULL;
	
    free(phead);
    phead = NULL;
	head = NULL;
}

void show_cable_delivery_system_descriptor(SDT_DESCRIPTOR_COMMON *ptmp)
{
    CABLE_DELIVERY_SYSTEM_DESC * tmp = (CABLE_DELIVERY_SYSTEM_DESC*)ptmp;
 	uprintf("\t\tdescriptor_tag     :   0x%x\n",tmp->descriptor_tag);
	uprintf("\t\tdescriptor_name    :   Calbe delivery system descriptor\n");
 	uprintf("\t\tdescriptor_length  :   0x%x\n",tmp->descriptor_length);
 	uprintf("\t\tfrquency           :   0x%x\n",tmp->frequency);
 	uprintf("\t\tFEC_outer          :   0x%x\n",tmp->FEC_outer);
 	uprintf("\t\tmodulation         :   0x%x\n",tmp->modulation);
 	uprintf("\t\tsymbol_rate        :   0x%x\n",tmp->symbol_rate);
 	uprintf("\t\tFEC_inner          :   0x%x\n",tmp->FEC_inner);
}
