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
	u8 rs485buf[8]    = { 0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0x0B }; //��ʪ�Ȳ�ѯ֡ 01 03 00 00 00 02 c4 0b
	u8 rs485bufNH3[8] = { 0x02,0x03,0x00,0x01,0x00,0x01,0xD5,0xF9 }; //������ѯ֡ 02 03 00 01 00 01 d5 f9
	u8 rs485bufCO2[8] = { 0x03,0x03,0x00,0x02,0x00,0x01,0x24,0x28 };//������̼��ѯ֡ 03 03 00 02 00 01 24 28
	//01 06 07 d1 00 02 59 46 �޸Ĳ�����
	//01 06 07 d0 00 02 08 86 �޸ĵ�ַ
	//01 06 07 d0 00 03 c9 46
	u16 flag = 0;	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	RS485_Init(9600);	//��ʼ��RS485Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD 	
	KEY_Init();				//������ʼ��		 
	

 							
	
	while(1)
	{
		key=KEY_Scan(0);
		if(1!=0){//if
			for(i=0;i<9;i++)
			{
				rs485buf[i]=cnt+i;//��䷢�ͻ�����
 			}	
			RS485_Send_EQuiry(cir++);			
			delay_ms(539);
		}	

		RS485_Receive_Data(rs485buf,&key);		
		
		if(key)//���յ�������
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
