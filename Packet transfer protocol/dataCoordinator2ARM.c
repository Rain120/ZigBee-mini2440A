typedef union dataCoordinator2ARM
{
	unsigned char C2Abuf[10];
	struct RFRXBUF
	{
		unsigned char C2AHead[2];          //head
		unsigned char C2AType[3];          //type
		unsigned char C2ANWK[4];         //NWK
		unsigned char C2AParentNWK[4];          //pNWK
		unsigned char C2ASensorValue[4];   //sensorValue
		unsigned char C2ATail;             //tail
	}C2ABUF;
}C2ARFTX;