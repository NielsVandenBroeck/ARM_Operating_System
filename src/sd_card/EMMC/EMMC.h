//addressen uit datasheet BCM2937 p66
const long emmcBase_address_address = 0x7E300000;
const long arg2_address = emmcBase_address_address + 0x0; //ACMD23 Argument
const long BLKSIZECNT_address = emmcBase_address_address + 0x4; //Block Size and Count
const long ARG1_address = emmcBase_address_address + 0x8; //Argument
const long CMDTM_address = emmcBase_address_address + 0xc; //Command and Transfer Mode
const long RESP0_address = emmcBase_address_address + 0x10; //Response bits 31 : 0
const long RESP1_address = emmcBase_address_address + 0x14; //Response bits 63 : 32
const long RESP2_address = emmcBase_address_address + 0x18; //Response bits 95 : 64
const long RESP3_address = emmcBase_address_address + 0x1c; //Response bits 127 : 96
const long DATA_address = emmcBase_address_address + 0x20; //Data
const long STATUS_address = emmcBase_address_address + 0x24; //Status
const long CONTROL0_address = emmcBase_address_address + 0x28; //Host Configuration bits
const long CONTROL1_address = emmcBase_address_address + 0x2c; //Host Configuration bits
const long INTERRUPT_address = emmcBase_address_address + 0x30; //Interrupt Flags
const long IRPT_MASK_address = emmcBase_address_address + 0x34; //Interrupt Flag Enable
const long IRPT_EN_address = emmcBase_address_address + 0x38; //Interrupt Generation Enable
const long CONTROL2_address = emmcBase_address_address + 0x3c; //Host Configuration bits
const long FORCE_IRPT_address = emmcBase_address_address + 0x50; //Force Interrupt Event
const long BOOT_TIMEOUT_address = emmcBase_address_address + 0x70; //Timeout in boot mode
const long DBG_SEL_address = emmcBase_address_address + 0x74; //Debug Bus Configuration
const long EXRDFIFO_CFG_address = emmcBase_address_address + 0x80; //Extension FIFO Configuration
const long EXRDFIFO_EN_address = emmcBase_address_address + 0x84; //Extension FIFO Enable
const long TUNE_STEP_address = emmcBase_address_address + 0x88; //Delay per card clock tuning step
const long TUNE_STEPS_STD_address = emmcBase_address_address + 0x8c; //Card clock tuning steps for SDR
const long TUNE_STEPS_DDR_address = emmcBase_address_address + 0x90; //Card clock tuning steps for DDR
const long SPI_INT_SPT_address = emmcBase_address_address + 0xf0; //SPI Interrupt Support
const long SLOTISR_VER_address = emmcBase_address_address + 0xfc; //Slot Interrupt Status and Version










