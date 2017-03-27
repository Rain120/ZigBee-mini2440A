
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

#include "OSAL_Nv.h"


#define USER_EVENT_TEST   0x0001

/*********************************************************************
 * LOCAL VARIABLES
 */
// 任务ID，优先级
byte GenericApp_TaskID;

// 设备状态
devStates_t GenericApp_NwkState;


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
  
  // 启动一次事件
  //osal_set_event(GenericApp_TaskID, USER_EVENT_TEST);
  // 延时后启动
  osal_start_timerEx( GenericApp_TaskID, USER_EVENT_TEST, 5000 );
//  GenericApp_NwkState = DEV_INIT;
  
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
        /*case ZDO_STATE_CHANGE:  // 设备状态发生改变
          GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if ( GenericApp_NwkState == DEV_ZB_COORD )  // 设备状态为终端设备，加入网络成功
          {
            // 启动用户事件
            osal_set_event(GenericApp_TaskID, USER_EVENT_TEST);
          }*/
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
  
  // 处理用户事件
  if (events & USER_EVENT_TEST)
  {
    uint8 w_value = 0x88;
    uint8 r_value = 0;
    
    osal_nv_item_init( TEST_NV, 1, NULL );
    // 往NV里写入0x88
    osal_nv_write( TEST_NV, 0, 1, &w_value );
    
    // 读取NV里的该值
    osal_nv_read( TEST_NV, 0, 1, &r_value );
    
    if (r_value == 0x88)
    {
      HalLedBlink(HAL_LED_ALL, 0, 50, 500);
    }
    else
    {
      HalLedSet(HAL_LED_ALL, HAL_LED_MODE_OFF);
    }
    
    return (events ^ USER_EVENT_TEST);   // 返回其他未处理的消息
  }
  
  
  // Discard unknown events
  return 0;
}

