typedef union Temperature
{
  uint8 temp[4];
  struct RFRXBUF
  {
    unsigned char Head;       // 命令头
    unsigned char values[2];  // 温度数据
    unsigned char Tail;       // 命令尾
  }Buf;
}Temperature_u;
