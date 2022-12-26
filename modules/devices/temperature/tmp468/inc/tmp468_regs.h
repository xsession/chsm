/*
 * tmp468_regs.h
 *
 *  Created on: 2021. aug. 25.
 *      Author: livanyi
 */

#ifndef TMP468_REGS_H_
#define TMP468_REGS_H_

#include <stdint.h>

// ------- Registers -------

#define TMP468_ADDR_GND 0x48
#define TMP468_ADDR_VDD 0x49
#define TMP468_ADDR_SDA 0x4A // EL-26-72-00
#define TMP468_ADDR_SCL 0x4B

#define TMP468_LOCAL_TEMP   0x00
#define TMP468_REMOTE1_TEMP 0x01
#define TMP468_REMOTE2_TEMP 0x02
#define TMP468_REMOTE3_TEMP 0x03
#define TMP468_REMOTE4_TEMP 0x04
#define TMP468_REMOTE5_TEMP 0x05
#define TMP468_REMOTE6_TEMP 0x06
#define TMP468_REMOTE7_TEMP 0x07
#define TMP468_REMOTE8_TEMP 0x08

#define TMP468_SOFTRESET             0x20
#define TMP468_NTHERMAL_STATUS1      0x21
#define TMP468_NTHERMAL_STATUS2      0x22
#define TMP468_REMOTE_CH_OPEN_STATUS 0x23

#define TMP468_CONFIG 0x30
// Local channel configs
#define TMP468_THERM_HYSTERESIS    0x38
#define TMP468_LOCAL_NTHERM_LIMIT1 0x39
#define TMP468_LOCAL_NTHEMP_LIMIT2 0x3A
// Remote 1 channel configs
#define TMP468_REMOTE1_TEMP_OFFSET   0x40
#define TMP468_REMOTE1_TEMP_N_FACTOR 0x41
#define TMP468_REMOTE1_NTHERM_LIMIT1 0x42
#define TMP468_REMOTE1_NTHEMP_LIMIT2 0x43
// Remote 2 channel configs
#define TMP468_REMOTE2_TEMP_OFFSET   0x48
#define TMP468_REMOTE2_TEMP_N_FACTOR 0x49
#define TMP468_REMOTE2_NTHERM_LIMIT1 0x4A
#define TMP468_REMOTE2_NTHEMP_LIMIT2 0x4B
// Remote 3 channel configs
#define TMP468_REMOTE3_TEMP_OFFSET   0x50
#define TMP468_REMOTE3_TEMP_N_FACTOR 0x51
#define TMP468_REMOTE3_NTHERM_LIMIT1 0x52
#define TMP468_REMOTE3_NTHEMP_LIMIT2 0x53
// Remote 4 channel configs
#define TMP468_REMOTE4_TEMP_OFFSET   0x58
#define TMP468_REMOTE4_TEMP_N_FACTOR 0x59
#define TMP468_REMOTE4_NTHERM_LIMIT1 0x5A
#define TMP468_REMOTE4_NTHEMP_LIMIT2 0x5B
// Remote 5 channel configs
#define TMP468_REMOTE5_TEMP_OFFSET   0x60
#define TMP468_REMOTE5_TEMP_N_FACTOR 0x61
#define TMP468_REMOTE5_NTHERM_LIMIT1 0x62
#define TMP468_REMOTE5_NTHEMP_LIMIT2 0x63
// Remote 6 channel configs
#define TMP468_REMOTE6_TEMP_OFFSET   0x68
#define TMP468_REMOTE6_TEMP_N_FACTOR 0x69
#define TMP468_REMOTE6_NTHERM_LIMIT1 0x6A
#define TMP468_REMOTE6_NTHEMP_LIMIT2 0x6B
// Remote 7 channel configs
#define TMP468_REMOTE7_TEMP_OFFSET   0x70
#define TMP468_REMOTE7_TEMP_N_FACTOR 0x71
#define TMP468_REMOTE7_NTHERM_LIMIT1 0x72
#define TMP468_REMOTE7_NTHEMP_LIMIT2 0x73
// Remote 8 channel configs
#define TMP468_REMOTE8_TEMP_OFFSET   0x78
#define TMP468_REMOTE8_TEMP_N_FACTOR 0x79
#define TMP468_REMOTE8_NTHERM_LIMIT1 0x7A
#define TMP468_REMOTE8_NTHEMP_LIMIT2 0x7B

