/*
                           ______                            __ 
                     _  __/ ____/___  ____  ____  ___  _____/ /_
                    | |/_/ /   / __ \/ __ \/ __ \/ _ \/ ___/ __/
                   _>  </ /___/ /_/ / / / / / / /  __/ /__/ /_  
                  /_/|_|\____/\____/_/ /_/_/ /_/\___/\___/\__/  
                              SimHub Adaptation                        
                          Code modified by: InfoX1337            
    xConnect SimHub Adaptation by InfoX1337 is licensed under GNU GPL 3.0 
    Copyright (C) 2024 InfoX1337
    Contact me at: hi@infox.dev or support@xconnect.cc

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
int rpmI, speedI, rpmGate, fineTuning, turnSig, highBeamsI, tcs, absLight, handbrakeI, battWarn, typeSel;
class SHCustomProtocol {
private:

public:
	void setup() { // Runs once

	}

	void read() { // Runs on every message sent 
		rpm = FlowSerialReadStringUntil(';');
    rpmI = rpm.toInt();
    if(rpmI > 6500) {
      rpmI = 6500;
    }
    rpmGate = round(rpmI/250);
    fineTuning = rpmI%250;
    speed = FlowSerialReadStringUntil(';');
    speedI = speed.toInt()/2.67151;
    fuel = FlowSerialReadStringUntil(';');
    temp = FlowSerialReadStringUntil(';');
    gear = FlowSerialReadStringUntil(';');
    turnSig = 0x00;
    turnL = FlowSerialReadStringUntil(';');
    if(turnL == "1") {
      turnSig += B00001000;
    }
    turnR = FlowSerialReadStringUntil(';');
    if(turnR == "1") {
      turnSig += B00010000;
    }
    ignition = FlowSerialReadStringUntil(';');
    handbrake = FlowSerialReadStringUntil(';');
    handbrakeI = 0x00;
    if(handbrake == "1") {
      handbrakeI = B1110111;
    }
    showLights = FlowSerialReadStringUntil(';');
    shiftLight = FlowSerialReadStringUntil(';');
    highBeams = FlowSerialReadStringUntil(';');
    highBeamsI = 0x00;
    if(highBeams == "1") {
      highBeamsI = 100;
    }
    cruiseControl = FlowSerialReadStringUntil(';');
    String concate = "";
    tcs = 0x00;
    battWarn = 100;
    absLight = 124;
    typeSel = 0;
    int p = 0;
    while(p < showLights.length()) { // This is a parser for the ShowLights raw data from simhub.
      if(showLights[p] == ',') {
        p+=2;
        if(concate == "DL_SHIFT") {

        } else if(concate == "DL_TC") {
          tcs = 100;
        } else if(concate == "DL_OILWARN") {

        } else if(concate == "DL_BATTERY") {
          battWarn = 0;
        } else if(concate == "DL_ABS") {
          absLight = 100;
        } else if(concate == "DL_HANDBRAKE") {
          handbrakeI = B1110111;
        } else if(concate == "DL_FULLBEAM") {
          highBeamsI = 100;
        } else if(concate == "XC_SPORT") {
          typeSel = 124;
        }
        concate="";
        continue;
      }
      concate=concate+showLights[p];
      p++;
    }
	}

	void loop() { // Called once per arduino loop
    //Ignition
    opSend(184, 0, B00010000, 0, 0, 0, 0, 0, B00111111);
    // Last byte is door status

    // RPM
    opSendH(256, 0, 0, 0, 0, rpmGate, fineTuning, 0, 0); 
    
    // Speedometer
    opSendH(341, 0, 0, 0, 0, 0, speedI, 0, 231); 

    // Turn signals
    opSend(224, turnSig, 0, 0, 0, 0, 0, 0, 0); 

    // Battery light
    opSend(16, 0, battWarn, 0, 0, 0, 0, 0, 0);

    // High beams and others
    opSend(1176, 0, 0, 0, 0, highBeamsI, 0, 0, 0);

    // ABS and handbrake
    opSendH(592, 0, handbrakeI, 0, 0, absLight, 0, 0, 0); 

    // HDC, TSC OFF, TSC
    opSendH(801, 0, 0, 0, 0, 0, tcs, 0, 0);

    // TERRAIN MODE SELECTION (red cluster when in sport)
    opSendH(847, typeSel, typeSel, typeSel, typeSel, typeSel, typeSel, typeSel, typeSel); 

    // Cruise control and engine heat
    opSendH(914, 0, 0, 0, 0, 0, 0, 0, 0); 
    
    // Seatbelt and air bag light
    opSendH(922, 0, 0, 0, 0, 0, 0, B10110000, 0); 
    
    // Yellow brake light fix
    opSendH(1290, 50, 50, 50, 50, 50, 50, 50, 50); 
	}

	// Called once between each byte read on arduino, do not add any code here, unless you know what you are doing.
	void idle() {}
};

#endif
