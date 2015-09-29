//*********************************************
#include <reg52.h>
						
#define DBL P0
#define SWITCH_TIME 4000 //默认2s

unsigned char a;
unsigned int pic;
bit delayflag=0;
//*********************************************
void Initial(void);
void Write_Command(unsigned char RegisterName);
void Write_Data(unsigned char RegisterValue);
void fill(unsigned char dat1,unsigned char dat2);
void Delay(unsigned int count);
void pane();
void showpic();
void pictrue();
//************************************************
sbit CS =P2^4;
sbit DC =P2^7;
sbit W_R =P2^6;
sbit R_D =P2^5;
sbit RES =P2^3;

sbit SCLK	 = P0^0;			// Serial Clock Input
sbit SDIN	 = P0^1;				// Serial Data Input

sbit key_stop = P1^0;//按键，按下为0暂停循环，松开为1循环跑动
sbit key_mod = P1^1;//并串模式切换按键，为0串行模式，为1为并行模式，仅在程序开机时判断

bit mod_flag = 1;

//********************************************

void Delay(unsigned int count)
{
	unsigned int i,j;
		for(i=0;i<count;i++)
	for(j=0;j<114;j++);
}

unsigned char code show[]=
{
	//-- 宽度x 高度=128x64 --*
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//吉润电子
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x08,0x48,0x48,0x48,0x48,0x48,0x7E,0x48,0x48,0x48,0x48,0x48,0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x6C,0x0C,0x00,
	0xF6,0x06,0x20,0x24,0xE4,0x24,0x24,0x04,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x90,0x90,0x90,0x90,0xFE,0x90,0x90,0x90,
	0x90,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
	0x82,0x82,0x82,0x82,0x82,0xE2,0xF2,0xBA,0x9E,0x8E,0x82,0x80,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x7E,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x7E,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x1E,0x07,0x00,
	0x7F,0x00,0x08,0x09,0x0F,0x09,0x48,0x40,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0x7F,0x64,0x44,0x44,
	0x44,0x47,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x40,0x40,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//TEL:13530309438
	0x00,0x01,0x7F,0x01,0x01,0x00,0x00,0x00,0x00,0x7F,0x49,0x49,0x41,0x00,0x00,0x00,
	0x00,0x7F,0x40,0x40,0x40,0x00,0x00,0x00,0x00,0x00,0x24,0x00,0x00,0x00,0x00,0x00,
	0x00,0x42,0x7F,0x40,0x00,0x00,0x00,0x00,0x00,0x22,0x49,0x49,0x36,0x00,0x00,0x00,
	0x00,0x4F,0x49,0x49,0x31,0x00,0x00,0x00,0x00,0x22,0x49,0x49,0x36,0x00,0x00,0x00,
	0x00,0x3E,0x41,0x41,0x3E,0x00,0x00,0x00,0x00,0x22,0x49,0x49,0x36,0x00,0x00,0x00,
	0x00,0x3E,0x41,0x41,0x3E,0x00,0x00,0x00,0x00,0x26,0x49,0x49,0x3E,0x00,0x00,0x00,
	0x00,0x38,0x26,0x7F,0x20,0x00,0x00,0x00,0x00,0x22,0x49,0x49,0x36,0x00,0x00,0x00,
	0x00,0x36,0x49,0x49,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //QQ:1903025895
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xA2,0xC2,0xFC,0x00,0x00,0x00,
	0x00,0x7C,0xA2,0xC2,0xFC,0x00,0x00,0x00,0x00,0x00,0x48,0x00,0x00,0x00,0x00,0x00,
	0x00,0x84,0xFE,0x80,0x00,0x00,0x00,0x00,0x00,0x4C,0x92,0x92,0x7C,0x00,0x00,0x00,
	0x00,0x7C,0x82,0x82,0x7C,0x00,0x00,0x00,0x00,0x44,0x92,0x92,0x6C,0x00,0x00,0x00,
	0x00,0x7C,0x82,0x82,0x7C,0x00,0x00,0x00,0x00,0xC4,0xA2,0x92,0x8C,0x00,0x00,0x00,
	0x00,0x9E,0x92,0x92,0x62,0x00,0x00,0x00,0x00,0x6C,0x92,0x92,0x6C,0x00,0x00,0x00,
	0x00,0x4C,0x92,0x92,0x7C,0x00,0x00,0x00,0x00,0x9E,0x92,0x92,0x62,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*
unsigned char code show[]=
{
/*-- 宽度x高度=128x64 ---此代码是显示 Topwin_OLED及电话号码字符
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x60,0x10,0x10,0x10,0xF0,0x10,0x10,0x10,0x60,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x80,0x00,0x00,0x10,0xF0,0x10,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0xF0,0x10,0x10,0x10,0x10,0x10,0x30,
0x40,0x00,0x00,0x10,0xF0,0x10,0x10,0x10,0x10,0x20,0xC0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
0xF8,0x04,0x02,0x02,0x02,0x02,0x04,0xF8,0x00,0x00,0x00,0x02,0xFE,0x04,0x02,0x02,
0x02,0x02,0x04,0xF8,0x00,0x02,0x0E,0xF2,0x00,0xF2,0x0E,0xF2,0x00,0xFA,0x0E,0x02,
0x00,0x00,0x00,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xFE,0x04,0x02,
0x02,0x02,0x02,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x7F,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x7F,0x00,0x00,0x00,0xFF,0x00,
0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0xFF,0x04,0x04,0x04,0x04,0x1F,0x00,
0x80,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0xC0,0x3F,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x02,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0x02,0x02,0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x10,0x1F,0x11,0x02,0x02,
0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
0x00,0x00,0x00,0x02,0x02,0x03,0x02,0x02,0x00,0x00,0x00,0x00,0x02,0x03,0x02,0x00,
0x00,0x00,0x02,0x03,0x02,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
0x00,0x00,0x00,0x00,0x01,0x02,0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x02,0x03,0x02,
0x02,0x02,0x02,0x02,0x03,0x00,0x00,0x00,0x02,0x03,0x02,0x02,0x02,0x02,0x02,0x03,
0x00,0x00,0x00,0x02,0x03,0x02,0x02,0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x03,0x41,0x7F,0x41,0x03,0x00,0x00,0x38,0x54,0x54,0x58,0x00,0x00,0x41,0x7F,0x40,
0x00,0x00,0x00,0x00,0x44,0x00,0x00,0x7C,0x82,0x82,0x7C,0x00,0x00,0x02,0xF2,0x0E,
0x02,0x00,0x00,0x4E,0x8A,0x8A,0x72,0x00,0x00,0x4E,0x8A,0x8A,0x72,0x00,0x08,0x08,
0x08,0x08,0x08,0x00,0x00,0x6C,0x92,0x92,0x6C,0x00,0x00,0x44,0x92,0x92,0x6C,0x00,
0x00,0x6C,0x92,0x92,0x6C,0x00,0x30,0x28,0x24,0xFF,0x20,0x00,0x00,0x0C,0x92,0x92,
0x7C,0x00,0x00,0x02,0xF2,0x0E,0x02,0x00,0x7C,0x92,0x92,0x92,0x60,0x00,0x00,0x7C,
0x82,0x82,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x08,0xF8,0x48,0xE8,0x18,0x00,0x40,0xA0,0xA0,0xC0,0x00,0x00,0x20,0x60,0xA0,0x60,
0x20,0x00,0x00,0x00,0x20,0x00,0x00,0xE0,0x10,0x10,0xE0,0x00,0x00,0x10,0x90,0x70,
0x10,0x00,0x00,0x70,0x50,0x50,0x90,0x00,0x00,0x70,0x50,0x50,0x90,0x00,0x40,0x40,
0x40,0x40,0x40,0x00,0x00,0x60,0x90,0x90,0x60,0x00,0x00,0x20,0x90,0x90,0x60,0x00,
0x00,0x60,0x90,0x90,0x60,0x00,0x80,0x40,0x20,0xF8,0x00,0x00,0x00,0x60,0x90,0x90,
0xE0,0x00,0x80,0x40,0x20,0xF8,0x00,0x00,0x00,0x60,0x90,0x90,0xE0,0x00,0x00,0x20,
0x10,0x90,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x02,0x03,0x02,0x00,0x00,0x00,0x01,0x02,0x02,0x03,0x02,0x00,0x02,0x03,0x02,0x03,
0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x03,0x04,0x04,0x03,0x00,0x00,0x00,0x07,0x00,
0x00,0x00,0x00,0x02,0x04,0x04,0x03,0x00,0x00,0x02,0x04,0x04,0x03,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x03,0x04,0x04,0x03,0x00,0x00,0x02,0x04,0x04,0x03,0x00,
0x00,0x03,0x04,0x04,0x03,0x00,0x01,0x01,0x01,0x07,0x01,0x00,0x00,0x00,0x04,0x04,
0x03,0x00,0x01,0x01,0x01,0x07,0x01,0x00,0x00,0x00,0x04,0x04,0x03,0x00,0x00,0x06,
0x05,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
*/
				// 8-bit 80XX Parallel
void Write_Command_Para(unsigned char RegisterName)
{
	CS=0;
	DC=0;
	DBL=RegisterName;
	W_R=0;
	W_R=1;
	CS=1;
}
//**************************************
void Write_Data_Para(unsigned char RegisterValue)
{
	CS=0;
	DC=1;
	DBL= (unsigned char)RegisterValue;
	W_R=0;
	W_R=1;
	CS=1;
}

				// 4-wire SPI
void Write_Command_SPI(unsigned char Data)
{
unsigned char i;

	CS=0;
	DC=0;
	for (i=0; i<8; i++)
	{
		SCLK=0;
		SDIN=(Data&0x80)>>7;
		Data = Data << 1;
	//	uDelay(1);
		SCLK=1;
	//	uDelay(1);
	}
//	SCLK=0;
	DC=1;
	CS=1;
}


void Write_Data_SPI(unsigned char Data)
{
unsigned char i;

	CS=0;
	DC=1;
	for (i=0; i<8; i++)
	{
		SCLK=0;
		SDIN=(Data&0x80)>>7;
		Data = Data << 1;
	//	uDelay(1);
		SCLK=1;
	//	uDelay(1);
	}
//	SCLK=0;
	DC=1;
	CS=1;
}

void Write_Command(unsigned char RegisterName)
{
	if(mod_flag == 1)
	{
		Write_Command_Para(RegisterName);
	}
	else
	{
		Write_Command_SPI(RegisterName);
	}
	
	//Delay(5);
	
}
//**************************************
void Write_Data(unsigned char RegisterValue)
{
	if(mod_flag == 1)
	{
		Write_Data_Para(RegisterValue);
	}
	else
	{
		Write_Data_SPI(RegisterValue);
	}
	
	//Delay(5);
}

void Initial(void)
{
Write_Command(0xAE); /* display off */
Write_Command(0xB3); /* set osc division */
Write_Command(0x91); /*100 */
Write_Command(0xA8); /* multiplex ratio */
Write_Command(0x3f); /* duty = 1/64 */
Write_Command(0xA2); /* set display offset --- */
Write_Command(0x44); /* 72 */
Write_Command(0xa1); /*set start line */
Write_Command(0x00); /* ------ */
Write_Command(0xA0); /* set segment remap------ */
Write_Command(0x63); /* disable Nibble Remap */
/* Horizontal Address Incremen */
/* Scan from COM[N-1] to COM0 */
/* Enable COM Split Odd Even */
Write_Command(0xAD); /*Set Master Config DC/DC Converter*/
Write_Command(0x02);
Write_Command(0x81); /* set contract current */
Write_Command(0x40);
Write_Command(0x86); /* Set Full Current Range */
Write_Command(0xBE); /* Set High Voltage Level of COM Pin */
Write_Command(0x02); /* 0.53Vref */
Write_Command(0xBF); /*set Low Voltage Level of SEG Pin */
Write_Command(0x0e);
Write_Command(0xA4); /* set display mode */
/* Clear Screen */
Write_Command(0x23); /*set graphic acceleration commmand */
Write_Command(0x01);
Write_Command(0x24); /* draw rectangle */
Write_Command(0x00); /* Gray Scale Level 1 */
Write_Command(0x00); /* Gray Scale Level 3 & 2 */
Write_Command(0x3f); /* Gray Scale Level 3 & 2 */
Write_Command(0x3f); /* Gray Scale Level 3 & 2 */
Write_Command(0x3f); /* Gray Scale Level 3 & 2 0x00*/
Write_Command(0xAF); /* display ON */
}

//void fill(unsigned char dat1,unsigned char dat2)
//{
//unsigned char x,y;
//Write_Command(0x15); /* set column address */
//Write_Command(0x00); /* set column start address */
//Write_Command(0x3f); /* set column end address */
//Write_Command(0x75); /* set row address */
//Write_Command(0x00); /* set row start address */
//Write_Command(0x3f); /* set row end address */
//for(y=0;y<64;y++)
//{
//for(x=0;x<32;x++)
//{
//Write_Data(dat1);
//Write_Data(dat2);
//}
//}
//Delay(2);
//}

//void Clear()
//{
//unsigned char x,y;
//Write_Command(0x15); /* set column address */
//Write_Command(0x00); /* set column start address */
//Write_Command(0x3f); /* set column end address */
//Write_Command(0x75); /* set row address */
//Write_Command(0x00); /* set row start address */
//Write_Command(0x4f); /* set row end address */
//for(y=0;y<64;y++)
//{
//for(x=0;x<80;x++)
//{
//Write_Data(0x00);
//}
//}
//}


//方框
void pane()
{
	unsigned char x,y;
	// row0
	Write_Command(0x15); /* set column address */
	Write_Command(0x00); /* set column start address */
	Write_Command(0x3f); /* set column end address */
	Write_Command(0x75); /* set row address */
	Write_Command(0); /* set row start address */
	Write_Command(0); /* set row end address */
	for(x=0;x<64;x++)
	{
	Write_Data(0xff);
	}
	Delay(2);
	//row 1 - 62
	for(y=1;y<63;y++)
	{
		Write_Command(0x15); /* set column address */
		Write_Command(0x00); /* set column start address */
		Write_Command(0x3f); /* set column end address */
		Write_Command(0x75); /* set row address */
		Write_Command(y); /* set row start address */
		Write_Command(y); /* set row end address */
		Write_Data(0x0f);//低位在前
		for(x=1;x<63;x++)
		{
		Write_Data(0x00);
		}
		Write_Data(0xf0);//低位在前
		Delay(2);
	}
	
	//row 63
	Write_Command(0x15); /* set column address */
	Write_Command(0x00); /* set column start address */
	Write_Command(0x3f); /* set column end address */
	Write_Command(0x75); /* set row address */
	Write_Command(63); /* set row start address */
	Write_Command(63); /* set row end address */
	for(x=0;x<64;x++)
	{
	Write_Data(0xff);
	}
	Delay(2);
}

void pictrue()
{
	unsigned char x,y,z;
	unsigned char value,temp_h,temp_l;
	Write_Command(0x15); /* set column address */
Write_Command(0x00); /* set column start address */
Write_Command(0x3f); /* set column end address */
Write_Command(0x75); /* set row address */
Write_Command(0x00); /* set row start address */
Write_Command(0x3f); /* set row end address */
	
	//x,y确定数组的位置，z确定数组的bit位数
	for(y=0;y<8;y++)
	{
		for(z=0;z<8;z++)
		{
			for(x=0;x<64;x++)
			{
				//value = ((show[y*128 + x*2] & (0x01<<z))>>z)*0x0f + ((show[y*128 + x*2+1] & (0x01<<z))>>z)*0xf0;
				temp_l = show[y*128 + x*2] & (0x01<<z);
				temp_h = show[y*128 + x*2+1] & (0x01<<z);
				if(temp_l > 0) value = 0x0f;
				else value = 0x00;
				if(temp_h > 0) value |= 0xf0;	
				
				Write_Data(value);
			}
		}
	}
}

//竖条纹
void pane2()
{
unsigned char x,y;
Write_Command(0x15); /* set column address */
Write_Command(0x00); /* set column start address */
Write_Command(0x3f); /* set column end address */
Write_Command(0x75); /* set row address */
Write_Command(0x00); /* set row start address */
Write_Command(0x3f); /* set row end address */
for(y=0;y<64;y++)
{
for(x=0;x<64;x++)
{
Write_Data(0x0f);
}
}
Delay(2);
}

//横条纹
void pane1()
{
unsigned char x,y;
Write_Command(0x15); /* set column address */
Write_Command(0x00); /* set column start address */
Write_Command(0x3f); /* set column end address */
Write_Command(0x75); /* set row address */
Write_Command(0x00); /* set row start address */
Write_Command(0x3f); /* set row end address */
for(y=0;y<32;y++)
{
for(x=0;x<64;x++)
{
Write_Data(0xff);
}

for(x=0;x<64;x++)
{
Write_Data(0x00);
}
}
Delay(1);
}

void snow()
{
unsigned char x,y;
Write_Command(0x15); /* set column address */
Write_Command(0x00); /* set column start address */
Write_Command(0x3f); /* set column end address */
Write_Command(0x75); /* set row address */
Write_Command(0x00); /* set row start address */
Write_Command(0x3f); /* set row end address */
for(y=0;y<32;y++)
{
	for(x=0;x<64;x++)
	{
	Write_Data(0x0f);
	}
	for(x=0;x<64;x++)
	{
	Write_Data(0xf0);
	}
}
Delay(1);
}

void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c,unsigned char d)
{
	unsigned char i,j;
	Write_Command(0x15); /* set column address */
	Write_Command(a); /* set column start address */
	Write_Command(b); /* set column end address */
	Write_Command(0x75); /* set row address */
	Write_Command(c); /* set row start address */
	Write_Command(d); /* set row end address */
	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<(b-a+1);j++)
		{
			Write_Data(Data);
			Write_Data(Data);
		}
	}
	Delay(2);
}

