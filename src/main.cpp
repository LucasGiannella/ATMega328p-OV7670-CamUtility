/*
   Author: Rodrigo Anjos de Souza
   Date: 11/10/2019

*/

/*  */
#include <Arduino.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>


/*  */
#define F_CPU 16000000
#define BAUD  1000000
#define BRC   ((F_CPU/16/BAUD) - 1)

/* Port Output Definitions */
#define    CLR(port,pin)  (port &= (~(1<<pin)))
#define    SET(port,pin)  (port |= ( (1<<pin)))
#define TOGGLE(port,pin)  (port ^= ( (1<<pin)))

/* ATMega328p Registers */
/***************************************************  TWI Interface *********************************************************/
/* TWI Status Codes */                            // p. 229, 232, 235, 238, 239
/* General Status Codes */                        // p. 229
#define   TW_START                  0x08
#define   TW_REP_START              0x10
/* Status Codes for Master Transmitter Mode */    // p. 229
#define   TW_MT_SLA_ACK             0x18
#define   TW_MT_SLA_NACK            0x20
#define   TW_MT_DATA_ACK            0x28
#define   TW_MT_DATA_NACK           0x30
#define   TW_MT_ARB_LOST            0x38
/* Status Codes for Master Receiver Mode */       // p. 232
#define   TW_MR_ARB_LOST            0x38
#define   TW_MR_SLA_ACK             0x40
#define   TW_MR_SLA_NACK            0x48
#define   TW_MR_DATA_ACK            0x50
#define   TW_MR_DATA_NACK           0x58
/* Status Codes for Slave Transmitter Mode */     // p. 235
#define   TW_ST_SLA_ACK             0xA8
#define   TW_ST_ARB_LOST_SLA_ACK    0xB0
#define   TW_ST_DATA_ACK            0xB8
#define   TW_ST_DATA_NACK           0xC0
#define   TW_ST_LAST_DATA           0xC8
/* Status Codes for Slave Receiver Mode */        // p. 238
#define   TW_SR_SLA_ACK             0x60
#define   TW_SR_ARB_LOST_SLA_ACK    0x68
#define   TW_SR_GCALL_ACK           0x70
#define   TW_SR_ARB_LOST_GCALL_ACK  0x78
#define   TW_SR_DATA_ACK            0x80
#define   TW_SR_DATA_NACK           0x88
#define   TW_SR_GCALL_DATA_ACK      0x90
#define   TW_SR_GCALL_DATA_NACK     0x98
#define   TW_SR_STOP                0xA0
/* Miscellaneous Status */                        // p. 239
#define   TW_NO_INFO                0xF8
#define   TW_BUS_ERROR              0x00

#define   TW_STATUS_MASK 0b11111000
#define   TW_STATUS   (TWSR & TW_STATUS_MASK)

/* TWI function status */
#define   TW_SUCCESSFULL_DATA_TRANSFER  0xFF
#define   TW_ERROR_UNKOWN               0x00

/* TWI Error Localizations */
#define TWI_W_STA               0x00
#define TWI_W_SLA_ADDR          0x01
#define TWI_W_SLA_REG           0x02
#define TWI_W_DATA              0x03

#define TWI_R_STA0              0x04
#define TWI_R_SLA_ADDR_GET0     0x05
#define TWI_R_SLA_REG           0x06
#define TWI_R_STA1              0x07
#define TWI_R_SLA_ADDR_GET1     0x08
#define TWI_R_SLA_REG_READ      0x09

/* OV7076 TWI Address */
#define OV7670_WRITE 0x42
#define OV7670_READ  0x43

/***************************************************  TWI Interface *********************************************************/

