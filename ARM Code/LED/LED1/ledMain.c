
#define rGPBCON (*(unsigned int *)0x56000010)
#define rGPBDAT (*(unsigned int *)0x56000014)

void delay(int loops);
void ledMain(void);
void marqueen(void);

void light2dark(void);//��--��
void twoLight2twoDarkCentre(void);//�м���������������
void twoLight2twoDarkOuter(void);//���������м�������

void lightOrder(void);//˳�����
void lightOpposite(void);//�������

void darkOrder(void);//˳��䰵
void darkOpposite(void);//����䰵

void ledMain(void){

	//rGPBCON = 0x400;
	///rGPBDAT = 0xffffffdf;
	
	//�� - �� - д
	//rGPBCON = rGPBCON &(~(3<<5)) | (1<<10)
	//rGPBCON |= (1<<10) | (1<<12) | (1<<14) | (1<<16);
	//rGPBDAT &= (~(1<<5)) & (~(1<<6)) & (~(1<<7)) & (~(1<<8));  //��
	
	marqueen();
	
	/*while(1){
		//��
		rGPBDAT &= (~(1<<5)) & (~(1<<6)) & (~(1<<7)) & (~(1<<8));
		//��ʱ
		delay(0x05000000);
		//��
 	    rGPBDAT |= (1<<5) | (1<<6) | (1<<7) | (1<<8);
 	    //��ʱ
		delay(0x05000000); 
	}*/
}

void delay(int loops){
	while(loops-- > 0);
}

void marqueen(void){
	while(1){
		twoLight2twoDarkCentre();
		lightOrder();
		darkOpposite();
		lightOpposite();
		darkOpposite();
		light2dark();
		twoLight2twoDarkOuter();
	}
}

void light2dark(void){
		//LED1��
		rGPBCON |= (1<<10);
		rGPBDAT &= (~(1<<5));
		//��ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<5);
			
		//LED2��
		rGPBCON |= (1<<12);
 	    rGPBDAT &= (~(1<<6));
 	    //��ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<6);
				
		//LED3��
		rGPBCON |= (1<<14);
		rGPBDAT &= (~(1<<7));
		//��ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<7);
				
		//LED4��
		rGPBCON |= (1<<16);
		rGPBDAT &= (~(1<<8));
		//��ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<8);
}

void twoLight2twoDarkCentre(void){
		//LED2,3��
		rGPBCON |= (1<<12) | (1<<14);
		rGPBDAT &= (~(1<<6)) & (~(1<<7));
		//��ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<6) | (1<<7);
			
		//LED1,4��
		rGPBCON |= (1<<10) | (1<<16);
 	    rGPBDAT &= (~(1<<5)) & (~(1<<8));
 	    //��ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<5) | (1<<8);

}

void twoLight2twoDarkOuter(void){	
		//LED1,4��
		rGPBCON |= (1<<10) | (1<<16);
 	    rGPBDAT &= (~(1<<5)) & (~(1<<8));
 	    //��ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<5) | (1<<8);
		
		//LED2,3��
		rGPBCON |= (1<<12) | (1<<14);
		rGPBDAT &= (~(1<<6)) & (~(1<<7));
		//��ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<6) | (1<<7);
}

void lightOrder(void){
		//LED1��
		rGPBCON |= (1<<10);
		rGPBDAT &= (~(1<<5));
		//��ʱ
		delay(0x01000000);
				
		//LED2��
		rGPBCON |= (1<<12);
 	    rGPBDAT &= (~(1<<6));
 	    //��ʱ
		delay(0x01000000);
				
		//LED3��
		rGPBCON |= (1<<14);
		rGPBDAT &= (~(1<<7));
		//��ʱ
		delay(0x01000000);
				
		//LED4��
		rGPBCON |= (1<<16);
		rGPBDAT &= (~(1<<8));
		//��ʱ
		delay(0x01000000);
}

void lightOpposite(void){
		//LED4��
		rGPBCON |= (1<<16);
		rGPBDAT &= (~(1<<8));
		delay(0x01000000);
		
		//LED3��
		rGPBCON |= (1<<14);
		rGPBDAT &= (~(1<<7));
		//��ʱ
		delay(0x01000000);
				
		//LED2��
		rGPBCON |= (1<<12);
 	    rGPBDAT &= (~(1<<6));
 	    //��ʱ
		delay(0x01000000);
		
		//LED1��
		rGPBCON |= (1<<10);
		rGPBDAT &= (~(1<<5));
		//��ʱ
		delay(0x01000000);
				
				
}

void darkOrder(void){		
		//LED1��
		rGPBDAT |= (1<<5);
		
		//LED2��������ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<6);
		
		//LED3��������ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<7);
		
		//LED4��������ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<8);
}

void darkOpposite(void){
		//LED4��
		rGPBDAT |= (1<<8);
		
		//LED3��������ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<7);
		
		//LED2��������ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<6);
		
		//LED1��������ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<5);
		
		//LED4��������ʱ
		delay(0x01000000);
		rGPBDAT |= (1<<8);
}
