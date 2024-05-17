// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
#include "SensorPacket.h"
#include "client.h"

/*** Config part */
// application ID
const uint32_t APP_ID = 0x1234abcd;

// channel
const uint8_t CHANNEL = 13;

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
	auto &&nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(Client::Parent) // set Logical ID. (0x00 means a parent device)
			<< NWK_SIMPLE::repeat_max(3);	// can repeat a packet up to three times. (being kind of a router)

	the_twelite.begin(); // start twelite!
}

/*** loop procedure (called every event) */
void loop()
{
}

void on_rx_packet(packet_rx &rx, bool_t &handled)
{
	auto pkt=rx.get_payload();
	switch (pkt[0]&0xF0)
	{
	case DeviceData::DeviceID::Altitude:
	{
		union{
			DeviceData::AltData data[LEN_ALT];
			uint8_t bytes[sizeof(DeviceData::AltData[LEN_ALT])];
		} spkt;
		expand_bytes(pkt.begin(),pkt.end(),spkt.bytes);
		for(int i=0;i<LEN_ALT;i++){
			Serial << format("{'id':%x,'t':%d",spkt.data[i].id,spkt.data[i].timestamp)
					<< format(",payload:{'alt':%f",spkt.data[i].alt)
					<< format("}}") << mwx::crlf;
		}
	}break;
	case DeviceData::DeviceID::IMU:
	{
		union{
			DeviceData::IMUData data[LEN_IMU];
			uint8_t bytes[sizeof(DeviceData::IMUData[LEN_IMU])];
		} spkt;
		expand_bytes(pkt.begin(),pkt.end(),spkt.bytes);
		for(int i=0;i<LEN_IMU;i++){
			Serial << format("{'id':%x,'t':%d",spkt.data[i].id,spkt.data[i].timestamp)
					<< format(",payload:{'w':%d,'x':%d,'y':%d,'z':%d",spkt.data[i].w,spkt.data[i].x,spkt.data[i].y,spkt.data[i].z)
					<< format("}}") << mwx::crlf;
		}
	}break;
	case DeviceData::DeviceID::Pitot:
	{
		union{
			DeviceData::PitotData data[LEN_PITOT];
			uint8_t bytes[sizeof(DeviceData::PitotData[LEN_PITOT])];
		} spkt;
		expand_bytes(pkt.begin(),pkt.end(),spkt.bytes);
		for(int i=0;i<LEN_PITOT;i++){
			Serial << format("{'id':%x,'t':%d",spkt.data[i].id,spkt.data[i].timestamp)
					<< format(",payload:{'v':%f",spkt.data[i].velocity)
					<< format("}}") << mwx::crlf;
		}
	}break;
	case DeviceData::DeviceID::ServoController:{
		union{
			DeviceData::ServoData data[LEN_SERVOCONTROLLER];
			uint8_t bytes[sizeof(DeviceData::ServoData[LEN_SERVOCONTROLLER])];
		} spkt;
		expand_bytes(pkt.begin(),pkt.end(),spkt.bytes);
		for(int i=0;i<LEN_SERVOCONTROLLER;i++){
			Serial << format("{'id':%x,'t':%d",spkt.data[i].id,spkt.data[i].timestamp)
					<< format(",payload:{'rudder':%f,'elevator':%f,'trim':%f,'status':%x",spkt.data[i].rudder,spkt.data[i].elevator,spkt.data[i].trim,spkt.data[i].status)
					<< format("}}") << mwx::crlf;
		}
		}break;
	case DeviceData::DeviceID::Tachometer:
	{
		union{
			DeviceData::TachometerData data[LEN_TACHOMETER];
			uint8_t bytes[sizeof(DeviceData::TachometerData[LEN_TACHOMETER])];
		} spkt;
		expand_bytes(pkt.begin(),pkt.end(),spkt.bytes);
		for(int i=0;i<LEN_TACHOMETER;i++){
			Serial << format("{'id':%x,'t':%d",spkt.data[i].id,spkt.data[i].timestamp)
					<< format(",payload:{'pwr':%d,'rpm':%d",spkt.data[i].power,spkt.data[i].rpm)
					<< format("}}") << mwx::crlf;
		}
	}break;
	default:{
		Serial << format("Unknown ID : %x",pkt[0]) << mwx::crlf;
	}break;
	}
}

/* Copyright (C) 2019-2021 Mono Wireless Inc. All Rights Reserved. *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE     *
 * AGREEMENT).                                                     */