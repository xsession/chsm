/*
 * sht30_regs.h
 *
 *  Created on: 2021. aug. 25.
 *      Author: livanyi
 */

#ifndef SHT30_REGS_H_
#define SHT30_REGS_H_

#include <stdint.h>

// ------- Registers -------

#define SHT30_ADDR_ADDR_PIN_LOW  0x44
#define SHT30_ADDR_ADDR_PIN_HIGH 0x45

// ------- Constants -------

#define POLYNOMIAL 0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001

// ------- Enums -------

// Sensor Commands
typedef enum
{
    SHT30_CMD_READ_SERIALNBR  = 0x3780, // read serial number
    SHT30_CMD_READ_STATUS     = 0xF32D, // read status register
    SHT30_CMD_CLEAR_STATUS    = 0x3041, // clear status register
    SHT30_CMD_HEATER_ENABLE   = 0x306D, // enabled heater
    SHT30_CMD_HEATER_DISABLE  = 0x3066, // disable heater
    SHT30_CMD_SOFT_RESET      = 0x30A2, // soft reset
    SHT30_CMD_MEAS_CLOCKSTR_H = 0x2C06, // measurement: clock stretching, high repeatability
    SHT30_CMD_MEAS_CLOCKSTR_M = 0x2C0D, // measurement: clock stretching, medium repeatability
    SHT30_CMD_MEAS_CLOCKSTR_L = 0x2C10, // measurement: clock stretching, low repeatability
    SHT30_CMD_MEAS_POLLING_H  = 0x2400, // measurement: polling, high repeatability
    SHT30_CMD_MEAS_POLLING_M  = 0x240B, // measurement: polling, medium repeatability
    SHT30_CMD_MEAS_POLLING_L  = 0x2416, // measurement: polling, low repeatability
    SHT30_CMD_MEAS_PERI_05_H  = 0x2032, // measurement: periodic 0.5 mps, high repeatability
    SHT30_CMD_MEAS_PERI_05_M  = 0x2024, // measurement: periodic 0.5 mps, medium repeatability
    SHT30_CMD_MEAS_PERI_05_L  = 0x202F, // measurement: periodic 0.5 mps, low repeatability
    SHT30_CMD_MEAS_PERI_1_H   = 0x2130, // measurement: periodic 1 mps, high repeatability
    SHT30_CMD_MEAS_PERI_1_M   = 0x2126, // measurement: periodic 1 mps, medium repeatability
    SHT30_CMD_MEAS_PERI_1_L   = 0x212D, // measurement: periodic 1 mps, low repeatability
    SHT30_CMD_MEAS_PERI_2_H   = 0x2236, // measurement: periodic 2 mps, high repeatability
    SHT30_CMD_MEAS_PERI_2_M   = 0x2220, // measurement: periodic 2 mps, medium repeatability
    SHT30_CMD_MEAS_PERI_2_L   = 0x222B, // measurement: periodic 2 mps, low repeatability
    SHT30_CMD_MEAS_PERI_4_H   = 0x2334, // measurement: periodic 4 mps, high repeatability
    SHT30_CMD_MEAS_PERI_4_M   = 0x2322, // measurement: periodic 4 mps, medium repeatability
    SHT30_CMD_MEAS_PERI_4_L   = 0x2329, // measurement: periodic 4 mps, low repeatability
    SHT30_CMD_MEAS_PERI_10_H  = 0x2737, // measurement: periodic 10 mps, high repeatability
    SHT30_CMD_MEAS_PERI_10_M  = 0x2721, // measurement: periodic 10 mps, medium repeatability
    SHT30_CMD_MEAS_PERI_10_L  = 0x272A, // measurement: periodic 10 mps, low repeatability
    SHT30_CMD_FETCH_DATA      = 0xE000, // readout measurements for periodic mode
    SHT30_CMD_R_AL_LIM_LS     = 0xE102, // read alert limits, low set
    SHT30_CMD_R_AL_LIM_LC     = 0xE109, // read alert limits, low clear
    SHT30_CMD_R_AL_LIM_HS     = 0xE11F, // read alert limits, high set
    SHT30_CMD_R_AL_LIM_HC     = 0xE114, // read alert limits, high clear
    SHT30_CMD_W_AL_LIM_HS     = 0x611D, // write alert limits, high set
    SHT30_CMD_W_AL_LIM_HC     = 0x6116, // write alert limits, high clear
    SHT30_CMD_W_AL_LIM_LC     = 0x610B, // write alert limits, low clear
    SHT30_CMD_W_AL_LIM_LS     = 0x6100, // write alert limits, low set
    SHT30_CMD_NO_SLEEP        = 0x303E,
} sht30_commands_ten;

// Measurement Repeatability
typedef enum
{
    REPEATAB_HIGH,   // high repeatability
    REPEATAB_MEDIUM, // medium repeatability
    REPEATAB_LOW,    // low repeatability
} sht30_repeatability_ten;

// Measurement Mode
typedef enum
{
    MODE_CLKSTRETCH, // clock stretching
    MODE_POLLING,    // polling
} sht30_mode_ten;

typedef enum
{
    SHT30_FREQUENCY_HZ5,  //  0.5 measurements per seconds
    SHT30_FREQUENCY_1HZ,  //  1.0 measurements per seconds
    SHT30_FREQUENCY_2HZ,  //  2.0 measurements per seconds
    SHT30_FREQUENCY_4HZ,  //  4.0 measurements per seconds
    SHT30_FREQUENCY_10HZ, // 10.0 measurements per seconds
} sht30_frequency_ten;

typedef enum
{
    NO_ERROR       = 0x00, // no error
    ACK_ERROR      = 0x01, // no acknowledgment error
    CHECKSUM_ERROR = 0x02, // checksum mismatch error
    TIMEOUT_ERROR  = 0x04, // timeout error
    PARM_ERROR     = 0x80, // parameter out of range error
} sht30_error_ten;

// ------- Structures -------

typedef union
{
    uint16_t all;
    struct
    {
#ifdef LITTLE_ENDIAN                 // bit-order is little endian
        uint16_t crc_status : 1;     // write data checksum status
        uint16_t cmd_status : 1;     // command status
        uint16_t unused4 : 2;        // reserved
        uint16_t reset_detected : 1; // system reset detected
        uint16_t unused3 : 5;        // reserved
        uint16_t T_alert : 1;        // temperature tracking alert
        uint16_t RH_alert : 1;       // humidity tracking alert
        uint16_t unused2 : 1;        // reserved
        uint16_t heater_status : 1;  // heater status
        uint16_t unused1 : 1;        // reserved
        uint16_t alert_pending : 1;  // alert pending status
#else                                // bit-order is big endian
        uint16_t alert_pending : 1;
        uint16_t unused1 : 1;
        uint16_t heater_status : 1;
        uint16_t unused2 : 1;
        uint16_t RH_alert : 1;
        uint16_t T_alert : 1;
        uint16_t unused3 : 5;
        uint16_t reset_detected : 1;
        uint16_t unused4 : 2;
        uint16_t cmd_status : 1;
        uint16_t crc_status : 1;
#endif
    } bit;
} sht30_status_reg_tun;

#endif /* SHT30_REGS_H_ */
