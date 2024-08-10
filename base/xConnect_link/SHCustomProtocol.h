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

// NCalc Formula: (paste into simhub "Custom Protocol")
/*
format(round([Rpms],0),'0') + ';' +
isnull(round([SpeedKmh],0),'0') + ';' +
isnull(round([FuelPercent],0),'0') + ';' +
isnull(round([WaterTemperature], 0),'0') + ';' +
isnull([Gear],'0') + ';' +
isnull([TurnIndicatorLeft],'0') + ';' +
isnull([TurnIndicatorRight],'0') + ';' +
isnull([EngineIgnitionOn],'0') + ';' +
isnull([Handbrake],'0') + ';' +
isnull([GameRawData.ShowLights],'0') + ';' +
if([CarSettings_RPMShiftLight1] > 0, 1, 0) + ';' +
isnull([DataCorePlugin.GameRawData.Lights.HighBeams],'0') + ';' +
isnull([DataCorePlugin.GameRawData.Drivetrain.CruiseControl],'0') + ';' 
*/


#ifndef __SHCUSTOMPROTOCOL_H__
#define __SHCUSTOMPROTOCOL_H__

#include <Arduino.h>
String rpm, speed, fuel, temp, gear, turnL, turnR, ignition, handbrake, showLights, shiftLight, highBeams, cruiseControl;

class SHCustomProtocol {
private:

public:
	void setup() { // Runs once

	}

	void read() { // Runs on every message sent 
		rpm = FlowSerialReadStringUntil(';');
    speed = FlowSerialReadStringUntil(';');
    fuel = FlowSerialReadStringUntil(';');
    temp = FlowSerialReadStringUntil(';');
    gear = FlowSerialReadStringUntil(';');
    turnL = FlowSerialReadStringUntil(';');
    turnR = FlowSerialReadStringUntil(';');
    ignition = FlowSerialReadStringUntil(';');
    handbrake = FlowSerialReadStringUntil(';');
    showLights = FlowSerialReadStringUntil(';');
    shiftLight = FlowSerialReadStringUntil(';');
    highBeams = FlowSerialReadStringUntil(';');
    cruiseControl = FlowSerialReadStringUntil(';');
    String concate = "";
    int p = 0;
    while(p < showLights.length()) { // This is a parser for the ShowLights raw data from simhub.
      if(showLights[p] == ',') {
        p+=2;
        if(concate == "DL_SHIFT") {

        } else if(concate == "DL_TC") {

        } else if(concate == "DL_OILWARN") {

        } else if(concate == "DL_BATTERY") {

        } else if(concate == "DL_ABS") {

        }
        concate="";
        continue;
      }
      concate=concate+showLights[p];
      p++;
    }
	}

	void loop() { // Called once per arduino loop
    opSend(0x40, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00, 0x69); // This is how you send canbus data, the first number is the IDENTIFIER, and the rest are DATA BYTES.
	}

	// Called once between each byte read on arduino, do not add any code here, unless you know what you are doing.
	void idle() {}
};

#endif
