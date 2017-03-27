
/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#include "Coordinator.h"
#include "DebugTrace.h"

#include <string.h>

#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"

#include "OSAL_Nv.h"

#include "aps_groups.h"


#define SHOW_INFO_EVENT   0x01
#define SEND_DATA_EVENT   0x02

/*********************************************************************
 * GLOBAL VARIABLES
 */
// This list should be filled with Application specific Cluster IDs.
const cId_t GenericApp_ClusterList[GENERICAPP_MAX_CLUSTERS] =
{
  GENERICAPP_CLUSTERID
};

const SimpleDescriptionFormat_t GenericApp_SimpleDesc =
{
  GENERICAPP_ENDPOINT,              //  int Endpoint;
  GENERICAPP_PROFID,                //  uint16 AppProfId[2];
  GENERICAPP_DEVICEID,              //  uint16 AppDeviceId[2];
  GENERICAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
  GENERICAPP_FLAGS,                 //  int   AppFlags:4;
  0,          //  byte  AppNumInClusters;
  (cId_t *)NULL,  //  byte *pAppInClusterList;
  GENERICAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)GenericApp_ClusterList   //  byte *pAppInClusterList;
};

// This is the Endpoint/Interface description.  It is defined here, but
// filled-in in GenericApp_Init().  Another way to go would be to fill
// in the structure here and make it a "const" (in code space).  The
// way it's defined in this sample app it is define in RAM.
endPointDesc_t GenericApp_epDesc;

/*********************************************************************
 * LOCAL VARIABLES
 */
byte GenericApp_TaskID;   // Task ID for internal task/event processing
                          // This variable will be received when
                          // GenericApp_Init() is called.

byte GenericApp_TransID;
devStates_t GenericApp_NwkState;


void ShowInfo(void);
void To_String(uint8 *dest, char* src, uint8 length);
void GenericApp_ProcessZDOMsgs(zdoIncomingMsg_t *inMsg);
void To_Str(uint8 *dest, char *src, uint8 length);

typedef struct RFXBUF
{
  uint8 myNWK[4];   // 存储本节点的网络地址
  uint8 myMAC[16];  // 存储本节点的MAC地址
  uint8 pNWK[4];    // 存放父节点的网络地址
  uint8 pMAC[16];   // 存放父节点的MAC地址  
}RFTX;
/*********************************************************************
 * @fn      GenericApp_Init
 *
 * @brief   Initialization function for the Generic App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void GenericApp_Init( uint8 task_id )
{
  halUARTCfg_t uartConfig;
  
  GenericApp_TaskID = task_id;
  GenericApp_TransID = 0;
  
  GenericApp_NwkState = DEV_INIT;
  

  // Fill out the endpoint description.
  GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;
  GenericApp_epDesc.task_id = &GenericApp_TaskID;
  GenericApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  GenericApp_epDesc.latencyReq = noLatencyReqs;
  afRegister(&GenericApp_epDesc);
  
  // 初始化串口
  uartConfig.configured = TRUE;
  uartConfig.baudRate = HAL_UART_BR_115200;
  uartConfig.flowControl = FALSE;
  uartConfig.callBackFunc = NULL;
   
  HalUARTOpen(0, &uartConfig);
  /*
  uint16 uctest = 0xfa12;
  char buf[4] = {0};
  To_String(buf, (uint8*)&uctest, 2);
  
  HalUARTWrite(0, buf, 4);
  HalUARTWrite(0, "\r\n", 2);
  */
  
  // 在应用层要相获得对IEEE_addr_rsp的响应，要进行注册。
  ZDO_RegisterForZDOMsg(GenericApp_TaskID, IEEE_addr_rsp);
}

/*********************************************************************
 * @fn      GenericApp_ProcessEvent
 *
 * @brief   Generic Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  none
 */
