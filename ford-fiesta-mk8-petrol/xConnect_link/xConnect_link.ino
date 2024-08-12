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

// --- Variable definitions, change as necessary ---
#define VERSION 'j'
#define DEVICE_NAME "FORD FIESTA MK8 V1.2" 
// #define MCB_EN // Board type, if not using Micro Control Boards by xConnect, do not define this.
#define LED_BUILTIN 2

#ifdef MCB_EN
  #define UP_BTN_PIN 32 
  #define OK_BTN_PIN 34
  #define DOWN_BTN_PIN 26
  #define MENU_BTN_PIN 35
  #define BACK_BTN_PIN 33
#endif

#ifndef MCB_EN
  #define MCP_EN_PIN 9 // Modify only when using arduino
  #define UP_BTN_PIN 11 
  #define OK_BTN_PIN 12
  #define DOWN_BTN_PIN 10
  #define MENU_BTN_PIN 7
  #define BACK_BTN_PIN 6
#endif
#define DEVICE_UNIQUE_ID "kck3e1gd-se47-t3jf-0rzk-xzpg-leoootft1dlo" // Generate ids randomly. Get a random ID from https://duid.xconnect.cc

// Custom Config:
#define HANDBRAKE_FLASH_PERIOD 22

// --- Required modules / vartiables for SimHub ---
#include <mcp_can.h>
#ifdef MCB_EN
  MCP_CAN CAN(5);
#else
  MCP_CAN CAN(MCP_EN_PIN);
#endif

void opSend(short addr, byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte h) {
  unsigned char data[8] = { a, b, c, d, e, f, g, h };
  CAN.sendMsgBuf(addr, 0, 8, data);
}
void pulseLED(int length) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(length);
  digitalWrite(LED_BUILTIN, LOW);
}
#include "FlowSerialRead.h"
#include "SHCustomProtocol.h"
SHCustomProtocol shCustomProtocol;
#include "SHCommands.h"
#include "CanBusHandler.h" // Init CanBus
char loop_opt;
unsigned long lastSerialActivity = 0;


// --- Setup function, runs once
void setup() {	
  FlowSerialBegin(19200);
  pinMode(LED_BUILTIN, OUTPUT);
  CAN_INIT:
    if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) {
      FlowSerialDebugPrintLn("CAN BUS CONTROLLER initiation ok");
      pulseLED(50);
      delay(100);
      pulseLED(50);
    } else {
      pulseLED(50);
      FlowSerialDebugPrintLn("CAN BUS CONTROLLER initiation failed, retrying in 500ms");
      delay(100);
      pulseLED(400);
      goto CAN_INIT;
    }
    // Set CAN BUS mode to MCP_ANY
    CAN.setMode(MCP_ANY);
    pinMode(OK_BTN_PIN, INPUT_PULLUP);
    pinMode(UP_BTN_PIN, INPUT_PULLUP);
    pinMode(DOWN_BTN_PIN, INPUT_PULLUP);
    pinMode(MENU_BTN_PIN, INPUT_PULLUP);
    pinMode(BACK_BTN_PIN, INPUT_PULLUP);
  }

// --- Loop function, runs continuously
void loop() {
	shCustomProtocol.loop();
  #ifdef MCB_EN
    vTaskDelay(10); // Delay for the MCB
  #endif
	// Wait for data
	if (FlowSerialAvailable() > 0) {
		if (FlowSerialTimedRead() == MESSAGE_HEADER) {
			lastSerialActivity = millis();
			// Read command
			loop_opt = FlowSerialTimedRead();
			if (loop_opt == '1') Command_Hello();
			else if (loop_opt == '8') Command_SetBaudrate();
			else if (loop_opt == 'J') Command_ButtonsCount();
			else if (loop_opt == '2') Command_TM1638Count();
			else if (loop_opt == 'B') Command_SimpleModulesCount();
			else if (loop_opt == 'A') Command_Acq();
			else if (loop_opt == 'N') Command_DeviceAcknowledge();
			else if (loop_opt == 'I') Command_UniqueId();
			else if (loop_opt == '0') Command_Features();
			else if (loop_opt == '4') Command_RGBLEDSCount();
			else if (loop_opt == '6') Command_RGBLEDSData();
			else if (loop_opt == 'R') Command_RGBMatrixData();
			else if (loop_opt == 'G') Command_GearData();
			else if (loop_opt == 'P') Command_CustomProtocolData();
			else if (loop_opt == 'X')
			{
				String xaction = FlowSerialReadStringUntil(' ', '\n');
				if (xaction == F("list")) Command_ExpandedCommandsList();
				else if (xaction == F("mcutype")) Command_MCUType();
			}
		}
	}
	if (millis() - lastSerialActivity > 5000) {
		Command_Shutdown();
	}
}
