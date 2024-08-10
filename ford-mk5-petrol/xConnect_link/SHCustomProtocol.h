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
int tempi, rpmgate, finetune, finetuner, distance, speedi, blinkerR, blinkerL;
class SHCustomProtocol {
private:

public:
	void setup() { // Runs once

	}

	void read() { // Runs on every message sent 
		rpm = FlowSerialReadStringUntil(';');
    int rpmi = rpm.toInt();
    rpmgate = 96 + (int)(rpmi / 500); 
    finetune = (int)(rpmi%500)/2;
    if(rpmgate > 115) {
      rpmgate = 115;
    }
    speed = FlowSerialReadStringUntil(';');
    speedi = speed.toInt();
    int ogSpeed = speedi;
    speedi  = ( speedi / 2.66 ); // for kmph
    finetuner = ((speedi * 3) - ogSpeed)*10; // Revisit in future due to inconsistency
    finetuner = finetuner - finetuner%100; 
    fuel = FlowSerialReadStringUntil(';');
    temp = FlowSerialReadStringUntil(';');
    tempi = temp.toInt();
    tempi+=60;
    gear = FlowSerialReadStringUntil(';');
    turnL = FlowSerialReadStringUntil(';');
    if(turnL == "1") {
      blinkerL = B01000000;
    } else {
      blinkerL = 0x00;
    }
    turnR = FlowSerialReadStringUntil(';');
    if(turnR == "1") {
      blinkerR = B00001000;
    } else {
      blinkerR = 0x00;
    }
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
     //Ignition
    opSend(0x3b3, 0x45, 0x00, 0x00, 0x8E, 0x00, 0x00, 0x13, 0x00);

    //Airbag
    opSend(0x04C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //Parking Brake und Lampen
    opSend(0x3c3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    // Doors, Turnsignals etc.
    opSend(0x3B2, 0x00, 0x00, 0x00, 0x00, blinkerR, 0x00, blinkerL, 0x00);

    // ABS
    opSend(0x416, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //Navigation Compass
    opSend(0x466, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //Coolant
    opSend(0x156, tempi, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00);

    //Power Steering
    opSend(0x877, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x00, 0x00);

    //Charging System?
    opSend(0x42c, 0x00, 0x00, 0xA6, 0x00, 0x00, 0x00, 0x00, 0x00); // First byte is tempomat light (0x90)
    
    //Dieselmotor
    opSend(0x421, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); 

    //Parking Brake
    opSend(0x213, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //Tire Pressure
    opSend(0x3b4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //AWD
    opSend(0x261, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //Key Status
    opSend(0x38D, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    
    opSend(0x204, 0x00, 0x00, 0x00, rpmgate, finetune, finetune, 0x00, 0x00);

    // Speedometer INOP
    opSend(0x202, 0x40, 0x00, 0x00, 0x00, distance, distance, speedi, finetuner);
    distance++;
    if(distance > 200) { // Make sure the distance doesnt get out of range
      distance = 0;
    }

    // Gears
    // if(gear.equalsIgnoreCase("P")) {
    //   opSend(0x171, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    // } else if(gear.equalsIgnoreCase("R")) {
    //   opSend(0x171, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24);
    // } else if(gear.equalsIgnoreCase("N")) {
    //   opSend(0x171, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46);    
    // } else if(gear.equalsIgnoreCase("D")) {
    //   opSend(0x171, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64); 
    // } else if(gear.equalsIgnoreCase("S")) {
    //   opSend(0x171, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96);  
    // }
    // Button Events
    if(digitalRead(OK_BTN_PIN) == LOW) {
      opSend(0x881, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
      while(digitalRead(OK_BTN_PIN) == LOW) {}
      opSend(0x881, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    }
	}

	// Called once between each byte read on arduino, do not add any code here, unless you know what you are doing.
	void idle() {}
};

#endif
