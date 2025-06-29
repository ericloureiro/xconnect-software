#include "binary.h"
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
// JS Formula: (paste into simhub "Custom Protocol")
/*
// Run once
let response = '';
let rpm = 0;
const clusterMaxRpm = 8000;
const indicatorDelay = 400;

let isToggleMenuOn = false;
let buttonPress = 0;

let lowBeam = 0;
let highBeam = 0;

const CAR_OFF = 1;
const IGNITION_ON = 2;
const ENGINE_ON = 3;
let ignition = CAR_OFF;

let ignitionEventCount = 0;
let engineEventCount = 0;

// Run on loop
ignitionEventCount++;
engineEventCount++;

response = ''; 
response += Math.round($prop('Rpms') / $prop('MaxRpm') * clusterMaxRpm) + ';';
response += Math.round($prop('SpeedKmh')) + ';';
response += Math.round($prop('FuelPercent')) + ';';
response += (readtextfile('c:\\repos\\open-weather\\output.txt') ?? 0) + ';';
response += Math.round(($prop('UDPConnectorDataPlugin.WaterTemperature') ?? $prop('WaterTemperature')) + 60) + ';';
response += $prop('Gear') + ';';
response += ((blink('left', indicatorDelay, $prop('UDPConnectorDataPlugin.TurningLeftLights') ?? false) || $prop('TurnIndicatorLeft')) ? 1 : 0) + ';';
response += ((blink('right', indicatorDelay, $prop('UDPConnectorDataPlugin.TurningRightLights') ?? false) || $prop('TurnIndicatorRight')) ? 1 : 0) + ';';

const ignitionEvent = $prop('InputStatus.JoystickPlugin.PXN-CB1________B11');
if (ignitionEvent && ignitionEventCount > 100) {
	ignitionEventCount = 0;
	
	if (ignition != CAR_OFF) {
		ignition = CAR_OFF;
	} else {
		ignition = IGNITION_ON;
	}
}

const engineEvent = $prop('InputStatus.JoystickPlugin.PXN-CB1________B10');
if (engineEvent && engineEventCount > 100) {
	engineEventCount = 0;
	
	if (ignition == IGNITION_ON) {
		ignition = ENGINE_ON;
	} 
}

response += (ignition != CAR_OFF && $prop('EngineIgnitionOn') ? ignition : CAR_OFF) + ';';
response += (($prop('UDPConnectorDataPlugin.Handbrake') || $prop('GameRawData.TruckValues.CurrentValues.MotorValues.BrakeValues.ParkingBrake')) ? 1 : 0) + ';';
response += $prop('ABSActive') + ';';
response += $prop('TCActive') + ';';

response += ($prop('GameRawData.Extensions.TCPresetCount') == 0 || ($prop('GameRawData.Extensions.TCPresetCount') > 0 && $prop('TCLevel') > 0) ? 1 : 0) + ';'; 

lowBeam = $prop('UDPConnectorDataPlugin.HeadlightsActive') || $prop('GameRawData.TruckValues.CurrentValues.LightsValues.BeamLow');
response += (lowBeam ? 1 : 0) + ';';

highBeam = $prop('UDPConnectorDataPlugin.HighBeam') || $prop('GameRawData.TruckValues.CurrentValues.LightsValues.BeamHigh');
response += (lowBeam && highBeam ? 1 : 0) + ';';

response += ($prop('CarSettings_RPMShiftLight1') > 0 ? 1 : 0) + ';';
response += ($prop('GameRawData.TruckValues.CurrentValues.DashboardValues.CruiseControlSpeed.Kph') ?? 0) + ';';

isToggleMenuOn = $prop('InputStatus.JoystickPlugin.PXN-CB1________B14');

buttonPress = 0;
if (!isToggleMenuOn) {
	buttonPress += 0;
} else if ($prop('InputStatus.JoystickPlugin.PXN-CB1________B09')) {
	buttonPress += 1;
} else if ($prop('InputStatus.JoystickPlugin.PXN-CB1________HATUp')) {
  	buttonPress += 2;
} else if ($prop('InputStatus.JoystickPlugin.PXN-CB1________HATDown')) {
  	buttonPress += 3;
} else if ($prop('InputStatus.JoystickPlugin.PXN-CB1________HATRight')) {
  	buttonPress += 4;
} else if ($prop('InputStatus.JoystickPlugin.PXN-CB1________HATLeft')) {
	buttonPress += 5;
}

response += buttonPress + ';';
response += ($prop('TCLevel') == 0 && ($prop('GameRawData.Physics.WheelSlip03') > 1 || $prop('GameRawData.Physics.WheelSlip04') > 1) ? 1 : 0) + ';';

return response;
*/

