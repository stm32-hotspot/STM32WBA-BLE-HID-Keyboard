/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    service1_app.c
  * @author  MCD Application Team
  * @brief   service1_app application definition.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "app_ble.h"
#include "ll_sys_if.h"
#include "dbg_trace.h"
#include "ble.h"
#include "hids_app.h"
#include "hids.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
  uint8_t modifier;
  int8_t OEM;
  int8_t KEY1;
  int8_t KEY2;
  int8_t KEY3;
  int8_t KEY4;
  int8_t KEY5;
  int8_t KEY6;
} keyboard_report_t;

typedef struct hidValueS {
  uint8_t key;
  uint8_t hid[2];
} hidValueType;

/* USER CODE END PTD */

typedef enum
{
  Inputrep_NOTIFICATION_OFF,
  Inputrep_NOTIFICATION_ON,
  /* USER CODE BEGIN Service1_APP_SendInformation_t */

  /* USER CODE END Service1_APP_SendInformation_t */
  HIDS_APP_SENDINFORMATION_LAST
} HIDS_APP_SendInformation_t;

typedef struct
{
  HIDS_APP_SendInformation_t     Inputrep_Notification_Status;
  /* USER CODE BEGIN Service1_APP_Context_t */
  uint8_t RxChar;
  uint8_t ShiftEnable;
  uint8_t ProtocolMode;
  uint8_t NumLock;
  uint8_t CapsLock;
  uint8_t ScrollLock;
  /* USER CODE END Service1_APP_Context_t */
  uint16_t              ConnectionHandle;
} HIDS_APP_Context_t;

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_0                                                               0x30
#define NUM_9                                                               0x39
#define CHAR_A                                                              0x41
#define CHAR_Z                                                              0x5A
#define CHAR_a                                                              0x61
#define CHAR_z                                                              0x7A
#define RETURN                                                              0x0D
#define BACKSPACE                                                           0x08
#define TAB                                                                 0x09
#define SPACE                                                               0x20
#define CAPS_LOCK                                                           0x14

#define KEY_TABLE_LEN                                                         33

#define KEY_MOD_LCTRL  0x01
#define KEY_MOD_LSHIFT 0x02
#define KEY_MOD_LALT   0x04
#define KEY_MOD_LMETA  0x08
#define KEY_MOD_RCTRL  0x10
#define KEY_MOD_RSHIFT 0x20
#define KEY_MOD_RALT   0x40
#define KEY_MOD_RMETA  0x80
  
#define KEY_NONE 0x00 // No key pressed
#define KEY_ERR_OVF 0x01 //  Keyboard Error Roll Over - used for all slots if too many keys are pressed ("Phantom key")
// 0x02 //  Keyboard POST Fail
// 0x03 //  Keyboard Error Undefined
#define KEY_A 0x04 // Keyboard a and A
#define KEY_B 0x05 // Keyboard b and B
#define KEY_C 0x06 // Keyboard c and C
#define KEY_D 0x07 // Keyboard d and D
#define KEY_E 0x08 // Keyboard e and E
#define KEY_F 0x09 // Keyboard f and F
#define KEY_G 0x0a // Keyboard g and G
#define KEY_H 0x0b // Keyboard h and H
#define KEY_I 0x0c // Keyboard i and I
#define KEY_J 0x0d // Keyboard j and J
#define KEY_K 0x0e // Keyboard k and K
#define KEY_L 0x0f // Keyboard l and L
#define KEY_M 0x10 // Keyboard m and M
#define KEY_N 0x11 // Keyboard n and N
#define KEY_O 0x12 // Keyboard o and O
#define KEY_P 0x13 // Keyboard p and P
#define KEY_Q 0x14 // Keyboard q and Q
#define KEY_R 0x15 // Keyboard r and R
#define KEY_S 0x16 // Keyboard s and S
#define KEY_T 0x17 // Keyboard t and T
#define KEY_U 0x18 // Keyboard u and U
#define KEY_V 0x19 // Keyboard v and V
#define KEY_W 0x1a // Keyboard w and W
#define KEY_X 0x1b // Keyboard x and X
#define KEY_Y 0x1c // Keyboard y and Y
#define KEY_Z 0x1d // Keyboard z and Z

