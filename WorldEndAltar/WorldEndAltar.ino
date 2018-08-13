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
	// ��ʼ������
	initPorts();
	bluetooth = new Bluetooth(115200);
	buf = new uint8_t[MAX_READ_EPBUF_LEN];

	// ����CH376оƬ
	digitalWrite(CH376_RST_OPORT, HIGH);
	delay(40);
	digitalWrite(CH376_RST_OPORT, LOW);

	// ��ʼ��CH376оƬ
	usbKeyboard.begin();
	usbKeyboard.initalize();

	// ��ʼ��ѯ���������ʵ������CH376��ɣ���ѯ�����CH376���᷵���ж�ֵ
	usbKeyboard.requestRead(tog);
	flipTog();

	delay(50);
}

void loop()
{
	// ����״̬����
	if (usbKeyboard.available())
	{
		if (usbKeyboard.readByte() == USB_INT_SUCCESS)
		{
			// �ɹ���ȡֵ�����и���
			size_t len = usbKeyboard.readBlock(buf, MAX_READ_EPBUF_LEN);
			if (len == 8)
			{
				// ������Ч������Ϊ8��ʱ���£���Modifier, Reversed, Key1-6 �˸��ֽ�
				bluetooth->updateKeyboard(buf);
			}
			// ������һ�ζ�ȡ����ʼ��ѯ
			usbKeyboard.requestRead(tog);
			flipTog();
		}
	}
	// ����ģ�������ݣ�ֻ������Num Lock�ȼ��̵�״̬��
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
 * ��Ҳ��֪����������Ǹ�ɶ�ġ�_�����Źٷ�����д�Ͷ���
 */
void flipTog()
{
	if (tog == 0x80) tog = 0x00;
	else tog = 0x80;
}