#ifndef __SHCUSTOMPROTOCOL_H__
#define __SHCUSTOMPROTOCOL_H__

#define OK_BTN 1
#define UP_BTN 2
#define DOWN_BTN 3
#define MENU_BTN 4
#define BACK_BTN 5

#define CAR_OFF 1
#define IGNITION_ON 2
#define ENGINE_ON 3

#include <Arduino.h>
String rpmS, speedS, fuel, outsideTempS, tempS, gearS, turnL, turnR, ignitionS, handbrakeS, lowBeamS, shiftLight, highBeamS, cruiseControlSpeedS, ABSS, tcActiveS, tcEnabledS, buttonPressedS, tyreSlipS;
int scaledRpm, rpm, newSpeed, speed, outsideTemp, temp, blinkerL, blinkerR, ignition, engineWarn, tcACtive, tcEnabled, tpms, battWarn, highBeam, targetCruiseControl, newCruiseControlSpeed, cruiseControlSpeed, cruiseControl, rpmgate, finetunerRpm, distance, finetunerSpeed, distanceTravelled, gearInd, absLight, arrow, handbrake, gear;

// Debug tools
uint32_t messageTimer;
int begin = 0;
int counter = 0;
int end = 255;
int messageThrottle = 500;


class SHCustomProtocol {
private:
  bool shouldSendMessage () {
    return millis() - messageTimer > messageThrottle;
  }

  void bruteForce() {
    if (counter > end) {
      counter = begin;
    }
    
    if (shouldSendMessage()) {
      messageTimer = millis();

      Serial.println(counter);

      opSend(1130, 0, 0, 97, 0, 0, 0, 0, 0);
      counter++;
    }
  }

  void setCruiseControl() {
    newCruiseControlSpeed = cruiseControlSpeedS.toInt();
    
    if (newCruiseControlSpeed == cruiseControlSpeed) {
      targetCruiseControl = 0x00;
     
      return;
    }

    targetCruiseControl = B00010000;
    cruiseControlSpeed = newCruiseControlSpeed;

    if (cruiseControlSpeed > 0) {
      cruiseControl = B10010000;
      
      return;
    }
    
    cruiseControl = 0x00;
  }

  void setRpm() {
    rpm = rpmS.toInt();
    // Custom code for Ford MK8
    if (rpm < 0) {
      rpm = 0;
    }

    scaledRpm = (int)(rpm); // Scale input to display correct RPM on cluster
    rpmgate = 96 + (int)(scaledRpm / 500);  // Calculate the gate and cast to int

    if (rpmgate > 115) {
      rpmgate = 115;
    }

    finetunerRpm = (int)(scaledRpm % 500) / 2;
  }

  void setSpeed() {
    speed = speedS.toInt();

    newSpeed = speed;
    speed = ( speed / 2.66 ); // for kmph

    finetunerSpeed = ((speed * 3) - newSpeed) * 10; // Revisit in future due to inconsistency
    finetunerSpeed = finetunerSpeed - finetunerSpeed % 200;
  }

  void sendOdometer() {
    distance++;

    if (distance > 200) { // Make sure the distance doesnt get out of range
      distance = 0;
    }

    // Gears, show upshift and downshift symbol
    if (arrow == 100) {
      gearInd = (gear+1)*10;
      if(gear >= 7) { // Disable the gear selector number when gear is 6 or more, since the cluster only supports 6 gears
        gearInd = 0;
      } else if(gear >= 5) {
        gearInd += 10;
      }
    } else if(rpm > 6500 && gear > 1) {
      gearInd = (gear+1)*10;
      if(gear >= 7) { // Disable the gear selector number when gear is 6 or more, since the cluster only supports 6 gears
        gearInd = 0;
      } else if(gear >= 5) {
        gearInd += 10;
      }
      arrow=100;
    } else if(rpm < 700 && battWarn == 0x00) {
      gearInd = (gear)*10;
      if(gear >= 8) { // Disable the gear selector number when gear is 6 or more, since the cluster only supports 6 gears
        gearInd = 0;
      } else if(gear >= 6) {
        gearInd += 20;
      } else if(gear >= 5) {
        gearInd += 10;
      }
      arrow = 150;
    } else {
      arrow = 0;
    }

    if (distance % 10 == 0) {
      distanceTravelled += (int)(speed/5);
    }

    if ((int)(distanceTravelled / 500) > 100) {
      distanceTravelled = (int)(distanceTravelled / 500);
    }

    // Odometer
    opSend(0xF979, arrow, 0x00, 0x00, 0x00, 200, distanceTravelled, gearInd, 0x00);
  }

