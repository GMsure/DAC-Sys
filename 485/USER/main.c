#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "rs485.h"
#include "stdio.h"
//#include "print.h"


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
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	RS485_Init(9600);	//初始化RS485为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	LCD_Init();			   	//初始化LCD 	
	KEY_Init();				//按键初始化		 
	

 							
	
	while(1)
	{
		key=KEY_Scan(0);
		if(1!=0){//if
			for(i=0;i<9;i++)
			{
				rs485buf[i]=cnt+i;//填充发送缓冲区
 			}	
			RS485_Send_EQuiry(cir++);			
			delay_ms(539);
		}	

		RS485_Receive_Data(rs485buf,&key);		
		
		if(key)//接收到有数据
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