#define KEY_1 0x1e // Keyboard 1 and !
#define KEY_2 0x1f // Keyboard 2 and @
#define KEY_3 0x20 // Keyboard 3 and #
#define KEY_4 0x21 // Keyboard 4 and $
#define KEY_5 0x22 // Keyboard 5 and %
#define KEY_6 0x23 // Keyboard 6 and ^
#define KEY_7 0x24 // Keyboard 7 and &
#define KEY_8 0x25 // Keyboard 8 and *
#define KEY_9 0x26 // Keyboard 9 and (
#define KEY_0 0x27 // Keyboard 0 and )

#define KEY_ENTER 0x28 // Keyboard Return (ENTER)
#define KEY_ESC 0x29 // Keyboard ESCAPE
#define KEY_BACKSPACE 0x2a // Keyboard DELETE (Backspace)
#define KEY_TAB 0x2b // Keyboard Tab
#define KEY_SPACE 0x2c // Keyboard Spacebar
#define KEY_MINUS 0x2d // Keyboard - and _
#define KEY_EQUAL 0x2e // Keyboard = and +
#define KEY_LEFTBRACE 0x2f // Keyboard [ and {
#define KEY_RIGHTBRACE 0x30 // Keyboard ] and }
#define KEY_BACKSLASH 0x31 // Keyboard \ and |
#define KEY_HASHTILDE 0x32 // Keyboard Non-US # and ~
#define KEY_SEMICOLON 0x33 // Keyboard ; and :
#define KEY_APOSTROPHE 0x34 // Keyboard ' and "
#define KEY_GRAVE 0x35 // Keyboard ` and ~
#define KEY_COMMA 0x36 // Keyboard , and <
#define KEY_DOT 0x37 // Keyboard . and >
#define KEY_SLASH 0x38 // Keyboard / and ?
#define KEY_CAPSLOCK 0x39 // Keyboard Caps Lock

#define KEY_F1 0x3a // Keyboard F1
#define KEY_F2 0x3b // Keyboard F2
#define KEY_F3 0x3c // Keyboard F3
#define KEY_F4 0x3d // Keyboard F4
#define KEY_F5 0x3e // Keyboard F5
#define KEY_F6 0x3f // Keyboard F6
#define KEY_F7 0x40 // Keyboard F7
#define KEY_F8 0x41 // Keyboard F8
#define KEY_F9 0x42 // Keyboard F9
#define KEY_F10 0x43 // Keyboard F10
#define KEY_F11 0x44 // Keyboard F11
#define KEY_F12 0x45 // Keyboard F12
#define KEY_F13 0x68 // Keyboard F13
#define KEY_F14 0x69 // Keyboard F14
#define KEY_F15 0x6A // Keyboard F15
#define KEY_F16 0x6B // Keyboard F16
#define KEY_F17 0x6C // Keyboard F17
#define KEY_F18 0x6D // Keyboard F18
#define KEY_F19 0x6E // Keyboard F19
#define KEY_F20 0x6F // Keyboard F20  
  
#define KEY_LEFTCTRL 0xe0 // Keyboard Left Control
#define KEY_LEFTSHIFT 0xe1 // Keyboard Left Shift
#define KEY_LEFTALT 0xe2 // Keyboard Left Alt
#define KEY_LEFTMETA 0xe3 // Keyboard Left GUI
#define KEY_RIGHTCTRL 0xe4 // Keyboard Right Control
#define KEY_RIGHTSHIFT 0xe5 // Keyboard Right Shift
#define KEY_RIGHTALT 0xe6 // Keyboard Right Alt
#define KEY_RIGHTMETA 0xe7 // Keyboard Right GUI

