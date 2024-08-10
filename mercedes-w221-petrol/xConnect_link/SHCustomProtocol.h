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
int rpmi, rpmc, rpmfine, spdi, spdc, spdf, fuellvl, fvala, fvalb, tempi, geari, turni, brakei, absi;
class SHCustomProtocol {
private:

public:
	void setup() { // Runs once

	}

	void read() { // Runs on every message sent 
		rpm = FlowSerialReadStringUntil(';');
    rpmi = rpm.toInt();
    rpmc = rpmi/250;
    rpmfine = rpmi%250;
    speed = FlowSerialReadStringUntil(';');
    spdi = speed.toInt();
    spdc = spdi/17;
    spdf = (spdi-spdc*17)*14;
    fuel = FlowSerialReadStringUntil(';');
    fuellvl = fuel.toInt();
    if(fuellvl > 40) {
      fvala = 0;
      fvalb = 210-(fuellvl-40)*3.166; 
    } else {
      fvalb = 210;
      fvala = (fuellvl-40)*-5.25;
    }
    temp = FlowSerialReadStringUntil(';');
    tempi = temp.toInt();
    if(tempi < 40) tempi = 40;
    tempi*=1.375;
    gear = FlowSerialReadStringUntil(';');
    geari = 0;
    if(gear != "N" || gear != "R") {
      geari = gear.toInt();
      geari = 48 + geari;
      if(geari > 55) {
        geari = 68;
      }
    } else if(gear == "R") {
      geari = 82;
    } else {
      geari = 0x4e;
    }
    turni = 0;
    turnL = FlowSerialReadStringUntil(';');
    if(turnL == "1") {
      turni += 100;
    }
    turnR = FlowSerialReadStringUntil(';');
    if(turnR == "1") {
      turni += 150;
    }
    ignition = FlowSerialReadStringUntil(';');
    handbrake = FlowSerialReadStringUntil(';');
    brakei = 0;
    if(handbrake == "1") {
      brakei = 4;
    }
    showLights = FlowSerialReadStringUntil(';');
    shiftLight = FlowSerialReadStringUntil(';');
    highBeams = FlowSerialReadStringUntil(';');
    cruiseControl = FlowSerialReadStringUntil(';');
    String concate = "";
    int p = 0;
    absi = 0;
    while(p < showLights.length()+1) { // This is a parser for the ShowLights raw data from simhub.
      if(showLights[p] == ',' || showLights[p] == ';') {
        p+=2;
        if(concate == "DL_SHIFT") {

        } else if(concate == "DL_TC") {
          absi += 1;
        } else if(concate == "DL_OILWARN") {

        } else if(concate == "DL_BATTERY") {

        } else if(concate == "DL_ABS") {
          absi += 8;
        } else if(concate == "DL_HANDBRAKE") {
          brakei = 4;
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
    opSend(1, 0xCC, 0x80, 0xCF, 0xAD, 0xAA, 0x07, 0x10, 0x55);

    // Gears  0x50 P, 0x52 R, 49-55 D1-D7, 68 D, 70 F, 78 N
    opSend(757, geari, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); 

    // Turnsignals
    opSend(41, turni, 200, 0, 0, 0, 0, 0, 0); // 150 - RIGHT turn | 100 - LEFT turn | 250 - HAZARD (1st byte)

    // SPEEDTRONIC / DISTRONIK / CRUISE CONTROL / PARK ASSIST / PRE-SAFE? (limits max spd)
    opSend(888, 0, 0, 0, 0, 0, 0, 0, 0); // 8th byte = Park Assist/Radar off 5th byte = CC speed set

    //High beam
    opSend(313, 0, 0, 0, 0, 0, 130, 0, 0);

    // Language
    opSend(861, 10, 10, 10, 10, 10, 10, 10, 10);

    // Speedo
    opSend(515, spdc, spdf, spdc, spdf, 0, 0, 0, 0); // H L H L (High adjustment / Low adjustment)

    // RPM
    opSend(261, rpmc, rpmfine, 0, 0, 0, 0, 0, 0); // 1st byte = RPM

    // Fuel
    opSend(248, fvala, fvalb, 0, 0, 0, 0, 0, 0);

    // ABS + HOLD
    opSend(583, 0, 0, absi, 0, 0, 120, 0, 0); // 5th byte = hold (0x2F)
    
    // Coolant temp + oil temp
    opSend(781, tempi, 0, 150, 0, 0, 0, 0, 0); // 1st byte = coolant temp 3rd byte = oil temp

    // Air bag
    opSend(885, 0, 0, 0, 0, 0, 0, 0, 0); // 3rd byte = seatbelt

    // Airtronic
    opSend(889, 0, 0, 0, 0, 0, 0, 0, 0);

    // Power steering
    opSend(832, 0, 0, 0, 0, 0, 0, 0, 0);

    // Park Brake
    opSend(378, 0, brakei, 0, 0, 0, 0, 0, 0);

    // CEL
    opSend(829, 0, 0, 0, 0, 0, 0, 0, 0);

    // Brake fluid lvl
    opSend(762, 0, 0, 0, 0, 0, 0, 0, 0);

    // Button events
    if (digitalRead(OK_BTN_PIN) == LOW) {
      opSend(0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00);
    } else if (digitalRead(UP_BTN_PIN) == LOW) {
      opSend(0x45, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00);
    } else if (digitalRead(DOWN_BTN_PIN) == LOW) {
      opSend(0x45, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00);
    } else if (digitalRead(LEFT_BTN_PIN) == LOW) {
      opSend(0x45, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00);
    } else if (digitalRead(RIGHT_BTN_PIN) == LOW) {
      opSend(0x45, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00);
    } else {
      opSend(0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    }

	}

	// Called once between each byte read on arduino, do not add any code here, unless you know what you are doing.
	void idle() {}
};

#endif
