/**
 * Include files
 **/
//! SL Wi-Fi SDK includes
/*
#include "sl_board_configuration.h"
#include "sl_wifi.h"
#include "sl_wifi_callback_framework.h"
#include "cmsis_os2.h"

//BLE Specific inclusions
#include <rsi_ble_apis.h>
#include "ble_config.h"
#include "rsi_ble_common_config.h"

#include <rsi_common_apis.h>
*/
#include "cmsis_os2.h"
#include "sl_wifi.h"
#include "sl_constants.h"
#include "ble_config.h"
#include "rsi_ble_common_config.h"
#include <rsi_ble_apis.h>
#include "rsi_bt_common_apis.h"
#include "console.h"
#include <stdio.h>
#include <string.h>
/*
// APP version
#define APP_FW_VERSION "0.4"

// Function prototypes
extern void sl_wifi_app_task(void);
extern void rsi_ble_configurator_task(void *argument);
void rsi_ble_configurator_init(void);
uint8_t magic_word;

osSemaphoreId_t wlan_thread_sem;
osSemaphoreId_t ble_thread_sem;
*/
#define DISABLE 0
#define ENABLE  1

#define LE_ONE_MBPS       1
#define LE_TWO_MBPS       2
#define LE_125_KBPS_CODED 4
#define LE_500_KBPS_CODED 8

#define DATA_PRBS9                 0x00
#define DATA_FOUR_ONES_FOUR_ZEROES 0x01
#define DATA_ALT_ONES_AND_ZEROES   0x02
#define DATA_PRSB15                0x03
#define DATA_ALL_ONES              0x04
#define DATA_ALL_ZEROES            0x05
#define DATA_FOUR_ZEROES_FOUR_ONES 0x06
#define DATA_ALT_ZEROES_AND_ONES   0x07

#define LE_ADV_CHNL_TYPE  0
#define LE_DATA_CHNL_TYPE 1

#define BURST_MODE     0
#define CONTIUOUS_MODE 1

#define NO_HOPPING     0
#define FIXED_HOPPING  1
#define RANDOM_HOPPING 2

#define BT_PER_STATS_CMD_ID 0x08
#define BLE_TRANSMIT_CMD_ID 0x13
#define BLE_RECEIVE_CMD_ID  0x14

#define BLE_ACCESS_ADDR    0x71764230
#define BLE_TX_PKT_LEN     32
#define BLE_PHY_RATE       LE_ONE_MBPS
#define BLE_RX_CHNL_NUM    10
#define BLE_TX_CHNL_NUM    10
#define BLE_TX_POWER_INDEX 8
#define SCRAMBLER_SEED     0
#define NUM_PKTS           0
#define RSI_INTER_PKT_GAP  0

#define ONBOARD_ANT_SEL 2
#define EXT_ANT_SEL     3

#define BLE_EXTERNAL_RF 0
#define BLE_INTERNAL_RF 1

#define NO_CHAIN_SEL      0
#define WLAN_HP_CHAIN_BIT 0
#define WLAN_LP_CHAIN_BIT 1
#define BT_HP_CHAIN_BIT   2
#define BT_LP_CHAIN_BIT   3

#define PLL_MODE_0 0
#define PLL_MODE_1 1

#define LOOP_BACK_MODE_DISABLE 0
#define LOOP_BACK_MODE_ENABLE  1

#define EXT_DATA_LEN_IND 1

#define DUTY_CYCLING_DISABLE 0
#define DUTY_CYCLING_ENABLE  1

typedef enum amk_ble_per_trx
{
  ble_per_tx_rx,
  ble_per_tx,
  ble_per_rx,
} amk_ble_per_trx_t;

typedef struct stats_periodic_args
{
  uint8_t trx_type;
}stats_periodic_args_t;

