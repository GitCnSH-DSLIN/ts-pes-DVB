/*********************************************************************
*
* Filename      :   sdt_descriptor.c
* Description   :   fundamental operation and framework of descriptors of sdt service.
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <print_debug.h>
#include <sdt.h>
#include <descriptors.h>

SDT_DESCRIPTOR_COMMON * decode_default(unsigned char * byteptr, int this_section_length)
{
    unsigned char * b = byteptr;

    SDT_DESCRIPTOR_COMMON *desc_default = (SDT_DESCRIPTOR_COMMON *)malloc(sizeof(SDT_DESCRIPTOR_COMMON));
    memset(desc_default, '\0', sizeof(SDT_DESCRIPTOR_COMMON));
    desc_default->descriptor_tag = b[0];
    desc_default->descriptor_length = b[1];
    
    return desc_default;
}

SDT_DESCRIPTOR_COMMON * (*do_decode_desc[64])(unsigned char * byteptr, int this_section_length) = {
    /*decode for each descriptors*/
    decode_network_name_desc,                       //0x40
    decode_service_list_desc,                       //0x41
    decode_default,                                 //0x42
    decode_default,                                       //0x43
    decode_cable_delivery_system_desc,              //0x44
    decode_default,                                       //0x45
    decode_default,                                       //0x46
    decode_default,                                       //0x47
    decode_service_desc,                            //0x48
    decode_default,                                       //0x49
    decode_default,                                       //0x4a
    decode_default,                                       //0x4b
    decode_default,                                       //0x4c
    decode_short_event_desc,                        //0x4d
    decode_default,                                       //0x4e
    decode_default,                                       //0x4f
    decode_component_desc,                          //0x50
    decode_default,                                       //0x51
    decode_default,                                       //0x52
    decode_CA_identifier_desc,                      //0x53
    decode_default,                                       //0x54
    decode_default,                                       //0x55
    decode_default,                                       //0x56
    decode_default,                                       //0x57
    decode_local_time_offset_desc,                  //0x58
    decode_default,                                       //0x59
    decode_default,                                       //0x5a
    decode_default,                                       //0x5b
    decode_default,                                       //0x5c
    decode_default,                                       //0x5d
    decode_default,                                       //0x5e
    decode_default,                                       //0x5f
    decode_default,                                       //0x60
    decode_default,                                       //0x61
    decode_default,                                       //0x62
    decode_default,                                       //0x63
    decode_default,                                       //0x64
    decode_default,                                       //0x65
    decode_default,                                       //0x66
    decode_default,                                       //0x67
    decode_default,                                       //0x68
    decode_default,                                       //0x69
    decode_default,                                       //0x6a
    decode_default,                                       //0x6b
    decode_default,                                       //0x6c
    decode_default,                                       //0x6d
    decode_default,                                       //0x6e
    decode_default,                                       //0x6f
    decode_default,                                       //0x70
    decode_default,                                       //0x71
    decode_default,                                       //0x72
    decode_default,                                       //0x73
    decode_default,                                       //0x74
    decode_default,                                       //0x75
    decode_default,                                       //0x76
    decode_default,                                       //0x77
    decode_default,                                       //0x78
    decode_default,                                       //0x79
    decode_default,                                       //0x7a
    decode_default,                                       //0x7b
    decode_default,                                       //0x7c
    decode_default,                                       //0x7d
    decode_default,                                       //0x7e
    decode_default,                                       //0x7f
};




SDT_DESCRIPTOR_COMMON * decode_desc(unsigned char * byteptr, int this_section_length)
{
    unsigned char descriptor_tag = byteptr[0];
    unsigned char * b = byteptr;
    int l = this_section_length;
    unsigned int desc_len = 0;
    SDT_DESCRIPTOR_COMMON * this_descriptor = NULL;

    if(descriptor_tag > 0x7F && descriptor_tag < 0xFF )
    {
        this_descriptor = decode_default(b,l);
    
    }
    else if(descriptor_tag >= 0x40 && descriptor_tag <= 0x7F)
    {
        this_descriptor = (*do_decode_desc[descriptor_tag - 0x40])(b,l);
    }
    else
    {
        printf("Bad descriptor_tag: 0x%x\n",descriptor_tag);
        printf("Prepare to Exit\n");
        exit(-1);
    
    }
    
    desc_len = this_descriptor->descriptor_length + 2;

    b += desc_len;
    l -= desc_len;

    if(l > 0)
        this_descriptor->next_desc = decode_desc(b, l);
    return this_descriptor;
}


