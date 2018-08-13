// 整个文件都很简单，没什么可以写的
#include "USBChipComm.h"

USBChipComm::USBChipComm()
{
}

USBChipComm::~USBChipComm()
{
}

void USBChipComm::begin()
{
	port.begin(9600L);
}

void USBChipComm::writeByte(uint8_t data)
{
	port.write(data);
}

void USBChipComm::writeBlockToEndpointBuffer(uint8_t* data, size_t size)
{
	sync();
	port.write(0x2C);
	port.write(size);
	port.write(data, size);
}

uint8_t USBChipComm::readByte()
{
	return port.available() ? port.read() : uint8_t(0x00);
}

size_t USBChipComm::readBlock(uint8_t* buf, size_t maxSize)
{
	return port.readBytes(buf, maxSize);
}

bool USBChipComm::waitForResponse()
{
	while (!port.available());
	return true;
}

void USBChipComm::issueToken(uint8_t endplog, uint8_t tokenPID)
{
	sync();
	port.write(0x4E);
	port.write(endplog);
	port.write(tokenPID);
}

void USBChipComm::sync()
{
	port.write(0x57);
	port.write(0xAB);
}

void USBChipComm::disposeReadBuffer()
{
	delay(5);
	while (port.available())
		port.read();
}

size_t USBChipComm::available()
{
	return port.available();
}
