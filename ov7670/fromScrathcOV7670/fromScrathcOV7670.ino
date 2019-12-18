#include <Wire.h>

/*Defining OV7670 I2C Address*/
#define camAddress 0x42
/*Defining OV7670 Internal Registers*/
#define GAIN  0x00
#define BLUE  0x01
#define RED   0x02
#define VREF  0x03
#define COM1  0x04
#define BAVE  0x05
#define GbAVE 0x06
#define AECHH 0x07
#define RAVE 0x08
#define COM2 0x09
#define PID 0x0A
#define VER 0x0B
#define COM3 0x0C
#define COM4 0x0D
#define COM5 0x0E
#define COM6 0x0F
#define AECH 0x10
#define CLKRC 0x11
#define COM7 0x12
#define COM8 0x13
#define COM9 0x14
#define COM10 0x15
/*16 Reserved Register*/
#define HSTART 0x17
#define HSTOP 0x18
#define VSTRT 0x19
#define VSTOP 0x1A
#define PSHIFT 0x1B
#define MIDH 0x1C
#define MIDL 0x1D
#define MVFP 0x1E
#define LAEC 0x1F
#define ADCCTR0 0x20
#define ADCCTR1 0x21
#define ADCCTR2 0x22
#define ADCCTR3 0x23
#define AEW 0x24
#define AEB 0x25
#define VPT 0x26
#define BBIAS 0x27
#define GbBIAS 0x28
/*29 Reserved Register*/
#define EXHCH 0x2A
#define EXHCL 0x2B
#define RBIAS 0x2C
#define ADVFL 0x2D
#define ADVFH 0x2E
#define YAVE 0x2F
#define HSYST 0x30
#define HSYEN 0x31
#define HREF 0x32
#define CHLF 0x33
#define ARBLM 0x34
/*35_36 Reserved Registers*/
#define REG_ADC 0x37
#define ACOM 0x38
#define OFON 0x39
#define TSLB 0x3A
#define COM11 0x3B
#define COM12 0x3C
#define COM13 0x3D
#define COM14 0x3E
#define EDGE 0x3F
#define COM15 0x40
#define COM16 0x41
#define COM17 0x42
#define AWBC1 0x43
#define AWBC2 0x44
#define AWBC3 0x45
#define AWBC4 0x46
#define AWBC5 0x47
#define AWBC6 0x48
/*49_4A Reserved Registers*/
#define REG4B 0x4B
#define DNSTH 0x4C
/*4D_4E Reserved Regidsters*/
#define MTX1 0x4F
#define MTX2 0x50
#define MTX3 0x51
#define MTX4 0x52
#define MTX5 0x53
#define MTX6 0x54
#define BRIGTH 0x55
#define CONTRAS 0x56
#define CONTRAS_CENTER 0x57
#define MTXS 0x58
/*59_61 Reserved Registers*/
#define LCC1 0x62
#define LCC2 0x63
#define LCC3 0x64
#define LCC4 0x65
#define LCC5 0x66
#define MANU 0x67
#define MANV 0x68
#define GFIX 0x69
#define GGAIN 0x6A
#define DBLV 0x6B
#define AWBCTR3 0x6C
#define AWBCTR2 0x6D
#define AWBCTR1 0x6E
#define AWBCTR0 0x6F
#define SCALING_XSC 0x70
#define SCALING_YSC 0x71
#define SCALING_DCWCTR 0x72
#define SCALING_PCLK_DIV 0x73
#define REG74 0x74
#define REG75 0x75
#define REG76 0x76
#define REG77 0x77
/*78_79 Reserved Registers*/
#define GAM1 0x7A
#define GAM2 0x7B
#define GAM3 0x7C
#define GAM4 0x7D
#define GAM5 0x7E
#define GAM6 0x7F
#define GAM7 0x80
/*8A_91 Reserved Registers*/
#define DM_LNL 0x92
#define DM_LNH 0x93
#define LCC6 0x94
#define LCC7 0x95
/*96_9C Reserved Registers*/
#define BD50ST 0x9D
#define BD60ST 0x9E
/*9F_AB Reserved Resgisters*/
#define STR_OPT 0xAC
#define STR_R 0xAD
#define STR_G 0xAE
#define STR_B 0xAF
/*B0_B2 Reserved Registers*/
#define THL_ST 0xB3
/*B4 Reserved Register*/
#define THL_DLT 0xB5
/*B6_BD Reserved Registers*/
#define AD_CHB 0xBE
#define AD_CHR 0xBF
#define AD_CHGb 0xC0
#define AD_CHGr 0xC1
/*C2_C8 Reserved Registers*/
#define SATCTR 0xC9



#define PCLK (PIND & 4)
#define VSYNC (PIND & 8)

/*"Normal"*/
//#define D_0 (PINC & 1)
//#define D_1 (PINC & 2)
//#define D_2 (PINC & 4)
//#define D_3 (PINC & 8)
//#define D_4 (PIND & 16)
//#define D_5 (PIND & 32)
//#define D_6 (PIND & 64)
//#define D_7 (PIND & 128)