void free_desc_default(SDT_DESCRIPTOR_COMMON* head)
{
    //uprintf("prepare to free : 0x%x\n",head->descriptor_tag);
    free_desc(head->next_desc);
    head->next_desc = NULL;

    free(head);
    head = NULL;
}


void (*do_free_descriptors[64])(SDT_DESCRIPTOR_COMMON *ptmp) = {
    /*free descriptor for each different descriptors*/
    free_network_name_desc,                 //0x40
    free_service_list_desc,                 //0x41
    free_desc_default,                                      //0x42
    free_desc_default,                                      //0x43
    free_cable_delivery_system_desc,        //0x44
    free_desc_default,                                      //0x45
    free_desc_default,                                      //0x46
    free_desc_default,                                      //0x47
    free_service_desc,                      //0x48
    free_desc_default,                                      //0x49
    free_desc_default,                                      //0x4a
    free_desc_default,                                      //0x4b
    free_desc_default,                                      //0x4c
    free_short_event_desc,                  //0x4d
    free_desc_default,                                      //0x4e
    free_desc_default,                                      //0x4f
    free_component_desc,                                    //0x50
    free_desc_default,                                      //0x51
    free_desc_default,                                      //0x52
    free_CA_identifier_desc,                //0x53
    free_desc_default,                                       //0x54
    free_desc_default,                                       //0x55
    free_desc_default,                                       //0x56
    free_desc_default,                                       //0x57
    free_local_time_offset_desc,            //0x58
    free_desc_default,                                       //0x59
    free_desc_default,                                       //0x5a
    free_desc_default,                                       //0x5b
    free_desc_default,                                       //0x5c
    free_desc_default,                                       //0x5d
    free_desc_default,                                       //0x5e
    free_desc_default,                                       //0x5f
    free_desc_default,                                       //0x60
    free_desc_default,                                       //0x61
    free_desc_default,                                       //0x62
    free_desc_default,                                       //0x63
    free_desc_default,                                       //0x64
    free_desc_default,                                       //0x65
    free_desc_default,                                       //0x66
    free_desc_default,                                       //0x67
    free_desc_default,                                       //0x68
    free_desc_default,                                       //0x69
    free_desc_default,                                       //0x6a
    free_desc_default,                                       //0x6b
    free_desc_default,                                       //0x6c
    free_desc_default,                                       //0x6d
    free_desc_default,                                       //0x6e
    free_desc_default,                                       //0x6f
    free_desc_default,                                       //0x70
    free_desc_default,                                       //0x71
    free_desc_default,                                       //0x72
    free_desc_default,                                       //0x73
    free_desc_default,                                       //0x74
    free_desc_default,                                       //0x75
    free_desc_default,                                       //0x76
    free_desc_default,                                       //0x77
    free_desc_default,                                       //0x78
    free_desc_default,                                       //0x79
    free_desc_default,                                       //0x7a
    free_desc_default,                                       //0x7b
    free_desc_default,                                       //0x7c
    free_desc_default,                                       //0x7d
    free_desc_default,                                       //0x7e
    free_desc_default,                                       //0x7f
};


void free_desc(SDT_DESCRIPTOR_COMMON *phead)
{
    if (NULL == phead)
    {
        //uprintf("Free completed\n");
        return;
    }
    
    unsigned char descriptor_tag = phead->descriptor_tag;

    if(descriptor_tag > 0x7F && descriptor_tag < 0xFF )
    {
        free_desc_default(phead);
    }
    else if(descriptor_tag >= 0x40 && descriptor_tag <= 0x7F)
    {
        (*do_free_descriptors[descriptor_tag - 0x40])(phead);
    }
    else
    {
        printf("Bad descriptor_tag: 0x%x\n",descriptor_tag);
        printf("Prepare to Exit\n");
        exit(-1);
    }
}



