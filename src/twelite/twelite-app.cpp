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

void onPacketReceived(const uint8_t *buffer, size_t size);

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
			<< NWK_SIMPLE::dup_check(16,50,5) // set 
			<< NWK_SIMPLE::repeat_max(0);	// can repeat a packet up to three times. (being kind of a router)
	the_twelite.begin();					// start twelite!
	Serial.begin(115200);
	ps.begin(115200);
	ps.setPacketHandler(&onPacketReceived);
}

/*** loop procedure (called every event) */
void loop()
{
	ps.update();
}

void onPacketReceived(const uint8_t *buffer, size_t size)
{
	if (auto &&pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()){
		pkt << tx_addr(0xFF)			  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x3)			  // set retry (0x3 send four times in total)
			<< tx_packet_delay(0, 200, 50); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)
		switch (size){
			case 8:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 8));
			}break;
			case 12:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 12));
			}break;
			case 16:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 16));
			}break;
			case 20:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 20));
			}break;
			case 24:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 24));
			}break;
			case 28:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 28));
			}break;
			case 32:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 32));
			}break;
			case 36:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 36));
			}break;
			case 40:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 40));
			}break;
			case 44:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 44));
			}break;
			case 48:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 48));
			}break;
			case 52:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 52));
			}break;
			case 56:{
				pack_bytes(pkt.get_payload(),make_pair(buffer, 56));
			}break;
			default:{
				return;
			}
		}
		// do transmit
		pkt.transmit();
		ps.send(buffer,size);
	}
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