#define KEY_MEDIA_PLAYPAUSE 0xe8
#define KEY_MEDIA_STOPCD 0xe9
#define KEY_MEDIA_PREVIOUSSONG 0xea
#define KEY_MEDIA_NEXTSONG 0xeb
#define KEY_MEDIA_EJECTCD 0xec
#define KEY_MEDIA_VOLUMEUP 0xed
#define KEY_MEDIA_VOLUMEDOWN 0xee
#define KEY_MEDIA_MUTE 0xef
#define KEY_MEDIA_WWW 0xf0
#define KEY_MEDIA_BACK 0xf1
#define KEY_MEDIA_FORWARD 0xf2
#define KEY_MEDIA_STOP 0xf3
#define KEY_MEDIA_FIND 0xf4
#define KEY_MEDIA_SCROLLUP 0xf5
#define KEY_MEDIA_SCROLLDOWN 0xf6
#define KEY_MEDIA_EDIT 0xf7
#define KEY_MEDIA_SLEEP 0xf8
#define KEY_MEDIA_COFFEE 0xf9
#define KEY_MEDIA_REFRESH 0xfa
#define KEY_MEDIA_CALC 0xfb

#define BOUNCE_THRESHOLD                                                      1U
#define BUTTON_PRESSED                                            GPIO_PIN_RESET
/* USER CODE END PD */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static HIDS_APP_Context_t HIDS_APP_Context;

uint8_t a_HIDS_UpdateCharData[247];

/* USER CODE BEGIN PV */
static uint8_t report_keyboard[] =
{
  0x05, 0x01,       // Usage Page (Generic Desktop)
  0x09, 0x06,       // Usage (Keyboard)
  0xA1, 0x01,       // Collection (Application)
  0x05, 0x07,       //  Usage Page (Key Codes)
  0x19, 0xe0,       //  Usage Minimum (224)
  0x29, 0xe7,       //  Usage Maximum (231)
  0x15, 0x00,       //  Logical Minimum (0)
  0x25, 0x01,       //  Logical Maximum (1)
  0x75, 0x01,       //  Report Size (1)
  0x95, 0x08,       //  Report Count (8)
  0x81, 0x02,       //  Input (Data, Variable, Absolute)

  0x95, 0x01,       //  Report Count (1)
  0x75, 0x08,       //  Report Size (8)
  0x81, 0x01,       //  Input (Constant) reserved byte(1)

  0x95, 0x05,       //  Report Count (5)
  0x75, 0x01,       //  Report Size (1)
  0x05, 0x08,       //  Usage Page (Page# for LEDs)
  0x19, 0x01,       //  Usage Minimum (1)
  0x29, 0x05,       //  Usage Maximum (5)
  0x91, 0x02,       //  Output (Data, Variable, Absolute), Led report
  
  0x95, 0x01,       //  Report Count (1)
  0x75, 0x03,       //  Report Size (3)
  0x91, 0x01,       //  Output (Data, Variable, Absolute), Led report padding

  0x95, 0x06,       //  Report Count (6)
  0x75, 0x08,       //  Report Size (8)
  0x15, 0x00,       //  Logical Minimum (0)
  0x25, 0x65,       //  Logical Maximum (101)
  0x05, 0x07,       //  Usage Page (Key codes)
  0x19, 0x00,       //  Usage Minimum (0)
  0x29, 0x65,       //  Usage Maximum (101)
  0x81, 0x00,       //  Input (Data, Array) Key array(6 bytes)

  0x09, 0x05,       //  Usage (Vendor Defined)
  0x15, 0x00,       //  Logical Minimum (0)
  0x26, 0xFF, 0x00, //  Logical Maximum (255)
  0x75, 0x08,       //  Report Size (8 bit)
  0x95, 0x02,       //  Report Count (2)
  0xB1, 0x02,       //  Feature (Data, Variable, Absolute)

  0xC0              // End Collection (Application)
};