#if 1
const sl_wifi_device_configuration_t sl_wifi_ble_configuration_cli = {
    .boot_option = LOAD_NWP_FW,
    .mac_address = NULL,
    .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
    .region_code = US,
    .boot_config = { .oper_mode = SL_SI91X_CLIENT_MODE,
                     .coex_mode = SL_SI91X_BLE_MODE,
#ifdef SLI_SI91X_MCU_INTERFACE
                     .feature_bit_map = (SL_SI91X_FEAT_WPS_DISABLE | RSI_FEATURE_BIT_MAP),
#else
                     .feature_bit_map = RSI_FEATURE_BIT_MAP,
#endif
#if RSI_TCP_IP_BYPASS
                     .tcp_ip_feature_bit_map = RSI_TCP_IP_FEATURE_BIT_MAP,
#else
                     .tcp_ip_feature_bit_map = (RSI_TCP_IP_FEATURE_BIT_MAP | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID),
#endif
                     .custom_feature_bit_map = (SL_SI91X_CUSTOM_FEAT_EXTENTION_VALID | RSI_CUSTOM_FEATURE_BIT_MAP),
                     .ext_custom_feature_bit_map =
                         (SL_SI91X_EXT_FEAT_XTAL_CLK | MEMORY_CONFIG
#if ENABLE_POWERSAVE_CLI
                          | SL_SI91X_EXT_FEAT_LOW_POWER_MODE
#endif
#ifdef SLI_SI917
                          | SL_SI91X_EXT_FEAT_FRONT_END_SWITCH_PINS_ULP_GPIO_4_5_0
#endif
                          | SL_SI91X_EXT_FEAT_BT_CUSTOM_FEAT_ENABLE),
                     .bt_feature_bit_map = (RSI_BT_FEATURE_BITMAP
#if (RSI_BT_GATT_ON_CLASSIC)
                                            | SL_SI91X_BT_ATT_OVER_CLASSIC_ACL /* to support att over classic acl link */
#endif
                                            ),
#ifdef RSI_PROCESS_MAX_RX_DATA
                     .ext_tcp_ip_feature_bit_map = (RSI_EXT_TCPIP_FEATURE_BITMAP | SL_SI91X_CONFIG_FEAT_EXTENTION_VALID
                                                    | SL_SI91X_EXT_TCP_MAX_RECV_LENGTH),
#else
                     .ext_tcp_ip_feature_bit_map = (RSI_EXT_TCPIP_FEATURE_BITMAP | SL_SI91X_CONFIG_FEAT_EXTENTION_VALID),
#endif
                   //!ENABLE_BLE_PROTOCOL in bt_feature_bit_map
                     .ble_feature_bit_map =
                       ((SL_SI91X_BLE_MAX_NBR_PERIPHERALS(RSI_BLE_MAX_NBR_PERIPHERALS)
                         | SL_SI91X_BLE_MAX_NBR_CENTRALS(RSI_BLE_MAX_NBR_CENTRALS)
                         | SL_SI91X_BLE_MAX_NBR_ATT_SERV(RSI_BLE_MAX_NBR_ATT_SERV)
                         | SL_SI91X_BLE_MAX_NBR_ATT_REC(RSI_BLE_MAX_NBR_ATT_REC))
                        | SL_SI91X_FEAT_BLE_CUSTOM_FEAT_EXTENTION_VALID | SL_SI91X_BLE_PWR_INX(RSI_BLE_PWR_INX)
                        | SL_SI91X_BLE_PWR_SAVE_OPTIONS(RSI_BLE_PWR_SAVE_OPTIONS)
                        | SL_SI91X_916_BLE_COMPATIBLE_FEAT_ENABLE
#if RSI_BLE_GATT_ASYNC_ENABLE
                        | SL_SI91X_BLE_GATT_ASYNC_ENABLE
#endif
                        ),

                     .ble_ext_feature_bit_map =
                       ((SL_SI91X_BLE_NUM_CONN_EVENTS(RSI_BLE_NUM_CONN_EVENTS)
                         | SL_SI91X_BLE_NUM_REC_BYTES(RSI_BLE_NUM_REC_BYTES))
#if RSI_BLE_INDICATE_CONFIRMATION_FROM_HOST
                        | SL_SI91X_BLE_INDICATE_CONFIRMATION_FROM_HOST //indication response from app
#endif
#if RSI_BLE_MTU_EXCHANGE_FROM_HOST
                        | SL_SI91X_BLE_MTU_EXCHANGE_FROM_HOST //MTU Exchange request initiation from app
#endif
#if RSI_BLE_SET_SCAN_RESP_DATA_FROM_HOST
                        | (SL_SI91X_BLE_SET_SCAN_RESP_DATA_FROM_HOST) //Set SCAN Resp Data from app
#endif
#if RSI_BLE_DISABLE_CODED_PHY_FROM_HOST
                        | (SL_SI91X_BLE_DISABLE_CODED_PHY_FROM_HOST) //Disable Coded PHY from app
#endif
#if BLE_SIMPLE_GATT
                        | SL_SI91X_BLE_GATT_INIT
#endif
                      ),
                     .config_feature_bit_map = 0 }
};
#endif

const osThreadAttr_t ble_stats_periodic_thread_attr = {
  .name       = "ble_stats_periodic_thread",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 1024,
  .priority   = 0,
  .tz_module  = 0,
  .reserved   = 0,
};

