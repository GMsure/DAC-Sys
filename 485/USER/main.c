#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "rs485.h"
#include "stdio.h"
//#include "print.h"

 
/************************************************
 ALIENTEK精英STM32开发板实验24
 485 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
 				 	void receiveDaat();
					
 int main(void)
 {	 
	u8 key = 66,key1 = 66, key2 = 66, key3 = 66;
	u8 i = 0,t = 0;
	u8 cnt = 0;
	u8 cir = 0; 
	u8 rs485buf[8]    = { 0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0x0B }; //温湿度查询帧 01 03 00 00 00 02 c4 0b
	u8 rs485bufNH3[8] = { 0x02,0x03,0x00,0x01,0x00,0x01,0xD5,0xF9 }; //氨气查询帧 02 03 00 01 00 01 d5 f9
	u8 rs485bufCO2[8] = { 0x03,0x03,0x00,0x02,0x00,0x01,0x24,0x28 };//二氧化碳查询帧 03 03 00 02 00 01 24 28
	//01 06 07 d1 00 02 59 46 修改波特率
	//01 06 07 d0 00 02 08 86 修改地址
	//01 06 07 d0 00 03 c9 46
	u16 flag = 0;	
	// TI = 1; 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	LCD_Init();			   	//初始化LCD 	
	KEY_Init();				//按键初始化		 
	
	RS485_Init(9600);	//初始化RS485
// 	POINT_COLOR=RED;//设置字体为红色 
//	LCD_ShowString(30,50,200,16,16,"ELITE STM32");
//	LCD_ShowString(30,70,200,16,16,"RS485 TEST");	
//	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
//	LCD_ShowString(30,110,200,16,16,"2015/1/15");	
//	LCD_ShowString(30,130,200,16,16,"KEY0:Send");	//显示提示信息		
// 
// 	POINT_COLOR=BLUE;//设置字体为蓝色	  
//	LCD_ShowString(30,150,200,16,16,"Count:");			//显示当前计数值	
//	LCD_ShowString(30,170,200,16,16,"Send Data:");		//提示发送的数据	
//	LCD_ShowString(30,210,200,16,16,"Receive Data:");	//提示接收到的数据		
 							
	
	while(1)
	{
		
		  //delay_ms(3000);
		key=KEY_Scan(0);
		if(1!=0){//if(key!=KEY0_PRES
		//delay_ms(3000);
			for(i=0;i<9;i++)
			{
				rs485buf[i]=cnt+i;//填充发送缓冲区
				//LCD_ShowxNum(30+i*32,190,rs485buf[i],3,16,0X80);	//显示数据
 			}
				//RS485_Send_Data(rs485buf,8);//发送5个字节 		
			RS485_Send_EQuiry(cir++);		
			
			//printf("2233=\n");			
			delay_ms(539);
		}	
		//
		//RS485_Send_EQuiry(cir);		
//		printf("2233= %d \n", cir);
//		
//		if(cir == 1){
//			printf("1111 \n");
//			RS485_Receive_Data(rs485buf,&key1);
//			
//			printf("k1= %d \n", key1);
//		}else if(cir == 2){
//			RS485_Receive_Data(rs485bufNH3,&key2);
//			printf("k2= %d \n", key2);
//		}else if(cir == 3){
//			RS485_Receive_Data(rs485bufCO2,&key3);
//			printf("k3= %d \n", key3);
//		}
//		
//		printf("2222 \n");
		RS485_Receive_Data(rs485buf,&key);

		
		
		
if(key)//接收到有数据
		//if(key1 && key2 && key3 && cir == 3)//接收到有数据
		{
			if(key>12) key = 11;//最大是5个数据.
			if(key1>12) key1 = 11;//最大是5个数据.
			if(key2>12) key2 = 11;//最大是5个数据.
			if(key3>12) key3 = 11;//最大是5个数据.
			//printf("233-");
			for(i=0;i<key;i++){
				printf("%02x ",rs485buf[i]);
				if(!flag) flag = 1;			
			}
// 			for(i=0;i<key1;i++){
//				printf("%02x ",rs485buf[i]);
//				if(!flag) flag = 1;			
//			}
//			printf("+ ");
//			for(i=0;i<key2;i++){
//				printf("%02x ",rs485bufNH3[i]);
//				if(!flag) flag = 1;			
//			}
//			printf("+ ");
//			for(i=0;i<key3;i++){
//				printf("%02x ",rs485bufCO2[i]);
//				if(!flag) flag = 1;			
//			}
		if(cir != 3){
			printf("+ ");
		}else{
			printf("-");
		}
 		}
		cir %= 3;// 0 1 2
		//receiveDaat();
		t++; 
		delay_ms(21);
		if(t==20)
		{
			LED0=!LED0;//提示系统正在运行	
			t=0;
			cnt++;
			if(flag)
				LED1=!LED1;

		}		
			
 	}		
		 
}

void receiveDaat(){
	u16 t;  
	u16 len;
if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			//printf("\r\n您发送的消息为:\r\n\r\n");
			//printf("<<");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			//printf("\n");//插入换行
			USART_RX_STA=0;
		}
}
