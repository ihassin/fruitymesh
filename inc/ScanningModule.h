/**

Copyright (c) 2014-2015 "M-Way Solutions GmbH"
FruityMesh - Bluetooth Low Energy mesh protocol [http://mwaysolutions.com/]

This file is part of FruityMesh

FruityMesh is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/*
 * The ScanModule should provide filtering so that all nodes are able to scan
 * for advertising messages and broadcast them through the mesh.
 * This module should also allow to trigger certain tasks after specific
 * packets have been scanned.
 *
 */

#pragma once

#include <Module.h>

#define SCAN_FILTER_NUMBER 1 //Number of filters that can be set

#define SCAN_BUFFERS_SIZE 10 //Max number of packets that are buffered

		enum groupingType {GROUP_BY_ADDRESS=1, NO_GROUPING=2};

		typedef struct
		{
			u8 active;
			u8 grouping;
			ble_gap_addr_t address;
			i8 minRSSI;
			i8 maxRSSI;
			u8 advertisingType;
			u8 byteMask[31];
			u8 mandatory[31];

		} scanFilterEntry;

class ScanningModule: public Module
{
	private:

		/*
		 * Filters coud be:
		 * 	- group all filtered packets by address and sum their RSSI and count
		 * 	- scan for specific packets and send them back
		 * 	-
		 *
		 * */



		typedef struct
		{
			ble_gap_addr_t address;
			u32 rssiSum;
			u16 count;
		} scannedPacket;

		//Module configuration that is saved persistently (size must be multiple of 4)
		struct ScanningModuleConfiguration : ModuleConfiguration{
			//Insert more persistent config values here
		};

		ScanningModuleConfiguration configuration;


		scanFilterEntry scanFilters[SCAN_FILTER_NUMBER];

		scannedPacket groupedPackets[SCAN_BUFFERS_SIZE];

		//Byte muss gesetzt sein, byte darf nicht gesetzt sein, byte ist egal
		bool setScanFilter(scanFilterEntry* filter);

	public:
		ScanningModule(u16 moduleId, Node* node, ConnectionManager* cm, const char* name, u16 storageSlot);

		void ConfigurationLoadedHandler();

		void ResetToDefaultConfiguration();

		void TimerEventHandler(u16 passedTime, u32 appTimer);

		void BleEventHandler(ble_evt_t* bleEvent);

		void NodeStateChangedHandler(discoveryState newState);

		bool TerminalCommandHandler(string commandName, vector<string> commandArgs);
};
