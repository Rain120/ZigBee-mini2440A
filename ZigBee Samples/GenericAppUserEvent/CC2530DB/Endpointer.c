
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


// This list should be filled with Application specific Cluster IDs.
const cId_t GenericApp_ClusterList[GENERICAPP_MAX_CLUSTERS] =
{
  GENERICAPP_CLUSTERID    // ����ID
};


// ������
const SimpleDescriptionFormat_t GenericApp_SimpleDesc =
{
  GENERICAPP_ENDPOINT,              //  int Endpoint; �˵��
  GENERICAPP_PROFID,                //  uint16 AppProfId[2];
  GENERICAPP_DEVICEID,              //  uint16 AppDeviceId[2];
  GENERICAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
  GENERICAPP_FLAGS,                 //  int   AppFlags:4;
  0,                                //  ���뼯Ⱥ��;
  (cId_t *)NULL,                    //  ���뼯Ⱥ;
  GENERICAPP_MAX_CLUSTERS,          //  �����Ⱥ��;
  (cId_t *)GenericApp_ClusterList   // �����Ⱥ;
};

// This is the Endpoint/Interface description.  It is defined here, but
// filled-in in GenericApp_Init().  Another way to go would be to fill
// in the structure here and make it a "const" (in code space).  The
// way it's defined in this sample app it is define in RAM.

endPointDesc_t GenericApp_epDesc;  // �˵�������


/*********************************************************************
 * LOCAL VARIABLES
 */

// ����ID
byte GenericApp_TaskID;   // Task ID for internal task/event processing
                          // This variable will be received when
                          // GenericApp_Init() is called.
// ����״̬
devStates_t GenericApp_NwkState;

// ��ϢID
byte GenericApp_TransID;  // This is the unique message ID (counter)


/*********************************************************************
 * LOCAL FUNCTIONS
 */

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
  GenericApp_TaskID = task_id;     // ��������ID
  
  GenericApp_NwkState = DEV_INIT;   // ��ʼ������״̬
  
  GenericApp_TransID = 0;   // ��ϢID

  
  // Fill out the endpoint description.  ��ʼ���˵�������
  GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;
  GenericApp_epDesc.task_id = &GenericApp_TaskID;
  GenericApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  GenericApp_epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF ע��˵�
  afRegister( &GenericApp_epDesc );
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

/*
typedef unsigned short (*pTaskEventHandlerFn)( unsigned char task_id, unsigned short event );

//unsigned short (*pTaskEventHandlerFn)( unsigned char task_id, unsigned short event );
//unsigned short *pTaskEventHandlerFn( unsigned char task_id, unsigned short event );


const pTaskEventHandlerFn tasksArr[] = {
  macEventLoop,    
  nwk_event_loop,
  Hal_ProcessEvent,
  GenericApp_ProcessEvent
};

pTaskEventHandlerFn pfunc = GenericApp_ProcessEvent;
pfunc(idx, events);

pfunc = tasksArr[3];
pfunc(idx, events);  => GenericApp_ProcessEvent(idx, events);
(tasksArr[3])(idx, events);  => GenericApp_ProcessEvent(idx, events);
*/
uint16 GenericApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  
  if ( events & SYS_EVENT_MSG )   // �ж��Ƿ���ϵͳ��Ϣ
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {
        case ZDO_STATE_CHANGE:   // �ж�����״̬�Ƿ�ı�
          GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if ((GenericApp_NwkState == DEV_ROUTER)
              || (GenericApp_NwkState == DEV_END_DEVICE) )   
          {
            // Start sending "the" message in a regular interval. ��������ɹ�
            // ��������
            GenericApp_SendTheMessage();
          }
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
  char theMessageData[] = "D1";
  
  // ��ַ��Ϣ
  afAddrType_t GenericApp_DstAddr;
  GenericApp_DstAddr.addr.shortAddr = 0x0000;   // Э�����ĵ�ַ
  GenericApp_DstAddr.addrMode = Addr16Bit;  // ��Ե�ͨ��ģʽ
  GenericApp_DstAddr.endPoint = GENERICAPP_ENDPOINT;
  
  if ( AF_DataRequest( &GenericApp_DstAddr, &GenericApp_epDesc,
                       GENERICAPP_CLUSTERID,
                       (byte)osal_strlen( theMessageData ) + 1,
                       (byte *)&theMessageData,
                       &GenericApp_TransID,
                       AF_DISCV_ROUTE, AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
    HalLedBlink(HAL_LED_2, 0, 50, 1000);
  }
  else
  {
    HalLedSet(HAL_LED_2, HAL_LED_MODE_ON);
  }
}

