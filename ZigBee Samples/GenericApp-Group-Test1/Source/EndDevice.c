
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

#include "aps_groups.h"

#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"


/*********************************************************************
 * GLOBAL VARIABLES
 */
// 群集列表
const cId_t GenericApp_ClusterList[GENERICAPP_MAX_CLUSTERS] =
{
  GENERICAPP_CLUSTERID
};

// 简单描述符：描述端点
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

// 端点描述符
endPointDesc_t GenericApp_epDesc;


/*********************************************************************
 * LOCAL VARIABLES
 */
// 任务ID，优先级
byte GenericApp_TaskID; 

// 设备状态
devStates_t GenericApp_NwkState;

// 发送数据序号
byte GenericApp_TransID;  // This is the unique message ID (counter)

// 定义组
aps_Group_t GenericApp_Group;
/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void GenericApp_SendTheMessage( void );
static void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pkt );

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
  GenericApp_TaskID = task_id;
  
  GenericApp_NwkState = DEV_INIT;  // 设备未连接任何网络
 
  GenericApp_TransID = 0;

  // Fill out the endpoint description.
  GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;
  GenericApp_epDesc.task_id = &GenericApp_TaskID;
  GenericApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  GenericApp_epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF
  afRegister( &GenericApp_epDesc );  //　没有注册不能使用OSAL提供的服务

    // 定义一个组，并初始化
  GenericApp_Group.ID = 0x0001;
  GenericApp_Group.name[0] = 6;
  osal_memcpy(&(GenericApp_Group.name[1]), "neusof", 6);
  aps_AddGroup( GENERICAPP_ENDPOINT, &GenericApp_Group );
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
  afIncomingMSGPacket_t *MSGpkt;  // 消息包指针
  
  if ( events & SYS_EVENT_MSG )   // 判断是否是系统事件
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {  
        case AF_INCOMING_MSG_CMD:
          GenericApp_MessageMSGCB(MSGpkt);
          break;
        case ZDO_STATE_CHANGE:  // 设备状态发生改变
          GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if ( GenericApp_NwkState == DEV_END_DEVICE
              || GenericApp_NwkState == DEV_ROUTER)  // 设备状态为终端设备，加入网络成功
          {
           // aps_AddGroup( GENERICAPP_ENDPOINT, &GenericApp_Group );
          }
        default:
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );  // 释放消息空间，防止内存泄漏

      // Next
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);   // 返回其他未处理的消息
  }
  // Discard unknown events
  return 0;
}

/*********************************************************************
 * @fn      GenericApp_SendTheMessage
 *
 * @brief   Send "the" message.
 *
 * @param   none
 *
 * @return  none
 */
static void GenericApp_SendTheMessage( void )
{
  char theMessageData[] = "Router Received!";
  
  // 目的地址
  afAddrType_t GenericApp_DstAddr;
  
  GenericApp_DstAddr.addr.shortAddr = 0x0000;  // 协调器的网络地址固定为0
  GenericApp_DstAddr.addrMode = afAddr16Bit;
  GenericApp_DstAddr.endPoint = GENERICAPP_ENDPOINT;  // 接收端的端点号
  
  if ( AF_DataRequest( &GenericApp_DstAddr, &GenericApp_epDesc,
                       GENERICAPP_CLUSTERID,
                       (byte)osal_strlen( theMessageData ) + 1,
                       (byte *)theMessageData,
                       &GenericApp_TransID,
                       AF_DISCV_ROUTE, AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
    // Successfully requested to be sent.
    HalLedSet( HAL_LED_1, HAL_LED_MODE_TOGGLE );  // 点灯
  }
  else
  {
    
    HalLedSet( HAL_LED_1, HAL_LED_MODE_OFF );   // 关灯
    // Error occurred in request to send.
  }
}

static void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
  uint8 buffer[10];
  switch ( pkt->clusterId )
  {
    case GENERICAPP_CLUSTERID:
      osal_memcpy(buffer, pkt->cmd.Data, 2);
      if (osal_memcmp(buffer, "D1", 2))
      {
        HalLedSet( HAL_LED_2, HAL_LED_MODE_TOGGLE );
        GenericApp_SendTheMessage();  // 回复数据
      }

      break;
  }
}