/***************************************************  PHY Interface *********************************************************/
/* Pin Definitions */
#define D2 (PIND & 0b00000100)
#define D3 (PIND & 0b00001000)
#define D4 (PIND & 0b00010000)
#define D5 (PIND & 0b00100000)
#define D6 (PIND & 0b01000000)
#define D7 (PIND & 0b10000000)
#define D8 (PINB & 0b00000001)
#define A0 (PINC & 0b00000001)
#define A1 (PINC & 0b00000010)
#define A2 (PINC & 0b00000100)
#define A3 (PINC & 0b00001000)
/***************************************************  PHY Interface *********************************************************/

/*Defining OV7670 Registers*/
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
/*End define OV7670 Registers*/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 2-WIRE SERIAL INTERFACE */////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Used OV7670 camera module does not allow access to SCCB_E bit, therefore
   in this example it is used a 2-wire variant of the SCCB Specification to
   control only one camera module
*/
/* TWI error handler */
uint8_t twi_error_handler(uint8_t localization, uint8_t slave_register, uint8_t write_data, uint8_t addr)                                                                          /*-*/
{
  // Check for TWI interface Errors and handle them
  // implement error correction and identification

  USART_Transmit('E');
  USART_Transmit('R');
  USART_Transmit('R');
  USART_Transmit(':');
  USART_Transmit(localization);
  USART_Transmit('-');
  USART_Transmit(TW_STATUS);
  USART_Transmit('-');
  USART_Transmit(slave_register);
  USART_Transmit('-');
  USART_Transmit(write_data);
  USART_Transmit('-');
  USART_Transmit(addr);
  USART_Transmit('\n');
  error_led();
  return TW_ERROR_UNKOWN;
}

/* Set register via TWI */
uint8_t twi_set_register(uint8_t slave_register, uint8_t write_data, uint8_t write_addr)
{
  /* Phase 0 - Get Control of the TWI Interface as Master */
  TWCR = (1 << TWINT ) | (1 << TWEN  ) | (1 << TWSTA );       /* Enter Transmit mode and transmit Start Condition */
  while (!(TWCR & (1 << TWINT)));                             /* Wait until transmission is completed             */
  if (TW_STATUS != TW_START)                                  /* Check TWI Status Register                        */
  {
    return twi_error_handler(TWI_W_STA, slave_register, write_data, write_addr);
  }

  /* Phase 1 - Get Slave ID address */
  TWDR = write_addr;                                          /* Set SLA_W ADDR                                   */
  TWCR = (1 << TWINT ) | (1 << TWEN  );                       /* Enter Transmit mode                              */
  while (!(TWCR & (1 << TWINT)));                             /* Wait until transmission is completed             */
  if (TW_STATUS != TW_MT_SLA_ACK)                             /* Check TWI Status Register                        */
  {
    return twi_error_handler(TWI_W_SLA_ADDR, slave_register, write_data, write_addr);
  }

  /* Phase 2 - Get Slave Sub-address/register */
  TWDR = slave_register;                                      /* Load Slave Sub-Address                           */
  TWCR = (1 << TWINT ) | (1 << TWEN  );                       /* Enter Transmit mode                              */
  while (!(TWCR & (1 << TWINT)));                             /* Wait until transmission is completed             */
  if (TW_STATUS != TW_MT_DATA_ACK)                            /* Check TWI Status Register                        */
  {
    return twi_error_handler(TWI_W_SLA_REG, slave_register, write_data, write_addr);
  }

  /* Phase 3 - Write slave register */
  TWDR = write_data;                                          /* Load Write Data                                  */
  TWCR = (1 << TWINT ) | (1 << TWEN  );                       /* Enter Transmit mode                              */
  while (!(TWCR & (1 << TWINT)));                             /* Wait until transmission is completed             */
  if (TW_STATUS != TW_MT_DATA_ACK)                            /* Check TWI Status Register                        */
  {
    return twi_error_handler(TWI_W_DATA, slave_register, write_data, write_addr);
  }

  /* Phase 4 - Send STOP condition to alert all slaves that the TWI interface is free */
  TWCR = (1 << TWINT ) | (1 << TWEN  ) | (1 << TWSTO );       /* Enter Transmit mode & Transmit Stop Condition    */
  _delay_ms(1);
  return TW_SUCCESSFULL_DATA_TRANSFER;                        /* Indicate successfull comunication                */
}

