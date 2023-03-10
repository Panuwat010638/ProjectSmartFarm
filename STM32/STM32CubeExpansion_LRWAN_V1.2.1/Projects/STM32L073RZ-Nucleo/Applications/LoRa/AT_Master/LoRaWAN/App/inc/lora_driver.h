/**
  ******************************************************************************
  * @file    lora_driver.h
  * @author  MCD Application Team
  * @brief   Header for lora driver module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LORA_DRIVER_H__
#define __LORA_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hw_conf.h"
//#include "atcmd.h"
#include ATCMD_MODEM        /* preprocessing definition in hw_conf.h*/

/* Exported types ------------------------------------------------------------*/
typedef enum RetCode
{
  MODULE_READY,
  MODULE_NO_READY,
  MODULE_UART_ERROR,
} RetCode_t;


/* LoRa modem State Machine states */
typedef enum eDevicState
{
    DEVICE_INIT,
    DEVICE_READY,
    DEVICE_JOINED,
    DEVICE_SEND,
    DEVICE_SLEEP,
    DEVICE_JOIN_ON_GOING
} DeviceState_t;


/* LoRa Driver modem param*/
typedef struct sLoRaDriverParam
{
  uint16_t  SensorCycleMeasure;     /*Sensor Cycle Measuremnt in ms*/
  uint8_t JoinMode;                /*LoRa Join Mode (OTAA or ABP)*/
  uint8_t BandPlan;
} LoRaDriverParam_t;


/* Lora driver modem callbacks*/
typedef struct sLoRaDriverCallback
{
  void ( *SensorMeasureData )( sSendDataBinary_t *PtrStruct  );   /*Get measure sensor data (pressure, humidity, Temperature and current battery level)*/
  void ( *Callback1 ) ( uint8_t *id);  /*free callback entry point if needed*/
} LoRaDriverCallback_t;

#ifdef USE_LRWAN_NS1
/* LoRa modem key types */
typedef enum eKeyTypes
{
  APPKEY,
  NWKSKEY,
  APPSKEY,
} KeyTypes_t;

/* LoRa modem delay types */
typedef enum eDelayTypes
{
  RX1DL,
  RX2DL,
  JN1DL,
  JN2DL,
} DelayTypes_t;

/* LoRa modem enter sleep mode on/off */
typedef enum eIsEnterSleepMode
{
  AUTOOFF,
  AUTOON,
} IsEnterSleepMode_t;
#endif

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define DELAY_FOR_JOIN_STATUS_REQ    15000  /* wait time to request Join status*/

#ifdef USE_LRWAN_NS1
/* LoRaMac TxPower definition */
#define TX_POWER_20_DBM                             20
#define TX_POWER_18_DBM                             18
#define TX_POWER_16_DBM                             16
#define TX_POWER_14_DBM                             14
#define TX_POWER_12_DBM                             12
#define TX_POWER_10_DBM                             10
#define TX_POWER_8_DBM                              8
#define TX_POWER_6_DBM                              6
#else
/* LoRaMac TxPower definition */
#define TX_POWER_20_DBM                             0
#define TX_POWER_14_DBM                             1
#define TX_POWER_11_DBM                             2
#define TX_POWER_08_DBM                             3
#define TX_POWER_05_DBM                             4
#define TX_POWER_02_DBM                             5
#endif

/* LoRaMac datarates definition */
#define DR_0                                        0  // SF12 - BW125
#define DR_1                                        1  // SF11 - BW125
#define DR_2                                        2  // SF10 - BW125
#define DR_3                                        3  // SF9  - BW125
#define DR_4                                        4  // SF8  - BW125
#define DR_5                                        5  // SF7  - BW125
#define DR_6                                        6  // SF7  - BW250
#define DR_7                                        7  // FSK

/* LoRa confirmation mode on send message */
#define  LORA_CONFIRM_MODE                          1
#define  LORA_UNCONFIRM_MODE                        0

