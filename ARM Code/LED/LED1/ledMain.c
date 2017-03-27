
#define rGPBCON (*(unsigned int *)0x56000010)
#define rGPBDAT (*(unsigned int *)0x56000014)

void delay(int loops);
void ledMain(void);
void marqueen(void);

void light2dark(void);//亮--暗
void twoLight2twoDarkCentre(void);//中间两个亮，两个暗
void twoLight2twoDarkOuter(void);//两个亮，中间两个暗

void lightOrder(void);//顺序变亮
void lightOpposite(void);//逆序变亮

void darkOrder(void);//顺序变暗
void darkOpposite(void);//逆序变暗

void ledMain(void){

	//rGPBCON = 0x400;
	///rGPBDAT = 0xffffffdf;
	
	//读 - 改 - 写
	//rGPBCON = rGPBCON &(~(3<<5)) | (1<<10)
	//rGPBCON |= (1<<10) | (1<<12) | (1<<14) | (1<<16);
	//rGPBDAT &= (~(1<<5)) & (~(1<<6)) & (~(1<<7)) & (~(1<<8));  //亮
	
	marqueen();
	
	/*while(1){
		//亮
		rGPBDAT &= (~(1<<5)) & (~(1<<6)) & (~(1<<7)) & (~(1<<8));
		//延时
		delay(0x05000000);
		//暗
 	    rGPBDAT |= (1<<5) | (1<<6) | (1<<7) | (1<<8);
 	    //延时
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
		//LED1亮
		rGPBCON |= (1<<10);
		rGPBDAT &= (~(1<<5));
		//延时
		delay(0x01000000);
		rGPBDAT |= (1<<5);
			
		//LED2亮
		rGPBCON |= (1<<12);
 	    rGPBDAT &= (~(1<<6));
 	    //延时
		delay(0x01000000);
		rGPBDAT |= (1<<6);
				
		//LED3亮
		rGPBCON |= (1<<14);
		rGPBDAT &= (~(1<<7));
		//延时
		delay(0x01000000);
		rGPBDAT |= (1<<7);
				
		//LED4亮
		rGPBCON |= (1<<16);
		rGPBDAT &= (~(1<<8));
		//延时
		delay(0x01000000);
		rGPBDAT |= (1<<8);
}

void twoLight2twoDarkCentre(void){
		//LED2,3亮
		rGPBCON |= (1<<12) | (1<<14);
		rGPBDAT &= (~(1<<6)) & (~(1<<7));
		//延时
		delay(0x01000000);
		rGPBDAT |= (1<<6) | (1<<7);
			
		//LED1,4亮
		rGPBCON |= (1<<10) | (1<<16);
 	    rGPBDAT &= (~(1<<5)) & (~(1<<8));
 	    //延时
		delay(0x01000000);
		rGPBDAT |= (1<<5) | (1<<8);

}

void twoLight2twoDarkOuter(void){	
		//LED1,4亮
		rGPBCON |= (1<<10) | (1<<16);
 	    rGPBDAT &= (~(1<<5)) & (~(1<<8));
 	    //延时
		delay(0x01000000);
		rGPBDAT |= (1<<5) | (1<<8);
		
		//LED2,3亮
		rGPBCON |= (1<<12) | (1<<14);
		rGPBDAT &= (~(1<<6)) & (~(1<<7));
		//延时
		delay(0x01000000);
		rGPBDAT |= (1<<6) | (1<<7);
}

void lightOrder(void){
		//LED1亮
		rGPBCON |= (1<<10);
		rGPBDAT &= (~(1<<5));
		//延时
		delay(0x01000000);
				
		//LED2亮
		rGPBCON |= (1<<12);
 	    rGPBDAT &= (~(1<<6));
 	    //延时
		delay(0x01000000);
				
		//LED3亮
		rGPBCON |= (1<<14);
		rGPBDAT &= (~(1<<7));
		//延时
		delay(0x01000000);
				
		//LED4亮
		rGPBCON |= (1<<16);
		rGPBDAT &= (~(1<<8));
		//延时
		delay(0x01000000);
}

void lightOpposite(void){
		//LED4亮
		rGPBCON |= (1<<16);
		rGPBDAT &= (~(1<<8));
		delay(0x01000000);
		
		//LED3亮
		rGPBCON |= (1<<14);
		rGPBDAT &= (~(1<<7));
		//延时
		delay(0x01000000);
				
		//LED2亮
		rGPBCON |= (1<<12);
 	    rGPBDAT &= (~(1<<6));
 	    //延时
		delay(0x01000000);
		
		//LED1亮
		rGPBCON |= (1<<10);
		rGPBDAT &= (~(1<<5));
		//延时
		delay(0x01000000);
				
				
}

void darkOrder(void){		
		//LED1暗
		rGPBDAT |= (1<<5);
		
		//LED2暗，并延时
		delay(0x01000000);
		rGPBDAT |= (1<<6);
		
		//LED3暗，并延时
		delay(0x01000000);
		rGPBDAT |= (1<<7);
		
		//LED4暗，并延时
		delay(0x01000000);
		rGPBDAT |= (1<<8);
}

void darkOpposite(void){
		//LED4暗
		rGPBDAT |= (1<<8);
		
		//LED3暗，并延时
		delay(0x01000000);
		rGPBDAT |= (1<<7);
		
		//LED2暗，并延时
		delay(0x01000000);
		rGPBDAT |= (1<<6);
		
		//LED1暗，并延时
		delay(0x01000000);
		rGPBDAT |= (1<<5);
		
		//LED4暗，并延时
		delay(0x01000000);
		rGPBDAT |= (1<<8);
}