/* Read register via TWI */
uint8_t twi_read_register(uint8_t slave_register, uint8_t read_addr, uint8_t write_addr)                         /*-*/
{
  uint8_t dat   = 0x00;
  uint8_t nack  = 0x01;

  /* Phase 0 - Get Control of the TWI Interface as Master */
  TWCR = (1 << TWINT ) | (1 << TWEN  ) | (1 << TWSTA );       /* Enter Transmit mode & Transmit Start Condition   */
  while (!(TWCR & (1 << TWINT)));                             /* Wait until transmission is completed             */
  if (TW_STATUS != TW_START)                                  /* Check TWI Status Register                        */
  {
    return twi_error_handler(TWI_R_STA0, slave_register, 0x00, read_addr);
  }

  /* Phase 1 - Get Slave ID address */
  TWDR = write_addr;                                          /* Set SLA_W ADDR                                   */
  TWCR = (1 << TWINT ) | (1 << TWEN  );                       /* Enter Transmit mode                              */
  while (!(TWCR & (1 << TWINT)));                             /* Wait until transmission is completed             */
  if (TW_STATUS != TW_MT_SLA_ACK)                             /* Check TWI Status Register                        */
  {
    return twi_error_handler(TWI_R_SLA_ADDR_GET0, slave_register, 0x00, read_addr);
  }

  /* Phase 2 - Get Slave Sub-address/register */
  TWDR = slave_register;                                      /* Load Slave Sub-Address                           */
  TWCR = (1 << TWINT ) | (1 << TWEN  );                       /* Enter Transmit mode                              */
  while (!(TWCR & (1 << TWINT)));                             /* Wait until transmission is completed             */
  if (TW_STATUS != TW_MT_DATA_ACK)                            /* Check TWI Status Register                        */
  {
    return twi_error_handler(TWI_R_SLA_REG, slave_register, 0x00, read_addr);
  }

  /* Phase 3 - Send STOP condition to alert all slaves that the TWI interface is free */
  TWCR = (1 << TWINT ) | (1 << TWEN  ) | (1 << TWSTO );       /* Enter Transmit mode & Transmit Stop Condition    */

  _delay_ms(1);                                                                                                       //Comment: how to determine a minimal delay?                                                          /*-*/

  /* Phase 0 - Get Control of the TWI Interface as Master */
  TWCR = (1 << TWINT ) | (1 << TWEN  ) | (1 << TWSTA );       /* Enter Transmit mode & Transmit Start Condition   */
  while (!(TWCR & (1 << TWINT)));                             /* Wait until transmission is completed             */
  if (TW_STATUS != TW_START)                                  /* Check TWI Status Register                        */
  {
    return twi_error_handler(TWI_R_STA1, slave_register, 0x00, read_addr);
  }

  /* Phase 1 - Get Slave ID address */
  TWDR = read_addr;                                           /* Set SLA_R ADDR                                   */
  TWCR = (1 << TWINT ) | (1 << TWEN  );                       /* Enter Transmit mode                              */
  while (!(TWCR & (1 << TWINT)));                             /* Wait until transmission is completed             */
  if (TW_STATUS != TW_MR_SLA_ACK)                             /* Check TWI Status Register                        */
  {
    return twi_error_handler(TWI_R_SLA_ADDR_GET1, slave_register, 0x00, read_addr);
  }

  /* Phase 2 - Read slave data */
  if (nack)
  {
    TWCR =  (1 << TWINT ) | (1 << TWEN  ) | (0 << TWEA  );    /* Enter Transmit mode & Expect NOT ACK from slave  */
    while (!(TWCR & (1 << TWINT)));                           /* Wait until transmission is completed             */
    if (TW_STATUS != TW_MR_DATA_NACK)                         /* Check TWI Status Register                        */
    {
      return twi_error_handler(TWI_R_SLA_REG_READ, slave_register, 0x00, read_addr);
    }
  }
  else
  {
    TWCR =  (1 << TWINT ) | (1 << TWEN  ) | (0 << TWEA  );    /* Enter Transmit mode & Expect ACK from slave      */
    while (!(TWCR & (1 << TWINT)));                           /* Wait until transmission is completed             */
    if (TW_STATUS != TW_MR_DATA_ACK)                          /* Check TWI Status Register                        */
    {
      return twi_error_handler(TWI_R_SLA_REG_READ, slave_register, 0x00, read_addr);
    }
  }

  dat = TWDR;

  /* Phase 3 - Send STOP condition to alert all slaves that the TWI interface is free */
  TWCR = (1 << TWINT ) | (1 << TWEN  ) | (1 << TWSTO );       /* Enter Transmit mode & Transmit Stop Condition    */

  _delay_ms(1); // Whyyy???                                                                                 /*-*/
  return dat;
}