uint16 GenericApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  if (events & SYS_EVENT_MSG)
  {
    MSGpkt = (afIncomingMSGPacket_t*) osal_msg_receive(GenericApp_TaskID);
    while(MSGpkt)
    {
      switch(MSGpkt->hdr.event)
      {
      case ZDO_STATE_CHANGE:
        GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
        if ((GenericApp_NwkState == DEV_ROUTER) 
            || (GenericApp_NwkState == DEV_END_DEVICE))            
        {
          osal_set_event((GenericApp_TaskID), SHOW_INFO_EVENT);
          osal_set_event((GenericApp_TaskID), SEND_DATA_EVENT);
        }
        break;
      case ZDO_CB_MSG:
        GenericApp_ProcessZDOMsgs((zdoIncomingMsg_t*)MSGpkt);
      default:
        break;
      }
      osal_msg_deallocate((uint8*)MSGpkt);
      MSGpkt = (afIncomingMSGPacket_t*)osal_msg_receive(GenericApp_TaskID);
    }
    return (events ^ SYS_EVENT_MSG);
  }
  
  if (events & SHOW_INFO_EVENT)
  {
    ShowInfo();
    // osal_start_timerEx(GenericApp_TaskID, SHOW_INFO_EVENT, 5000);
    return (events ^ SHOW_INFO_EVENT);
  }
  
  if (events & SEND_DATA_EVENT)
  {
    ShowInfo();
    ZDP_IEEEAddrReq(0x0000, 0, 0, 0);  // 请求协调器的IEEE地址。
    osal_start_timerEx(GenericApp_TaskID, SEND_DATA_EVENT, 5000);
    return (events ^ SEND_DATA_EVENT);
  }
  
  return 0;
}


void ShowInfo(void)
{
  RFTX rftx;
  uint16 nwk;
  uint8 buf[8];
  uint8 changline[2] = {0x0a, 0x0d};   // 回车换行的ASCII码
  
 // HalUARTWrite(0, changline, 2);

 // HalUARTWrite(0, "test", 4);
  
 // HalUARTWrite(0, changline, 2);
  
  nwk = NLME_GetShortAddr();
  To_String(rftx.myNWK, (uint8*)&nwk, 2);
  To_String(rftx.myMAC, NLME_GetExtAddr(), 8);
  
  nwk = NLME_GetCoordShortAddr();
  To_String(rftx.pNWK, (uint8*)&nwk, 2);
  NLME_GetCoordExtAddr(buf);
  To_String(rftx.pMAC, buf, 8);
  
  HalUARTWrite(0, "NWK:", osal_strlen("NWK:"));
  HalUARTWrite(0, rftx.myNWK, 4);
  HalUARTWrite(0, "  MAC:", osal_strlen("  MAC:"));
  HalUARTWrite(0, rftx.myMAC, 16);
  
  HalUARTWrite(0, "  p-NWK:", osal_strlen("  p-NWK:"));
  HalUARTWrite(0, rftx.pNWK, 4);
  HalUARTWrite(0, "  p-MAC:", osal_strlen("  p-MAC:"));
  HalUARTWrite(0, rftx.pMAC, 16);
  
  HalUARTWrite(0, changline, 2);
}

void GenericApp_ProcessZDOMsgs(zdoIncomingMsg_t *inMsg)
{
  char buf[16];
  char changeline[2] = {0x0A, 0x0D};
  
  switch (inMsg->clusterID)
  {
  case IEEE_addr_rsp:
    {
      ZDO_NwkIEEEAddrResp_t *pRsp = ZDO_ParseAddrRsp(inMsg);  // 解析数据包
      if (pRsp)
      {
        if (pRsp->status == ZSuccess)
        {
          To_Str(buf, pRsp->extAddr, 8);
          HalUARTWrite(0, "Coordinator MAC:", osal_strlen("Coordinator MAC:"));
          HalUARTWrite(0, buf, 16);
          HalUARTWrite(0, changeline, 2);
        }
        osal_mem_free(pRsp);
      }
      break;
    }
  }
}



void To_String(uint8 *dest, char* src, uint8 length)
{
  int i = length-1;
  int j = 0;
  while(i >= 0)
  {
    uint8 uctemp;
    if (j % 2 == 0)
    {
      uctemp = (src[i] >> 4) & 0xf;
    }
    else
    {
      uctemp = src[i] & 0xf;
    }
    
     if (uctemp < 0xa)
    {
      dest[j] = uctemp + 0x30;
    }
    else
    {
      dest[j] = uctemp + 0x41 - 0xa;
    }
    j++;
    if (j % 2 == 0)
    {
      i--;
    }
  }
}

void To_Str(uint8 *dest, char *src, uint8 length)
{
  uint8 *xad;
  uint8 i = 0;
  uint8 ch;
  xad = src + length -1;  // 指向最后一个字节
  for (i=0; i<length; i++, xad--)
  {
    ch = (*xad >> 4) & 0x0f;
    dest[i<<1] = ch + ((ch < 10) ? '0' : '7');
    ch = *xad & 0x0f;
    dest[(i<<1)+1] = ch + ((ch < 10) ? '0' :'7');
  }  
}