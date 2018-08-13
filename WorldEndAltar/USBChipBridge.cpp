#include "USBChipBridge.h"

USBChipBridge::USBChipBridge()
{
}

void USBChipBridge::begin()
{
	usbChip.begin();
}


void USBChipBridge::initalize()
{
	// 直接依次运行各个初始化流程
	delay(50);
	changeMode();
	waitForDevice(); 
	allocateAddress();
	setConfiguration();
	setPort();
	useBootProtocol();
}

uint8_t bsLEDStatus[] = { 0x21, 0x09, 0x00, 0x02, 0x00, 0x00, 0x01, 0x00 };
void USBChipBridge::updateLEDStatus(uint8_t status)
{
	cancelCurrentNAK(); // 先取消当前NAK尝试，否则CH376无法接受命令

	for (int i = 0;i < 2;i++) // 这里很迷，要执行两次，不懂。。。（电脑测试结果）
	{
		delay(15);

		// 启动控制传输
		// SETUP阶段，发送SETUP报文
		usbChip.writeBlockToEndpointBuffer(bsLEDStatus, 8);
		usbChip.issueToken(0x00, 0x0D);
		usbChip.waitForResponse();
		usbChip.readByte();
		
		// 准备数据，进入DATA阶段
		uint8_t tbuf[1] = { status };
		usbChip.writeBlockToEndpointBuffer(tbuf, 1);
		usbChip.issueToken(0x40, 0x01);
		usbChip.waitForResponse();
		usbChip.readByte();
	}
}

void USBChipBridge::requestRead(uint8_t tog)
{
	// 请求一个IN事务即可
	usbChip.issueToken(tog, (0x81 << 4) | 0x09);
}

size_t USBChipBridge::readBlock(uint8_t *buf, size_t maxLen)
{
	usbChip.sync();
	usbChip.writeByte(0x27);
	usbChip.waitForResponse();
	size_t len = usbChip.readByte();
	return usbChip.readBlock(buf, min(maxLen, len));
}

void USBChipBridge::changeMode()
{
	// 重置总线
	usbChip.sync();
	usbChip.writeByte(0x15);
	usbChip.writeByte(0x07); // MODE7, USB host mode, reset bus
	delayMicroseconds(500);
	usbChip.waitForResponse();
	usbChip.readByte();

	// 启动USB Host模式
	delayMicroseconds(50);
	usbChip.sync();
	usbChip.writeByte(0x15);
	usbChip.writeByte(0x06); // MODE6, USB host mode, automaticly generate SOF packet
	delayMicroseconds(500);
	usbChip.waitForResponse();
	usbChip.readByte();
}

void USBChipBridge::waitForDevice()
{
	usbChip.waitForResponse();
	while (usbChip.readByte() != 0x15)
	{
		usbChip.waitForResponse();
	}
}

#define ALLOCATED_USB_ADDR 0x05
void USBChipBridge::allocateAddress()
{
	// 分配地址 0x05
	usbChip.sync();
	usbChip.writeByte(0x45);
	usbChip.writeByte(ALLOCATED_USB_ADDR);
	usbChip.waitForResponse();
	if (usbChip.readByte() != USB_INT_SUCCESS)
		return;

	// 也需要设置CH376自身的通信地址
	usbChip.sync();
	usbChip.writeByte(0x13);
	usbChip.writeByte(ALLOCATED_USB_ADDR);
}

void USBChipBridge::setConfiguration()
{
	// IKBC G-104只有一个配置
	usbChip.sync();
	usbChip.writeByte(0x49);
	usbChip.writeByte(0x01);
	usbChip.waitForResponse();
	usbChip.readByte();
}

uint8_t bsPort[] = { 0x01, 0x11, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };
void USBChipBridge::setPort()
{
	// 启动控制传输，无数据阶段，只需要SETUP阶段。设置端口1
	usbChip.writeBlockToEndpointBuffer(bsPort, 8);
	usbChip.issueToken(0x00, 0x0D);
	usbChip.waitForResponse();
	if (usbChip.readByte() != USB_INT_SUCCESS)
		return;
}

uint8_t bsBoot[] = { 0x29, 0x0B, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };
void USBChipBridge::useBootProtocol()
{
	// 启动控制传输，无数据阶段，只需要SETUP阶段。设置端点1（对应IKBC G-104的Boot Keyboard，采用最简单的USB报文格式）
	usbChip.writeBlockToEndpointBuffer(bsBoot, 8);
	usbChip.issueToken(0x00, 0x0D);
	usbChip.waitForResponse();
	if (usbChip.readByte() != USB_INT_SUCCESS)
		return;
}

void USBChipBridge::cancelCurrentNAK()
{
	usbChip.sync();
	usbChip.writeByte(0x17);
	usbChip.disposeReadBuffer(); // 会发个0x2E错误代码
}

size_t USBChipBridge::available()
{
	return usbChip.available();
}

uint8_t USBChipBridge::readByte()
{
	return usbChip.readByte();
}