/* Write to register Localization via TWI */
uint8_t twi_write_register(uint8_t slave_register, uint8_t write_val, uint8_t write_loc, uint8_t write_addr, uint8_t read_addr)
{
  uint8_t reg_val = twi_read_register(slave_register, read_addr, write_addr);   /* Get current register value     */
  if(write_val)                                                                 /* Modify specified bit           */
  {
     reg_val |=  (1 << write_loc);
     reg_write_led();
  }
  else                                                                          /* Modify specified bit           */
  {
     reg_val &= !(1 << write_loc);
  }
  twi_set_register(slave_register, reg_val, write_addr);                        /* Write modified register to TWI */
}


/* Get Register Map */

#define FULL_MEMORY_MAP   0x00
#define OUTPUT_FORMAT_MAP 0x01
#define IMAGE_SCALER_MAP  0x02

uint8_t twi_get_register_map(uint8_t mode)
{
  switch(mode)
  {
    case 0:                                                   /* Map all of the internal memory */
      {

      }
    case 1:                                                   /* Map Register associated with OutputFormats */
      {
        USART_Transmit_Register(COM7  , twi_read_register(COM7  , OV7670_READ, OV7670_WRITE));
        USART_Transmit_Register(COM15 , twi_read_register(COM15 , OV7670_READ, OV7670_WRITE));
        USART_Transmit_Register(RGB444, twi_read_register(RGB444, OV7670_READ, OV7670_WRITE));
      }
  }
}

/* 2-WIRE SERIAL INTERFACE  */////////////////////////////////////////////////////////////////////////////////////
/* OV7670 FUNCTIONS         */////////////////////////////////////////////////////////////////////////////////////
/* Set OV7670 Register */
void OV7670_set_register(uint8_t reg, uint8_t val)
{
  while(twi_set_register(reg, val, OV7670_WRITE))
  {
    /* Reset TWI Line */
    TWCR = (1 << TWINT)|(1 << TWSTO);
    _delay_ms(1000);
  }
}

void OV7670_set_register(uint8_t reg, uint8_t val, uint8_t loc)
{
  while(twi_write_register(reg, val, loc, OV7670_WRITE, OV7670_READ))
  {
    /* Reset TWI Line */
    TWCR = (1 << TWINT)|(1 << TWSTO);
    _delay_ms(1000);
  }
}

/* Reset All OV7670 Registers to their Default Values */
void OV7670_set_default()
{
  //OV7670_set_register(COM7, (1 << SCCBRES));
  OV7670_set_register(COM7, 0b10000000);
}

/*  */

/* Set OV7670 Output Format */

