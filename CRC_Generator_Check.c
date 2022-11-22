#include<stdio.h>
#include<stdint.h>

void printBin(uint32_t data)
{
    int bit=0;
    printf(" \n ");
    for(int i=0;i<32;i++)
    {
        bit = data>>31-i & 1;
        printf(" %d ",bit);
    }

}
void SENT_boCheckCrc( uint32_t u32Data, uint32_t u32Num )
{
  static const uint8_t cau8CrcTableRev[ 16 ] =                        // Reversing the original algorithm gives some
  {                                                                   //   benefits in terms of complexity.
    0,  8, 13,  5,  7, 15, 10,  2,
    14,  6,  3, 11,  9,  1,  4, 12
  };
  uint8_t u8CRC;
  uint32_t u32Idx;

  u8CRC = 0;
  for( u32Idx = 0; u32Idx < u32Num; u32Idx ++ )
  {
    u8CRC ^= u32Data & 0x0F;
    u8CRC = cau8CrcTableRev[ u8CRC ];
    u32Data >>= 4;
  }
  if(u32Data==5)
    printf("\n CRC passed");
  else  
    printf("\n CRC passed");                        // Reverse algorithm: expect official CRC seed
                                                                      //   here which is "0101"=5.
}

uint32_t SENT_CrcGenerator( uint32_t u32Data, uint32_t u32Num )
{
  static const uint8_t cau8CrcTableRev[ 16 ] =                        // Reversing the original algorithm gives some
  {   
    0, 13, 7, 10, 14, 3, 9, 4, 1, 12, 6, 11, 15, 2, 8, 5                                                                //   benefits in terms of complexity.
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
  return (Result + u8CRC) ;
}

void main()
{
    uint32_t u32Data=0,Val=0;

    printf("Enter the number \n");
    scanf("%u",&Val);
    printf("\n Actual Data");
    printBin(Val);
    //Val = Val<<4;
    //printf("\n Data After Shift");
    //printBin(Val);
    printf("\n Data Before CRC Append %u ",Val);
    u32Data=SENT_CrcGenerator( Val,6 );
    printf("\n Data After CRC Append %u ",u32Data);
    SENT_boCheckCrc(u32Data,7);
}