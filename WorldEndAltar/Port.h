#ifndef WORLD_END_ALTAR_PORTDEF
#define WORLD_END_ALTAR_PORTDEF

// 定义一些用到的端口

#define CH376_RST_OPORT 4

#define KB_RX_PORT 8
#define KB_TX_PORT 9

// 初始化端口状态
inline void initPorts()
{
	pinMode(CH376_RST_OPORT, OUTPUT);

	digitalWrite(CH376_RST_OPORT, LOW);
}

#endif
