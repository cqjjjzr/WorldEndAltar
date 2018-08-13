#ifndef USBCHIP_COMM
#define USBCHIP_COMM

#include <SoftwareSerial.h>
#include <Arduino.h>

#define USB_CHIP_DEFAULT_RX 2
#define USB_CHIP_DEFAULT_TX 3

#define CH376_CMD_RET_SUCCESS 0x51
#define CH376_CMD_RET_ABORT   0x5F

/**
 * �������ڶ�CH376оƬ�ĵײ㴫��
 */
class USBChipComm
{
public:
	USBChipComm();
	~USBChipComm();

	// ��������
	void begin();

	// ֱ����ģ��д���ֽ�
	void writeByte(uint8_t data);
	// ��ģ��˵㻺����д���ֽڣ�ʹ��0x2C���
	void writeBlockToEndpointBuffer(uint8_t* data, size_t size);

	// ֱ�Ӵ�ģ���ȡ�ֽ�
	uint8_t readByte();
	// ֱ�Ӵ�ģ���ȡ�ֽڿ飨ע�ⲻ�ǴӶ˵㻺��������������
	size_t readBlock(uint8_t* buf, size_t maxSize);
	// �ȴ�ģ����Ӧ
	bool waitForResponse();

	// ����ͬ�����ƣ�������������
	void issueToken(uint8_t endplog, uint8_t tokenPID);

	// ����UARTͬ����־��ÿ����һ��������뷢��0x57 0xABͬ����־
	void sync();
	// ������ȡ����������������
	void disposeReadBuffer();
	size_t available();

private:
	SoftwareSerial port = SoftwareSerial(8,9); // ����8,9�����

	const int TIMEOUT_MILLIS = 60000;
	const int CONNECTIVITY_VAL = 0x55;
};

#endif