/*Side separetaed*/
#define D_7 (PINC & 1)
#define D_5 (PINC & 2)
#define D_3 (PINC & 4)
#define D_1 (PINC & 8)

#define D_0 (PIND & 16)
#define D_2 (PIND & 32)
#define D_4 (PIND & 64)
#define D_6 (PIND & 128)


void initUno() {
  ASSR &= ~(_BV(EXCLK) | _BV(AS2));
  TCCR2A = (1 << COM2A0) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << WGM22) | (1 << CS20);
  OCR2A = 1;//(F_CPU)/(2*(X+1))
}

void modeSet(int mode) {
  switch (mode) {
    case 0: /*VGA YUV*/
      Wire.beginTransmission(camAddress);
      Wire.write(CLKRC);            Wire.write(0x01);
      Wire.write(COM7);             Wire.write(0x00);
      Wire.write(COM3);             Wire.write(0x00);
      Wire.write(COM14);            Wire.write(0x00);
      Wire.write(SCALING_XSC);      Wire.write(0x3A);
      Wire.write(SCALING_YSC);      Wire.write(0x35);
      Wire.write(SCALING_DCWCTR);   Wire.write(0x11);
      Wire.write(SCALING_PCLK_DIV); Wire.write(0xF0);
      Wire.endTransmission();
      break;

    case 1: /*QVGA YUV*/
      Wire.beginTransmission(camAddress);
      Wire.write(CLKRC);            Wire.write(0x01);
      Wire.write(COM7);             Wire.write(0x00);
      Wire.write(COM3);             Wire.write(0x04);
      Wire.write(COM14);            Wire.write(0x19);
      Wire.write(SCALING_XSC);      Wire.write(0x3A);
      Wire.write(SCALING_YSC);      Wire.write(0x35);
      Wire.write(SCALING_DCWCTR);   Wire.write(0x11);
      Wire.write(SCALING_PCLK_DIV); Wire.write(0xF1);
      Wire.endTransmission();
      break;

    case 2: /*QQVGA YUV*/
      Wire.beginTransmission(camAddress);
      Wire.write(CLKRC);            Wire.write(0x01);
      Wire.write(COM7);             Wire.write(0x00);
      Wire.write(COM3);             Wire.write(0x04);
      Wire.write(COM14);            Wire.write(0x1A);
      Wire.write(SCALING_XSC);      Wire.write(0x3A);
      Wire.write(SCALING_YSC);      Wire.write(0x35);
      Wire.write(SCALING_DCWCTR);   Wire.write(0x22);
      Wire.write(SCALING_PCLK_DIV); Wire.write(0xF2);
      Wire.endTransmission();
      break;

    case 3: /*VGA Raw Bayer RGB*/
      Wire.beginTransmission(camAddress);
      Wire.write(CLKRC);            Wire.write(0x01);
      Wire.write(COM7);             Wire.write(0x01);
      Wire.write(COM3);             Wire.write(0x00);
      Wire.write(COM14);            Wire.write(0x00);
      Wire.write(SCALING_XSC);      Wire.write(0x3A);
      Wire.write(SCALING_YSC);      Wire.write(0x35);
      Wire.write(SCALING_DCWCTR);   Wire.write(0x11);
      Wire.write(SCALING_PCLK_DIV); Wire.write(0xF0);
      Wire.endTransmission();
      break;

    case 4: /*QVGA Processed Bayer RGB*/
      Wire.beginTransmission(camAddress);
      Wire.write(CLKRC);            Wire.write(0x01);
      Wire.write(COM7);             Wire.write(0x11);
      Wire.write(COM3);             Wire.write(0x04);
      Wire.write(COM14);            Wire.write(0x1A);
      Wire.write(SCALING_XSC);      Wire.write(0x3A);
      Wire.write(SCALING_YSC);      Wire.write(0x35);
      Wire.write(SCALING_DCWCTR);   Wire.write(0x11);
      Wire.write(SCALING_PCLK_DIV); Wire.write(0xF9);
      Wire.endTransmission();
      break;

    default:
      // statements
      break;
  }
}

void captureImgUno(uint16_t wg, uint16_t hg) {
  uint16_t y, x;

  Serial.println("*RDY*");
  while (!VSYNC);//wait for high
  while (VSYNC);//wait for low
  y = hg;
  while (y--) {
    x = wg;
    while (x--) {
      while (PCLK);//wait for low
      Serial.write(D_0 | D_1 | D_2 | D_3 | D_4 | D_5 | D_6 | D_7);
      while (!PCLK);//wait for high
      while (PCLK);//wait for low
      while (!PCLK);//wait for high
    }
  }
}

void setup() {
  Serial.begin(9600);
  modeSet(1);
}

void loop() {
  captureImgUno(320, 240);
}
