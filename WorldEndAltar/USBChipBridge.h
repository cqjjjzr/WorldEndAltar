#ifndef _USBCHIPBRIDGE_h
#define _USBCHIPBRIDGE_h
#include "USBChipComm.h"

#define USB_INT_SUCCESS 0x14
// ��������CH376ģ��ϸߣ�USB�����ͨ��
 
class USBChipBridge
{
public:
	USBChipBridge();

	// ����ͨ��
	void begin();
	// ��ʼ�����ֲ�����ע��USB�豸
	void initalize();
	
	// ����LED״̬��Num Lock�ȣ�
	void updateLEDStatus(uint8_t status);
	// ������ѯ���������ʵ������CH376��ɣ���ѯ�����CH376���᷵���ж�ֵ��
	void requestRead(uint8_t tog);
	// ��CH376�˵㻺������ȡһ�����ݣ�����0x27���
	size_t readBlock(uint8_t *buf, size_t maxLen);
	// ȡ��CH376��ǰ��ѯ���豸��û�з���ֵ��ʱ��᷵��NAK����CH376�᲻�ϳ���ֱ����ֵ��ȡ����һ������ִ���µ����
	void cancelCurrentNAK();
	
	// ��ȡ�������ֽ�����
	size_t available();
	// ��ȡһ���ֽ�
	uint8_t readByte();
private:
	USBChipComm usbChip;

	// ����USB���߲�����USB Hostģʽ
	void changeMode();
	// �ȴ�USB�豸���루һ���˲����룬��ΪCH376ֱ�Ӻͼ��̺���һ���ˣ�
	void waitForDevice();
	// ����USB��ַ������ͨ��
	void allocateAddress();
	// ����USB����
	void setConfiguration();
	// ���棺����ĺ����е�һЩӲ������ַ����н�������IKBC G-104���̣�����

	// ����USB HID�˿�
	void setPort();
	// ����USB HID Interface��ѡ�� Boot Protocol������������ӵ�Report Protocol���ģ�
	void useBootProtocol();
};

#endif

