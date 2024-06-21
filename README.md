# STM32WBA-BLE-Human-Interface-Device-Keyboard

* The STM32WBA-Human-Interface-Device-Keyboard demonstrating Bluetooth® SIG [Human Interface Device (HID) Over GATT Profile 1.0](https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=245141) example, based on STM32CubeWBA V1.3.1
 
## Hardware Needed

  * This example runs on STM32WBAxx devices.
  * Connect the Nucleo Board to your PC with a USB cable type A to mini-B to ST-LINK connector (USB_STLINK). 

## User's Guide

1) Run this Bluetooth® Low Energy HID Keyboard project where the STM32WBA will advertise

2) Use the ANDROID/IOS/PC Bluetooth® Low Energy to connect with this [Human Interface Device (HID) Over GATT Profile 1.0](https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=245141) (HID_XX where XX is the 2 last digit of the Bluetooth® Device Address)
  
   Scan on ANDROID/IOS:
   <div align="center">
       <img src=Utilities/Media/BLE_PHONE_SCAN.jpg>
   </div>

   Pairing on ANDROID/IOS:
   <div align="center">
       <img src=Utilities/Media/BLE_PHONE_CONNECTION1.jpg>
   </div>

   Connection on ANDROID/IOS:
   <div align="center">
       <img src=Utilities/Media/BLE_PHONE_CONNECTION2.jpg>
   </div>

   On STM32WB55 Nucleo Board:
   
    * each B1 button press send [HID Input Report](https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=245140) with the following key characters 0, 1, 2, ..., 9, a, b, c, ..., z, "enter", 0, 1 ...
    * each B2 button press set/reset the Caps Lock key and send [HID Input Report](https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=245140)
    * each B3 button press set/reset the Upper Case Key to select Upper key characters on B1 button press

   Received [HID Input Report](https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=245140) are displayed on the ANDROID/IOS screen: 
   <div align="center">
       <img src=Utilities/Media/BLE_HID_KEYBOARD_INPUT2.jpg>
   </div>
	
   Scan on PC:
   <div align="center">
       <img src=Utilities/Media/BLE_PC_SCAN.jpg>
   </div>
   Pairing on PC:
   <div align="center">
       <img src=Utilities/Media/BLE_PC_CONNECTION2.jpg>
       <img src=Utilities/Media/BLE_PC_CONNECTION1.jpg>
   </div>
   Connection on PC:
   <div align="center">
       <img src=Utilities/Media/BLE_PC_CONNECTION3.jpg>
       <img src=Utilities/Media/BLE_PC_CONNECTION4.jpg>
   </div>
   On STM32WB55 Nucleo Board:
   
    * each B1 button press send [HID Input Report](https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=245140) with the following key characters 0, 1, 2, ..., 9, a, b, c, ..., z, "enter", 0, 1 ...
    * each B2 button press set/reset the Caps Lock key and send [HID Input Report](https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=245140)
    * each B3 button press set/reset the Upper Case Key to select Upper key characters on B1 button press

   Received [HID Input Report](https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=245140) are displayed on the PC screen and PC keyboard LED status for Caps Lock, Scroll Lock and Num Lock:

   <div align="center">
       <img src=Utilities/Media/BLE_HID_KEYBOARD_INPUT.jpg>
   </div>
   On PC keyboard each press of Num Lock, Scroll Lock and Caps Lock keys send [HID Output Report](https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=245140) to STM32WB55 Nucleo Board BLE HID Keyboard application 
   
   with a display of the status on the Terminal:

   <div align="center">
       <img src=Utilities/Media/BLE_HID_KEYBOARD_OUTPUT.jpg>
   </div>

3) Use terminal programs like Tera Term to see the logs of each board via the onboard ST-Link. (115200/8/1/n)

   <div align="center">
       <img src=Utilities/Media/BLE_HID_KEYBOARD_LOG.jpg>
   </div>


## Troubleshooting

**Caution** : Issues and the pull-requests are **not supported** to submit problems or suggestions related to the software delivered in this repository. The STM32WBA-BLE-Human-Interface-Device-Keyboard example is being delivered as-is, and not necessarily supported by ST.

**For any other question** related to the product, the hardware performance or characteristics, the tools, the environment, you can submit it to the **ST Community** on the STM32 MCUs related [page](https://community.st.com/s/topic/0TO0X000000BSqSWAW/stm32-mcus).