hidValueType lookupTable[KEY_TABLE_LEN] = 
{
  {0x21, {TRUE,  0x1E}},
  {0x22, {TRUE,  0x34}},
  {0x23, {TRUE,  0x20}},
  {0x24, {TRUE,  0x21}},
  {0x25, {TRUE,  0x22}},
  {0x26, {TRUE,  0x24}},
  {0x27, {FALSE, 0x34}},
  {0x28, {TRUE,  0x26}},
  {0x29, {TRUE,  0x27}},
  {0x2A, {TRUE,  0x25}},
  {0x2B, {TRUE,  0x2E}},
  {0x2C, {FALSE, 0x36}},
  {0x2D, {FALSE, 0x2D}},
  {0x2E, {FALSE, 0x37}},
  {0x2F, {FALSE, 0x38}},
  {0x3A, {TRUE,  0x33}},
  {0x3B, {FALSE, 0x33}},
  {0x3C, {TRUE,  0x36}},
  {0x3D, {FALSE, 0x2E}},
  {0x3E, {TRUE,  0x37}},
  {0x3F, {TRUE,  0x38}},
  {0x40, {TRUE,  0x1F}},
  {0x5B, {FALSE, 0x2F}},
  {0x5C, {FALSE, 0x31}},
  {0x5D, {FALSE, 0x30}},
  {0x5E, {TRUE,  0x23}},
  {0x5F, {TRUE,  0x2D}},
  {0x60, {FALSE, 0x35}},
  {0x7B, {TRUE,  0x2F}},
  {0x7C, {TRUE,  0x31}},
  {0x7D, {TRUE,  0x30}},
  {0x7E, {TRUE,  0x35}},
  {0x7F, {FALSE, 0x4C}},
};

static uint8_t usb_hid_version[4] = {0x01, 0x01, 0x00, 0x03}; // BASE USB HID SPEC VERSION

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void HIDS_Inputrep_SendNotification(void);

