// Arduino pins
// Assignment of these pins (except I2C's SCL/SDA) can be re-configured here.
//
#if defined(__AVR_ATmega328P__)
//                           0;  //        PD0 (Used by serial port)
//                           1;  //        PD1 (Used by serial port)
const int SHUTTER_PIN      = 2;  //        PD2 interrupt pin w/o software debounce
//                           3;  //        PD3 (Not in use)
//                           4;  //        PD4 (Not in use)
//                           5;  //        PD5 (Not in use)
//                           6;  //        PD6 (Not in use)
//                           7;  //        PD7 (Not in use)
//                           8;  //        PB0 (Not in use)
//                           9;  //        PB1 (Not in use)
//                           10; // (SS)   PB2 (Not in use)
//                           11; // (MOSI) PB3 (Not in use)
//                           12; // (MISO) PB4 (Not in use)
const int BUILTINLED       = 13; // (SCK)  PB5 built-in LED
//                               //        PB6 XTAL1
//                               //        PB7 XTAL2
const int HBUSRDY          = A0; // (14)   PC0
const int PWRBTN           = A1; // (15)   PC1
const int I2CINT           = A2; // (16)   PC2 used by pin change int
const int BPRDY            = A3; // (17)   PC3
//                           A4; // (18)   PC4 (Used by I2C SDA)
//                           A5; // (19)   PC5 (Used by I2C SCL) 
//                           A6; // (20)       Analog only
//                           A7; // (21)       Analog only
#else
#error CPU not supported
#endif

const int GET_CAMERA_INFO           = ('c' << 8) + 'v';
const int GET_CAMERA_WIFI_STATUS    = ('w' << 8) + 'i';
const int GET_CAMERA_SETTING        = ('t' << 8) + 'd';
const int SET_CAMERA_SETTING        = ('T' << 8) + 'D';
const int SET_CAMERA_POWER_STATE    = ('P' << 8) + 'W';
const int SET_CAMERA_VIDEO_OUTPUT   = ('V' << 8) + 'O';
const int SET_CAMERA_SLAVE_SETTINGS = ('X' << 8) + 'S';
const int SET_CAMERA_FAULT          = ('F' << 8) + 'N';
const int SET_CAMERA_3D_SYNCHRONIZE = ('S' << 8) + 'Y';
const int SET_CAMERA_SHUTTER_ACTION = ('S' << 8) + 'H';
const int SET_CAMERA_USBMODE        = ('U' << 8) + 'M';
const int SET_BACPAC_SHUTTER_ACTION = ('S' << 8) + 'H';

// commands relating to TD SET_CAMERA_SETTING
// buffer to store current camera settings
const int TD_BUFFER_SIZE                    = 0x29;