static stats_periodic_args_t trx_run = {ble_per_tx_rx};
static volatile uint8_t amk_ble_stats_periodic = 0;
static rsi_ble_per_transmit_t rsi_ble_per_tx;
static rsi_ble_per_receive_t rsi_ble_per_rx;
static rsi_bt_per_stats_t per_stats;
static rsi_bt_resp_get_local_name_t rsi_app_resp_get_local_name = { 0 };
static uint8_t rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN]      = { 0 };
uint8_t local_dev_addr[18]    = { 0 };

uint8_t adv[31] = { 2, 1, 6 };
uint32_t total_crc_fail_cnt=0, total_crc_pass_cnt=0, total_tx_dones=0;
float total_per=0;

void amk_ble_stats_print(void* args);
sl_status_t amk_bt_per_stats_periodic_new_thread(void* args);

sl_status_t rsi_ble_per_transmit_command_handler(console_args_t *arguments)
{
  sl_status_t status   = SL_STATUS_OK;
  //uint32_t pkt_num = 0;
  rsi_ble_per_tx.cmd_ix                       = BLE_TRANSMIT_CMD_ID;
  rsi_ble_per_tx.transmit_enable              = (uint8_t)GET_COMMAND_ARG(arguments, 0);
  if(rsi_ble_per_tx.transmit_enable == 0)
  {
    amk_ble_stats_periodic = 0;
    trx_run.trx_type = ble_per_tx_rx;
  }
  else
  {
    amk_ble_stats_periodic = 1;
    trx_run.trx_type = ble_per_tx;
  }

  *(uint32_t *)&rsi_ble_per_tx.access_addr[0] = BLE_ACCESS_ADDR;
  *(uint16_t *)&rsi_ble_per_tx.pkt_len[0]     = (uint16_t)GET_COMMAND_ARG(arguments, 1);
  rsi_ble_per_tx.phy_rate                     = (uint8_t)GET_COMMAND_ARG(arguments, 2);
  rsi_ble_per_tx.rx_chnl_num                  = (uint8_t)GET_COMMAND_ARG(arguments, 3);
  rsi_ble_per_tx.tx_chnl_num                  = (uint8_t)GET_COMMAND_ARG(arguments, 3);
  rsi_ble_per_tx.scrambler_seed               = SCRAMBLER_SEED;
  rsi_ble_per_tx.payload_type                 = GET_OPTIONAL_COMMAND_ARG(arguments, 11, DATA_PRBS9, const uint8_t); // DATA_PRBS9;
  rsi_ble_per_tx.le_chnl_type                 = LE_DATA_CHNL_TYPE;
  rsi_ble_per_tx.tx_power                     = (uint8_t)GET_COMMAND_ARG(arguments, 4);
  rsi_ble_per_tx.transmit_mode                = (uint8_t)GET_COMMAND_ARG(arguments, 5);
  rsi_ble_per_tx.freq_hop_en                  = GET_OPTIONAL_COMMAND_ARG(arguments, 6, NO_HOPPING, const uint8_t);
  rsi_ble_per_tx.ant_sel                      = GET_OPTIONAL_COMMAND_ARG(arguments, 7, ONBOARD_ANT_SEL, const uint8_t);
  rsi_ble_per_tx.inter_pkt_gap                = GET_OPTIONAL_COMMAND_ARG(arguments, 8, 0, const uint8_t);
  rsi_ble_per_tx.pll_mode                     = PLL_MODE_0;
  rsi_ble_per_tx.rf_type                      = BLE_INTERNAL_RF;
  rsi_ble_per_tx.rf_chain                     = GET_OPTIONAL_COMMAND_ARG(arguments, 9, BT_HP_CHAIN_BIT, const uint8_t);
  *(uint32_t *)&rsi_ble_per_tx.num_pkts[0]     = GET_OPTIONAL_COMMAND_ARG(arguments, 10, 0, const uint32_t);
  status = rsi_ble_per_transmit(&rsi_ble_per_tx);
  osDelay(1000);
  status = amk_bt_per_stats_periodic_new_thread(&trx_run);
  //status = rsi_bt_per_stats(BT_PER_STATS_CMD_ID, &per_stats);

  VERIFY_STATUS_AND_RETURN(status);
  return status;
}