/* USER CODE BEGIN PFP */
static void RxCpltCallback(uint8_t *pdata, uint16_t size, uint8_t error);
static void HIDS_APP_UpdateReport(void);
static uint8_t HIDS_APP_KeyboardMap(uint8_t charac, uint8_t *upperCase);
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void HIDS_Notification(HIDS_NotificationEvt_t *p_Notification)
{
  /* USER CODE BEGIN Service1_Notification_1 */

  /* USER CODE END Service1_Notification_1 */
  switch(p_Notification->EvtOpcode)
  {
    /* USER CODE BEGIN Service1_Notification_Service1_EvtOpcode */

    /* USER CODE END Service1_Notification_Service1_EvtOpcode */

    case HIDS_INPUTREP_READ_EVT:
      /* USER CODE BEGIN Service1Char1_READ_EVT */

      /* USER CODE END Service1Char1_READ_EVT */
      break;

    case HIDS_INPUTREP_WRITE_EVT:
      /* USER CODE BEGIN Service1Char1_WRITE_EVT */

      /* USER CODE END Service1Char1_WRITE_EVT */
      break;

    case HIDS_INPUTREP_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN Service1Char1_NOTIFY_ENABLED_EVT */
      LOG_INFO_APP("HIDS_REP_NOTIFY_ENABLED_EVT\n");
      /* USER CODE END Service1Char1_NOTIFY_ENABLED_EVT */
      break;

    case HIDS_INPUTREP_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN Service1Char1_NOTIFY_DISABLED_EVT */
      LOG_INFO_APP("HIDS_REP_NOTIFY_DISABLED_EVT\n");
      /* USER CODE END Service1Char1_NOTIFY_DISABLED_EVT */
      break;

    case HIDS_REM_READ_EVT:
      /* USER CODE BEGIN Service1Char2_READ_EVT */

      /* USER CODE END Service1Char2_READ_EVT */
      break;

    case HIDS_HII_READ_EVT:
      /* USER CODE BEGIN Service1Char3_READ_EVT */

      /* USER CODE END Service1Char3_READ_EVT */
      break;

    case HIDS_HCP_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN Service1Char4_WRITE_NO_RESP_EVT */
      if(((p_Notification->DataTransfered.p_Payload[0]) & 0x01) == 0x00)
      {
        LOG_INFO_APP("Suspend\r\n");
      }
      else
      {
        LOG_INFO_APP("Exit suspend\r\n");
      }
      /* USER CODE END Service1Char4_WRITE_NO_RESP_EVT */
      break;

    case HIDS_OUTPUTREP_READ_EVT:
      /* USER CODE BEGIN Service1Char5_READ_EVT */

      /* USER CODE END Service1Char5_READ_EVT */
      break;

    case HIDS_OUTPUTREP_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN Service1Char5_WRITE_NO_RESP_EVT */
      if(((p_Notification->DataTransfered.p_Payload[0]) & 0x01) == 0x00)
      {
        if(HIDS_APP_Context.NumLock > 0)
        {
          HIDS_APP_Context.NumLock = 0;
          LOG_INFO_APP("NUM LOCK OFF\r\n");
        }
      }
      else
      {
        if(!HIDS_APP_Context.NumLock)
        {
          HIDS_APP_Context.NumLock = 1;
          LOG_INFO_APP("NUM LOCK ON\r\n");
        }
      }
      if(((p_Notification->DataTransfered.p_Payload[0]) & 0x02) == 0x00)
      {
        if(HIDS_APP_Context.CapsLock > 0)
        {
          HIDS_APP_Context.CapsLock = 0;
          BSP_LED_Off(LED_BLUE);
          LOG_INFO_APP("CAPS LOCK OFF\r\n");
        }
      }
      else
      {
        if(!HIDS_APP_Context.CapsLock)
        {
          HIDS_APP_Context.CapsLock = 1;
          BSP_LED_On(LED_BLUE);
          LOG_INFO_APP("CAPS LOCK ON\r\n");
        }
      }
      if(((p_Notification->DataTransfered.p_Payload[0]) & 0x04) == 0x00)
      {
        if(HIDS_APP_Context.ScrollLock > 0)
        {
          HIDS_APP_Context.ScrollLock = 0;
          LOG_INFO_APP("SCROLL LOCK OFF\r\n");
        }
      }
      else
      {
        if(!HIDS_APP_Context.ScrollLock)
        {
          HIDS_APP_Context.ScrollLock = 1;
          LOG_INFO_APP("SCROLL LOCK ON\r\n");
        }
      }
      /* USER CODE END Service1Char5_WRITE_NO_RESP_EVT */
      break;

    case HIDS_OUTPUTREP_WRITE_EVT:
      /* USER CODE BEGIN Service1Char5_WRITE_EVT */

      /* USER CODE END Service1Char5_WRITE_EVT */
      break;

    case HIDS_FEATUREREP_READ_EVT:
      /* USER CODE BEGIN Service1Char6_READ_EVT */

      /* USER CODE END Service1Char6_READ_EVT */
      break;

    case HIDS_FEATUREREP_WRITE_EVT:
      /* USER CODE BEGIN Service1Char6_WRITE_EVT */

      /* USER CODE END Service1Char6_WRITE_EVT */
      break;

    default:
      /* USER CODE BEGIN Service1_Notification_default */

      /* USER CODE END Service1_Notification_default */
      break;
  }
  /* USER CODE BEGIN Service1_Notification_2 */

  /* USER CODE END Service1_Notification_2 */
  return;
}

