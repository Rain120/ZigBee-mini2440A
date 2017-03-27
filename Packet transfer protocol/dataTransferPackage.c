typedef union dataTansferPackage
{
	unsigned char databuf[18];
	struct RFRXBUF
	{
		unsigned char dataHead[2];          //head
		unsigned char dataType[3];          //type
		unsigned char dataNWK[4];         //NWK
		unsigned char dataParentNWK[4];          //pNWK
		unsigned char dataSensorValue[4];   //sensorValue
		unsigned char dataTail;             //tail
	}dataBUF;
}dataRFTX;