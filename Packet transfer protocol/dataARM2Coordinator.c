typedef union dataARM2Coordinator
{
	unsigned char A2Cbuf[27];
	struct RFRXBUF
	{
		unsigned char A2CHead[2];          //head
		unsigned char A2CType[3];          //type
		unsigned char A2CNWK[4];         //NWK
		unsigned char A2CParentNWK[4];          //pNWK
		unsigned char A2CSensorValue[4];   //sensorValue
		unsigned char A2CTail;             //tail
	}A2CBUF;
}A2CRFTX;