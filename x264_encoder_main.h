#ifndef X264ENCODER__X264_ENCODER_MAIN_H_
#define X264ENCODER__X264_ENCODER_MAIN_H_

#include <memory>

#include "x264_encoder.h"

using namespace IOPlugin;

class X264EncoderMain final : public X264Encoder
{

public:
	static const uint8_t s_UUID[];

public:
	X264EncoderMain();
	~X264EncoderMain();

	static StatusCode s_RegisterCodecs(HostListRef* p_pList);
	static StatusCode s_GetEncoderSettings(HostPropertyCollectionRef* p_pValues, HostListRef* p_pSettingsList);
};

#endif // X264ENCODER__X264_ENCODER_MAIN_H_