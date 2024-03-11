


#ifndef __ZM16x32_H
#define __ZM16x32_H


// ------------------  ������ģ�����ݽṹ���� ------------------------ //
struct  typFNT_ZM32                 // ������ģ���ݽṹ 
{
       u8  Index[3];               // ������������	
       u8   Msk[64];               // ���������� 
};

/////////////////////////////////////////////////////////////////////////
// ������ģ��                                                          //
// ���ֿ�: ����32.dot,����ȡģ���λ,��������:�����Ҵ��ϵ���         //
/////////////////////////////////////////////////////////////////////////
const struct  typFNT_ZM32 codeZM_32c[] =          // ������ĸ
{


///��012�����������ֵ�win  
/*--  ����:  W  --*/
/*--  Script24;  �������¶�Ӧ�ĵ���Ϊ����x��=18x36   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x36  --*/
"0",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x0C,0x36,0x1C,
0x63,0x18,0x63,0x18,0x63,0x38,0x33,0x39,0x1B,0x39,0x03,0x7B,0x03,0x7B,0x03,0x7B,
0x03,0xDE,0x03,0xDE,0x03,0xDE,0x07,0x9C,0x07,0x9C,0x07,0x9C,0x07,0x18,0x07,0x18,
0x07,0x38,0x07,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


/*--  ����:  I  --*/
/*--  Script24;  �������¶�Ӧ�ĵ���Ϊ����x��=18x36   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x36  --*/
"1",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x03,0x60,
0x46,0x60,0x46,0x60,0xC6,0x60,0x86,0x60,0x86,0x60,0x03,0x60,0x03,0x60,0x03,0x60,
0x03,0xC0,0x01,0xC0,0x01,0xC0,0x00,0xC0,0x39,0xC0,0x61,0xE0,0x61,0x80,0x63,0x00,
0x63,0x00,0x66,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


/*--  ����:  N  --*/
/*--  Script24;  �������¶�Ӧ�ĵ���Ϊ����x��=18x36   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x36  --*/
"2",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x7C,0x36,0xCC,
0x63,0xC6,0x63,0x86,0x63,0x86,0x33,0x86,0x1B,0x06,0x07,0x06,0x07,0x0C,0x07,0x0C,
0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x0C,0x18,0x0C,0x18,0x0C,0x19,0x0C,0x19,
0x0C,0x1B,0x18,0x1F,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,



/*--  ����24;  �������¶�Ӧ�ĵ���Ϊ����x��=16x33   --*/
" ",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,



/*--  ����:  G  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"G",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFC,0x0F,0xFC,
0x1E,0x1E,0x3C,0x0E,0x38,0x0E,0x78,0x06,0x78,0x00,0x78,0x00,0x70,0x00,0x70,0x00,
0x70,0x00,0x70,0x00,0x70,0x3F,0x78,0x1F,0x78,0x0E,0x78,0x0E,0x38,0x0E,0x3C,0x0E,
0x1E,0x1E,0x0F,0xBC,0x07,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  A  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"A",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC0,0x03,0xC0,0x03,0xC0,
0x03,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x06,0xF0,0x0E,0xF0,0x0E,0xF0,0x0E,0x70,
0x1C,0x70,0x1C,0x78,0x1F,0xF8,0x1C,0x78,0x38,0x38,0x38,0x3C,0x38,0x3C,0x38,0x3C,
0x70,0x3C,0x78,0x1E,0xFC,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  M  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/

"M",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x1F,0x7C,0x1F,
0x3C,0x1E,0x3E,0x3E,0x3E,0x3E,0x3E,0x3E,0x3E,0x3E,0x3E,0x7E,0x3F,0x7E,0x3F,0x7E,
0x3F,0x7E,0x37,0xEE,0x37,0xEE,0x37,0xEE,0x37,0xEE,0x37,0xCE,0x33,0xCE,0x33,0xCE,
0x33,0xCE,0x7B,0x9F,0xFF,0xBF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  E  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"E",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFE,0x3E,0x1E,
0x1C,0x0F,0x1C,0x07,0x1C,0x03,0x1C,0x00,0x1C,0x18,0x1C,0x18,0x1C,0x38,0x1F,0xF8,
0x1C,0x78,0x1C,0x38,0x1C,0x18,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x03,0x1C,0x07,
0x1C,0x0F,0x3E,0x3E,0x7F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  O  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"O",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xF0,0x0F,0x78,
0x1C,0x1C,0x3C,0x1E,0x38,0x0E,0x78,0x0F,0x78,0x0F,0x78,0x0F,0x70,0x07,0x70,0x07,
0x70,0x07,0x70,0x07,0x70,0x07,0x78,0x0F,0x78,0x0F,0x78,0x0F,0x38,0x0E,0x3C,0x1E,
0x1C,0x1C,0x0F,0x78,0x07,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  V  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"V",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x1F,0x3C,0x0F,
0x1C,0x0E,0x1E,0x0E,0x1E,0x0E,0x1E,0x0C,0x0F,0x1C,0x0F,0x1C,0x0F,0x1C,0x0F,0x18,
0x07,0xB8,0x07,0xB8,0x07,0xB8,0x03,0xF0,0x03,0xF0,0x03,0xF0,0x03,0xF0,0x01,0xE0,
0x01,0xE0,0x01,0xE0,0x01,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  R  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"R",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xF8,0x3E,0x7C,
0x1C,0x1E,0x1C,0x1E,0x1C,0x0E,0x1C,0x0E,0x1C,0x1E,0x1C,0x1E,0x1C,0x7C,0x1F,0xF8,
0x1D,0xE0,0x1D,0xF0,0x1C,0xF0,0x1C,0xF0,0x1C,0xF8,0x1C,0x78,0x1C,0x7C,0x1C,0x3C,
0x1C,0x3C,0x3E,0x3E,0x7F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  C  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"C",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFE,0x0F,0xBE,
0x1E,0x0F,0x3C,0x07,0x3C,0x07,0x78,0x03,0x78,0x00,0x78,0x00,0x70,0x00,0x70,0x00,
0x70,0x00,0x70,0x00,0x70,0x00,0x78,0x00,0x78,0x00,0x78,0x03,0x38,0x07,0x3C,0x07,
0x1C,0x0E,0x0F,0x3C,0x07,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  L  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"L",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x3E,0x00,
0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,
0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x03,0x1C,0x07,
0x1C,0x07,0x3E,0x1F,0x7F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


/*--  ����:  N  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"N",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x3F,0x7E,0x1E,
0x3E,0x0C,0x3E,0x0C,0x3F,0x0C,0x3F,0x0C,0x3F,0x8C,0x37,0x8C,0x37,0xCC,0x33,0xCC,
0x33,0xEC,0x31,0xEC,0x31,0xFC,0x30,0xFC,0x30,0xFC,0x30,0x7C,0x30,0x7C,0x30,0x3C,
0x30,0x3C,0x78,0x1C,0xFC,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


/*--  ����:  W  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"W",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x79,0xE7,
0x79,0xC7,0x79,0xC7,0x79,0xE6,0x79,0xEE,0x79,0xEE,0x39,0xEE,0x3F,0xEE,0x3F,0xFE,
0x3F,0xFC,0x3F,0xFC,0x3F,0xFC,0x1F,0xFC,0x1F,0x7C,0x1F,0x78,0x1E,0x78,0x1E,0x78,
0x1E,0x78,0x1E,0x78,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


/*--  ����:  S  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"S",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFE,0x1F,0x7E,
0x3C,0x1E,0x3C,0x0E,0x38,0x06,0x38,0x00,0x3C,0x00,0x3E,0x00,0x1F,0x80,0x0F,0xF0,
0x03,0xF8,0x00,0xFE,0x00,0x3F,0x00,0x0F,0x00,0x0F,0x30,0x07,0x38,0x07,0x38,0x0F,
0x3C,0x0F,0x1F,0xBE,0x1F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


/*--  ����:  U  --*/
/*--  ������24;  �������¶�Ӧ�ĵ���Ϊ����x��=17x33   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=24x33  --*/
"U",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x3F,0x7C,0x1E,
0x38,0x0C,0x38,0x0C,0x38,0x0C,0x38,0x0C,0x38,0x0C,0x38,0x0C,0x38,0x0C,0x38,0x0C,
0x38,0x0C,0x38,0x0C,0x38,0x0C,0x38,0x0C,0x38,0x0C,0x38,0x0C,0x38,0x0C,0x3C,0x1C,
0x3C,0x1C,0x1F,0x78,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};



