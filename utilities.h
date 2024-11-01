#pragma once

#define USE_WIRE

#if defined(USE_WIRE)

// Motion Sensor
#define SENSOR_SDA  6
#define SENSOR_SCL  7
#define SENSOR_IRQ  -1
#define SENSOR_INT1 4
#define SENSOR_INT2 3

// Probably the boot button
#define BOOT_BUTTON 0

// Battery connection?
#define MX125_ADC 1

// LCD
#define LCD_BL 2
#define TP_INT 5
#define TP_SDA 6
#define TP_SCL 7
#define LCD_DC 8
#define LCD_CS 9
#define LCD_CLK 10 
#define LCD_MOSI 11
#define LCD_MISO 12
#define TP_RST 13
#define LCD_RST 14

//GPIO
#define GPIO15 15
#define GPIO16 16
#define GPIO17 17
#define GPIO18 18
#define GPIO21 21
#define GPIO33 33

#endif