/* LoRa network join mode */
#define  OTAA_JOIN_MODE                             1
#define  ABP_JOIN_MODE                              0

/* LoRa Adaptative Data Rate */
#define  ADAPT_DATA_RATE_ENABLE                      1
#define  ADAPT_DATA_RATE_DISABLE                     0

/* LoRa Class */
#define  LORA_CLASS_A                                0
#define  LORA_CLASS_B                                1
#define  LORA_CLASS_C                                2

/* LoRa duty cycle (only for test) */
#define  DUTY_CYCLE_ENABLE                           1
#define  DUTY_CYCLE_DISABLE                          0

/* LoRa public network mode */
#define  PUBLIC_NETWORK_ON                           1
#define  PUBLIC_NETWORK_OFF                          0


/* Exported functions ------------------------------------------------------- */

/************ LoRa Driver modem Finate State Machine  ************/


/******************************************************************************
  * @Brief lora Modem state machine
  * @param void
  * @retval None
******************************************************************************/
void Lora_fsm( void);


/******************************************************************************
  * @Brief Context InitialModem following the lora device modem used
  * @param Periode to do sensors measurement
  * @retval None
******************************************************************************/
void Lora_Ctx_Init(LoRaDriverCallback_t  *PtrLoRaDriverCallbacks,
                              LoRaDriverParam_t *PtrLoRaDriverParam);


/************ connection management ************/

/**************************************************************
 * @brief  Check if the LoRa module is working
 * @param  void
 * @retval status of the module (ready or not ready)
**************************************************************/
RetCode_t Lora_Init(void);

/**************************************************************
 * @brief  reset of the LoRa module
 * @param  void
 * @retval void
**************************************************************/
void Lora_Reset(void);

/**************************************************************
 * @brief  Do a request to establish a LoRa Connection with the gateway
 * @param  Mode: by OTAA or by ABP
 * @retval LoRA return code
**************************************************************/
ATEerror_t Lora_Join(uint8_t Mode);

/**************************************************************
 * @brief  Do a request to set the Network join Mode
 * @param  Mode : OTA, ABP
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetJoinMode(uint8_t Mode);

/**************************************************************
 * @brief  Do a request to get the Network join Mode
 * @param  pointer to the Join mode out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetJoinMode(uint8_t *Mode);

#ifdef USE_LRWAN_NS1
/**************************************************************
* @brief  Do a request to reset LoRaWAN AT modem to factory default configuration.
* @param void
* @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetDefault(void);

#else
/**************************************************************
 * @brief  Wait for join accept notification either in ABP or OTAA
 * @param  void
 * @retval LoRA return code
 * @Nota this function supports either USI protocol or MDM32L07X01 protocol
**************************************************************/
ATEerror_t Lora_JoinAccept(void);
#endif

/************ MiB management *******************/

/**************************************************************
 * @brief  Set the Application Identifier
 * @param  pointer to the APPEUI in value
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_SetAppID(uint8_t *PtrAppID);

/**************************************************************
 * @brief  Set the device extended universal indentifier
 * @param  pointer to the DEUI in value
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_SetDeviceID(uint8_t *PtrDeviceID);

/**************************************************************
 * @brief  Set the device address
 * @param  pointer to the DADDR in value
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_SetDeviceAddress(uint32_t DeviceAddr);

/**************************************************************
 * @brief  key configuration
 * @param  KeyType : APPKEY, NWKSKE, APPSKEY
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_SetKey(ATCmd_t KeyType, uint8_t *PtrKey);


/**************************************************************
 * @brief  Request the key type configuration
 * @param  KeyType : APPKEY, NWKSKE, APPSKEY
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_GetKey(ATCmd_t KeyType, uint8_t *PtrKey);

/**************************************************************
 * @brief  Request the Application Identifier
 * @param  pointer to the APPEUI out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_GetAppID(uint8_t *AppEui);

/**************************************************************
 * @brief  Request the device extended universal indentifier
 * @param  pointer to the DEUI out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_GetDeviceID(uint8_t *PtrDeviceID);

/**************************************************************
 * @brief  Request the device address
 * @param  pointer to the DADDR out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_GetDeviceAddress(uint32_t *DeviceAddr);

/**************************************************************
 * @brief  Set the NetWork ID
 * @param  NWKID in value
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_SetNetworkID(uint32_t NetworkID);

/**************************************************************
 * @brief  Request the network ID
 * @param  pointer to the NWKID out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_GetNetworkID(uint32_t *NetworkID);


       /************ Network Management ***************/