#define OUT_FORMAT_RAW_BAYER_RGB   0x00
#define OUT_FORMAT_PROC_BAYER_RGB  0x01
#define OUT_FORMAT_YUV422          0x02
#define OUT_FORMAT_GRB422          0x03
#define OUT_FORMAT_RGB565          0x04
#define OUT_FORMAT_RGB555          0x05
#define OUT_FORMAT_RGB444_XRGB     0x06
#define OUT_FORMAT_RGB444_RGBX     0x07

void OV7670_set_outformat(uint8_t format)
{
  switch(format)
  {
    case OUT_FORMAT_RAW_BAYER_RGB:                                      /* Raw Bayer RGB        */
    {
      OV7670_set_register(COM7  , 1, CSPCSEL0       );
      OV7670_set_register(COM7  , 0, CSPCSEL1       );
      OV7670_set_register(COM15 , 0, RGBSEL0        );
      OV7670_set_register(COM15 , 0, RGBSEL1        );
      OV7670_set_register(RGB444, 0, RGB444EN       );
      OV7670_set_register(RGB444, 0, RGB444WF       );
      break;
    }
    case OUT_FORMAT_PROC_BAYER_RGB:                                     /* Processed Bayer RGB  */
    {
      OV7670_set_register(COM7  , 1, CSPCSEL0       );
      OV7670_set_register(COM7  , 1, CSPCSEL1       );
      OV7670_set_register(COM15 , 0, RGBSEL0        );
      OV7670_set_register(COM15 , 0, RGBSEL1        );
      OV7670_set_register(RGB444, 0, RGB444EN       );
      OV7670_set_register(RGB444, 0, RGB444WF       );
      break;
    }
    case OUT_FORMAT_YUV422:                                             /* YUV422               */
    {
      OV7670_set_register(COM7  , 0, CSPCSEL0       );
      OV7670_set_register(COM7  , 0, CSPCSEL1       );
      OV7670_set_register(COM15 , 0, RGBSEL0        );
      OV7670_set_register(COM15 , 0, RGBSEL1        );
      OV7670_set_register(RGB444, 0, RGB444EN       );
      OV7670_set_register(RGB444, 0, RGB444WF       );
      break;
    }
    case OUT_FORMAT_GRB422:                                             /* GRB422               */
    {
      OV7670_set_register(COM7  , 0, CSPCSEL0       );
      OV7670_set_register(COM7  , 1, CSPCSEL1       );
      OV7670_set_register(COM15 , 0, RGBSEL0        );
      OV7670_set_register(COM15 , 0, RGBSEL1        );
      OV7670_set_register(RGB444, 0, RGB444EN       );
      OV7670_set_register(RGB444, 0, RGB444WF       );
      break;
    }
    case OUT_FORMAT_RGB565:                                             /* RGB565               */
    {
      OV7670_set_register(COM7  , 0, CSPCSEL0       );
      OV7670_set_register(COM7  , 1, CSPCSEL1       );
      OV7670_set_register(COM15 , 1, RGBSEL0        );
      OV7670_set_register(COM15 , 0, RGBSEL1        );
      OV7670_set_register(RGB444, 0, RGB444EN       );
      OV7670_set_register(RGB444, 0, RGB444WF       );
      break;
    }
    case OUT_FORMAT_RGB555:                                             /* RGB555               */
    {
      OV7670_set_register(COM7  , 0, CSPCSEL0       );
      OV7670_set_register(COM7  , 1, CSPCSEL1       );
      OV7670_set_register(COM15 , 1, RGBSEL0        );
      OV7670_set_register(COM15 , 1, RGBSEL1        );
      OV7670_set_register(RGB444, 0, RGB444EN       );
      OV7670_set_register(RGB444, 0, RGB444WF       );
      break;
    }
    case OUT_FORMAT_RGB444_XRGB:                                        /* RGB444_XRGB          */
    {
      OV7670_set_register(COM7  , 0, CSPCSEL0       );
      OV7670_set_register(COM7  , 1, CSPCSEL1       );
      OV7670_set_register(COM15 , 0, RGBSEL0        );
      OV7670_set_register(COM15 , 0, RGBSEL1        );
      OV7670_set_register(RGB444, 1, RGB444EN       );
      OV7670_set_register(RGB444, 0, RGB444WF       );
      break;
    }
    case OUT_FORMAT_RGB444_RGBX:                                        /* RGB444_RGBX          */
    {
      OV7670_set_register(COM7  , 0, CSPCSEL0       );
      OV7670_set_register(COM7  , 1, CSPCSEL1       );
      OV7670_set_register(COM15 , 0, RGBSEL0        );
      OV7670_set_register(COM15 , 0, RGBSEL1        );
      OV7670_set_register(RGB444, 1, RGB444EN       );
      OV7670_set_register(RGB444, 1, RGB444WF       );
      break;
    }
    default:
    {
      /* Send Error */
      twi_get_register_map(OUTPUT_FORMAT_MAP);
      break;
    }
  }
  twi_get_register_map(OUTPUT_FORMAT_MAP);
}

