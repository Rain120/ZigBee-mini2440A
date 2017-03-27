
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

#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"


#define TEST_SEND_DATA  0x01

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Ⱥ���б�
const cId_t GenericApp_ClusterList[GENERICAPP_MAX_CLUSTERS] =
{
  GENERICAPP_CLUSTERID
};

// ���������������˵�
const SimpleDescriptionFormat_t GenericApp_SimpleDesc =
{
  GENERICAPP_ENDPOINT,              //  int Endpoint;
  GENERICAPP_PROFID,                //  uint16 AppProfId[2];
  GENERICAPP_DEVICEID,              //  uint16 AppDeviceId[2];
  GENERICAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
  GENERICAPP_FLAGS,                 //  int   AppFlags:4;
  GENERICAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)GenericApp_ClusterList,  //  byte *pAppInClusterList;
  0,          //  byte  AppNumInClusters;
  (cId_t *)NULL   //  byte *pAppInClusterList;
};

// �˵�������
endPointDesc_t GenericApp_epDesc;


/*********************************************************************
 * LOCAL VARIABLES
 */
// ����ID�����ȼ�
byte GenericApp_TaskID; 

// �����������
byte GenericApp_TransID;  // This is the unique message ID (counter)

// �豸״̬
devStates_t GenericApp_NwkState;
/*********************************************************************
 * LOCAL FUNCTIONS
 */

// ��������
static void GenericApp_SendTheMessage( void );

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
 
  GenericApp_TransID = 0;

  // Fill out the endpoint description.
  GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;
  GenericApp_epDesc.task_id = &GenericApp_TaskID;
  GenericApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  GenericApp_epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF
  afRegister( &GenericApp_epDesc );  //��û��ע�᲻��ʹ��OSAL�ṩ�ķ���

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
  afIncomingMSGPacket_t *MSGpkt;  // ��Ϣ��ָ��

  if ( events & SYS_EVENT_MSG )   // �ж��Ƿ���ϵͳ�¼�
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )  // ����ϵͳ��Ϣ�����ͷֱ���
      {        
        case ZDO_STATE_CHANGE:   // ���յ����������¼�
          GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if (GenericApp_NwkState == DEV_ZB_COORD)
          {
            GenericApp_SendTheMessage();
            osal_start_timerEx( GenericApp_TaskID, TEST_SEND_DATA, 5000 );
          }
          break;
        default:
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );  // �ͷ���Ϣ�ռ䣬��ֹ�ڴ�й©

      // Next
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);   // ��������δ�������Ϣ
  }
  
  // ����
  if (events & TEST_SEND_DATA)
  {
    GenericApp_SendTheMessage();
    osal_start_timerEx( GenericApp_TaskID, TEST_SEND_DATA, 5000 );
            
    return (events ^ TEST_SEND_DATA);   // ��������δ�������Ϣ
  }
  // Discard unknown events
  return 0;
}

static void GenericApp_SendTheMessage( void )
{
  char *theMessageData = "0123456789";
  
  afAddrType_t GenericApp_DstAddr;
  
  GenericApp_DstAddr.addr.shortAddr = 0xffff;  // Э�����������ַ�̶�Ϊ0
  GenericApp_DstAddr.addrMode = afAddrBroadcast;
  GenericApp_DstAddr.endPoint = GENERICAPP_ENDPOINT;  // ���ն˵Ķ˵��
  
  if ( AF_DataRequest( &GenericApp_DstAddr, &GenericApp_epDesc,
                       GENERICAPP_CLUSTERID,
                       (byte)osal_strlen( theMessageData ) + 1,
                       (byte *)theMessageData,
                       &GenericApp_TransID,
                       AF_DISCV_ROUTE, AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
    // Successfully requested to be sent.
    HalLedSet( HAL_LED_2, HAL_LED_MODE_ON );  // ���
  }
  else
  {
    
    HalLedSet( HAL_LED_2, HAL_LED_MODE_OFF );   // �ص�
    // Error occurred in request to send.
  }
  
}
