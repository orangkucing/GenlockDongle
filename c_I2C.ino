#define I2C_NOSTOP false
#define I2C_STOP true

#define WIRE Wire

byte buf[MEWPRO_BUFFER_LENGTH], recv[MEWPRO_BUFFER_LENGTH];
int bufp = 1;
volatile boolean recvq = false;

// I2C slave addresses
const int I2CEEPROM = 0x50;
const int SMARTY = 0x60;

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
      char tmp[4];
      sprintf(tmp, " %02x", p[i]);
      Serial.print(tmp);
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
  int buflen = buf[0] & 0x7f;
  
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
  pinMode(I2CINT, INPUT_PULLUP);
  attachPinChangeInterrupt(I2CINT, readHandler, FALLING); 
  recvq = false;
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

