
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"
#include <string.h>
#include "GenericApp.h"

#include "Sensor.h"

#if !defined (WIN32)
#include "OnBoard.h"
#endif

#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"

#define SEND_DATA_EVENT  0x01


typedef union h
{
  uint8 temp[4];
  struct RFRXBUF
  {
    unsigned char Head;       // 命令头
    unsigned char values[2];  // 温度数据
    unsigned char Tail;       // 命令尾
  }Buf;
}Temperature_u;

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
  (cId_t *) NULL,  //  byte *pAppInClusterList;
  GENERICAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)GenericApp_ClusterList   //  byte *pAppInClusterList;
};


endPointDesc_t GenericApp_epDesc;

byte GenericApp_TaskID;   // Task ID for internal task/event processing
                          // This variable will be received when

byte GenericApp_TransID;  // This is the unique message ID (counter)
devStates_t GenericApp_NwkState;


static void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
static void GenericApp_SendTheMessage(void);
int8 readTemp(void);


void GenericApp_Init(byte task_id)
{
  GenericApp_TaskID = task_id;
  GenericApp_NwkState = DEV_INIT;
  GenericApp_TransID = 0;
  
  // Fill out the endpoint description.
  GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;
  GenericApp_epDesc.task_id = &GenericApp_TaskID;
  GenericApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  GenericApp_epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF
  afRegister( &GenericApp_epDesc );

  
}


uint16 GenericApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
 
  if ( events & SYS_EVENT_MSG )
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {
        case ZDO_STATE_CHANGE:
          GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if (GenericApp_NwkState == DEV_END_DEVICE)
          {
            osal_set_event(GenericApp_TaskID, SEND_DATA_EVENT);
          }
          break;        
        
        case AF_INCOMING_MSG_CMD:
          GenericApp_MessageMSGCB( MSGpkt );
          break;

        default:
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );

      // Next
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
  
  if (events & SEND_DATA_EVENT)
  {
    GenericApp_SendTheMessage();
    osal_start_timerEx(GenericApp_TaskID, SEND_DATA_EVENT, 1000);
    return (events ^ SEND_DATA_EVENT);
  }
  
  
  // Discard unknown events
  return 0;
}



/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      GenericApp_MessageMSGCB
 *
 * @brief   Data message processor callback.  This function processes
 *          any incoming data - probably from other devices.  So, based
 *          on cluster ID, perform the intended action.
 *
 * @param   none
 *
 * @return  none
 */
static void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{

  
  switch ( pkt->clusterId )
  {
    case GENERICAPP_CLUSTERID:
      break;
  }
}


void GenericApp_SendTheMessage(void)
{
  uint8 tvalues;
  Temperature_u temperature;
  temperature.Buf.Head = '&';
  tvalues = GetTemperature();
  temperature.Buf.values[0] = tvalues / 10 + '0';
  temperature.Buf.values[1] = tvalues % 10 + '0';
  temperature.Buf.Tail = 'C';
  
  afAddrType_t myDstAddr;
  myDstAddr.addrMode = (afAddrMode_t)Addr16Bit;
  myDstAddr.endPoint = GENERICAPP_ENDPOINT;
  myDstAddr.addr.shortAddr = 0x0000;
  AF_DataRequest(&myDstAddr, &GenericApp_epDesc,
                 GENERICAPP_CLUSTERID,
                 sizeof(temperature),
                 (uint8*)&temperature,
                 &GenericApp_TransID,
                 AF_DISCV_ROUTE,
                 AF_DEFAULT_RADIUS);
}
