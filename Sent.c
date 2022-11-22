#include<stdint.h>
#include<stdio.h>
#include<conio.h>

int offset=12;
uint32_t DataSent=0;
int SENTData[]={0,0,0,0,0,0,0,0,0,0};

union SENTDATAFRAME{
	struct{
			int SynchPulse; // This is SynchroNysastion Pulse           idx=0
			int StatsPulse; //  //Nibble 0                              idx=1
			int Data1;			//Nibble 1 - Max Val allowed is - 15    idx=2
			int Data2;			//Nibble 2 - Max Val allowed is - 15    idx=3
			int Data3;			//Nibble 3 - Max Val allowed is - 15    idx=4
			int Data4;			//Nibble 4 - Max Val allowed is - 15    idx=5
			int Data5;			//Nibble 5 - Max Val allowed is - 15    idx=6
			int Data6;			//Nibble 6 - Max Val allowed is - 15    idx=7
			int Crc;			//Nibble 7 - Max Val allowed is - 15    idx=8
	      }SentDataSet;
	int SentFrame[9];        // Complete Sent Data
};
union SENTDATAFRAME Data;

uint8_t SENT_CrcGenerator( uint32_t u32Data, uint32_t u32Num );
void PressureAndTempScale(float pressure,float temp,int mode);
void GetCrcGenerate(void);
void SENTProtocolUpdate(void);

void PrintStruct(void)
{
    printf("\n Data.SentFrame = [ ");
    for(int i=1;i<=8;i++)
    {
        printf("%d ",Data.SentFrame[i]);
    }
    printf("]");
}


uint8_t SENT_CrcGenerator( uint32_t u32Data, uint32_t u32Num )
{
  static const uint8_t cau8CrcTableRev[ 16 ] =                        // Reversing the original algorithm gives some
  {
    0, 13, 7, 10, 14, 3, 9, 4, 1, 12, 6, 11, 15, 2, 8, 5             //   benefits in terms of complexity.
  };
  uint8_t u8CRC;
  uint32_t u32Idx , Result=u32Data;

  u8CRC = 5;
  for( u32Idx = 0; u32Idx < u32Num; u32Idx ++ )
  {
    uint8_t crcDatum = (u32Data>>24) & 0x0F;

    u8CRC = cau8CrcTableRev[ u8CRC ];

    u8CRC = u8CRC^crcDatum;

    u32Data <<= 4;
  }
  u8CRC = cau8CrcTableRev[ u8CRC ];
  printf("\n CRC = %u ",u8CRC);
  return u8CRC ;
}
void PressureAndTempScale(float pressure,float temp,int mode)
{
	//For 50bar pressure k=74.06 and c=193     Counts= p x k + c
	float k=74.06f, c=193.0f;
	k =(mode==0)?74.06f:(mode==1)?13.225f:(mode==2)?8.8167f:6.1717f;
	int countP=0,countT=0;
	countP = (int)(pressure * k + c);
	//For Temperature
	countT=(int)((temp + 73.025)/0.125f);
	//Pressure

	Data.SentDataSet.Data1=0xF & countP>>8;
	Data.SentDataSet.Data2=0xF & countP>>4;
	Data.SentDataSet.Data3=0xF & countP>>0;

	//Temperature
	Data.SentDataSet.Data4=0xF & countT>>8;
	Data.SentDataSet.Data5=0xF & countT>>4;
	Data.SentDataSet.Data6=0xF & countT>>0;

	//Updating Uint32 SENT data word
	//GetCrcGenerate(); // CRC generated
	//SENTProtocolUpdate(); // Update the Array
}
void GetCrcGenerate(void)
{
	Data.SentDataSet.StatsPulse=9;
	Data.SentDataSet.Crc=0;
    PrintStruct();
	for(int i=1;i<9;i++)
		{
			DataSent<<=4;
			DataSent |= (0xF & Data.SentFrame[i]);
		}
    printf("%u",DataSent);
	Data.SentDataSet.Crc=(int)SENT_CrcGenerator(DataSent,6); // CRC is generation
}
void SENTProtocolUpdate(void)
{
	for(int i=0;i<9;i++)
	{
		SENTData[i]=(Data.SentFrame[i] + offset)*4;
	}
	SENTData[8]=0;
}

void main()
{

    float press=0.0f,temp=0.0f;
    printf("This is to Generate SENT Signal");
    printf("\n Enter Pressure -> ");
    scanf("%f",&press);
    printf("\n Enter temp -> ");
    scanf("%f",&temp);
    PressureAndTempScale(press,temp,0);
    GetCrcGenerate();
    

    printf("\n SENTData = [ ");
    for(int i=0;i<=7;i++)
    {
        printf("%d ",SENTData[i]);
    }
    printf("]");
    printf("\n Data Value is= %u",DataSent);

}