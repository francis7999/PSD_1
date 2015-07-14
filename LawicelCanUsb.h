#pragma once

#include "lawicel_can.h"

// data types
typedef int LW_ERR_CODE;

// CAN baud rates
enum CanBaud
{
    BAUD_10_KBPS,
    BAUD_20_KBPS,
    BAUD_50_KBPS,
    BAUD_100_KBPS,
    BAUD_250_KBPS,
    BAUD_500_KBPS,
    BAUD_800_KBPS,
    BAUD_1000_KBPS,
};

// Message Object
struct CanMsgObject
{
    unsigned int  id;
    unsigned char msg[8];
    unsigned char size;
    bool ext_id;
    bool rtr;

    unsigned long timestamp;
};

// Lawicel CANUSB dongle driver
class CLawicelCanUsb
{
public:
    CLawicelCanUsb(void);
    virtual ~CLawicelCanUsb(void);

public:
    bool Init(CanBaud baud);            // Initialize

    LW_ERR_CODE Transmit(const CanMsgObject& msg_object);
    LW_ERR_CODE Receive(CanMsgObject& msg_object);

    void Close();
    void  RegisterRxCallbackHandle(LPFNDLL_RECEIVE_CALLBACK pfn);

    char* GetAdapterDescription();
    int   GetAviableAdapterNum();

private:
    char*      m_szAdapter;
    int        m_nAdapterAviableNum;
    CANHANDLE  m_hHandle;

    LPFNDLL_RECEIVE_CALLBACK m_pfReceiveCallback;
};