void HIDS_APP_EvtRx(HIDS_APP_ConnHandleNotEvt_t *p_Notification)
{
  /* USER CODE BEGIN Service1_APP_EvtRx_1 */

  /* USER CODE END Service1_APP_EvtRx_1 */

  switch(p_Notification->EvtOpcode)
  {
    /* USER CODE BEGIN Service1_APP_EvtRx_Service1_EvtOpcode */

    /* USER CODE END Service1_APP_EvtRx_Service1_EvtOpcode */
    case HIDS_CONN_HANDLE_EVT :
      /* USER CODE BEGIN Service1_APP_CONN_HANDLE_EVT */

      /* USER CODE END Service1_APP_CONN_HANDLE_EVT */
      break;

    case HIDS_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN Service1_APP_DISCON_HANDLE_EVT */

      /* USER CODE END Service1_APP_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN Service1_APP_EvtRx_default */

      /* USER CODE END Service1_APP_EvtRx_default */
      break;
  }

  /* USER CODE BEGIN Service1_APP_EvtRx_2 */

  /* USER CODE END Service1_APP_EvtRx_2 */

  return;
}

void HIDS_APP_Init(void)
{
  UNUSED(HIDS_APP_Context);
  HIDS_Init();

  /* USER CODE BEGIN Service1_APP_Init */
  HIDS_Data_t msg_conf;
  
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
   
  /* Register Input Report task */
  UTIL_SEQ_RegTask( 1<< CFG_TASK_HID_UPDATE_REQ_ID, UTIL_SEQ_RFU, HIDS_APP_UpdateReport );

  /* On connection the Protocol Mode is initialised to Report Protocol Mode */
  HIDS_APP_Context.ProtocolMode = REPORT_PROTOCOL_MODE;
  
  /* Set the Keyboard Report Map */
  memset((void*)a_HIDS_UpdateCharData, 0, sizeof(a_HIDS_UpdateCharData));
  memcpy((void*)a_HIDS_UpdateCharData, (void *)&report_keyboard, sizeof(report_keyboard));
  msg_conf.p_Payload = a_HIDS_UpdateCharData;
  msg_conf.Length = sizeof(report_keyboard);
  result = HIDS_UpdateValue(HIDS_REM, &msg_conf);
  if( result != BLE_STATUS_SUCCESS )
  {
    LOG_INFO_APP("Sending of Report Map Failed error 0x%X\n", result);
  }
  
  /* Set the HID Information */
  memset((void*)a_HIDS_UpdateCharData, 0, sizeof(a_HIDS_UpdateCharData));
  memcpy((void*)a_HIDS_UpdateCharData, (void *)&usb_hid_version, sizeof(usb_hid_version));
  msg_conf.p_Payload = a_HIDS_UpdateCharData;
  msg_conf.Length = sizeof(usb_hid_version);
  result = HIDS_UpdateValue(HIDS_HII, &msg_conf);
  if( result != BLE_STATUS_SUCCESS )
  {
    LOG_INFO_APP("Sending of HID Information Failed error 0x%X\n", result);
  }
  
  /* Start UART for reception */
  UART_StartRx(RxCpltCallback);

  /* USER CODE END Service1_APP_Init */
  return;
}

/* USER CODE BEGIN FD */
static void RxCpltCallback(uint8_t *pdata, uint16_t size, uint8_t error)
{
  HIDS_APP_Context.RxChar = *pdata;
  UTIL_SEQ_SetTask( 1<<CFG_TASK_HID_UPDATE_REQ_ID, CFG_SEQ_PRIO_0);
}

static uint8_t HIDS_APP_KeyboardMap(uint8_t charac, uint8_t *upperCase)
{
  uint8_t hidValue, i;

  hidValue = 0;
  *upperCase = FALSE;

  if ((charac >= NUM_0) && (charac <= NUM_9)) 
  {
    hidValue = charac - 0x30;
    if (hidValue == 0)
      hidValue = 0x27;
    else
      hidValue += 0x1D;
  }
  
  if ((charac >= CHAR_A)  && (charac <= CHAR_Z)) 
  {
    hidValue = charac - 0x41 + 0x04;
    *upperCase = TRUE;
  }

  if ((charac >= CHAR_a)  && (charac <= CHAR_z)) 
  {
    hidValue = charac - 0x61 + 0x04;
  } 
  else 
  {
    for (i=0; i<KEY_TABLE_LEN; i++) 
    {
      if (lookupTable[i].key == charac) 
      {
        *upperCase = lookupTable[i].hid[0];
        hidValue = lookupTable[i].hid[1];
        break;
      }
    }
  }

  switch(charac) 
  {
    case RETURN:
      hidValue = 0x28;
      break;
    case BACKSPACE:
      hidValue = 0x02A;
      break;
    case SPACE:
      hidValue = 0x2C;
      break;
    case TAB:
      hidValue = 0x2B;
      break;
    case CAPS_LOCK:
      hidValue = KEY_CAPSLOCK;
      break;
  }

  return hidValue;
}