/* Set OV7670 Resolution */
void setRes(void)
{

}

/* Initialize camera with default values */
void OV7670_init(void)
{
  /* Write Default Values */
  twi_set_register(COM7, 0x20, OV7670_WRITE);
  OV7670_set_default();
  USART_Transmit_Register(COM7  , twi_read_register(COM7  , OV7670_READ, OV7670_WRITE));
  twi_get_register_map(OUTPUT_FORMAT_MAP);

  //twi_write_register(COM7, 1, CSPCSEL0, OV7670_WRITE, OV7670_READ);
  //twi_write_register(COM7, 1, CSPCSEL1, OV7670_WRITE, OV7670_READ);

  //twi_get_register_map(OUTPUT_FORMAT_MAP);
  /* Select Output Format */
  //OV7670_set_outformat(OUT_FORMAT_PROC_BAYER_RGB);
}

/* OV7670 FUNCTIONS */

/* Blink Error LED for ATMega328p */
void error_led(void)
{
  DDRB |= 32;//make sure led is output
  PORTB ^= 32;// toggle led
  _delay_ms(10);
  PORTB ^= 32;// toggle led
  _delay_ms(10);
  PORTB ^= 32;// toggle led
  _delay_ms(10);
  PORTB ^= 32;// toggle led
  _delay_ms(10);
  PORTB ^= 32;// toggle led
  _delay_ms(10);
  PORTB ^= 32;// toggle led
  _delay_ms(10);
  PORTB ^= 32;// toggle led
  _delay_ms(10);
  PORTB ^= 32;// toggle led
  _delay_ms(10);
  PORTB ^= 32;// toggle led
  _delay_ms(10);
  PORTB ^= 32;// toggle led
  _delay_ms(10);
}

void reg_write_led(void)
{
  DDRB |= 32;//make sure led is output
  PORTB ^= 32;// toggle led
  _delay_ms(50);
  PORTB ^= 32;// toggle led
  _delay_ms(50);
  PORTB ^= 32;// toggle led
  _delay_ms(50);
  PORTB ^= 32;// toggle led
  _delay_ms(50);
}

void USART_Transmit(uint8_t data)               // (p. 184)
{
  /* Wait until TX data frame is empty */
  while ( ! ( UCSR0A & (1 << UDRE0)))           // (p. 195)
  {;}

  /* Fill Tx data frame with data*/
  UDR0 = data;                                  // (p. 195)
}

void USART_Transmit_Register(uint8_t reg, uint8_t val)
{
  USART_Transmit('R');
  USART_Transmit('V');
  USART_Transmit(':');
  USART_Transmit(reg);
  USART_Transmit('-');
  USART_Transmit(val);
  USART_Transmit('\n');
}