void show_desc_default(SDT_DESCRIPTOR_COMMON *ptmp)
{
    SDT_DESCRIPTOR_COMMON * tmp = ptmp;
	uprintf("\t\tdescriptor_tag     :   0x%x\n",tmp->descriptor_tag);
	uprintf("\t\tdescriptor_name    :   user defined descriptor(unknown)\n");
	uprintf("\t\tdescriptor_length  :   0x%x\n\n",tmp->descriptor_length);
}


void (*do_show_descriptors_info[64])(SDT_DESCRIPTOR_COMMON *ptmp) = {
    /*show info for each descriptors*/
    show_network_name_descriptor,               //0x40
    show_service_list_descriptor,               //0x41
    show_desc_default,                                       //0x42
    show_desc_default,                                       //0x43
    show_cable_delivery_system_descriptor,      //0x44
    show_desc_default,                                       //0x45
    show_desc_default,                                       //0x46
    show_desc_default,                                       //0x47
    show_service_descriptor,                    //0x48
    show_desc_default,                                       //0x49
    show_desc_default,                                       //0x4a
    show_desc_default,                                       //0x4b
    show_desc_default,                                       //0x4c
    show_short_event_descriptor,                //0x4d
    show_desc_default,                                       //0x4e
    show_desc_default,                                       //0x4f
    show_component_descriptor,                  //0x50
    show_desc_default,                                       //0x51
    show_desc_default,                                       //0x52
    show_CA_identifier_descriptor,                          //0x53
    show_desc_default,                                       //0x54
    show_desc_default,                                       //0x55
    show_desc_default,                                       //0x56
    show_desc_default,                                       //0x57
    show_local_time_offset_descriptor,          //0x58
    show_desc_default,                                       //0x59
    show_desc_default,                                       //0x5a
    show_desc_default,                                       //0x5b
    show_desc_default,                                       //0x5c
    show_desc_default,                                       //0x5d
    show_desc_default,                                       //0x5e
    show_desc_default,                                       //0x5f
    show_desc_default,                                       //0x60
    show_desc_default,                                       //0x61
    show_desc_default,                                       //0x62
    show_desc_default,                                       //0x63
    show_desc_default,                                       //0x64
    show_desc_default,                                       //0x65
    show_desc_default,                                       //0x66
    show_desc_default,                                       //0x67
    show_desc_default,                                       //0x68
    show_desc_default,                                       //0x69
    show_desc_default,                                       //0x6a
    show_desc_default,                                       //0x6b
    show_desc_default,                                       //0x6c
    show_desc_default,                                       //0x6d
    show_desc_default,                                       //0x6e
    show_desc_default,                                       //0x6f
    show_desc_default,                                       //0x70
    show_desc_default,                                       //0x71
    show_desc_default,                                       //0x72
    show_desc_default,                                       //0x73
    show_desc_default,                                       //0x74
    show_desc_default,                                       //0x75
    show_desc_default,                                       //0x76
    show_desc_default,                                       //0x77
    show_desc_default,                                       //0x78
    show_desc_default,                                       //0x79
    show_desc_default,                                       //0x7a
    show_desc_default,                                       //0x7b
    show_desc_default,                                       //0x7c
    show_desc_default,                                       //0x7d
    show_desc_default,                                       //0x7e
    show_desc_default,                                       //0x7f
};


void show_sdt_service_descriptors_info(SDT_SERVICE * sdtService)
{
	SDT_DESCRIPTOR_COMMON *HeaderDesc = (SDT_DESCRIPTOR_COMMON *)sdtService->first_desc;
    SDT_DESCRIPTOR_COMMON *ptmp = HeaderDesc;
    unsigned char descriptor_tag;
    unsigned char descriptor_length;

    while (NULL != ptmp)
    {
        descriptor_tag = ptmp->descriptor_tag;
        descriptor_length = ptmp->descriptor_length;
        
        if(descriptor_tag > 0x7F && descriptor_tag < 0xFF )
        {
            show_desc_default(ptmp);
        }
        else if(descriptor_tag >= 0x40 && descriptor_tag <= 0x7F)
        {
            (*do_show_descriptors_info[descriptor_tag - 0x40])(ptmp);
        }
        else
        {
            printf("Bad descriptor_tag: 0x%x\n",descriptor_tag);
            printf("Prepare to Exit\n");
            exit(-1);
    
        }
        
        ptmp = (SDT_DESCRIPTOR_COMMON *)ptmp->next_desc;
    }
}


