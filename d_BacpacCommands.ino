#define MEWPRO_VERSION_STRING "2015062601"

byte td[TD_BUFFER_SIZE] = {
  // default mode below will be overwritten so don't worry about the detailed settings here
  0x28, 'T', 'D', 0x0f, 0x01, 0x12, 0x03, 0x21,
  0x32, MODE_VIDEO, 0x05, 0xff, 0x03, 0x07, 0x00, 0x00, 
  0x02, 0x00, 0x02, 0x00, 0x01, 0x02, 0x00, 0x00,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
  0x0a,
};

void cameraCommand()
{
  switch ((recv[1] << 8) + recv[2]) {
  case GET_CAMERA_INFO:
    memcpy((char *)buf, "\x25\x00\x01\x04\x0cHD3.11.03.00\x14HERO3+ Black Edition", 0x26); // the dongle will act as a HERO3+ Black
    // memcpy((char *)buf, "\x20\x00\x01\x04\x0fHD4.01.02.00.00\x0cHERO4 Silver", 0x21); // not supported yet
    // memcpy((char *)buf, "\x1f\x00\x01\x04\x0fHD4.02.02.00.00\x0bHERO4 Black", 0x20); // not supported yet
    SendBufToBacpac();
    break;
  case GET_CAMERA_WIFI_STATUS:
    if (isMaster) {
      buf[0] = 2; buf[1] = 0; buf[2] = 0; // wifi off
    } else {
      buf[0] = 1; buf[1] = 1; // NAK
    }
    SendBufToBacpac();
    break;
/*********************************************************************
 data structure for {GET|SET}_CAMERA_SETTING is the same as follows: */
    //
    // buf[] recv[] meaning and/or relating bacpac command
    // -----+-----+-----------------------------------------------------
    // 0x00  0x00   packet length (0x27 or 0x28)
    // 0x01         always 0
    //       0x01   always 'T'
    //       0x02   always 'D'
    //              TM SET_BACPAC_DATE_TIME if [3:8] == 0 then no need to set time
    // 0x02  0x03     year (0-99)    
    // 0x03  0x04     month (1-12)
    // 0x04  0x05     day (1-31)
    // 0x05  0x06     hour (0-23)
    // 0x06  0x07     minute (0-59)
    // 0x07  0x08     second (0-59)
    // 0x08  0x09   CM SET_BACPAC_MODE
    // 0x09  0x0a   PR SET_BACPAC_PHOTO_RESOLUTION
    // 0x0a  0x0b   VR SET_BACPAC_VIDEORESOLUTION (Defunct; always 0xff)
    // 0x0b  0x0c   VV SET_BACPAC_VIDEORESOLUTION_VV
    // 0x0c  0x0d   FS SET_BACPAC_FRAMES_PER_SEC
    // 0x0d  0x0e   FV SET_BACPAC_FOV
    // 0x0e  0x0f   EX SET_BACPAC_EXPOSURE
    // 0x0f  0x10   TI SET_BACPAC_PHOTO_XSEC
    // 0x10  0x11   TS SET_BACPAC_TIME_LAPSE (Defunct; always 0xff)
    // 0x11  0x12   BS SET_BACPAC_BEEP_SOUND
    // 0x12  0x13   VM SET_BACPAC_NTSC_PAL
    // 0x13  0x14   DS SET_BACPAC_ONSCREEN_DISPLAY
    // 0x14  0x15   LB SET_BACPAC_LEDBLINK
    // 0x15  0x16   PN SET_BACPAC_PHOTO_INVIDEO
    // 0x16  0x17   LO SET_BACPAC_LOOPING_MODE
    // 0x17  0x18   CS SET_BACPAC_CONTINUOUS_SHOT
    // 0x18  0x19   BU SET_BACPAC_BURST_RATE
    // 0x19  0x1a   PT SET_BACPAC_PROTUNE_MODE
    // 0x1a  0x1b   AO SET_BACPAC_AUTO_POWER_OFF
    // 0x1b  0x1c   WB SET_BACPAC_WHITE_BALANCE
    // 0x1c  0x1d   (reserved)
    // 0x1d  0x1e   (reserved)
    // 0x1e  0x1f   (reserved)
    // 0x1f  0x20   (reserved)
    // 0x20  0x21   (reserved)
    // 0x21  0x22   (reserved)
    // 0x22  0x23   UP SET_BACPAC_FLIP_MIRROR
    // 0x23  0x24   DM SET_BACPAC_DEFAULT_MODE
    // 0x24  0x25   CO SET_BACPAC_PROTUNE_COLOR
    // 0x25  0x26   GA SET_BACPAC_PROTUNE_GAIN
    // 0x26  0x27   SP SET_BACPAC_PROTUNE_SHARPNESS
    // 0x27  0x28   EV SET_BACPAC_PROTUNE_EXPOSURE_VALUE
    // -----+-----+-----------------------------------------------------
  case GET_CAMERA_SETTING:
    memcpy(buf + 2, td + 3, TD_BUFFER_SIZE - 3);
    buf[0] = 0x27; buf[1] = 0;
    SendBufToBacpac();
    heartBeatIsOn = true;
    break;
  case SET_CAMERA_SETTING:
    __debug(F("camera settings received"));
    memcpy(td, recv, TD_BUFFER_SIZE);
    // Upside is always up
    td[TD_FLIP_MIRROR] = 1;
    //
    if (heartBeatIsOn) { // send to slaves
      char tmp[3];
      Serial.print("TD");
      for (int i = 3; i < TD_BUFFER_SIZE; i++) {
        sprintf(tmp, "%02X", td[i]);
        Serial.print(tmp);
      }
      Serial.println("");
    }
    buf[0] = 1; buf[1] = 0; // ok
    SendBufToBacpac();
    if (!heartBeatIsOn && !isMaster) {
      // Apply received TD
      buf[0] = 3; buf[1] = 'F'; buf[2] = 'N'; buf[3] = 0x0C;
      // finished!
      SendBufToBacpac(); // report to Primary Camera
      heartBeatIsOn = true;
    }
    break;
 /*********************************************************************/
  case SET_CAMERA_VIDEO_OUTPUT:
    buf[0] = 2; buf[1] = 0; buf[2] = recv[3];
    SendBufToBacpac();
    break;
  case SET_CAMERA_SLAVE_SETTINGS:
    if (isMaster) {
      // received slave camera configurations
    } else {
      switch (recv[3]) {
      case 0:
        heartBeatIsOn = false;
        break;
      case 1:
        heartBeatIsOn = true;
        break;
      default:
        break;
      }
    }
    buf[0] = 1; buf[1] = 0; // ok
    SendBufToBacpac();
    break;
  case SET_CAMERA_POWER_STATE:
    if (recv[3] == 0) {
      if (!dontSendPW) { 
        if (heartBeatIsOn) { // send to slaves
          Serial.println(F("PW00"));
        }
        digitalWrite(BUILTINLED, HIGH);
        digitalWrite(HBUSRDY, LOW);
      }
      dontSendPW = false;
      heartBeatIsOn = false;
    }
    break;
  case SET_CAMERA_FAULT:
    switch (recv[3]) {
    case 0x01:
      __debug(F("low battery"));
      buf[0] = 1; buf[1] = 0;
      SendBufToBacpac();
      return;
    case 0x03:
      __debug(F("No SD"));
      break;
    case 0x05:
      __debug(F("cameras incompatible"));
      break;
    case 0x06:
      __debug(F("3D Sync Lost"));
      buf[0] = 2; buf[1] = 0; buf[2] = 1;
      SendBufToBacpac();
      // panic!!
      digitalWrite(BUILTINLED, HIGH);
      digitalWrite(HBUSRDY, LOW); // show bacpac as if the camera has rebooted
      delay(200);
      digitalWrite(HBUSRDY, HIGH);
      return;
    case 0x0b:
      __debug(F("capture end received"));
      break;
    case 0x0c:
      __debug(F("SC_APPLIED_TD received"));
      break;
    default:
      __debug(F("Unknown notification received"));
      break;
    }
    buf[0] = 2; buf[1] = 0; buf[2] = recv[3]; // "ok"
    SendBufToBacpac();
    break;
  case SET_CAMERA_3D_SYNCHRONIZE:
    // Master shutter button depressed
    {
      char tmp[5];
      if (heartBeatIsOn) { // send to slaves
        sprintf(tmp, "SY%02X", recv[3]);
        digitalWrite(BUILTINLED, HIGH);
        Serial.println(tmp);
        digitalWrite(BUILTINLED, LOW);
      }
      // in order to sync the first frame, a short delay seems to be necessary
      delay(140); // 140ms
      buf[0] = 0x83; buf[1] = 'S'; buf[2] = 'R';
      if (!isMaster && recv[3] == 0 && td[TD_MODE] != MODE_TIMELAPSE) {
        // video capture end
        buf[3] = 3; // notify video saved
      } else {
        buf[3] = recv[3];
      }
      SendBufToBacpac();
      if (td[TD_MODE] == MODE_TIMELAPSE) {
        switch (recv[3]) {
        case 1:
        case 2:
          timelapse = 1900;
          break;
        default:
          timelapse = 0;
        }
        previous_shutter = millis();
      }
    }
    break;
  default:
    break;
  }
}