//16x32ϸ����ĸ
const struct  typFNT_ZM32 codeZM_32x[] =          // ���ݱ� 
{

/*--  ����:  N  --*/
/*--  ������22;  �������¶�Ӧ�ĵ���Ϊ����x��=15x29   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=16x29  --*/
"N",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x7C,
0x78,0x18,0x3C,0x10,0x3C,0x10,0x2E,0x10,0x2E,0x10,0x27,0x10,0x27,0x10,0x27,0x90,
0x23,0x90,0x23,0x90,0x21,0xD0,0x21,0xD0,0x20,0xF0,0x20,0xF0,0x20,0x70,0x20,0x70,
0x30,0x30,0xF8,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  E  --*/
/*--  ������22;  �������¶�Ӧ�ĵ���Ϊ����x��=15x29   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=16x29  --*/
"E",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xF8,
0x3C,0x3C,0x18,0x0C,0x18,0x04,0x18,0x00,0x18,0x20,0x18,0x20,0x18,0x60,0x1F,0xE0,
0x18,0x60,0x18,0x60,0x18,0x20,0x18,0x00,0x18,0x00,0x18,0x04,0x18,0x0C,0x18,0x0C,
0x3C,0x3C,0x7F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  W  --*/
/*--  ������22;  �������¶�Ӧ�ĵ���Ϊ����x��=15x29   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=16x29  --*/
"W",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFB,0xDE,
0x73,0x8C,0x71,0x8C,0x71,0x8C,0x71,0xCC,0x71,0xC8,0x31,0xC8,0x3B,0xD8,0x3B,0xD8,
0x3B,0xF8,0x3A,0xF8,0x3E,0xF8,0x1E,0xF0,0x1E,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,
0x1C,0x70,0x08,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  G  --*/
/*--  ������22;  �������¶�Ӧ�ĵ���Ϊ����x��=15x29   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=16x29  --*/
"G",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xF0,
0x1E,0xF0,0x18,0x30,0x38,0x30,0x30,0x10,0x70,0x00,0x70,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0xFC,0x70,0x78,0x70,0x30,0x70,0x30,0x30,0x30,0x38,0x30,
0x1E,0xF0,0x0F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  A  --*/
/*--  ������22;  �������¶�Ӧ�ĵ���Ϊ����x��=15x29   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=16x29  --*/
"A",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x80,
0x07,0x80,0x07,0x80,0x07,0x80,0x0D,0xC0,0x0D,0xC0,0x0D,0xC0,0x08,0xC0,0x18,0xE0,
0x18,0xE0,0x18,0xE0,0x1F,0xE0,0x30,0x70,0x30,0x70,0x30,0x70,0x20,0x30,0x60,0x38,
0x60,0x38,0xF8,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  M  --*/
/*--  ������22;  �������¶�Ӧ�ĵ���Ϊ����x��=15x29   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=16x29  --*/
"M",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x3E,
0x78,0x3C,0x38,0x38,0x3C,0x78,0x3C,0x78,0x3C,0x78,0x3C,0xD8,0x2E,0xD8,0x2E,0xD8,
0x2E,0xD8,0x2F,0xD8,0x27,0x98,0x27,0x98,0x27,0x98,0x27,0x98,0x27,0x18,0x23,0x18,
0x73,0x3C,0xFB,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  E  --*/
/*--  ������22;  �������¶�Ӧ�ĵ���Ϊ����x��=15x29   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=16x29  --*/
"E",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xF8,
0x3C,0x3C,0x18,0x0C,0x18,0x04,0x18,0x00,0x18,0x20,0x18,0x20,0x18,0x60,0x1F,0xE0,
0x18,0x60,0x18,0x60,0x18,0x20,0x18,0x00,0x18,0x00,0x18,0x04,0x18,0x0C,0x18,0x0C,
0x3C,0x3C,0x7F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  R  --*/
/*--  ������22;  �������¶�Ӧ�ĵ���Ϊ����x��=15x29   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=16x29  --*/
"R",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xE0,
0x3C,0xF8,0x18,0x38,0x18,0x18,0x18,0x18,0x18,0x38,0x18,0x38,0x18,0xF0,0x1F,0xE0,
0x19,0xC0,0x19,0xC0,0x19,0xE0,0x18,0xE0,0x18,0xE0,0x18,0x70,0x18,0x70,0x18,0x70,
0x3C,0x38,0x7E,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  U  --*/
/*--  ������22;  �������¶�Ӧ�ĵ���Ϊ����x��=15x29   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=16x29  --*/
"U",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x7C,
0x78,0x38,0x30,0x10,0x30,0x10,0x30,0x10,0x30,0x10,0x30,0x10,0x30,0x10,0x30,0x10,
0x30,0x10,0x30,0x10,0x30,0x10,0x30,0x10,0x30,0x10,0x30,0x10,0x30,0x30,0x38,0x30,
0x1C,0xE0,0x0F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  S  --*/
/*--  ������22;  �������¶�Ӧ�ĵ���Ϊ����x��=15x29   --*/
/*--  ���Ȳ���8�ı������ֵ���Ϊ������x�߶�=16x29  --*/
"S",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xA0,
0x39,0xE0,0x70,0x60,0x70,0x60,0x60,0x20,0x70,0x00,0x70,0x00,0x7C,0x00,0x1F,0x00,
0x0F,0xC0,0x03,0xE0,0x00,0xF0,0x00,0x70,0x40,0x30,0x40,0x30,0x60,0x70,0x70,0x70,
0x7D,0xE0,0x7F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


/*--  ����24;  �������¶�Ӧ�ĵ���Ϊ����x��=16x33   --*/
" ",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

 	
};

#endif