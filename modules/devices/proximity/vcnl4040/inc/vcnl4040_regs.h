#ifndef _ADAFRUIT_VCNL4040_H
#define _ADAFRUIT_VCNL4040_H

#define VCNL4040_I2CADDR_DEFAULT 0x60 ///< VCNL4040 default i2c address

// All addresses are for 16bit registers;
// duplicates are for high or low bytes that aren't used together
#define VCNL4040_ALS_CONFIG 0x00 ///< Ambient light sensor configuration register
#define VCNL4040_ALS_THDH 0x01 ///< Ambient light high threshold register
#define VCNL4040_ALS_THDL 0x02 ///< Ambient light low threshold register
#define VCNL4040_PS_CONF1_L 0x03 ///< Proximity sensor configuration 1/2 register
#define VCNL4040_PS_MS_H 0x04  ///< Proximity sensor configuration 1/2 register
#define VCNL4040_PS_THDL 0x06  ///< Proximity sensor low threshold register
#define VCNL4040_PS_THDH 0x07  ///< Proximity sensor high threshold register
#define VCNL4040_PS_DATA 0x08  ///< Proximity sensor data register
#define VCNL4040_ALS_DATA 0x09 ///< Ambient light sensor data register
#define VCNL4040_WHITE_DATA 0x0A ///< White light sensor data register
#define VCNL4040_INT_FLAG 0x0B   ///< Interrupt status register
#define VCNL4040_DEVICE_ID 0x0C  ///< Device ID

typedef enum led_current {
  VCNL4040_LED_CURRENT_50MA,
  VCNL4040_LED_CURRENT_75MA,
  VCNL4040_LED_CURRENT_100MA,
  VCNL4040_LED_CURRENT_120MA,
  VCNL4040_LED_CURRENT_140MA,
  VCNL4040_LED_CURRENT_160MA,
  VCNL4040_LED_CURRENT_180MA,
  VCNL4040_LED_CURRENT_200MA,
} VCNL4040_LEDCurrent;

typedef enum led_duty_cycle {
  VCNL4040_LED_DUTY_1_40,
  VCNL4040_LED_DUTY_1_80,
  VCNL4040_LED_DUTY_1_160,
  VCNL4040_LED_DUTY_1_320,
} VCNL4040_LEDDutyCycle;

typedef enum ambient_integration_time {
  VCNL4040_AMBIENT_INTEGRATION_TIME_80MS,
  VCNL4040_AMBIENT_INTEGRATION_TIME_160MS,
  VCNL4040_AMBIENT_INTEGRATION_TIME_320MS,
  VCNL4040_AMBIENT_INTEGRATION_TIME_640MS,
} VCNL4040_AmbientIntegration;

typedef enum proximity_integration_time {
  VCNL4040_PROXIMITY_INTEGRATION_TIME_1T,
  VCNL4040_PROXIMITY_INTEGRATION_TIME_1_5T,
  VCNL4040_PROXIMITY_INTEGRATION_TIME_2T,
  VCNL4040_PROXIMITY_INTEGRATION_TIME_2_5T,
  VCNL4040_PROXIMITY_INTEGRATION_TIME_3T,
  VCNL4040_PROXIMITY_INTEGRATION_TIME_3_5T,
  VCNL4040_PROXIMITY_INTEGRATION_TIME_4T,
  VCNL4040_PROXIMITY_INTEGRATION_TIME_8T,
} VCNL4040_ProximityIntegration;

typedef enum proximity_type {
  VCNL4040_PROXIMITY_INT_DISABLE,
  VCNL4040_PROXIMITY_INT_CLOSE,
  VCNL4040_PROXIMITY_INT_AWAY,
  VCNL4040_PROXIMITY_INT_CLOSE_AWAY,
} VCNL4040_ProximityType;

typedef enum interrupt_type {
  VCNL4040_PROXIMITY_AWAY,
  VCNL4040_PROXIMITY_CLOSE,
  VCNL4040_AMBIENT_HIGH = 4,
  VCNL4040_AMBIENT_LOW,
} VCNL4040_InterruptType;