/**************************************************************
 * @brief  Do a request to set the adaptive data rate
 * @param  ADR in value 0(off) / 1(on)
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetAdaptiveDataRate(uint8_t Rate);

/**************************************************************
 * @brief  Do a request to get the adaptive data rate
 * @param  pointer to ADR out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetAdaptiveDataRate(uint8_t *Rate);

/**************************************************************
 * @brief  Do a request to set the LoRa Class
 * @param  CLASS in value [0,1,2]
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetClass(uint8_t Class);

/**************************************************************
 * @brief  Do a request to get the LoRa class
 * @param  pointer to CLASS out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetClass(uint8_t *Class);

/**************************************************************
 * @brief  Do a request to set the duty cycle
 * @brief  only used in test mode
 * @param  DCS in value 0(disable) / 1(enable)
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetDutyCycle(uint8_t DutyCycle);

/**************************************************************
 * @brief  Do a request to get the duty cycle
 * @param  pointer to DR out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetDutyCycle(uint8_t *DutyCycle);

/**************************************************************
 * @brief  Do a request to set the data Rate
 * @param  DR in value [0,1,2,3,4,5,6,7]
 *         If use LRWAN_NS1, DR range in 0~15 corresponding to band plans
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetDataRate(uint8_t DataRate);

/**************************************************************
 * @brief  Do a request to set the Public Network mode
 * @param  PNM in value 0(off) / 1(on)
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetPublicNetworkMode(uint8_t NetworkMode);

/**************************************************************
 * @brief  Do a request to set the transmit Tx power
 * @param  TXP in value [0,1,2,3,4,5]
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetTxPower(uint8_t TransmitTxPower);

/**************************************************************
 * @brief  Do a request to get the data Rate
 * @param  pointer to DR out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetDataRate(uint8_t *DataRate);

/**************************************************************
 * @brief  Do a request to set the join accept delay between
 * @brief  the end of the Tx and the join Rx#n window
 * @param  RxWindowType : JN1DL, JN2DL
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_SetJoinDelayRxWind(ATCmd_t RxWindowType, uint32_t JoinDelayInMs);

/**************************************************************
 * @brief  Do a request to get the join accept delay between
 * @brief  the end of the Tx and the join Rx#n window
 * @param  RxWindowType : JN1DL, JN2DL
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_GetJoinDelayRxWind(ATCmd_t RxWindowType,uint32_t *JoinDelayInMs);

/**************************************************************
 * @brief  Do a request to get the Public Network mode
 * @param  pointer to PNM out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetPublicNetworkMode(uint8_t *NetworkMode);

/**************************************************************
 * @brief  Do a request to set the delay between the end of the Tx
 * @brief  the end of the Tx and the join Rx#n window
 * @param  RxWindowType : RX1DL, RX2DL
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_SetDelayRxWind(ATCmd_t RxWindowType, uint32_t RxDelayInMs);

/**************************************************************
 * @brief  Do a request to get the delay between the end of the Tx
 * @brief  the end of the Tx and the join Rx#n window
 * @param  RxWindowType : RX1DL, RX2DL
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_GetDelayRxWind(ATCmd_t RxWindowType,uint32_t *RxDelayInMs);

/**************************************************************
 * @brief  Do a request to get the transmit Tx Power
 * @param  pointer to TXP out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetTxPower(uint8_t *TransmitTxPower);

/**************************************************************
 * @brief  Do a request to set the frame counter
 * @param  FrameCounterType : FCD, FCU
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_SetFrameCounter(ATCmd_t FrameCounterType,
                                uint32_t FrameCounternumber);


/**************************************************************
 * @brief  Request the frame counter number
 * @param  frameCounterType : FCD, FCU
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_GetFrameCounter(ATCmd_t FrameCounterType,
                                uint32_t *FrameCounternumber);

/**************************************************************
 * @brief  Set the frequency of the Rx2 window
 * @param  pointer to the RX2FQ in value
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_SetFreqRxWind2(uint32_t Rx2WindFrequency);


/**************************************************************
 * @brief  Request the frequency of the Rx2 window
 * @param  pointer to the RX2FQ out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_GetFreqRxWind2(uint32_t *Rx2WindFrequency);

/**************************************************************
 * @brief  Do a request to set the data Rate of Rx2 window
 * @param  RX2DR in value [0,1,2,3,4,5,6,7]
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetDataRateRxWind2(uint8_t DataRateRxWind2);


/**************************************************************
 * @brief  Do a request to get the data Rate of Rx2 window
 * @param  pointer to RX2DR out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetDataRateRxWind2(uint8_t *DataRateRxWind2);

#ifdef USE_LRWAN_NS1
/**************************************************************
 * @brief  Do a request to set the band plan
 * @param  BandPlan
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetDeviceBand(uint8_t DeviceBand);

/**************************************************************
 * @brief  Do a request to set the data Rate of Rx2 window
 * @param  Freq: Frequency
 * @param  DRoSF: Spread factor value or date rate value
 * @Param  BW: Bandwidth
 * @note   When param BW is 0, the param DRoSF is date rate.
 * @note   When param BW is not 0, the param DRoSF is spread factor
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetRxWind2(float Freq, uint8_t DRoSF, uint16_t BW);

/**************************************************************
* @brief  Do a request to set port
* @param  port
* @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetPort(uint8_t port);

/**************************************************************
* @brief  Do a request to get the maximum payload length which is supported to send according to current data rate
* @param len point to the length out value
* @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetLwLEN(uint8_t *len);

#endif

      /************ Data Path Management ***************/

