#include "USBChipBridge.h"
#include "Bluetooth.h"
#include "Port.h"

#define MAX_READ_EPBUF_LEN 32

Bluetooth *bluetooth;
USBChipBridge usbKeyboard;
uint8_t tog = 0x00;
uint8_t *buf = nullptr;

void flipTog();

void setup()
{
	// 初始化程序
	initPorts();
	bluetooth = new Bluetooth(115200);
	buf = new uint8_t[MAX_READ_EPBUF_LEN];

	// 重启CH376芯片
	digitalWrite(CH376_RST_OPORT, HIGH);
	delay(40);
	digitalWrite(CH376_RST_OPORT, LOW);

	// 初始化CH376芯片
	usbKeyboard.begin();
	usbKeyboard.initalize();

	// 开始轮询（这个过程实际上由CH376完成，轮询到结果CH376将会返回中断值
	usbKeyboard.requestRead(tog);
	flipTog();

	delay(50);
}

void loop()
{
	// 键盘状态更新
	if (usbKeyboard.available())
	{
		if (usbKeyboard.readByte() == USB_INT_SUCCESS)
		{
			// 成功获取值，进行更新
			size_t len = usbKeyboard.readBlock(buf, MAX_READ_EPBUF_LEN);
			if (len == 8)
			{
				// 输入有效（长度为8）时更新，即Modifier, Reversed, Key1-6 八个字节
				bluetooth->updateKeyboard(buf);
			}
			// 请求下一次读取，开始轮询
			usbKeyboard.requestRead(tog);
			flipTog();
		}
	}
	// 蓝牙模块有数据（只可能是Num Lock等键盘灯状态）
	if (bluetooth->available())
	{
		size_t len = bluetooth->readBlock(buf, MAX_READ_EPBUF_LEN);
		delayMicroseconds(500);
		if (len > 1 && buf[0] == 0x01)
		{
			delay(1);
			usbKeyboard.updateLEDStatus(buf[1]);
			flipTog();
			usbKeyboard.requestRead(tog);
			flipTog();
		}
	}
}

/*
 * 我也不知道这个函数是干啥的→_→照着官方例程写就对了
 */
void flipTog()
{
	if (tog == 0x80) tog = 0x00;
	else tog = 0x80;
}