void Grayscale()
{
		/* Level 16 => Column 1~8 */
	Fill_Block(0xFF,0x00,0x03,0x00,0x3f);
	/* Level 15 => Column 9~16*/
	Fill_Block(0xEE,0x04,0x07,0x00,0x3f);
	/* Level 14 => Column 17~24*/
	Fill_Block(0xDD,0x08,0x0B,0x00,0x3f);
	/* Level 13 => Column 25~32*/
	Fill_Block(0xCC,0x0C,0x0F,0x00,0x3f);
	/* Level 12 => Column 33~40*/
	Fill_Block(0xBB,0x10,0x13,0x00,0x3f);
	/* Level 11 => Column 41~48*/
	Fill_Block(0xAA,0x14,0x17,0x00,0x3f);
	/* Level 10 => Column 48~56*/
	Fill_Block(0x99,0x18,0x1B,0x00,0x3f);
	/* Level 9 => Column 57~64*/
	Fill_Block(0x88,0x1C,0x1F,0x00,0x3f);
	/* Level 8 => Column 65~72*/
	Fill_Block(0x77,0x20,0x23,0x00,0x3f);
	/* Level 7 => Column 73~80*/
	Fill_Block(0x66,0x24,0x27,0x00,0x3f);
	/* Level 6 => Column 81~86*/
	Fill_Block(0x55,0x28,0x2B,0x00,0x3f);
	/* Level 5 => Column 87~94*/
	Fill_Block(0x44,0x2C,0x2F,0x00,0x3f);
	/* Level 4 => Column 95~104*/
	Fill_Block(0x33,0x30,0x33,0x00,0x3f);
	/* Level 3 => Column 105~112*/
	Fill_Block(0x22,0x34,0x37,0x00,0x3f);
	/* Level 2 => Column 113~120*/
	Fill_Block(0x11,0x38,0x3B,0x00,0x3f);
	/* Level 1 => Column 121~128*/
	Fill_Block(0x00,0x3C,0x3f,0x00,0x3f);
	
}