/**************************************************************
 * @brief  Do a request to get the battery level of the modem (slave)
 * @param  BAT in value
 *              0:    battery connected to external power supply
 *       [1..254]:    1 being at minimum and 254 being at maximum
*             255:    not able to measure the battery level
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetBatLevel(uint32_t *BatLevel);

/**************************************************************
 * @brief  Send text data to a giving prot number
 * @param  SEND in data struct (ptrString,port)
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SendData(sSendDataString_t *PtrStructData);

/**************************************************************
 * @brief  Send binary data to a giving port number
 * @param  SENDB in value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SendDataBin(sSendDataBinary_t *PtrStructData);

#ifdef USE_LRWAN_NS1

/**************************************************************
 * @brief  Do a request to get rx data
 * @brief  received by the Slave
 * @note   Used in CLASS C, and the rx data is stored in "dt.msg"
 * @param  void
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_ReceiveData(void);

#else



/**************************************************************
 * @brief  Do a request to get the last data (in raw format)
 * @brief  received by the Slave
 * @param  pointer to RECV out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_ReceivedData(sReceivedDataString_t *PtrStructData);


/**************************************************************
 * @brief  Trap an asynchronous event coming from external modem (only USI device)
 * @param  Pointer to RCV out value if any
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_AsyncDownLinkData(sReceivedDataBinary_t *PtrStructData);

/**************************************************************
 * @brief  Do a request to get the last data (in binary format)
 * @brief  received by the Slave
 * @param  pointer to RECVB out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_ReceivedDataBin(sReceivedDataBinary_t *PtrStructData);

/**************************************************************
 * @brief  Do a request to set the confirmation mode
 * @param  CFM in value 0(unconfirm) / 1(confirm)
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetSendMsgConfirm(uint8_t ConfirmMode);

/**************************************************************
 * @brief  Do a request to get the confirmation mode
 * @param  pointer to CFM out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetSendMsgConfirm(uint8_t *ConfirmMode);


/**************************************************************
 * @brief  Do a request to get the msg status of the last send cmd
 * @param  CFS in value 0(unconfirm) / 1(confirm)
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetSendMsgStatus(uint8_t *MsgStatus);

/**************************************************************
 * @brief  Do a request to get the RSSI of the last received packet
 * @param  RSSI in value [in dbm]
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetRSSI(int32_t *SigStrengthInd);

/**************************************************************
 * @brief  Do a request to get the SNR of the last received packet
 * @param  SNR in value [in db]
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetSNR(uint32_t *SigToNoice);

/**************************************************************
 * @brief  Do a request to set the country band code for LoRaWAN
 * @brief  Need to write to DCT and Reset module to enable this setting
 * @param  BAND in value 0(EU-868 Band) / 1(US-Band)
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetDeviceBand(uint8_t DeviceBand);

/**************************************************************
 * @brief  Do a request to get the country band code for LoRaWAN
 * @brief  only used in test mode
 * @param  pointer to BAND out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetDeviceBand(uint8_t *DeviceBand);


/**************************************************************
 * @brief  Do a request to get the firmware version of the modem (slave)
 * @param  pointer to VER out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetVersion(uint8_t *PtrVersion);


/**************************************************************
 * @brief  Do a request to get the firmware version of the modem (slave)
 * @param  pointer to FWVERSION out value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_GetFWVersion(uint8_t *PtrFWVersion);
#endif

    /************ Power Control Commands (for USI board and RisingHF board) ***************/
