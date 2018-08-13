#ifndef _BLUETOOTH_h
#define _BLUETOOTH_h

#include "Arduino.h"

#define HID_NBKEYS 6

/**
 * �������ڴ���RN-42����ģ��
 */
class Bluetooth
{
public:
	Bluetooth(uint32_t baudRate);
	~Bluetooth();

	/**
	 * ���±��ģ����Ͱ�������
	 */
	void updateKeyboard(uint8_t* buf) const;

	/**
	 * ���ڻ�ȡ���������ȡ���ȡ���ģ�ֻ������LED���ݣ�����������
	 */
	size_t available() const;
	size_t readBlock(byte* buf, size_t maxLen) const;
private:
	HardwareSerial * _stream = &Serial; // ��������ģ�����TXD,RXD����ϣ�����ֱ��ʹ��Ӳ������
};

#endif

