// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
#include <SensorPacket.h>
#define UART0
#include "PacketSerial/PacketSerial.h"

/*** Config part */
// application ID
const uint32_t APP_ID = 0x1234abcd;

// channel
const uint8_t CHANNEL = 13;

PacketSerial ps;

/*** setup procedure (run once at cold boot) */
void setup()
{
	/*** SETUP section */

	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)	// set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();	// open receive circuit (if not set, it can't listen packts from others)

	auto &&nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0x00) // set Logical ID. (0x00 means a parent device)
			<< NWK_SIMPLE::repeat_max(3);	// can repeat a packet up to three times. (being kind of a router)
	the_twelite.begin();					// start twelite!
	ps.begin(115200);
}

/*** loop procedure (called every event) */
void loop()
{
}

void on_rx_packet(packet_rx &rx, bool_t &handled)
{
	uint8_t bytes[128];
	auto packet = rx.get_payload();
	for(int i=0;i<rx.get_length();i++){
		bytes[i] = packet[i];
	}
	ps.send(bytes, rx.get_length());
}

/* Copyright (C) 2019-2021 Mono Wireless Inc. All Rights Reserved. *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE     *
 * AGREEMENT).                                                     */