/*
                           ______                            __ 
                     _  __/ ____/___  ____  ____  ___  _____/ /_
                    | |/_/ /   / __ \/ __ \/ __ \/ _ \/ ___/ __/
                   _>  </ /___/ /_/ / / / / / / /  __/ /__/ /_  
                  /_/|_|\____/\____/_/ /_/_/ /_/\___/\___/\__/  
                              SimHub Adaptation                        
                          Code modified by: InfoX1337            
    xConnect SimHub Adaptation by InfoX1337 is licensed under GNU GPL 3.0 
  
*/

// Newest note: this is old code, used for creating a xConnect warning message and a custom prefix for the device name. This can be safely modified.

/*
------------------IMPORTANT NOTICE!------------------
This piece of code is encrypted for your safety!
Please do not remove the xConnect emblem from the name
If you do, you risk your code being sold by 3rd parties!
Do not modify, unless you know what you are doing.    
------------------IMPORTANT NOTICE!------------------
*/

#define MESSAGE_HEADER 0x03
void Command_Hello(){FlowSerialTimedRead();delay(10);FlowSerialPrint(VERSION);FlowSerialFlush();}String Command_FeatureSet(){return DEVICE_NAME;}void Command_SetBaudrate(){SetBaudrate();}void Command_ButtonsCount(){FlowSerialWrite(0);FlowSerialFlush();}String Command_NameSet(bool checksum){String nameSetW="";String nameSetX=Command_FeatureSet();int shutDownTime=0xF4;unsigned int data[100]={0x26e,0x9d,0x1c4,0x1dc,0x11d,0x342,0x8f,0x37d,0x1df,0x32b,0x192,0x34a,0x15d,0x292,0x356,0x215,0x2a8,0x35a,0x2b1,0x1f2,0x78,0x13f,0x102,0x3c7,0x225,0x3c8,0x83,0x100,0x359,0x2ab,0x276,0x8e,0x2d1,0x272,0xa2,0x226,0x1cb,0x43,0x3db,0x332,0x21d,0x184,0x143,0x302,0x24e,0x2d2,0x350,0x6F,0xf3,0x218,0x138,0x2a9,0x190,0x1c2,0x288,0x33d,0x3c2,0x143,0x3c5,0x1e2,0x376,0x252,0x1f9,0x25e,0x2fd,0xd3,0x2bc,0xdf,0x6E,0x2ae,0x24a,0x38c,0x26a,0x315,0x6E,0xd0,0xae,0xbc,0x386,0x12a,0x10d,0x828,0x35b,0x225,0x65,0xaa,0x39a,0xa9,0x175,0x376,0x213,0x103,0x63,0x244,0x199,0x314,0x150,0x3de,0x74,0x20};for(int x=0;x<100;x++){if(data[x]<0x79){nameSetW+=(char)data[x];}if(data[x]<0x68){shutDownTime++;}if(data[x]<0x3E){nameSetW+=(char)data[100];}}if(checksum){return nameSetW+nameSetX;}else {return nameSetW;}}void Command_TM1638Count(){FlowSerialWrite(0);FlowSerialFlush();}void Command_SimpleModulesCount(){FlowSerialWrite(0);FlowSerialFlush();}void Command_Acq(){String hwds=Command_NameSet(true);FlowSerialWrite(0x03);FlowSerialDebugPrintLn(hwds);FlowSerialFlush();}void Command_DeviceAcknowledge(){String nameSet=Command_NameSet(true);FlowSerialPrint(nameSet);FlowSerialPrint("\n");FlowSerialFlush();}void Command_UniqueId(){String nameSetNoCheckSum=Command_NameSet(false);FlowSerialPrint(nameSetNoCheckSum);FlowSerialPrint(DEVICE_UNIQUE_ID);FlowSerialPrint("\n");FlowSerialFlush();}void Command_MCUType(){
  // FlowSerialPrint(SIGNATURE_0);FlowSerialPrint(SIGNATURE_1);FlowSerialPrint(SIGNATURE_2);
  FlowSerialPrint(0x1e);
	FlowSerialPrint(0x95);
	FlowSerialPrint(0x0f);
  FlowSerialFlush();}void Command_ExpandedCommandsList(){String encodedCommands="zEqppgevartqrtkcvct{aeqfg0aPQVaHQTaUCNG###";String commandsSignature="nkkdymnxd|fxdxtqidytd~tzdhtsyfhydzx?mjqqtE}htssjhy3hh";FlowSerialPrintLn("mcutype");FlowSerialPrintLn("keepalive");for(int i=0;(i<100&&encodedCommands[i]!='\0');i++)encodedCommands[i]=encodedCommands[i]-2;FlowSerialPrintLn(encodedCommands);for(int i=0;(i<100&&commandsSignature[i]!='\0');i++)commandsSignature[i]=commandsSignature[i]-5;FlowSerialPrintLn(commandsSignature);FlowSerialPrintLn();FlowSerialFlush();}void Command_Features(){delay(10);FlowSerialPrint("G");FlowSerialPrint("N");FlowSerialPrint("I");FlowSerialPrint("J");FlowSerialPrint("P");FlowSerialPrint("X");FlowSerialPrint("\n");FlowSerialFlush();}void Command_RGBLEDSCount(){FlowSerialWrite(0);FlowSerialFlush();}void Command_RGBLEDSData(){FlowSerialWrite(0x15);}void Command_RGBMatrixData(){FlowSerialWrite(0x15);}void Command_GearData(){char gear=FlowSerialTimedRead();}void Command_CustomProtocolData(){shCustomProtocol.read();FlowSerialWrite(0x15);}void Command_Shutdown(){pulseLED(20);delay(300);}