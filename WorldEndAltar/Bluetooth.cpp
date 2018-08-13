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
	// ��ͷ
	_stream->write(static_cast<uint8_t>(0xFD));
	_stream->write(static_cast<uint8_t>(0x09)); // ���Ⱥ����9
	_stream->write(static_cast<uint8_t>(0x01)); // ��һ�������ֽ�
	for (uint8_t i = 0; i < HID_NBKEYS + 2; ++i) // ����HIDЭ����ʵ���Ƕ�USB HIDЭ��İ�װ�����Ը�ʽͨ�ã�ֱ��д��ȥ����
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
	while (_stream->read() != 0xFE && _stream->available() > 0); // 0xFE Ϊ������ģ���������
	delayMicroseconds(500);
	if (_stream->available() > 0)
	{
		size_t len = _stream->read(); // ��һ�ֽ�Ϊ����
		delayMicroseconds(500);
		
		return _stream->readBytes(buf, min(len, maxLen));
	}
	return 0;
}
