// Lawicel CAN USB driver code
// Arthor  : jzhao@sinoev.com
// Version : 1.0

#include "StdAfx.h"
#include "LawicelCanUsb.h"

// Construction
CLawicelCanUsb::CLawicelCanUsb(void)
{
	// 接收数据时的回调函数接口
    m_pfReceiveCallback  = NULL;

	// 可用适配器数量
    m_nAdapterAviableNum = 0;

	// 总线句柄
    m_hHandle            = 0;

	// 填装数据，初始化
    m_szAdapter          = new char[32];
    memset(m_szAdapter, 0, 32);
}

// Destruction
CLawicelCanUsb::~CLawicelCanUsb(void)
{
    Close();

    delete []m_szAdapter;
}

// Get description
char* CLawicelCanUsb::GetAdapterDescription()
{
    return (this->m_szAdapter);
}

// Get aviable adapter number
int CLawicelCanUsb::GetAviableAdapterNum()
{
    return (this->m_nAdapterAviableNum);
}

// Set receive handle
// This function must be called before Init() method, else, you must re-call the Init() method to apply the changes.
// You can set the paramter pfn to NULL and re-init the adapter to disable the callback function.
void CLawicelCanUsb::RegisterRxCallbackHandle(LPFNDLL_RECEIVE_CALLBACK pfn)
{
    m_pfReceiveCallback = pfn;
}

// Initialize
bool CLawicelCanUsb::Init(CanBaud baud)
{
    bool flag      = true;
    CHAR* baud_str = new CHAR[5];
    
    memset(baud_str, 0, 5);

    switch (baud)
    {
        case BAUD_10_KBPS:
            ::strcpy_s(baud_str, sizeof("10"), "10");
            break;

        case BAUD_20_KBPS:
            ::strcpy_s(baud_str, sizeof("20"), "20");
            break;

        case BAUD_50_KBPS:
            ::strcpy_s(baud_str, sizeof("50"), "50");
            break;

        case BAUD_100_KBPS:
            ::strcpy_s(baud_str, sizeof("100"), "100");
            break;

        case BAUD_250_KBPS:
            ::strcpy_s(baud_str, sizeof("250"), "250");
            break;

        case BAUD_500_KBPS:
            ::strcpy_s(baud_str, sizeof("500"), "500");
            break;

        case BAUD_800_KBPS:
            ::strcpy_s(baud_str, sizeof("800"), "800");
            break;

        case BAUD_1000_KBPS:
            ::strcpy_s(baud_str, sizeof("1000"), "1000");
            break;

        default:
            flag = false;
            break;
    }

    if (flag)
    {
		// 目的是找到“一个”可用的适配器
        // Open first adapter
        m_nAdapterAviableNum = ::canusb_getFirstAdapter(m_szAdapter, 32);
        
        // Open driver 
        if (m_nAdapterAviableNum > 0)
        {
            for (int dev_index = 0; dev_index < m_nAdapterAviableNum; dev_index++)
            {
                m_hHandle = ::canusb_Open(m_szAdapter, baud_str, CANUSB_ACCEPTANCE_CODE_ALL, CANUSB_ACCEPTANCE_MASK_ALL, 0);

                if (0 == m_hHandle)
                {
                    // Open failed, try next adapter
                    ::canusb_getNextAdapter(m_szAdapter, 32);                                
                }
                else
                {
                    // Set receive callback function if the callback function enabled
                    if (m_pfReceiveCallback != NULL)
                    {
                        ::canusb_setReceiveCallBack(m_hHandle, m_pfReceiveCallback);
                    }

                    break;
                }
            }

            if (0 == m_hHandle)
            {
                flag = false;           
            }
        }
        else
        {
            flag = false;        
        }
    }

    return (flag);
}

// Close
void CLawicelCanUsb::Close()
{
    if (m_hHandle)
    {
        ::canusb_Close(m_hHandle);    
    }

    m_hHandle = 0;
}

// Function   : Transmit 1 frame message
// Parameters : Message object which is used to transmit
// Returns    : Error code for message transmit. The typical values are:
//              ERROR_CANUSB_OK: Transmit successful
//              ERROR_CANUSB_TX_FIFO_FULL: Bus busy, please wait

LW_ERR_CODE CLawicelCanUsb::Transmit(const CanMsgObject& msg)
{
	// 错误代码
    LW_ERR_CODE err_code;

    CANMsg *pMsgObject = new CANMsg();

    pMsgObject->id         = msg.id;
    pMsgObject->flags     |= (msg.ext_id ? 0x80 : 0x00);
    pMsgObject->flags     |= (msg.rtr ? 0x40 : 0x00);
    pMsgObject->timestamp  = ::GetTickCount();
    pMsgObject->len        = msg.size;

	// 把消息内容传递给CAN总线消息体
    memcpy(pMsgObject->data, msg.msg, msg.size);

	// 写总线，返回错误代码
    err_code = ::canusb_Write(m_hHandle, pMsgObject);

    delete pMsgObject;

    return (err_code);
}

// Function  : Receive message
// Paramters : The message object which is used to save received message.
// Returns   : Error code of receive operation. The typical value is:
//             ERROR_CANUSB_NO_MESSAGE: Message not received
//             ERROR_CANUSB_OK: OK

LW_ERR_CODE CLawicelCanUsb::Receive(CanMsgObject& msg)
{
    LW_ERR_CODE err_code;

    CANMsg* pMsgObject = new CANMsg();

	// 读总线
    err_code = ::canusb_Read(m_hHandle, pMsgObject);

    if (ERROR_CANUSB_OK == err_code)
    {
        msg.id        = pMsgObject->id;
        msg.ext_id    = pMsgObject->flags & 0x80 ? true : false;
        msg.rtr       = pMsgObject->flags & 0x40 ? true : false;
        msg.size      = pMsgObject->len;
        msg.timestamp = pMsgObject->timestamp;

        memcpy(msg.msg, pMsgObject->data, pMsgObject->len);
    }

    delete pMsgObject;

    return (err_code);
}
