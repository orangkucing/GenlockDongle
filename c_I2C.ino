#define I2C_NOSTOP false
#define I2C_STOP true

#define WIRE Wire

byte buf[MEWPRO_BUFFER_LENGTH], recv[MEWPRO_BUFFER_LENGTH];
int bufp = 1;
int recvb = 0, recve = 0;
volatile boolean recvq;
#define RECV(a) (recv[(recvb + (a)) % MEWPRO_BUFFER_LENGTH])

// I2C slave addresses
const int I2CEEPROM = 0x50;
const int SMARTY = 0x60;

byte td[TD_BUFFER_SIZE];
const byte td_init[TD_BUFFER_SIZE] PROGMEM = {
  // default mode below will be overwritten so don't worry about the detailed settings here
  // NOTE: TD_MODE must be equal to TD_DEFAULT_MODE at camera boot
#define MODE_DEFAULT MODE_VIDEO
  0x28, 'T', 'D', 0x10, 0x02, 0x06, 0x17, 0x0a,
  0x27, MODE_DEFAULT, 0x07, 0xff, 0x05, 0x04, 0x00, 0x00, 
  0x02, 0x00, 0x02, 0x00, 0x01, 0x02, 0x00, 0x00,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, MODE_DEFAULT, 0x00, 0x00, 0x00,
  0x0a,
};

void readHandler()
{
  recvq = true;
}

// print out debug information to Arduino serial console
void __debug(const __FlashStringHelper *p)
{
  if (debug) {
    Serial.println(p);
  }
}

void _printInput(int b)
{
  if (debug) {
    int buflen = recv[b] & 0x7f;
    Serial.print('>');
    for (int i = 0; i <= buflen; i++) {
      if (i == 1 && isprint(recv[(b + 1) % MEWPRO_BUFFER_LENGTH]) || i == 2 && recv[(b + 1) % MEWPRO_BUFFER_LENGTH] != 0 && isprint(recv[(b + 2) % MEWPRO_BUFFER_LENGTH])) {
        if (i == 1) {
          Serial.print(' ');
        }
        Serial.print((char) recv[(b + i) % MEWPRO_BUFFER_LENGTH]);
      } else {
        Serial.print(" ");
        printHex(recv[(b + i) % MEWPRO_BUFFER_LENGTH], false);
      }
    }
    Serial.println("");
  }
}

void SendBufToBacpac() {
  // some command need to be executed in Dongle side before sending it to Bacpac
  int buflen = buf[0] & 0x7f;
  int command = (buf[1] << 8) + buf[2];
  switch (command) {
  case SET_BACPAC_POWER_DOWN:
    if (buf[3] == 0) {
      heartBeatIsOn = false;
    }
    break;
  case SET_CAMERA_USBMODE:
    if (heartBeatIsOn) { // send to slaves
      Serial.print("UM");
      printHex(buf[3], true);
      Serial.println("");
    }
    return; // not send to Bacpac as "UM" is not a SET_BACPAC_* command
  case SET_CAMERA_SETTING:
    if (heartBeatIsOn) { // send to slaves
      char tmp[3];
      memcpy(td, buf, TD_BUFFER_SIZE);
      Serial.print("TD");
      for (int i = 3; i < TD_BUFFER_SIZE; i++) {
        printHex(buf[i], true);
      }
      Serial.println("");
    }
    break; // Bacpac understands "TD"
  default:
    // other commands are listed in tdtable[]
    for (int offset = 0x09; offset < TD_BUFFER_SIZE; offset++) {
      if (pgm_read_word(tdtable + offset - 0x09) == command) {
        td[offset] = buf[3];
        break;
      }
    }
    break;
  }
  dontSendPW = false;
  if (debug) {
    int buflen = buf[0] & 0x7f;
    Serial.print('<');
    for (int i = 0; i <= buflen; i++) {
      if (i == 1 && isprint(buf[1]) || i == 2 && buf[1] != 0 && isprint(buf[2])) {
        if (i == 1) {
          Serial.print(' ');
        }
        Serial.print((char) buf[i]);
      } else {
        Serial.print(" ");
        printHex(buf[i], false);
      }
    }
    Serial.println("");
  }
  WIRE.beginTransmission(SMARTY);
  WIRE.write(buf, buflen + 1);
  WIRE.endTransmission(I2C_STOP);
}

void resetI2C()
{
  WIRE.begin();
  digitalWrite(I2CINT, LOW);
  pinMode(I2CINT, OUTPUT);
  recvb = 0; recve = 0;
  readEEPROM();
  queueIn("vs");
  delay(1000);
  attachPinChangeInterrupt(I2CINT, readHandler, FALLING); 
  pinMode(I2CINT, INPUT_PULLUP);
}

// SET_CAMERA_3D_SYNCHRONIZE START_RECORD
void startRecording()
{
  queueIn("SH1");
}

// SET_CAMERA_3D_SYNCHRONIZE STOP_RECORD
void stopRecording()
{
  queueIn("SH0");
}