/**
 * @brief  HID Update Report 
 * @param  None
 * @retval None
 */
static void HIDS_APP_UpdateReport(void)
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  keyboard_report_t keyboard_report = {0};
  uint8_t upperCase, nmbTimes;
  HIDS_Data_t msg_conf;
  HIDS_CharOpcode_t report_type = HIDS_INPUTREP;
  
  keyboard_report.KEY1 = HIDS_APP_KeyboardMap(HIDS_APP_Context.RxChar, &upperCase);
  if(keyboard_report.KEY1 != KEY_CAPSLOCK)
  {
    HIDS_APP_Context.CapsLock = BSP_LED_GetState(LED_BLUE);
    
    if (upperCase)
    {
      if(HIDS_APP_Context.CapsLock)
      {
        keyboard_report.modifier = 0x00;
      }
      else
      {
        keyboard_report.modifier = KEY_MOD_LSHIFT;
      }
    }
    else
    {
      if(HIDS_APP_Context.CapsLock)
      {
        keyboard_report.modifier = KEY_MOD_LSHIFT;
      }
      else
      {
        keyboard_report.modifier = 0x00;
      }
    }
  }
  
  LOG_INFO_APP("keyboard_report.modifier 0x%x\n", keyboard_report.modifier);
  LOG_INFO_APP("keyboard_report.KEY1 0x%x\n", keyboard_report.KEY1);

  memset((void*)a_HIDS_UpdateCharData, 0, sizeof(a_HIDS_UpdateCharData));
  memcpy((void*)a_HIDS_UpdateCharData, (void *)&keyboard_report, sizeof(keyboard_report));
 
  msg_conf.p_Payload = a_HIDS_UpdateCharData;
  msg_conf.Length = sizeof(keyboard_report);
  result = HIDS_UpdateValue(report_type, &msg_conf);
  if(result != BLE_STATUS_SUCCESS)
  {
    LOG_INFO_APP("HIDS_UpdateValue fails\n");
  }

  keyboard_report.modifier = 0x00;
  keyboard_report.KEY1 = 0;
  memset((void*)a_HIDS_UpdateCharData, 0, sizeof(a_HIDS_UpdateCharData));
  memcpy((void*)a_HIDS_UpdateCharData, (void *)&keyboard_report, sizeof(keyboard_report));
 
  msg_conf.p_Payload = a_HIDS_UpdateCharData;
  msg_conf.Length = sizeof(keyboard_report);
  
  nmbTimes = 0;
  do 
  {
    result = HIDS_UpdateValue(report_type, &msg_conf);
    nmbTimes++;
  } while ((result != BLE_STATUS_SUCCESS) && (nmbTimes < 200));
  
  /* Once a character has been sent, put back the device in reception mode */
  UART_StartRx(RxCpltCallback);
}

