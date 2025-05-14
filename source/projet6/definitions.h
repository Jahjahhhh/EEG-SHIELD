// HSPI (towards ADS1299-4)
#define H_CS_PIN      15
#define H_SCLK_PIN    14
#define H_MISO_PIN    12
#define H_MOSI_PIN    13
#define DRDY_PIN      32
#define START_PIN     33

// VSPI (towards PI)
#define V_CS_PIN       5
#define V_SCLK_PIN    18
#define V_MISO_PIN    19
#define V_MOSI_PIN    23

// POWER
#define ADS_PWDN_PIN  25
#define ADS_RESET_PIN 26

// LEDs for debugging
#define LED1_PIN      22
#define LED2_PIN       1 // Also TX0
#define LED3_PIN       3 // Also RX0
#define LED4_PIN      21
#define LED5_PIN       4
#define LED6_PIN       0
#define LED7_PIN       2
#define LED8_PIN      27

// SPI Commands
// System Commands
#define WAKEUP_CMD   0x02
#define STANDBY_CMD  0x04
#define RESET_CMD    0x06
#define START_CMD    0x08
#define STOP_CMD     0x0A

// Data Read Commands
#define RDATAC_CMD 0x10 // Read Data Continuously
#define SDATAC_CMD 0x11 // Stop RDATAC
#define RDATA_CMD  0x12 // Read Data by Command

// Register Read Commands (Ignored if RDATAC is enabled)
// RREG: Read  n nnnn registers starting at addr r rrrr, 001r rrrr (2xh), 000n nnnn
#define RREG 0x20 // ex: RREG | 0x00 to start reading at addr 0
// WREG: Write n nnnn registers starting at addr r rrrr, 010r rrrr (4xh), 000n nnnn
#define WREG 0x40 // same was RREG but to write

// Register adresses (to read, gotta send 0x11 SPI CMD first)
// Global Settings Across Channels
#define ID_ADDR      0x0
#define CONFIG1_ADDR 0x1
#define CONFIG2_ADDR 0x2
#define CONFIG3_ADDR 0x3
#define LOFF         0x4

// Channel-Specific Settings
#define CH1SET_ADDR      0x5
#define CH2SET_ADDR      0x6
#define CH3SET_ADDR      0x7
#define CH4SET_ADDR      0x8
#define CH5SET_ADDR      0x9
#define CH6SET_ADDR      0xA
#define CH7SET_ADDR      0xB
#define CH8SET_ADDR      0xC
#define BIAS_SENSP_ADDR  0xD
#define BIAS_SENSN_ADDR  0xE
#define LOFF_SENSP_ADDR  0xF
#define LOFF_SENSN_ADDR 0x10
#define LOFF_FLIP_ADDR  0x11

// Lead-Off Status Registers (Read-Only Registers)
#define LOFF_STATP      0x12
#define LOFF_STATN      0x13

// GPIO and OTHER Registers
#define GPIO_ADDR       0x14
#define MISC1_ADDR      0x15
#define MISC2_ADDR      0x16
#define CONFIG4_ADDR    0x17
