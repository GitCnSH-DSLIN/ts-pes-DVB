/*********************************************************************
*
* Filename      :   desc_cable_delivery_system.h
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/

#ifndef _DESC_CABLE_DELIVERY_SYSTEM_H_
#define _DESC_CABLE_DELIVERY_SYSTEM_H_
#include <sdt.h>

typedef struct cable_delivery_system_desc{
    unsigned char descriptor_tag;
    unsigned char descriptor_length;
    struct sdt_descriptor_common * next_desc; //add by myself
    unsigned int  frequency;
    unsigned reserved_future_use : 12;
    unsigned FEC_outer : 4;
    unsigned char modulation;
    unsigned symbol_rate : 28;
    unsigned FEC_inner : 4;
}CABLE_DELIVERY_SYSTEM_DESC, *P_CALBE_DELIVERY_SYSTEM_DESC;

#define CABLE_DELIVERY_SYSTEM_DESC_TAG(b)               (b[0])
#define CABLE_DELIVERY_SYSTEM_DESC_LENGTH(b)            (b[1])
#define CABLE_DELIVERY_SYSTEM_DESC_FREQUENCY(b)         ((b[2] << 24) | (b[3] << 16) | \
                                                            (b[4] << 8) | b[5])
#define CABLE_DELIVERY_SYSTEM_DESC_FEC_OUTER(b)         (b[7] & 0x0f)
#define CABLE_DELIVERY_SYSTEM_DESC_MODULATION(b)        (b[8])
#define CABLE_DELIVERY_SYSTEM_DESC_SYMBOL_RATE(b)       ((b[9] << 20) | (b[10] << 12) | \
                                                            (b[11] << 4) | (b[12] & 0xf0) >> 4)
#define CABLE_DELIVERY_SYSTEM_DESC_FEC_INNER(b)         (b[12] & 0x0f)

SDT_DESCRIPTOR_COMMON * decode_cable_delivery_system_desc(unsigned char* byteptr, int this_section_length);

void free_cable_delivery_system_desc(SDT_DESCRIPTOR_COMMON * head);

void show_cable_delivery_system_descriptor(SDT_DESCRIPTOR_COMMON *ptmp);

#endif /* _DESC_SERVICE_H_ */

