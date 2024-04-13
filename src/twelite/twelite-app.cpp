// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
#include "SensorPacket.h"
#include "client.h"
#include <bno055/BNO055_support.h>
#define P
/*** Config part */
// application ID
const uint32_t APP_ID = 0x96fb64cd;

// channel
const uint8_t CHANNEL = 7;
bno055_t bno;

/*** setup procedure (run once at cold boot) */
void setup()
{
	/*** SETUP section */

	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)	// set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();	// open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	#ifdef P
	auto &&nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(Client::Parent) // set Logical ID. (0x00 means a parent device)
			<< NWK_SIMPLE::repeat_max(3);	// can repeat a packet up to three times. (being kind of a router)
	#else
	auto &&nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(Client::Addr::Children) // set Logical ID. (0x00 means a parent device)
			<< NWK_SIMPLE::repeat_max(3);	// can repeat a packet up to three times. (being kind of a router)
	#endif

	the_twelite.begin(); // start twelite!
	Analogue.setup(true);
	Analogue.begin(pack_bits(PIN_ANALOGUE::A1), 50);
	Serial.begin(115200);
	BNO_Init(&bno);
	delay(1000);
#ifdef P
	DeviceData::MainData data;
	Client::transmit<DeviceData::MainData>(
		DeviceData::DeviceID::MainBoard,
		data,
		millis(),
		(float)analogRead_mv(PIN_ANALOGUE::A1)*2.0/1000.0,
		Client::Addr::Children);
	delay(200);
	Serial << "sending..." << mwx::crlf;
#endif
}

/*** loop procedure (called every event) */
void loop()
{
#ifndef P
	DeviceData::PitotData data;
	data.velocity = 7.2;
	Client::transmit<DeviceData::PitotData>(
		DeviceData::DeviceID::Pitot,
		data,
		millis(),
		(float)analogRead_mv(PIN_ANALOGUE::A1)*2.0/1000.0,
		Client::Addr::Parent);
	Serial << ".";
	delay(200);
#endif
}

void on_rx_packet(packet_rx &rx, bool_t &handled)
{
#ifdef P
	// Client::on_rx_packet(rx);
	// auto packet = rx.get_payload();
	// if (packet[0] == DeviceData::DeviceID::Pitot)
	// {
	// 	DeviceData::SensorPacket<DeviceData::PitotData> spkt;
	// 	expand_bytes(packet.begin(), packet.end(), spkt.bytes);
	// 	Serial << format("id:%x timestamp:%d voltage:%f velocity%f",spkt.id,spkt.timestamp,spkt.voltage,spkt.data.velocity) << mwx::crlf;
	// }
#else
	// Client::on_rx_packet(rx);
#endif
}

/* Copyright (C) 2019-2021 Mono Wireless Inc. All Rights Reserved. *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE     *
 * AGREEMENT).                                                     */