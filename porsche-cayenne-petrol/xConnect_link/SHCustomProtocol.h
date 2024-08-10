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
String fuel, gearS, turnL, turnR, ignition, handbrake, showLights, shiftLight, highBeams, cruiseControl;
int rpm, speed, temp, calc, calca, calcb, spdTr, turnsignals, gear, count, eabs, emerbrake;
class SHCustomProtocol {
private:

public:
	void setup() { // Runs once
    
	}

	void read() { // Runs on every message sent 
    rpm = FlowSerialReadStringUntil(';').toInt();
    if(rpm > 8000) rpm = 8000;
    calc = rpm/2;
    calca = calc/400;
    calcb = (calc/1.568627451)-calca*255;
    if(calcb < 14) calcb=14;
    speed = FlowSerialReadStringUntil(';').toInt( );
    spdTr = speed*0.611371;
    
    fuel = FlowSerialReadStringUntil(';');
    temp = FlowSerialReadStringUntil(';').toInt();
    gearS = FlowSerialReadStringUntil(';');
    if(gearS == "N") {
      gearS = "0";
    } else if(gearS == "R") {
      gearS = "-1";
    }
    gear = gearS.toInt();
    if(gear <=0) {
      if(gear == 0) {
        gear = 36;
      } else {
        gear = 35;
      }
    } else {
      gear = 22+gear*16;
    }
    turnsignals = 0;
    turnL = FlowSerialReadStringUntil(';');
    if(turnL == "1") {
      turnsignals += 150;
    }
    turnR = FlowSerialReadStringUntil(';');
    if(turnR == "1") {
      turnsignals += 105;
    }
    ignition = FlowSerialReadStringUntil(';');
    handbrake = FlowSerialReadStringUntil(';');
    bool emer = false;
    if(handbrake == "1") {
      if(speed > 5) {
        emer = true;
      }
    }
    showLights = FlowSerialReadStringUntil(';');
    shiftLight = FlowSerialReadStringUntil(';');
    highBeams = FlowSerialReadStringUntil(';');
    cruiseControl = FlowSerialReadStringUntil(';');
    String concate = "";
    int p = 0;
    eabs = 0;
    while(p < showLights.length()) { // This is a parser for the ShowLights raw data from simhub.
      if(showLights[p] == ',') {
        p+=2;
        if(concate == "DL_SHIFT") {

        } else if(concate == "DL_TC") {
          eabs += 22;
        } else if(concate == "DL_OILWARN") {

        } else if(concate == "DL_BATTERY") {

        } else if(concate == "DL_ABS") {
          eabs += 32;
        } else if(concate == "DL_HANDBRAKE") {
          if(speed > 5) {
            emer = true;
          }
        }
        concate="";
        continue;
      }
      concate=concate+showLights[p];
      p++;
    }
    emerbrake = 0;
    if(emer) {
      emerbrake = 150;
    }
	}

	void loop() { // Called once per arduino loop
    opSend(0x3C0, 0, 0, 3, 0, 0, 0, 0, 0); // Ignition
    opSend(0x30d, emerbrake, 0, 0, 0, 0, 0, 0, 0); // Emergency braking on.
    opSend(900, 0, 0 , 0 , 0 , 0 , 0 , 0 , 0); // Brake pads need change
    opSend(776, eabs, 0, 0, 0, 0, 0, 0, 0); // 22 - TCS ~ 32 - ABS ~ 54 (22+32) ABS + TCS
    // opSend(734, random(0,255), random(0,255), random(0,255), random(0,255), random(0,255), random(0,255), random(0,255), random(0,255)); // ESC and ABS
    opSend(263, 0, 0, 0, calcb, calca, 0, 0, 0); // RPM Byte 4 must always be above 14, byte 5 works as gate byte. 1k rpm = 75a, 1b 2krpm = 155a, 2b 5krpm = 120a, 6b
    opSend(259, 0, 0, 0, 0, spdTr, spdTr, 0, spdTr); // Speedo
    opSend(915, 0, 0, 0, gear, 0, 0, 0, 0); // Gear Selector and gear ind 4th byte 34 - Park 35 - Reverse 36 - Neutral D1- 38 D2-54 D3- 70 D4-86 D5-102 D6-118 D7-134 D8-150 
    opSend(933, 0, 0, 0, 0, 0, 0, 0, 0); // Gear Up Indicator (wrong bytes)
    opSend(1634, 0, 0, B00000001, 0, 0, 0, 0, 0); // Lights and backlight
    opSend(1600, 200, 0, 170, 200, 0, 0, 0, 0); // Coolant and oil  Byte 1&4: Coolant
    opSend(867, 0, 0, turnsignals, 0, 0, 0, 0, 0); // Turn signals  3rd bit 150-Left 105 - Right Both: 255
    opSend(0x677, 0, 0x20, 78, 0, 0, 0, 80, 0); // Sport PLUS + temp
    if(digitalRead(OK_BTN_PIN) == LOW) {
      opSend(0x5bf, 0x00, 0x13, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00);
    } else if(digitalRead(BACK_BTN_PIN) == LOW) {
      opSend(0x5bf, 0x03, 0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00);
    } else if(digitalRead(UP_BTN_PIN) == LOW) {
      if(count == 0) {
        opSend(0x5bf, 0x12, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00);
      }
      count++;
    } else if(digitalRead(DWN_BTN_PIN) == LOW) {
      if(count == 0) {
        opSend(0x5bf, 0x12, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00);
      }
      count++;
    } else {
      opSend(0x5bf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    }
    if(count >= 10) {
      count = 0;
    }
	}
  
	// Called once between each byte read on arduino, do not add any code here, unless you know what you are doing.
	void idle() {}
};

#endif
