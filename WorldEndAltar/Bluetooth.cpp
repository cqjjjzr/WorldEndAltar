#include "Bluetooth.h"

Bluetooth::Bluetooth(uint32_t baudRate)
{
	_stream->begin(baudRate);
}

Bluetooth::~Bluetooth()
{
}

void Bluetooth::updateKeyboard(uint8_t* buf) const
{
	// 报头
	_stream->write(static_cast<uint8_t>(0xFD));
	_stream->write(static_cast<uint8_t>(0x09)); // 长度恒等于9
	_stream->write(static_cast<uint8_t>(0x01)); // 第一个保留字节
	for (uint8_t i = 0; i < HID_NBKEYS + 2; ++i) // 蓝牙HID协议其实就是对USB HID协议的包装，所以格式通用，直接写进去即可
	{
		_stream->write(buf[i]);
	}
}

size_t Bluetooth::available() const
{
	return _stream->available();
}

size_t Bluetooth::readBlock(byte* buf, size_t maxLen) const
{
	while (_stream->read() != 0xFE && _stream->available() > 0); // 0xFE 为输出报文，其他丢弃
	delayMicroseconds(500);
	if (_stream->available() > 0)
	{
		size_t len = _stream->read(); // 第一字节为长度
		delayMicroseconds(500);
		
		return _stream->readBytes(buf, min(len, maxLen));
	}
	return 0;
}