/* Initialize ATMega328p -> Arduino UNO */
void ATMega328p_init(void)
{
  // Disable Interrup Service Routines
  cli();

  /************************SET XCLK*********************************/
  /*
    As per ATMega328p Datasheet p. 122, if WGM2:0 = 7, TOP values used in compare match for Fast PWM Mode
    is equal to OCR2A. This will enable PWM with clocks of up to 8MHz, used by the camera.
  */
  // Clear all usd registers
  ASSR   = 0x00;
  TCCR2A = 0x00;
  TCCR2B = 0x00;
  /*
  Internal OV7670 clock [XCLK] is defined by arduino pins. Therefore is must be set (OV7670 Datasheet p.4).
  Seek p. 2 from OV7670 Datasheet for more information about how to XCLK is linked to the rest of the circuit
  */
  /* Set pin PB3 as output. This pin in Arduino UNO pltform is
     correspondent to pin 11
  */
  DDRB |= (1 << 3);                                                                              //Comment: use defined methods for pins

  /* Set ATMega328p pin PB3 as clock output source for OV7670 camera, and set
     it to Fast PWM mode. (ATMega328p p. 122)
  */
  // Enable External Clock Input [TOSC1], wich is direclty attached to 16MHz crystal
  ASSR    &= ~((1 << EXCLK) | (1 << AS2));

  // Set No Prescaling
  TCCR2B  |= (1 << CS20) | (0 << CS21) | (0 << CS22);

  // Fast PWM Mode Set
  TCCR2A  |= (1 << WGM20) | (1 << WGM21);
  TCCR2B  |= (1 << WGM22);

  // Set Compare Output Mode: Toggle OC2A on compare match
  TCCR2A  |= (1 << COM2A0)  | (0 << COM2A1);

  // Set OCR2A value:
  OCR2A    = 0;
  /************************SET XCLK*********************************/


  /************************SET GPIO*********************************/
  /*
  Arduino UNO Pinout  -  Function             - OV7670

  A0 [PC0]            - Data PIN              - D0
  A1 [PC1]            - Data PIN              - D1
  A2 [PC2]            - Data PIN              - D2
  A3 [PC3]            - Data PIN              - D3

  D2 [PD2]            - Pixel clock           - PCLK
  D3 [PD3]            - Vertical Sync         - VSYNC

  D4 [PD4]            - Data PIN              - D4
  D5 [PD5]            - Data PIN              - D5
  D6 [PD6]            - Data PIN              - D6
  D7 [PD7]            - Data PIN              - D7

  D8 [PB0]            - Horizontal Reference  - HREF
  */
  /*
  Set pins to Input and High-Z
  */
  // Set A0~A3
  DDRC  &= ~(0b00001111);
  PORTC &= ~(0b00001111);

  // Set D2~D7
  DDRD  &= ~(0b11111100);
  PORTD &= ~(0b11111100);

  // Set D8
  DDRB  &= ~(0b00000001);
  PORTB &= ~(0b00000001);
  /************************SET GPIO*********************************/



  //set up twi for 100khz
  TWSR &= ~3;   //disable prescaler for TWI
  TWBR = 72;    //set to 100khz

  //* Setup USART Interface *//
  /* Set Baudrate for TX & RX*/                         // (p. 183)
  UBRR0H  =  (BRC >> 8);
  UBRR0L  =  (BRC     );
  /* Enable Transmitter and Receiver */
  UCSR0B  =  (1 << RXEN0) |(1 << TXEN0);                  // (p. 183)
  /* Set USART to normal speed operation */
  UCSR0A &= ~(1 << U2X0);
  /* Set up frame size for TX & RX to 8-bit*/
  UCSR0C  =  (1<< UCSZ01) | (1<< UCSZ00);                 // (p. 198)

  // Enable Interrup Service Routines
  sei();
}

/* Transmit Data via Serial */
void StringPgm(const char * str)
{
  do {
    while (!(UCSR0A & (1 << UDRE0)));//wait for byte to transmit
    UDR0 = pgm_read_byte_near(str);
    while (!(UCSR0A & (1 << UDRE0)));//wait for byte to transmit
  } while (pgm_read_byte_near(++str));
}

