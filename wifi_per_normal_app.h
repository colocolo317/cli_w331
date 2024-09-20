/***************************************************************************/ /**
 * @file app.h
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef WIFI_PER_NORMAL_APP_H
#define WIFI_PER_NORMAL_APP_H
#include "stdint.h"

#define IP_ADDR_MAX_LEN 15
#define TX_RX_MASK (1 << 0)
#define UDP_TCP_MASK (1 << 1)

typedef enum wifi_device_mode_e
{
  wifi_sta_mode,
  wifi_ap_mode,
} wifi_device_mode_t;

typedef enum troughput_type_e
{
  udp_tx, //0b0000
  udp_rx, //0b0001
  tcp_tx, //0b0010
  tcp_rx, //0b0011
  tls_tx, //0b0100
  tls_rx, //0b0101
} troughput_type_t;

typedef struct
{
  uint8_t ap_sta_mode;
  uint8_t tcp_udp_x_tx_rx;
  uint8_t security; // open/wpa2
  uint16_t channel;
  uint8_t region;
  uint16_t timeout_sec;
  char* str_ip;  // len: [IP_ADDR_MAX_LEN+1];  for sta mode
}
wifi_per_normal_config_t;

/***************************************************************************/ /**
 * Initialize application.
 ******************************************************************************/
void wifi_per_normal_start(void *args);


#endif // APP_H
