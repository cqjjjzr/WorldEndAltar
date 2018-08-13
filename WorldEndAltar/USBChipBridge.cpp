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
	// ֱ���������и�����ʼ������
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
	cancelCurrentNAK(); // ��ȡ����ǰNAK���ԣ�����CH376�޷���������

	for (int i = 0;i < 2;i++) // ������ԣ�Ҫִ�����Σ����������������Բ��Խ����
	{
		delay(15);

		// �������ƴ���
		// SETUP�׶Σ�����SETUP����
		usbChip.writeBlockToEndpointBuffer(bsLEDStatus, 8);
		usbChip.issueToken(0x00, 0x0D);
		usbChip.waitForResponse();
		usbChip.readByte();
		
		// ׼�����ݣ�����DATA�׶�
		uint8_t tbuf[1] = { status };
		usbChip.writeBlockToEndpointBuffer(tbuf, 1);
		usbChip.issueToken(0x40, 0x01);
		usbChip.waitForResponse();
		usbChip.readByte();
	}
}

void USBChipBridge::requestRead(uint8_t tog)
{
	// ����һ��IN���񼴿�
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
	// ��������
	usbChip.sync();
	usbChip.writeByte(0x15);
	usbChip.writeByte(0x07); // MODE7, USB host mode, reset bus
	delayMicroseconds(500);
	usbChip.waitForResponse();
	usbChip.readByte();

	// ����USB Hostģʽ
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
	// �����ַ 0x05
	usbChip.sync();
	usbChip.writeByte(0x45);
	usbChip.writeByte(ALLOCATED_USB_ADDR);
	usbChip.waitForResponse();
	if (usbChip.readByte() != USB_INT_SUCCESS)
		return;

	// Ҳ��Ҫ����CH376�����ͨ�ŵ�ַ
	usbChip.sync();
	usbChip.writeByte(0x13);
	usbChip.writeByte(ALLOCATED_USB_ADDR);
}

void USBChipBridge::setConfiguration()
{
	// IKBC G-104ֻ��һ������
	usbChip.sync();
	usbChip.writeByte(0x49);
	usbChip.writeByte(0x01);
	usbChip.waitForResponse();
	usbChip.readByte();
}

uint8_t bsPort[] = { 0x01, 0x11, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };
void USBChipBridge::setPort()
{
	// �������ƴ��䣬�����ݽ׶Σ�ֻ��ҪSETUP�׶Ρ����ö˿�1
	usbChip.writeBlockToEndpointBuffer(bsPort, 8);
	usbChip.issueToken(0x00, 0x0D);
	usbChip.waitForResponse();
	if (usbChip.readByte() != USB_INT_SUCCESS)
		return;
}

uint8_t bsBoot[] = { 0x29, 0x0B, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };
void USBChipBridge::useBootProtocol()
{
	// �������ƴ��䣬�����ݽ׶Σ�ֻ��ҪSETUP�׶Ρ����ö˵�1����ӦIKBC G-104��Boot Keyboard��������򵥵�USB���ĸ�ʽ��
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
	usbChip.disposeReadBuffer(); // �ᷢ��0x2E�������
}

size_t USBChipBridge::available()
{
	return usbChip.available();
}

uint8_t USBChipBridge::readByte()
{
	return usbChip.readByte();
}
