#include "Adafruit_TinyUSB.h"
#include "hardware/flash.h"

uint8_t const conv_table[128][2] =  { HID_ASCII_TO_KEYCODE };

//ID HID enum
enum
{
  RID_KEYBOARD = 1,
  RID_MOUSE,
  RID_CONSUMER_CONTROL, 
};

// message ID
enum
{
  KEYBOARD_MESSAGE = 0,
  MOUSE_MESSAGE,
  CONSUMER_CONTROL_MESSAGE, // Media, volume etc ..
};

// HID report descriptor using TinyUSB's template
uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(RID_KEYBOARD) ),
  TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(RID_MOUSE) ),
  TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(RID_CONSUMER_CONTROL) )
};

Adafruit_USBD_HID usb_hid;

bool sendHIDInfo(void);

void setup(){
  pinMode(17, INPUT_PULLUP);
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("RubberDucky");
  
  usb_hid.begin();
  while( !TinyUSBDevice.mounted() ) delay(1);
  sendHIDInfo();
}

void loop()
{
 
}

bool sendHIDInfo(){
  if ( TinyUSBDevice.suspended())
  {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    TinyUSBDevice.remoteWakeup();
  }
  while(true){
    if(!digitalRead(17)){
      /*------------- Mouse -------------*/
      if ( usb_hid.ready()){
        int8_t const delta = 20;
        usb_hid.mouseMove(RID_MOUSE, delta, delta); // right + down
        delay(10);
      }
    
      /*------------- Keyboard -------------*/
      if ( usb_hid.ready() ){
        // use to send key release report
        static bool has_key = false;
    
        if (!has_key){
//          uint8_t keycode[6] = { 0 };
//          keycode[0] = HID_KEY_A;
//          usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
//          has_key = true;

          uint8_t keycode[6] = { 0 };
          uint8_t modifier   = 0;
          char chr = 'a';
          
          if ( conv_table[chr][0] ) modifier = KEYBOARD_MODIFIER_LEFTSHIFT;
          keycode[0] = conv_table[chr][1];
          usb_hid.keyboardReport(RID_KEYBOARD, modifier, keycode);
          has_key = true;
          
        }else{
          // send empty key report if previously has key pressed
          if (has_key) usb_hid.keyboardRelease(RID_KEYBOARD);
          has_key = false;
        }

        
    
        // delay a bit before attempt to send consumer report
        delay(10);
      }
    
      /*------------- Consumer Control -------------*/
      if ( usb_hid.ready() ){
        // Consumer Control is used to control Media playback, Volume, Brightness etc ...
        // Consumer report is 2-byte containing the control code of the key
        // For list of control check out https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h#L544
    
        // use to send consumer release report
        static bool has_consumer_key = false;
    
          // send volume down (0x00EA)
          usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT);
          has_consumer_key = true;
          delay(10);
        }
      }
  }
}
