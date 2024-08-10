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
String fuel, temp, gear, turnL, turnR, ignition, handbrake, showLights, shiftLight, highBeams, cruiseControl;
int rpm, rpmGate, fineTuning, speed, spdGran, spdTune, parkBrake, turn, beams, ignit;
class SHCustomProtocol {
private:

public:
	void setup() { // Runs once
    fineTuning = 20;
	}

	void read() { // Runs on every message sent 
		rpm = FlowSerialReadStringUntil(';').toInt();
    if(rpm > 8000) {
      rpm = 8000;
    }

    rpmGate = round(rpm/770);
    fineTuning = (rpm - rpmGate*750)*0.31875;
    if(rpm == 0) {
      fineTuning = 20;
    }
    if(fineTuning > 255) {
      fineTuning = 255;
    }
    speed = FlowSerialReadStringUntil(';').toInt();
    if(speed > 320) {
      speed = 320;
    }
    spdGran = speed/4.2;
    spdTune = (speed - spdGran*4.2)*50;
    if(spdTune > 255) {
      spdTune = 255;
    }
    fuel = FlowSerialReadStringUntil(';');
    temp = FlowSerialReadStringUntil(';');
    gear = FlowSerialReadStringUntil(';');
    turnL = FlowSerialReadStringUntil(';');
    turn = 0;
    if(turnL == "1") {
      turn += 100;
    } 
    turnR = FlowSerialReadStringUntil(';');
    if(turnR == "1") {
      turn += 155;
    }
    ignition = FlowSerialReadStringUntil(';');
    ignit = 0x13;
    handbrake = FlowSerialReadStringUntil(';');
    if(handbrake == "1") {
      parkBrake = 120;
    } else {
      parkBrake = 0;
    }
    showLights = FlowSerialReadStringUntil(';');
    shiftLight = FlowSerialReadStringUntil(';');
    highBeams = FlowSerialReadStringUntil(';');
    if(highBeams == "1" ) {
      beams = 120;
    } else {
      beams = 0;
    }
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
    // Ignition
    opSend(0x3C0, 0, 0, ignit, 0,0,0,0,0);

    // RPM 4th byte FINE TUNE 5th byte SECTOR
    opSend(263, 0, 0, 0, fineTuning, rpmGate ,0 ,0 ,0);

    // Speedometer 1st byte: ABS BRAKE TCS 2nd byte TCS OFF / LAUNCH CONTROL 3rd byte little speed control / granular 4th byte: speedo big control 6th byte=SAFE DO NOT TOUCH 7th some error
    opSend(0x308, 0, 0, spdTune ,spdGran, 0, 0, 0, 0);

    // EPC light
    opSend(2309, 0, 0, 0, 0,0 ,0 ,0 ,0);


    //Parking Brake
    opSend(0x30D, parkBrake, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    //Byte 1: 4=Parking Brake
    //Byte 2: 1=Autohold (Green PARK)

    //TPMS
    opSend(0x64A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //Suspension
    opSend(0x396, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //Airbag and Seatbelt
    opSend(0x040, 0x00, 0x00, 0x00, 0x00, B00000000, 0x00, 0x00, 0x00);

    //Dimming
    opSend(0x5F0, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD);

    // Lights  
    opSend(0x662, 0x00, 0x00, beams, 0x00, 0, 0x00, 0x00, 0x00);
    //Byte 3: 4=High Beam
    //Byte 6: 16=Auto High Beam

    //Turn Signals 3rd byte Blinkers 120 R 135 L 255 BOTH
    opSend(0x363, 0x00, 0x00, turn, 0x00, 0x00, 0x00, 0x00, 0x00); 

    //Lane Assist
    opSend(0x397, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //Rear Seatbelts
    opSend(0x661, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    // Brake error 
    opSend(0x101, 0, 0, 0, 0, 0, 0, 0, 0);
	}

	// Called once between each byte read on arduino, do not add any code here, unless you know what you are doing.
	void idle() {}
};

#endif