static void HIDS_APP_B1Pressed(void)
{
  static uint32_t k = 0;
  tBleStatus ret;
  uint8_t keys[] = 
  {
    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
    KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
    KEY_ENTER
  };
  HIDS_CharOpcode_t report_type = HIDS_INPUTREP;
  
  keyboard_report_t keyboard_report = {0};
  HIDS_Data_t msg_conf;
  uint8_t nmbTimes;
  
  if((HIDS_APP_Context.ShiftEnable))
  {
    keyboard_report.modifier = KEY_MOD_LSHIFT;
  }

  keyboard_report.KEY1 = (uint8_t) keys[k];
  
  LOG_INFO_APP("keyboard_report.KEY1 0x%x\n", keyboard_report.KEY1);
  LOG_INFO_APP("keyboard_report.modifier 0x%x\n", keyboard_report.modifier);

  memset((void*)a_HIDS_UpdateCharData, 0, sizeof(a_HIDS_UpdateCharData));
  memcpy((void*)a_HIDS_UpdateCharData, (void *)&keyboard_report, sizeof(keyboard_report));
 
  msg_conf.p_Payload = a_HIDS_UpdateCharData;
  msg_conf.Length = sizeof(keyboard_report);
  ret = HIDS_UpdateValue(report_type, &msg_conf);
  if(ret != BLE_STATUS_SUCCESS)
  {
    LOG_INFO_APP("HIDS_UpdateValue fails\n");
  }
  else
  {
    LOG_INFO_APP("  Success: Keyboard notify\n\r");
    k = (k + 1) % (sizeof(keys) / sizeof(keys[0]));
  }

  keyboard_report.modifier = 0x00;
  keyboard_report.KEY1 = 0;
  memset((void*)a_HIDS_UpdateCharData, 0, sizeof(a_HIDS_UpdateCharData));
  memcpy((void*)a_HIDS_UpdateCharData, (void *)&keyboard_report, sizeof(keyboard_report));
 
  msg_conf.p_Payload = a_HIDS_UpdateCharData;
  msg_conf.Length = sizeof(keyboard_report);
  
  nmbTimes = 0;
  do 
  {
    ret = HIDS_UpdateValue(report_type, &msg_conf);
    nmbTimes++;
  } while ((ret != BLE_STATUS_SUCCESS) && (nmbTimes < 200));
  LOG_INFO_APP("  Success: Keyboard notify\n\r");
}

static void HIDS_APP_B2Pressed(void)
{
  uint8_t capslock = CAPS_LOCK;

  if(BSP_LED_GetState(LED_BLUE))
  {
    BSP_LED_Off(LED_BLUE);
    HIDS_APP_Context.CapsLock = 0;
  }
  else
  {
   BSP_LED_On(LED_BLUE);
   HIDS_APP_Context.CapsLock = 1;
  }  

  RxCpltCallback(&capslock, 1, 0);
}

static void HIDS_APP_B3Pressed(void)
{
  HIDS_APP_Context.ShiftEnable ^= 0x01; /* toggle shift enable variable */
  LOG_INFO_APP("Is shift key enabled == %01d\r\n", HIDS_APP_Context.ShiftEnable);
  if(HIDS_APP_Context.ShiftEnable)
  {
    BSP_LED_On(LED_RED);
  }
  else
  {
    BSP_LED_Off(LED_RED);
  } 
}

void HIDS_APP_UpdateButtonState(Button_TypeDef button, int isPressed)
{
  if(button == B1)
  {
    /* Button 1 short press action */
    HIDS_APP_B1Pressed();
  }
  else if(button == B2)
  {
    HIDS_APP_B2Pressed();
  }
  else if(button == B3)
  {
    HIDS_APP_B3Pressed();
  }
}
/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
__USED void HIDS_Inputrep_SendNotification(void) /* Property Notification */
{
  HIDS_APP_SendInformation_t notification_on_off = Inputrep_NOTIFICATION_OFF;
  HIDS_Data_t hids_notification_data;

  hids_notification_data.p_Payload = (uint8_t*)a_HIDS_UpdateCharData;
  hids_notification_data.Length = 0;

  /* USER CODE BEGIN Service1Char1_NS_1*/

  /* USER CODE END Service1Char1_NS_1*/

  if (notification_on_off != Inputrep_NOTIFICATION_OFF)
  {
    HIDS_UpdateValue(HIDS_INPUTREP, &hids_notification_data);
  }

  /* USER CODE BEGIN Service1Char1_NS_Last*/

  /* USER CODE END Service1Char1_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/
