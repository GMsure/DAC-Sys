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
 ALIENTEK��ӢSTM32������ʵ��24
 485 ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
 				 	void receiveDaat();
					
 int main(void)
 {	 
	u8 key = 66,key1 = 66, key2 = 66, key3 = 66;
	u8 i = 0,t = 0;
	u8 cnt = 0;
	u8 cir = 0; 
	u8 rs485buf[8]    = { 0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0x0B }; //��ʪ�Ȳ�ѯ֡ 01 03 00 00 00 02 c4 0b
	u8 rs485bufNH3[8] = { 0x02,0x03,0x00,0x01,0x00,0x01,0xD5,0xF9 }; //������ѯ֡ 02 03 00 01 00 01 d5 f9
	u8 rs485bufCO2[8] = { 0x03,0x03,0x00,0x02,0x00,0x01,0x24,0x28 };//������̼��ѯ֡ 03 03 00 02 00 01 24 28
	//01 06 07 d1 00 02 59 46 �޸Ĳ�����
	//01 06 07 d0 00 02 08 86 �޸ĵ�ַ
	//01 06 07 d0 00 03 c9 46
	u16 flag = 0;	
	// TI = 1; 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD 	
	KEY_Init();				//������ʼ��		 
	
	RS485_Init(9600);	//��ʼ��RS485
// 	POINT_COLOR=RED;//��������Ϊ��ɫ 
//	LCD_ShowString(30,50,200,16,16,"ELITE STM32");
//	LCD_ShowString(30,70,200,16,16,"RS485 TEST");	
//	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
//	LCD_ShowString(30,110,200,16,16,"2015/1/15");	
//	LCD_ShowString(30,130,200,16,16,"KEY0:Send");	//��ʾ��ʾ��Ϣ		
// 
// 	POINT_COLOR=BLUE;//��������Ϊ��ɫ	  
//	LCD_ShowString(30,150,200,16,16,"Count:");			//��ʾ��ǰ����ֵ	
//	LCD_ShowString(30,170,200,16,16,"Send Data:");		//��ʾ���͵�����	
//	LCD_ShowString(30,210,200,16,16,"Receive Data:");	//��ʾ���յ�������		
 							
	
	while(1)
	{
		
		  //delay_ms(3000);
		key=KEY_Scan(0);
		if(1!=0){//if(key!=KEY0_PRES
		//delay_ms(3000);
			for(i=0;i<9;i++)
			{
				rs485buf[i]=cnt+i;//��䷢�ͻ�����
				//LCD_ShowxNum(30+i*32,190,rs485buf[i],3,16,0X80);	//��ʾ����
 			}
				//RS485_Send_Data(rs485buf,8);//����5���ֽ� 		
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

		
		
		
if(key)//���յ�������
		//if(key1 && key2 && key3 && cir == 3)//���յ�������
		{
			if(key>12) key = 11;//�����5������.
			if(key1>12) key1 = 11;//�����5������.
			if(key2>12) key2 = 11;//�����5������.
			if(key3>12) key3 = 11;//�����5������.
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
			LED0=!LED0;//��ʾϵͳ��������	
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
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			//printf("\r\n�����͵���ϢΪ:\r\n\r\n");
			//printf("<<");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			//printf("\n");//���뻻��
			USART_RX_STA=0;
		}
}