//
// td[] meanings and associated bacpac command
const short int SET_BACPAC_DATE_TIME              = ('T' << 8) + 'M';
const int TD_DATE_TIME_year                 = 0x03; // year (0-99)    
const int TD_DATE_TIME_month                = 0x04; // month (1-12)
const int TD_DATE_TIME_day                  = 0x05; // day (1-31)
const int TD_DATE_TIME_hour                 = 0x06; // hour (0-23)
const int TD_DATE_TIME_minute               = 0x07; // minute (0-59)
const int TD_DATE_TIME_second               = 0x08; // second (0-59)
const short int SET_BACPAC_MODE                   = ('C' << 8) + 'M';
const int TD_MODE                           = 0x09;
const short int SET_BACPAC_PHOTO_RESOLUTION       = ('P' << 8) + 'R';
const int TD_PHOTO_RESOLUTION               = 0x0a;
const short int SET_BACPAC_VIDEORESOLUTION        = ('V' << 8) + 'R';
const int TD_VIDEORESOLUTION                = 0x0b; // (Defunct; always 0xff)
const short int SET_BACPAC_VIDEORESOLUTION_VV     = ('V' << 8) + 'V';
const int TD_VIDEORESOLUTION_VV             = 0x0c;
const short int SET_BACPAC_FRAMES_PER_SEC         = ('F' << 8) + 'S';
const int TD_FRAMES_PER_SEC                 = 0x0d;
const short int SET_BACPAC_FOV                    = ('F' << 8) + 'V';
const int TD_FOV                            = 0x0e;
const short int SET_BACPAC_EXPOSURE               = ('E' << 8) + 'X';
const int TD_EXPOSURE                       = 0x0f;
const short int SET_BACPAC_PHOTO_XSEC             = ('T' << 8) + 'I';
const int TD_PHOTO_XSEC                     = 0x10;
const short int SET_BACPAC_TIME_LAPSE             = ('T' << 8) + 'S';
const int TD_TIME_LAPSE                     = 0x11; // (Defunct; always 0)
const short int SET_BACPAC_BEEP_SOUND             = ('B' << 8) + 'S';
const int TD_BEEP_SOUND                     = 0x12;
const short int SET_BACPAC_NTSC_PAL               = ('V' << 8) + 'M';
const int TD_NTSC_PAL                       = 0x13;
const short int SET_BACPAC_ONSCREEN_DISPLAY       = ('D' << 8) + 'S';
const int TD_ONSCREEN_DISPLAY               = 0x14;
const short int SET_BACPAC_LEDBLINK               = ('L' << 8) + 'B';
const int TD_LEDBLINK                       = 0x15;
const short int SET_BACPAC_PHOTO_INVIDEO          = ('P' << 8) + 'N';
const int TD_PHOTO_INVIDEO                  = 0x16;
const short int SET_BACPAC_LOOPING_MODE           = ('L' << 8) + 'O';
const int TD_LOOPING_MODE                   = 0x17;
const short int SET_BACPAC_CONTINUOUS_SHOT        = ('C' << 8) + 'S';
const int TD_CONTINUOUS_SHOT                = 0x18;
const short int SET_BACPAC_BURST_RATE             = ('B' << 8) + 'U';
const int TD_BURST_RATE                     = 0x19;
const short int SET_BACPAC_PROTUNE_MODE           = ('P' << 8) + 'T';
const int TD_PROTUNE_MODE                   = 0x1a;
const short int SET_BACPAC_AUTO_POWER_OFF         = ('A' << 8) + 'O';
const int TD_AUTO_POWER_OFF                 = 0x1b;
const short int SET_BACPAC_WHITE_BALANCE          = ('W' << 8) + 'B';
const int TD_WHITE_BALANCE                  = 0x1c;
// 0x1d (reserved)
// 0x1e (reserved)
// 0x1f (reserved)
// 0x20 (reserved)
// 0x21 (reserved)
// 0x22 (reserved)
const short int SET_BACPAC_FLIP_MIRROR            = ('U' << 8) + 'P';
const int TD_FLIP_MIRROR                    = 0x23;
const short int SET_BACPAC_DEFAULT_MODE           = ('D' << 8) + 'M';
const int TD_DEFAULT_MODE                   = 0x24;
const short int SET_BACPAC_PROTUNE_COLOR          = ('C' << 8) + 'O';
const int TD_PROTUNE_COLOR                  = 0x25;
const short int SET_BACPAC_PROTUNE_GAIN           = ('G' << 8) + 'A';
const int TD_PROTUNE_GAIN                   = 0x26;
const short int SET_BACPAC_PROTUNE_SHARPNESS      = ('S' << 8) + 'P';
const int TD_PROTUNE_SHARPNESS              = 0x27;
const short int SET_BACPAC_PROTUNE_EXPOSURE_VALUE = ('E' << 8) + 'V';
const int TD_PROTUNE_EXPOSURE_VALUE         = 0x28;

#define MODE_VIDEO 0x00
#define MODE_PHOTO 0x01
#define MODE_BURST 0x02
#define MODE_TIMELAPSE 0x03
#define MODE_DUAL 0x08

const short int tdtable[] PROGMEM = {
  SET_BACPAC_MODE, // 0x09
  SET_BACPAC_PHOTO_RESOLUTION, // 0x0a
  SET_BACPAC_VIDEORESOLUTION, // 0x0b
  SET_BACPAC_VIDEORESOLUTION_VV, // 0x0c
  SET_BACPAC_FRAMES_PER_SEC, // 0x0d
  SET_BACPAC_FOV, // 0x0e
  SET_BACPAC_EXPOSURE, // 0x0f
  SET_BACPAC_PHOTO_XSEC, // 0x10
  SET_BACPAC_TIME_LAPSE, // 0x11
  SET_BACPAC_BEEP_SOUND, // 0x12
  SET_BACPAC_NTSC_PAL, // 0x13
  SET_BACPAC_ONSCREEN_DISPLAY, // 0x14
  SET_BACPAC_LEDBLINK, // 0x15
  SET_BACPAC_PHOTO_INVIDEO, // 0x16
  SET_BACPAC_LOOPING_MODE, // 0x17
  SET_BACPAC_CONTINUOUS_SHOT, // 0x18
  SET_BACPAC_BURST_RATE, // 0x19
  SET_BACPAC_PROTUNE_MODE, // 0x1a
  SET_BACPAC_AUTO_POWER_OFF, // 0x1b
  SET_BACPAC_WHITE_BALANCE, // 0x1c
  -1, // 0x1d (reserved)
  -1, // 0x1e (reserved)
  -1, // 0x1f (reserved)
  -1, // 0x20 (reserved)
  -1, // 0x21 (reserved)
  -1, // 0x22 (reserved)
  SET_BACPAC_FLIP_MIRROR, // 0x23
  SET_BACPAC_DEFAULT_MODE, // 0x24
  SET_BACPAC_PROTUNE_COLOR, // 0x25
  SET_BACPAC_PROTUNE_GAIN, // 0x26
  SET_BACPAC_PROTUNE_SHARPNESS, // 0x27
  SET_BACPAC_PROTUNE_EXPOSURE_VALUE, // 0x28
};

