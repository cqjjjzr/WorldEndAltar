#ifndef _USBCHIPBRIDGE_h
#define _USBCHIPBRIDGE_h
#include "USBChipComm.h"

#define USB_INT_SUCCESS 0x14
// 本类用于CH376模块较高（USB）层次通信
 
class USBChipBridge
{
public:
	USBChipBridge();

	// 启动通信
	void begin();
	// 初始化各种参数，注册USB设备
	void initalize();
	
	// 更新LED状态（Num Lock等）
	void updateLEDStatus(uint8_t status);
	// 启动轮询（这个过程实际上由CH376完成，轮询到结果CH376将会返回中断值）
	void requestRead(uint8_t tog);
	// 从CH376端点缓冲区读取一组数据（调用0x27命令）
	size_t readBlock(uint8_t *buf, size_t maxLen);
	// 取消CH376当前轮询（设备在没有返回值的时候会返回NAK，而CH376会不断尝试直到有值，取消这一尝试以执行新的命令）
	void cancelCurrentNAK();
	
	// 获取缓冲区字节数量
	size_t available();
	// 读取一个字节
	uint8_t readByte();
private:
	USBChipComm usbChip;

	// 重置USB总线并启动USB Host模式
	void changeMode();
	// 等待USB设备接入（一般会瞬间接入，因为CH376直接和键盘焊在一起了）
	void waitForDevice();
	// 分配USB地址，启动通信
	void allocateAddress();
	// 设置USB配置
	void setConfiguration();
	// 警告：下面的函数中的一些硬编码的字符序列仅适用于IKBC G-104键盘！！！

	// 设置USB HID端口
	void setPort();
	// 设置USB HID Interface并选择 Boot Protocol（避免解析复杂的Report Protocol报文）
	void useBootProtocol();
};

#endif

