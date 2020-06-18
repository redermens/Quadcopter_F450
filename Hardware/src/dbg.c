#include "dbg.h"
#include "cfg_func_f407.h"

void SPI_DATA_ImgUnZip1(unsigned int* img_org, unsigned char* img_out)
{
  unsigned short T_count;
  for(T_count=0;T_count<450;T_count++)
  {
    img_out[2*T_count]   = (unsigned char)((img_org[T_count]&0xff00)>>8); 
    img_out[2*T_count+1] = (unsigned char)(img_org[T_count]&0xff);
  }
}

void SPI_DATA_ImgUnZip0(unsigned char* img_org, unsigned char* img_out)
{
    unsigned short T_count;
    for(T_count=0;T_count<900;T_count++)
    {
      img_out[8*T_count]   =  (img_org[T_count]>>7)&0x01 ;
      img_out[8*T_count+1] =  (img_org[T_count]>>6)&0x01 ;
      img_out[8*T_count+2] =  (img_org[T_count]>>5)&0x01 ;
      img_out[8*T_count+3] =  (img_org[T_count]>>4)&0x01 ;
      img_out[8*T_count+4] =  (img_org[T_count]>>3)&0x01 ;
      img_out[8*T_count+5] =  (img_org[T_count]>>2)&0x01 ;
      img_out[8*T_count+6] =  (img_org[T_count]>>1)&0x01 ;
      img_out[8*T_count+7] =  img_org[T_count]&0x01 ;
    }
}