#define TMP468_LOCAL_TEMP_AUTO_INC_PTR   0x80
#define TMP468_REMOTE1_TEMP_AUTO_INC_PTR 0x81
#define TMP468_REMOTE2_TEMP_AUTO_INC_PTR 0x82
#define TMP468_REMOTE3_TEMP_AUTO_INC_PTR 0x83
#define TMP468_REMOTE4_TEMP_AUTO_INC_PTR 0x84
#define TMP468_REMOTE5_TEMP_AUTO_INC_PTR 0x85
#define TMP468_REMOTE6_TEMP_AUTO_INC_PTR 0x86
#define TMP468_REMOTE7_TEMP_AUTO_INC_PTR 0x87
#define TMP468_REMOTE8_TEMP_AUTO_INC_PTR 0x88

#define TMP468_LOCK 0xC4

#define TMP468_MAN_ID 0xFE
#define TMP468_DEV_ID 0xFF

// ------- Constants -------

#define TMP468_LOCK_LOCK_VALUE   0X5CA6
#define TMP468_LOCK_UNLOCK_VALUE 0XEB19
#define TMP468_LOCK_IS_LOCK      0x8000
#define TMP468_LOCK_IS_UNLOCK    0x0000

#define TMP468_MAN_ID_VALUE 0X5449
#define TMP468_DEV_ID_VALUE 0x0468

// ------- Enums -------

typedef enum
{
    TMP468_FREQUENCY_HZ0625 = 0,
    TMP468_FREQUENCY_HZ125,
    TMP468_FREQUENCY_HZ25,
    TMP468_FREQUENCY_HZ5,
    TMP468_FREQUENCY_1HZ,
    TMP468_FREQUENCY_2HZ,
    TMP468_FREQUENCY_4HZ,
    TMP468_FREQUENCY_7HZ
} tmp468_conversion_rate_ten;

// ------- Structures -------

typedef union
{
    uint16_t all;
    struct
    {
        uint16_t unused : 1;
        uint16_t BUSY : 1; // 1 when the ADC is converting
        uint16_t CR : 3;   // conversion_rate
        uint16_t SD : 1;   // shutdown
        uint16_t OS : 1;   // one_shut_conversion
        uint16_t LEN : 1;  // local channel conversion
        uint16_t REN : 8;  // respective remote channel 8 through 1 conversions
    } bit;
} tmp468_config_reg_tun;

typedef union
{
    uint16_t all;
    struct
    {
        uint16_t unused : 15;
        uint16_t RST : 1;
    } bit;
} tmp468_software_reset_reg_tun;

typedef union
{
    uint16_t all;
    struct
    {
        uint16_t unused : 8;
        uint16_t R1OPEN : 1;
        uint16_t R2OPEN : 1;
        uint16_t R3OPEN : 1;
        uint16_t R4OPEN : 1;
        uint16_t R5OPEN : 1;
        uint16_t R6OPEN : 1;
        uint16_t R7OPEN : 1;
        uint16_t R8OPEN : 1;
    } bit;
} tmp468_remote_ch_open_status_reg_tun;

typedef union
{
    uint16_t all;
    struct
    {
        uint16_t unused : 7;
        uint16_t LTH : 1;
        uint16_t R1TH : 1;
        uint16_t R2TH : 1;
        uint16_t R3TH : 1;
        uint16_t R4TH : 1;
        uint16_t R5TH : 1;
        uint16_t R6TH : 1;
        uint16_t R7TH : 1;
        uint16_t R8TH : 1;
    } bit;
} tmp468_therm_status_reg_tun;

typedef union
{
    uint16_t all;
    struct
    {
        uint16_t unused : 7;
        uint16_t LTH : 1;
        uint16_t R1TH2 : 1;
        uint16_t R2TH2 : 1;
        uint16_t R3TH2 : 1;
        uint16_t R4TH2 : 1;
        uint16_t R5TH2 : 1;
        uint16_t R6TH2 : 1;
        uint16_t R7TH2 : 1;
        uint16_t R8TH2 : 1;
    } bit;
} tmp468_therm2_status_reg_tun;

typedef union
{
    tmp468_config_reg_tun                config_reg;
    tmp468_software_reset_reg_tun        software_reset;
    tmp468_remote_ch_open_status_reg_tun remote_ch_open_status;
    tmp468_therm_status_reg_tun          therm_status_reg;
    tmp468_therm2_status_reg_tun         therm2_status_reg;
} tmp468_reg_tun;

#endif /* TMP468_REGS_H_ */
