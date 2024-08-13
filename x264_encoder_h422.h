#ifndef X264ENCODER__X264_ENCODER_H422_H_
#define X264ENCODER__X264_ENCODER_H422_H_

#include <memory>

#include "x264_encoder.h"

using namespace IOPlugin;

class X264EncoderH422 final : public X264Encoder
{

public:
	static const uint8_t s_UUID[];

public:
	X264EncoderH422();
	~X264EncoderH422();

	static StatusCode s_RegisterCodecs(HostListRef* p_pList);
	static StatusCode s_GetEncoderSettings(HostPropertyCollectionRef* p_pValues, HostListRef* p_pSettingsList);

protected:
	StatusCode DoProcess(HostBufferRef* p_pBuff) override;
};

#endif // X264ENCODER__X264_ENCODER_H422_H_