sl_status_t rsi_ble_per_receive_command_handler(console_args_t *arguments)
{
  sl_status_t status   = SL_STATUS_OK;

  rsi_ble_per_rx.cmd_ix                       = BLE_RECEIVE_CMD_ID;
  rsi_ble_per_rx.receive_enable               = (uint8_t)GET_COMMAND_ARG(arguments, 0);
  if(rsi_ble_per_rx.receive_enable == 0)
  {
    amk_ble_stats_periodic = 0;
    trx_run.trx_type = ble_per_tx_rx;
  }
  else
  {
    amk_ble_stats_periodic = 1;
    trx_run.trx_type = ble_per_rx;
  }

  *(uint32_t *)&rsi_ble_per_rx.access_addr[0] = BLE_ACCESS_ADDR;
  rsi_ble_per_rx.ext_data_len_indication      = EXT_DATA_LEN_IND;
  rsi_ble_per_rx.phy_rate                     = (uint8_t)GET_COMMAND_ARG(arguments, 1);
  rsi_ble_per_rx.rx_chnl_num                  = (uint8_t)GET_COMMAND_ARG(arguments, 2);
  rsi_ble_per_rx.tx_chnl_num                  = (uint8_t)GET_COMMAND_ARG(arguments, 2);
  rsi_ble_per_rx.scrambler_seed               = SCRAMBLER_SEED;
  rsi_ble_per_rx.le_chnl_type                 = LE_DATA_CHNL_TYPE;
  rsi_ble_per_rx.loop_back_mode               = LOOP_BACK_MODE_DISABLE;
  rsi_ble_per_rx.freq_hop_en                  = NO_HOPPING;
  rsi_ble_per_rx.ant_sel                      = GET_OPTIONAL_COMMAND_ARG(arguments, 3, ONBOARD_ANT_SEL, const uint8_t);
  rsi_ble_per_rx.duty_cycling_en              = DUTY_CYCLING_DISABLE;
  rsi_ble_per_rx.pll_mode                     = PLL_MODE_0;
  rsi_ble_per_rx.rf_type                      = BLE_INTERNAL_RF;
  //rsi_ble_per_rx.rf_chain                     = GET_OPTIONAL_COMMAND_ARG(arguments, 4, BT_HP_CHAIN_BIT, const uint8_t); //stone change 2024/6/6 for ble rx test
  rsi_ble_per_rx.rf_chain                     = GET_OPTIONAL_COMMAND_ARG(arguments, 4, 2, const uint8_t); //stone change 2024/6/6 for ble rx test

  //! start the Receive PER functionality
  status = rsi_ble_per_receive(&rsi_ble_per_rx);
  osDelay(1000);
  status = amk_bt_per_stats_periodic_new_thread(&trx_run);

  VERIFY_STATUS_AND_RETURN(status);
  return status;
}

sl_status_t rsi_bt_per_stats_command_handler(console_args_t *arguments)
{
  sl_status_t status   = SL_STATUS_OK;
  status = rsi_bt_per_stats(BT_PER_STATS_CMD_ID, &per_stats);
  VERIFY_STATUS_AND_RETURN(status);
  float fail_rate;

  switch(arguments->arg[0])
  {
    case ble_per_tx:
      total_tx_dones = total_tx_dones + per_stats.tx_dones;
      printf(
          "id_pkts_send: %u "
          "tx_pkt_send: %u total_tx_pkt: %lu \r\n",
          per_stats.id_pkts_rcvd,
          per_stats.tx_dones, total_tx_dones
      );
      break;
    case ble_per_rx:
      fail_rate = 100.0 * per_stats.crc_fail_cnt/(per_stats.crc_fail_cnt + per_stats.crc_pass_cnt);
      total_crc_fail_cnt = total_crc_fail_cnt + per_stats.crc_fail_cnt;
      total_crc_pass_cnt = total_crc_pass_cnt + per_stats.crc_pass_cnt;
      total_per = 100.0 * total_crc_fail_cnt/(total_crc_fail_cnt + total_crc_pass_cnt);
      printf(
          "id_pkts_rcvd: %6u crc_fail: %3u crc_pass: %8u faild_rate: %.3f%%\r\n"
          "rssi: %4d [Summary] crc_fail: %3lu crc_pass: %8lu faild_rate: %.3f%%\r\n\r\n",
          per_stats.id_pkts_rcvd, per_stats.crc_fail_cnt, per_stats.crc_pass_cnt, fail_rate,
          per_stats.rssi, total_crc_fail_cnt, total_crc_pass_cnt, total_per
      );
      break;

    case ble_per_tx_rx:
    default:
      total_tx_dones = total_tx_dones + per_stats.tx_dones;
      fail_rate = 100.0 * per_stats.crc_fail_cnt/(per_stats.crc_fail_cnt + per_stats.crc_pass_cnt);
      total_crc_fail_cnt = total_crc_fail_cnt + per_stats.crc_fail_cnt;
      total_crc_pass_cnt = total_crc_pass_cnt + per_stats.crc_pass_cnt;
      total_per = 100.0 * total_crc_fail_cnt/(total_crc_fail_cnt + total_crc_pass_cnt);
      printf(
          "id_pkts_send: %u "
          "tx_pkt_send: %u total_tx_pkt: %lu \r\n",
          per_stats.id_pkts_rcvd,
          per_stats.tx_dones, total_tx_dones
      );
      printf(
          "id_pkts_rcvd: %6u crc_fail: %3u crc_pass: %8u faild_rate: %.3f%%\r\n"
          "rssi: %4d [Summary] crc_fail: %3lu crc_pass: %8lu faild_rate: %.3f%%\r\n\r\n",
          per_stats.id_pkts_rcvd, per_stats.crc_fail_cnt, per_stats.crc_pass_cnt, fail_rate,
          per_stats.rssi, total_crc_fail_cnt, total_crc_pass_cnt, total_per
      );

      break;
  }

  return status;
}

