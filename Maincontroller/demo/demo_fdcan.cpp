/*
 * demo_fdcan.cpp
 *
 *  Created on: 2022年8月21日
 *      Author: 25053
 */

#include "maincontroller.h"

void fdcan_init(void){//在系统初始化的时候运行一次
	//首先配置CAN接收过滤器
	fdcan1FilterConfig->IdType = FDCAN_STANDARD_ID;
	fdcan1FilterConfig->FilterIndex = 0;
	fdcan1FilterConfig->FilterType = FDCAN_FILTER_MASK;
	fdcan1FilterConfig->FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	fdcan1FilterConfig->FilterID1 = 0x0000;
	fdcan1FilterConfig->FilterID2 = 0x0000; /* For acceptance, MessageID and FilterID1 must match exactly */
	//配置can发送数据的header
	fdcan1TxHeader->Identifier = 0x200;
	fdcan1TxHeader->IdType = FDCAN_STANDARD_ID;
	fdcan1TxHeader->TxFrameType = FDCAN_DATA_FRAME;
	fdcan1TxHeader->DataLength = FDCAN_DLC_BYTES_8;
	fdcan1TxHeader->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	fdcan1TxHeader->BitRateSwitch = FDCAN_BRS_OFF;
	fdcan1TxHeader->FDFormat = FDCAN_CLASSIC_CAN;
	fdcan1TxHeader->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	fdcan1TxHeader->MessageMarker = 0;
	set_fdcan1_recieve_multifold(2);//设置CAN总线接收倍率,倍率数对应于开辟的接收缓存数
	fdcan1_init();//启动fdcan1
	set_fdcan1_Silent(false);//取消fdcan1的静默
}

static uint32_t notify=0;
void fdcan_update(void){//以自己需要的频率循环运行即可
	//CAN数据的接收
	if(get_fdcan1_notification()!=notify){
		notify=get_fdcan1_notification();
		//获取接收到的can header,本demo直接打印了header中的id.由于开辟了两个接收缓存,所以调用的时候需要指明缓存编号为0和1
		usb_printf("id:%x|%x\n",get_fdcan1RxHeader_prt(0)->Identifier, get_fdcan1RxHeader_prt(1)->Identifier);
		//获取接收到的can data,打印了缓存0接收的8字节数据
		usb_printf("data:%d|%d|%d|%d|%d|%d|%d|%d\n",get_fdcan1RxData_prt(0)[0],get_fdcan1RxData_prt(0)[1],get_fdcan1RxData_prt(0)[2],get_fdcan1RxData_prt(0)[3],get_fdcan1RxData_prt(0)[4],get_fdcan1RxData_prt(0)[5],get_fdcan1RxData_prt(0)[6],get_fdcan1RxData_prt(0)[7]);
		//获取接收到的can data,打印了缓存1接收的8字节数据
		usb_printf("data:%d|%d|%d|%d|%d|%d|%d|%d\n",get_fdcan1RxData_prt(1)[0],get_fdcan1RxData_prt(1)[1],get_fdcan1RxData_prt(1)[2],get_fdcan1RxData_prt(1)[3],get_fdcan1RxData_prt(1)[4],get_fdcan1RxData_prt(1)[5],get_fdcan1RxData_prt(1)[6],get_fdcan1RxData_prt(1)[7]);
	}
	//CAN数据的发送
	fdcan1TxHeader->Identifier = 0x200;//配置发送数据id
	fdcan1TxData[0]=5000>>8;
	fdcan1TxData[1]=5000&0xFF;
	fdcan1TxData[2]=5000>>8;
	fdcan1TxData[3]=5000&0xFF;
	fdcan1TxData[4]=5000>>8;
	fdcan1TxData[5]=5000&0xFF;
	fdcan1TxData[6]=5000>>8;
	fdcan1TxData[7]=5000&0xFF;
	fdcan1_send_data();//demo演示了发送8字节数据
}
