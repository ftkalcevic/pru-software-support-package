#ifndef _DCC_H_
#define _DCC_H_

#define MAX_DCC_PACKET_LEN  6
#define MAX_DECODERS        64
#define MAX_QUEUE_SIZE      16

enum ERecordType
{
    Empty,              // Empty Slot
    Broadcast,          
    MultiFunction,      // locomotive decoder
    Accessory           // accessory decoder
};

struct SDCCRecord
{
    ERecordType type;
    uint32_t address;
    uint32_t len;
    uint8_t msg[MAX_DCC_PACKET_LEN];
    uint32_t resends;
};

struct SDCCInterface
{
    uint32_t trackEnabled;
    uint32_t maxRecords;
    SDCCRecord records[MAX_DECODERS];
};

struct SDCCMessageQueue
{
    uint32_t head;
    uint32_t tail;
    SDCCRecord queue[MAX_QUEUE_SIZE];
};

#endif


