#ifndef _BLUETOOTH_h
#define _BLUETOOTH_h

#include "Arduino.h"

#define HID_NBKEYS 6

/**
 * 本类用于处理RN-42蓝牙模块
 */
class Bluetooth
{
public:
	Bluetooth(uint32_t baudRate);
	~Bluetooth();

	/**
	 * 更新报文，发送按键数据
	 */
	void updateKeyboard(uint8_t* buf) const;

	/**
	 * 用于获取缓冲区长度、读取报文（只可能是LED数据，其他丢弃）
	 */
	size_t available() const;
	size_t readBlock(byte* buf, size_t maxLen) const;
private:
	HardwareSerial * _stream = &Serial; // 由于蓝牙模块接在TXD,RXD针脚上，所以直接使用硬件引脚
};

#endif

