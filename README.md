# ZigBee-mini2440A
ARM 物联网综合课程设计<br />
  课程设计要求：<br />
    编写一个监控系统，该程序的具备以下功能：<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;1.模拟一个热启动的过程（程序下载20s后，开始运行功能代码）。<br />
     &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;2.实现一个串口控制台程序，至少具备以下命令，要求所有命令都保存到24C08中，每次开机后，自动读取。<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;管理命令：查看所有命令，添加命令，删除命令，设置报警阈值<br />
   &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;本地控制命令：LLEDON, LLEDOFF, LGETVOL（获取实时电压值）<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;远程控制命令：查看ZigBee网络中所有节点信息，查看实时温度和电压，<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;RLED_FLASH：广播所有节点LED1闪烁 <br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;RLED_ON nwk地址：点亮指定节点的LED2<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;RLED_OFF nwk地址：熄灭指定节点的LED2<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;RLED_BLINK(n)：加入到同一个组的节点LED3闪烁n次，如果n=0,表示永久闪烁。 <br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;3.本地监控功能：监控本电路板上的电压值，若电压值超过当前的电压限制则通过蜂鸣器报警，通过按键解除按键报警，具体要求如下：<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;要求：<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;1）周期性（如每隔1s）采集1次电路板电压值（可采用RTC TIME TICK完成） 每次电压采集完成后，比较当前的电压值是否在正常的电压阈值范围内（1V~2.5V），可能会超过上限或下线。若当前采集的电压值异常，则声光报警，通过蜂鸣器发出报警信号，并且LED闪烁。<br />
   &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;2）报警必须通过人工解除的。通过电路板上的KEY1控制（通过按下KEY1使得蜂蜜器停止鸣叫）。即如果一次采集到了3V，报警，下次采集到了2V，正常， 但是不能关闭beep,必须通过人工解除。<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;4.远程监控功能：实现一个无线传感网络远程数据采集控制系统，协调器建立网络，路由器和终端节点加入网络后，周期性的采集温度和电压发送给协调器，协调器通过串口与主控板（mini2440）相连。<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;1）周期性的采集各个节点的传感器数据（如温度和电压等），发送到主控板（mini2440），与阈值进行比较，如果超出阈值范围，则控制板及对应节点发出声光报警，控制台每隔1s打印正在报警的节点信息及其当前采集到的数据。<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;2）报警必须通过人工解除的。通过电路板上的KEY2控制（通过按下KEY2使得蜂鸣器停止鸣叫，LED停止闪烁）。<br />
    &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;5.蜂鸣器报警音乐根据异常的情况，发出不一样的声音。<br />