  void sendButtonMenu() {
    // Button events
    switch(buttonPressedS.toInt()) {
      case OK_BTN:
        opSend(0x082, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
        return;
      case UP_BTN:
        opSend(0x082, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
        return;
      case DOWN_BTN:
        opSend(0x082, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
        return;
      case MENU_BTN:
        opSend(0x082, 0x00, 255, 255, 0x00, 0x00, 0x00, 0x00, 0x00);
        return;
      case BACK_BTN:
        opSend(0x082, 0x00, 76, 0, 0x00, 0x00, 0x00, 0x00, 0x00);
        return;
      default:
        opSend(0x082, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
        return;
    }
  }

public:
  // Runs once
	void setup() {
    messageTimer = millis();
  }

  // Runs on every message sent
	void read() {
		rpmS = FlowSerialReadStringUntil(';');
    speedS = FlowSerialReadStringUntil(';');
    // TODO: not being used
    fuel = FlowSerialReadStringUntil(';');
    outsideTempS = FlowSerialReadStringUntil(';');
    outsideTemp = outsideTempS.toInt();
    tempS = FlowSerialReadStringUntil(';');
    temp = tempS.toInt();
    gearS = FlowSerialReadStringUntil(';');
    turnL = FlowSerialReadStringUntil(';');
    turnR = FlowSerialReadStringUntil(';');
    ignitionS = FlowSerialReadStringUntil(';');
    handbrakeS = FlowSerialReadStringUntil(';');
    ABSS = FlowSerialReadStringUntil(';');
    tcActiveS = FlowSerialReadStringUntil(';');
    tcEnabledS = FlowSerialReadStringUntil(';');
    lowBeamS = FlowSerialReadStringUntil(';');
    highBeamS = FlowSerialReadStringUntil(';');
    shiftLight = FlowSerialReadStringUntil(';');
    cruiseControlSpeedS = FlowSerialReadStringUntil(';');
    buttonPressedS = FlowSerialReadStringUntil(';');
    tyreSlipS = FlowSerialReadStringUntil(';');

    switch(ignitionS.toInt()) {
      case IGNITION_ON:
        battWarn = B00010001;
        engineWarn = B00000100;
        ignition = B01000000;
        handbrake = B11111111;
        highBeam =  B00010000;
        cruiseControlSpeed = 0;
        cruiseControl = 0x00;
        rpm = 0x00;
        speed = 0x00;
        break;
      case ENGINE_ON:
        battWarn = 0x00;
        engineWarn = 0x00;

        if (lowBeamS == "1") {
          ignition = B01000100;
        } else {
          ignition = B01000000;
        }

        if (highBeamS == "1") {
          highBeam = B00110000;
        } else {
          highBeam = B00010000;
        }

        if (handbrakeS == "1") {
          handbrake = B00010000;
        } else {
          handbrake = 0x00;
        }

        if (gearS != "N" || gearS != "R") {
          gear = gearS.toInt();
          gear++;
        } else {
          gear = 0x00;
        }

        if (turnL == "1") {
          blinkerL = B11111100;
        } else {
          blinkerL = B10111100;
        }

        if (turnR == "1") {
          blinkerR = B00001000;
        } else {
          blinkerR = 0x00;
        }

        if (ABSS == "1") {
          absLight = B01000000;
        } else {
          absLight = 0x00;
        }

        if (tcActiveS == "1" || tyreSlipS == "1") {
          tcACtive = B00000010;
        } else {
          tcACtive = 0x00;
        }
        
        if (tcEnabledS == "1") {
          tcEnabled = 0x00;
        } else {
          tcEnabled = B01100001;
        }

        if (shiftLight == "1") {
          arrow = B01100100;
        } else {
          arrow = 0x00;
        }

        setCruiseControl();
        setRpm();
        setSpeed();
        break;
      default:
        battWarn = 0x00;
        engineWarn = 0x00;
        ignition = 0x00;
        handbrake = 0x00;
        cruiseControlSpeed = 0;
        cruiseControl = 0x00;
        highBeam = 0x00;
        newSpeed = 0x00;
        speed = 0x00;
        break;
    }
	}

	void loop() { // Called once per arduino loop
    opSend(0x591, 0x91, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    //Ignition
    opSend(0x3B2, ignition, 0x00, 0x00, B1000000, blinkerR, 0x00, blinkerL, 0x00);
    // Byte 1: B01000010 fog light 2, B01000100 means day lights, B01000000 screen on, B10000000 screen off, B01000001 door ajar light
    // Byte 4: B00000001 means dim, B0000100 means less dim, B0001000 means even less dim B0010000 means full brightness
    // Byte 5: B00001000 right blinker on, 0x00 right blinker off
    // Byte 7: B11111100 left blinker on, B10111100 left blinker off
    // Byte 8: B00000001 fog lights

    //Coolant
    opSend(0x156, temp, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00);
    //Byte 1: Temperature

    //Airbag
    opSend(0x04C, 0x00, B01010101, B01010101, 0x00, 0x00, 0x00, 0x00, 0x00);
    //Byte 2&3: B01010101=All Seatbelts applied (5)

    //Parking Brake (Fusion 2013)
    opSend(0x213, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //Vehicle Alarm + Parking Brake
    opSend(0x3C3, 0x00, B11111111, handbrake, 0x00, 0x00, 0x00, 0x00, 0x00);
    //Bytes 1/2: B11111111=Alarm
    //Byte 3: B00010000=Park Brake
    //Byte 4: B11111111=Brake Applied

    //ABS
    opSend(0x416, 0x0E, 0x00, 0x00, 0x00, 0x00, tcACtive, absLight, 0x00);
    //Byte 6: B00000100=Slow flashing TC, B00000010=Solid TC Light
    //Byte 7: B10000000=Rapid flashing ABS, B01000000=Solid ABS

    //TPMS
    opSend(0x3b4, tpms, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    //Byte 1: B00110001=Shows Low Tire Pressure Menu, B00000100 shows still tire warning, B00001000 shows flashing tire warning
    //Byte 2: Highlights Front R and L tires
    //Byte 3: Same as above but rear

    //ECU
    opSend(0x421, engineWarn, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    //Byte 1: B00001000=Engine Flashing CEL, B00000100=Engine warning Solid CEL

    //Power Steering
    opSend(0x877, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //Localization
    opSend(0x191, B01000001, B00010001, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    //Byte 1: B01000001=KM and CELSIUS units
    //Byte 2: B00010001=English, B00100011=German

    //Outside Temp
    opSend(0x3B3, 0x00, 0x00, 0x00, 0x00, outsideTemp * 2 + 80, 0x00, 0xFF, 0x00);
    //Byte 5: Outside Temp Value -> function x = temp * 2 + 80. i.e: 200=60°C, 150=35°C, 125=22°C, 100=10°C, 80=0°C

    //Front Camera
    opSend(0x3D8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    //Tractioncontrol
    opSend(0x46A, 0x00, 0x00, tcEnabled, 0x00, 0x00, 0x00, 0x00, 0x00);

    //High Beam + Auto Hed
    opSend(0x46B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, highBeam);
    //Byte 8: B00110000=High Beam

    // Rear Park Pilot
    opSend(0x3AA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    // Reset autoStop system
    opSend(0x166, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    // Reset MyKey system
    opSend(0x432, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    // RPM
    opSend(0x42F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, rpmgate, finetunerRpm);

    // Speedometer
    opSend(0x202, 0x40, 0x00, 0x00, 0x00, distance, distance, speed, finetunerSpeed);

    //Cruise Control
    opSend(0x77, targetCruiseControl, 0x00, 0x00, 0x00, cruiseControlSpeed, 0x00, 0x00, 0x00);
    //Byte 1: B00010000=Enable target speed change
    //Byte 5: Target speed

    //Charging System
    opSend(0x42c, cruiseControl, battWarn, 0xA6, 0x00, 0x00, 0x00, 0x00, 0x00);
    //Byte 1: B10010000=Cruise Control bold icon, B01110111=Cruise Control gray icon w/ strikethrouhg, 
    //Byte 2: B00010001=Battery warn
    
    sendOdometer();

    sendButtonMenu();
	}

	// Called once between each byte read on arduino, do not add any code here, unless you know what you are doing.
	void idle() {}
};

#endif
