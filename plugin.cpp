#include <assert.h>
#include <cstring>

#include "plugin.h"
#include "x264_encoder.h"
#include "x264_encoder_main.h"
#include "x264_encoder_high.h"
#include "x264_encoder_h422.h"

static const uint8_t pMyUUID[] = { 0x5e, 0x43, 0xce, 0x60, 0x45, 0x11, 0x4f, 0x58, 0x87, 0xde, 0xf3, 0x02, 0x80, 0x1e, 0x7b, 0xbc };

using namespace IOPlugin;

StatusCode g_HandleGetInfo(HostPropertyCollectionRef* p_pProps)
{
	StatusCode err = p_pProps->SetProperty(pIOPropUUID, propTypeUInt8, pMyUUID, 16);
	if (err == errNone) {
		err = p_pProps->SetProperty(pIOPropName, propTypeString, "X264Encoder Plugin", static_cast<int>(strlen("X264Encoder Plugin")));
	}

	return err;
}

StatusCode g_HandleCreateObj(unsigned char* p_pUUID, ObjectRef* p_ppObj)
{

	// baseline

	if (memcmp(p_pUUID, X264Encoder::s_UUID, 16) == 0) {
		*p_ppObj = new X264Encoder();
		return errNone;
	}

	// main

	if (memcmp(p_pUUID, X264EncoderMain::s_UUID, 16) == 0) {
		*p_ppObj = new X264EncoderMain();
		return errNone;
	}

	// high

	if (memcmp(p_pUUID, X264EncoderHigh::s_UUID, 16) == 0) {
		*p_ppObj = new X264EncoderHigh();
		return errNone;
	}

	// high422

	if (memcmp(p_pUUID, X264EncoderH422::s_UUID, 16) == 0) {
		*p_ppObj = new X264EncoderH422();
		return errNone;
	}

	return errUnsupported;
}

StatusCode g_HandlePluginStart()
{
	// perform libs initialization if needed
	return errNone;
}

StatusCode g_HandlePluginTerminate()
{
	return errNone;
}

StatusCode g_ListCodecs(HostListRef* p_pList)
{
	StatusCode err = X264Encoder::s_RegisterCodecs(p_pList);
	if (err != errNone) {
		return err;
	}

	err = X264EncoderMain::s_RegisterCodecs(p_pList);
	if (err != errNone) {
		return err;
	}

	err = X264EncoderHigh::s_RegisterCodecs(p_pList);
	if (err != errNone) {
		return err;
	}

	err = X264EncoderH422::s_RegisterCodecs(p_pList);
	if (err != errNone) {
		return err;
	}

	return errNone;
}

StatusCode g_ListContainers(HostListRef* p_pList)
{
	return errNone;
}

StatusCode g_GetEncoderSettings(unsigned char* p_pUUID, HostPropertyCollectionRef* p_pValues, HostListRef* p_pSettingsList)
{
	if (memcmp(p_pUUID, X264Encoder::s_UUID, 16) == 0) {
		return X264Encoder::s_GetEncoderSettings(p_pValues, p_pSettingsList);
	}

	if (memcmp(p_pUUID, X264EncoderMain::s_UUID, 16) == 0) {
		return X264EncoderMain::s_GetEncoderSettings(p_pValues, p_pSettingsList);
	}

	if (memcmp(p_pUUID, X264EncoderHigh::s_UUID, 16) == 0) {
		return X264EncoderHigh::s_GetEncoderSettings(p_pValues, p_pSettingsList);
	}

	if (memcmp(p_pUUID, X264EncoderH422::s_UUID, 16) == 0) {
		return X264EncoderH422::s_GetEncoderSettings(p_pValues, p_pSettingsList);
	}

	return errNoCodec;
}
