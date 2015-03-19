#MewPro Genlock Dongle

A hardware emulator of GoPro Hero 3+ Black: the dongle communicates with GoPro Dual Hero System using I2C and with MewPro using UART, and extracts/transmits genlock signals. 

Resources:
* Introduction to MewPro: [http://mewpro.cc/?p=226]
* Schematic Drawing: [http://mewpro.cc/?p=204]
* List of GoPro Serial Commands: [http://mewpro.cc/2014/10/14/list-of-i%C2%B2c-commands/]
* Herobus Pinout of GoPro Hero 3+ Black: [http://mewpro.cc/?p=207]

------

###Compile
MewPro Genlock Dongle uses Arduino Pro Mini 328 3.3V 8MHz. The source code is checked to compile/work with Arduino IDE 1.5.8+

------

###Serial Line Commands
To communicate with MewPros, Genlock Dongle uses the serial line commands of MewPro. All the commands are listed at https://gist.github.com/orangkucing/45dd2046b871828bf592#file-gopro-i2ccommands-md . You can simply type a command string to the serial console followed by a return; for example,

+ `PW0` : shutdown Dual Hero (and other MewPros/GoPros)
+ `SH1` : shutter (camera mode) or start record (video mode)
+ `SH0` : stop record (video mode)

Almost all listed commands that have a label named SET_BACPAC_xxx are usable. Moreover several special commands are implemented in Genlock Dongle:

+ `@` : power on

------

###Sensors
The code here includes an example to support sensors:

+ e_Shutter.ino : Using external shutters such as CANON Timer Remote Controller TC-80N3.