/**************** Send Image via Serial ****************/

/*
The following method sends a single byte per pixel
It only works for YUV422 image format
*/
static void captureImgUno(uint16_t width, uint16_t height)
{
  // [Figure 5] Horizontal Timing (p. 7)
  uint16_t y, x;

  StringPgm(PSTR("*RDY*"));                           // Warn incoming imagem via Serial

  /* Wait for VSYNC to ping (p. 7)*/
  while (!D3);                                        // Wait VSYNC high
  while (D3);                                         // Wait VSYNC low

  /*  */
  y = height;
  while (y--)
  {                                         // Scan from y=height to y=0
    x = width;
    while (x--)
    {                                       // Scan from x=width to x=0
      /* Capture First Byte */
      while (D2);                                       // Wait PCLK low
      UDR0 = (PINC & 0b00001111) | (PIND & 0b11110000); // Read data [A0, A1, A2, A3, D4, D5, D6, D7]
      while (!(UCSR0A & (1 << UDRE0)));                 // Wait USART0/ UDRO register to clear [UDER0]
      while (!D2);                                      // Wait PCLK high

      /* Capture Second Byte */
      while (D2);                                       // Wait PCLK low
      /* Pegar o segundo Byte e enviar */
      while (!D2);                                      // Wait PCLK high
    }
  }
  UDR0 = 0x10;                                          // Send LF a.k.a. \n via serial
  while (!(UCSR0A & (1 << UDRE0)));                     // Wait USART0/ UDRO register to clear [UDER0]
}

static void captureImgUnoRGB(uint16_t width, uint16_t height)
{
  // [Figure 5] Horizontal Timing (p. 7)
  uint16_t y, x;

  StringPgm(PSTR("*RDY*"));                           // Warn incoming imagem via Serial

  /* Wait for VSYNC to ping (p. 7)*/
  while (!D3);                                        // Wait VSYNC high
  while (D3);                                         // Wait VSYNC low

  /*  */
  y = height;
  while (y--)
  {                                         // Scan from y=height to y=0
    x = width;
    while (x--)
    {                                       // Scan from x=width to x=0
      /* Capture First Byte */
      while (D2);                                       // Wait PCLK low
      UDR0 = (PINC & 0b00001111) | (PIND & 0b11110000); // Read data [A0, A1, A2, A3, D4, D5, D6, D7]
      while (!(UCSR0A & (1 << UDRE0)));                 // Wait USART0/ UDRO register to clear [UDER0]
      while (!D2);                                      // Wait PCLK high

      /* Capture Second Byte */
      while (D2);                                       // Wait PCLK low
      UDR0 = (PINC & 0b00001111) | (PIND & 0b11110000); // Read data [A0, A1, A2, A3, D4, D5, D6, D7]
      while (!(UCSR0A & (1 << UDRE0)));                 // Wait USART0/ UDRO register to clear [UDER0]
      while (!D2);                                      // Wait PCLK high
    }
  }
  UDR0 = 0x10;                                      // Send LF a.k.a. \n via serial
  while (!(UCSR0A & (1 << UDRE0)));                 // Wait USART0/ UDRO register to clear [UDER0]
}

/**************** Send Image via Serial ****************/



void setup()
{
  _delay_ms(1000);
  ATMega328p_init();                          /* Initialize ATMega328p*/
  OV7670_init();                              /* Initialize OV7670    */
  //setRes();                                 // Set Camera Resolution
  //setColor(0);                              // Set Camera ColorSpace
  //twi_set_register(0x11, 16, OV7670_WRITE); // Set Horizontal Sincronization

}


void loop()
{
  /*
  if(digitalRead(13) == 0)
  {
   captureImg(240,320);// Capture & Send Image via Serial
  }
  */
}