void readEEPROM()
{
  char c, tmp[4];
  if (debug) {
    Serial.print(F("eeprom:"));
  }
  for (int i = 0; i < 16; i++) {
    WIRE.beginTransmission(I2CEEPROM);
    WIRE.write((byte)i);
    WIRE.endTransmission(I2C_NOSTOP);
    WIRE.requestFrom(I2CEEPROM, 1, I2C_STOP);
    if (WIRE.available()) {
      c = WIRE.read();
      if (i == 0)
        isMaster = (c == 4);
      sprintf(tmp, " %02x", c);
      if (debug) {
        Serial.print(tmp);
      }
    }
  }
  if (debug) {
    Serial.println("");
  }
  // notify SMARTY that ROM has read successfully
  digitalWrite(PWRBTN, LOW); pinMode(PWRBTN, OUTPUT);
  delay(200);
  pinMode(PWRBTN, INPUT_PULLUP);
}

void _I2CmasterRead()
{
  int datalen;
  // since data length is variable and not yet known, read one byte first.
  WIRE.requestFrom(SMARTY, 1, I2C_NOSTOP);
  datalen = WIRE.read() & 0x7f;
  // request again
  WIRE.requestFrom(SMARTY, datalen + 1, I2C_STOP);
  for (int i = 0; i <= datalen; i++) {
    recv[i] = WIRE.read();
  }
  recvq = false;
  _printInput();
}