//***************************************
void main(void)
{
	key_stop = 1;
	key_mod = 1;
	Delay(5);
	RES=0;
	Delay(10);
	RES=1;
	Delay(10);
	if(key_mod == 0) mod_flag = 0;//串行模式
	if(key_mod == 1) mod_flag = 1;//并行模式
	Initial();
	while(1)
	{
//		fill(0xff,0x00); Delay(SWITCH_TIME);  //束条
//		while(key_stop == 0);
//		fill(0x55,0x55); Delay(SWITCH_TIME);  //横条
//		while(key_stop == 0);
//		fill(0x55,0xaa); Delay(SWITCH_TIME);   //雪花
//		while(key_stop == 0);
//		pane(); Delay(SWITCH_TIME);             //方框
//		while(key_stop == 0);
//		pictrue();Delay(SWITCH_TIME);          //字符显示
//		while(key_stop == 0);
		pane2();
		Delay(SWITCH_TIME);
		while(key_stop == 0);
	//	Clear();
		pane1();
		Delay(SWITCH_TIME);
		while(key_stop == 0);
//		Clear();
		snow();
		Delay(SWITCH_TIME);
		while(key_stop == 0);
		Grayscale();
		Delay(SWITCH_TIME);
		while(key_stop == 0);
		pane();
		Delay(SWITCH_TIME);
		while(key_stop == 0);
		pictrue();
		Delay(SWITCH_TIME);
		while(key_stop == 0);
		//Clear();
	}
}