#if USE_LRWAN_NS1
/**************************************************************
* @brief  Do a request to make module enter sleep mode with ultra-low power consumption
* @param IsEnterSleepMode: The param can be "ATUOON"/"AUTOOFF"
*               AUTOON: Enter extremely low power mode
*               AUTOOFF: Exit extremely low power mode
* @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SleepMode(uint8_t IsEnterSleepMode);

/**************************************************************
* @brief  Do a request to turn on/off internal watchdog of the modlue
* @note The watchdog is on by default, this will enhance the module stability
* @note After WDT is turned on, the sleep current will be inreased by around 0.7uA
* @param onoff: The param can be "ON"/"OFF"
* @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetWDT(uint8_t onoff);

#if USE_I_NUCLEO_LRWAN1
/**************************************************************
 * @brief  Do a request to enter the slave in sleep (MCU STOP mode)
 * @param  Void
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SleepMode(void);

/**************************************************************
 * @brief  Do a request to set the power control settings of the MCU (slave)
 * @param  Power control IN value
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_SetMCUPowerCtrl(sPowerCtrlSet_t *PtrStructData);

/**************************************************************
 * @brief  Do a Dumy request to resynchronize the Host and the modem
 * @note   A simple AT cmd where we do not trap the return code
 * @param  void
 * @retval LoRa return code
**************************************************************/
ATEerror_t LoRa_DumyRequest(void);


              /************ DCT commands ***************/

/**************************************************************
 * @brief  Do a request to restore DCT content table with default values
 * @param  void
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_RestoreConfigTable(void);


/**************************************************************
 * @brief  Do a request to update the DCT content table with new values
 * @param  void
 * @retval LoRa return code
**************************************************************/
ATEerror_t Lora_UpdateConfigTable(void);
#endif
#endif

/******************************************************************************
  * @Brief get the current device finate state
  * @param void
  * @retval deviceState
******************************************************************************/
DeviceState_t lora_getDeviceState( void );

#ifdef __cplusplus
}
#endif

#endif /* __LORA_DRIVER_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
