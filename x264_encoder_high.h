#ifndef X264ENCODER__X264_ENCODER_HIGH_H_
#define X264ENCODER__X264_ENCODER_HIGH_H_

#include <memory>

#include "x264_encoder.h"

using namespace IOPlugin;

class X264EncoderHigh final : public X264Encoder
{

public:
	static const uint8_t s_UUID[];

public:
	X264EncoderHigh();
	~X264EncoderHigh();

	static StatusCode s_RegisterCodecs(HostListRef* p_pList);
	static StatusCode s_GetEncoderSettings(HostPropertyCollectionRef* p_pValues, HostListRef* p_pSettingsList);
};

#endif // X264ENCODER__X264_ENCODER_H422_H_