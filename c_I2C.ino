#define I2C_NOSTOP false
#define I2C_STOP true

#define WIRE Wire

byte buf[MEWPRO_BUFFER_LENGTH], recv[MEWPRO_BUFFER_LENGTH];
int bufp = 1;
volatile boolean recvq = false;

// I2C slave addresses
const int I2CEEPROM = 0x50;
const int SMARTY = 0x60;

byte td[TD_BUFFER_SIZE] = {
  // default mode below will be overwritten so don't worry about the detailed settings here
  0x28, 'T', 'D', 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x04, 0x05, 0xff, 0x0a, 0x07, 0x00, 0x00, 
  0x02, 0x00, 0x02, 0x00, 0x01, 0x02, 0x00, 0x00,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
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

void __printBuf(byte *p)
{
  int buflen = p[0] & 0x7f;

  for (int i = 0; i <= buflen; i++) {
    if (i == 1 && isprint(p[1]) || i == 2 && p[1] != 0 && isprint(p[2])) {
      if (i == 1) {
        Serial.print(' ');
      }
      Serial.print((char) p[i]);
    } else {
      Serial.print(" ");
      printHex(p[i], false);
    }
  }
  Serial.println("");
}

void _printInput()
{
  if (debug) {
    Serial.print('>');
    __printBuf(recv);
  }
}

void SendBufToBacpac() {
  // some command need to be executed in Dongle side before sending it to Bacpac
  int buflen = buf[0] & 0x7f;
  int command = (buf[1] << 8) + buf[2];
  switch (command) {
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
    Serial.print('<');
    __printBuf(buf);
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
  recvq = false;
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