void amk_ble_stats_print(void* args)
{
  static console_args_t console_arg = {0};
  stats_periodic_args_t trx_run = *(stats_periodic_args_t*) args;
  console_arg.arg[0] = trx_run.trx_type;
  do
  {
    rsi_bt_per_stats_command_handler(&console_arg);
  }  while(osDelay(1000) == osOK && amk_ble_stats_periodic != 0);

  //amk_ble_stats_periodic = 1;
  osThreadExit();
}

sl_status_t amk_bt_per_stats_periodic_new_thread(void* args)
{
  if(osThreadNew( amk_ble_stats_print, args, &ble_stats_periodic_thread_attr) != NULL)
  { return SL_STATUS_OK; }
  else
  { return SL_STATUS_FAIL; }
}

sl_status_t rsi_bt_per_stats_periodic_command_handler(console_args_t *arguments)
{
  UNUSED_PARAMETER(arguments);
  return amk_bt_per_stats_periodic_new_thread(&trx_run);
}

sl_status_t rsi_bt_set_local_name_command_handler(console_args_t *arguments)
{
  UNUSED_PARAMETER(arguments);
  sl_status_t status   = SL_STATUS_OK;
  status = rsi_bt_set_local_name((uint8_t *)RSI_BLE_LOCAL_NAME);
  VERIFY_STATUS_AND_RETURN(status);
  return status;
}

sl_status_t rsi_bt_get_local_name_command_handler(console_args_t *arguments)
{
  UNUSED_PARAMETER(arguments);
  sl_status_t status   = SL_STATUS_OK;
  status = rsi_bt_get_local_name(&rsi_app_resp_get_local_name);
  VERIFY_STATUS_AND_RETURN(status);

  printf("\r\n%s\r\n",rsi_app_resp_get_local_name.name);
  return status;
}

sl_status_t rsi_bt_get_local_device_address_command_handler(console_args_t *arguments)
{
  UNUSED_PARAMETER(arguments);
  sl_status_t status   = SL_STATUS_OK;
  status = rsi_bt_get_local_device_address(rsi_app_resp_get_dev_addr);
  VERIFY_STATUS_AND_RETURN(status);

  rsi_6byte_dev_address_to_ascii(local_dev_addr, (uint8_t *)rsi_app_resp_get_dev_addr);
  printf("\r\n%s\r\n", local_dev_addr);
  return status;
}

sl_status_t rsi_ble_set_advertise_data_command_handler(console_args_t *arguments)
{
  UNUSED_PARAMETER(arguments);
  sl_status_t status   = SL_STATUS_OK;

  //! prepare advertise data //local/device name
  adv[3] = strlen(RSI_BLE_LOCAL_NAME) + 1;
  adv[4] = 9;
  strcpy((char *)&adv[5], RSI_BLE_LOCAL_NAME);

  status = rsi_ble_set_advertise_data(adv, strlen(RSI_BLE_LOCAL_NAME) + 5);
  VERIFY_STATUS_AND_RETURN(status);
  return status;
}

sl_status_t rsi_ble_start_advertising_command_handler(console_args_t *arguments)
{
  UNUSED_PARAMETER(arguments);
  sl_status_t status   = SL_STATUS_OK;
  status = rsi_ble_start_advertising();
  VERIFY_STATUS_AND_RETURN(status);
  return status;
}

sl_status_t rsi_ble_stop_advertising_command_handler(console_args_t *arguments)
{
  UNUSED_PARAMETER(arguments);
  sl_status_t status   = SL_STATUS_OK;
  status = rsi_ble_stop_advertising();
  VERIFY_STATUS_AND_RETURN(status);
  return status;
}
