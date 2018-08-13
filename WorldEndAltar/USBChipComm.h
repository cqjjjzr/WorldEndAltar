#ifndef USBCHIP_COMM
#define USBCHIP_COMM

#include <SoftwareSerial.h>
#include <Arduino.h>

#define USB_CHIP_DEFAULT_RX 2
#define USB_CHIP_DEFAULT_TX 3

#define CH376_CMD_RET_SUCCESS 0x51
#define CH376_CMD_RET_ABORT   0x5F

/**
 * 本类用于对CH376芯片的底层传输
 */
class USBChipComm
{
public:
	USBChipComm();
	~USBChipComm();

	// 启动传输
	void begin();

	// 直接向模块写入字节
	void writeByte(uint8_t data);
	// 向模块端点缓冲区写入字节（使用0x2C命令）
	void writeBlockToEndpointBuffer(uint8_t* data, size_t size);

	// 直接从模块读取字节
	uint8_t readByte();
	// 直接从模块读取字节块（注意不是从端点缓冲区！！！！）
	size_t readBlock(uint8_t* buf, size_t maxSize);
	// 等待模块响应
	bool waitForResponse();

	// 申请同步令牌，启动传输事务
	void issueToken(uint8_t endplog, uint8_t tokenPID);

	// 用于UART同步标志，每发送一条命令都必须发送0x57 0xAB同步标志
	void sync();
	// 丢弃读取缓冲区中所有数据
	void disposeReadBuffer();
	size_t available();

private:
	SoftwareSerial port = SoftwareSerial(8,9); // 接在8,9针脚上

	const int TIMEOUT_MILLIS = 60000;
	const int CONNECTIVITY_VAL = 0x55;
};

#endif
