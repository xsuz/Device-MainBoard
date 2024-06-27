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
			<< NWK_SIMPLE::repeat_max(0);	// can repeat a packet up to three times. (being kind of a router)
	the_twelite.begin();					// start twelite!
	Serial.begin(115200);
}

/*** loop procedure (called every event) */
void loop()
{
}

void on_rx_packet(packet_rx &rx, bool_t &handled)
{
	uint8_t buf[256];
	auto pkt = rx.get_payload();
	auto len = rx.get_length();
	uint8_t buf_idx=0;

	for (uint8_t i = 0; i < len; i++)
	{
		if (pkt[i]==0x00){
			Serial.write(buf_idx+1);
			for (uint8_t j = 0; j < buf_idx; j++)
			{
				Serial.write(buf[j]);
			}
			buf_idx=0;
		}else{
			buf[buf_idx]=pkt[i];
			buf_idx++;
		}
	}
	Serial.write(buf_idx+1);
	for (uint8_t j = 0; j < buf_idx; j++)
	{
		Serial.write(buf[j]);
	}
	Serial.write(0x00);
}

/* Copyright (C) 2019-2021 Mono Wireless Inc. All Rights Reserved. *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE     *
 * AGREEMENT).                                                     */