void checkCommands()
{
  if (recvq) {
    _I2CmasterRead();
    cameraCommand();
  }
  while (inputAvailable())  {
    static boolean shiftable;
    byte c = myRead();
    switch (c) {
      case ' ':
        continue;
      case '\r':
      case '\n':
        if (bufp != 1) {
          buf[0] = bufp - 1 | 0x80;
          bufp = 1;
          SendBufToBacpac();
        }
        return;
      case '&':
        bufp = 1;
        debug = !debug;
        __debug(F("debug messages on"));
        while (inputAvailable()) {
          if (myRead() == '\n') {
            return;
          }
        }
        return;  
      case '@':
        bufp = 1;
        __debug(F("camera power on"));
        poweron = true;
        if (1) {
          Serial.println("");
          Serial.println("@"); // power on slaves
        }
        while (inputAvailable()) {
          if (myRead() == '\n') {
            return;
          }
        }
        return; 
      case '/':
        Serial.println(MEWPRO_VERSION_STRING);
        return; 
      default:
        if (bufp >= 3 && isxdigit(c)) {
          c -= '0';
          if (c >= 10) {
            c = (c & 0x0f) + 9;
          }    
        }
        if (bufp < 4) {
          shiftable = true;
          buf[bufp++] = c;
        } else {
          if (shiftable) { // TM requires six args; "TM0e080a0b2d03" sets time to 2014 Aug 10 11:45:03
            buf[bufp-1] = (buf[bufp-1] << 4) + c;
          } else {
            buf[bufp++] = c;
          }
          shiftable = !shiftable;      
        }
        break;
    }
  }
}

void SendHeartBeat()
{
  buf[0] = 0x83; buf[1] = 'H'; buf[2] = 'B'; buf[3] = 0xff;
  SendBufToBacpac();
  if (!isMaster) {
//    queueIn("XS0202011D000001E3000404FFFF00"); if wifi on
  }
}
