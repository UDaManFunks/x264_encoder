#include "x264_encoder_high.h"

#include <vector>

const uint8_t X264EncoderHigh::s_UUID[] = { 0x6b, 0x88, 0xe8, 0x41, 0xd8, 0xe4, 0x41, 0x4b, 0x87, 0x9e, 0xa4, 0x80, 0xfc, 0x90, 0xda, 0xbb };

StatusCode X264EncoderHigh::s_GetEncoderSettings(HostPropertyCollectionRef* p_pValues, HostListRef* p_pSettingsList)
{
	HostCodecConfigCommon commonProps;
	commonProps.Load(p_pValues);

	UISettingsController settings(commonProps);
	settings.Load(p_pValues);

	return settings.Render(p_pSettingsList);
}

StatusCode X264EncoderHigh::s_RegisterCodecs(HostListRef* p_pList)
{
	// add x264 encoder
	HostPropertyCollectionRef codecInfo;
	if (!codecInfo.IsValid()) {
		return errAlloc;
	}

	codecInfo.SetProperty(pIOPropUUID, propTypeUInt8, X264EncoderHigh::s_UUID, 16);

	const char* pCodecName = "Auto";
	codecInfo.SetProperty(pIOPropName, propTypeString, pCodecName, static_cast<int>(strlen(pCodecName)));

	const char* pCodecGroup = "X264 High";
	codecInfo.SetProperty(pIOPropGroup, propTypeString, pCodecGroup, static_cast<int>(strlen(pCodecGroup)));

	uint32_t val = 'avc1';
	codecInfo.SetProperty(pIOPropFourCC, propTypeUInt32, &val, 1);

	val = mediaVideo;
	codecInfo.SetProperty(pIOPropMediaType, propTypeUInt32, &val, 1);

	val = dirEncode;
	codecInfo.SetProperty(pIOPropCodecDirection, propTypeUInt32, &val, 1);

	val = clrNV12;
	codecInfo.SetProperty(pIOPropColorModel, propTypeUInt32, &val, 1);

	// Optionally enable both Data Ranges, Video will be default for "Auto" thus "0" value goes first
	std::vector<uint8_t> dataRangeVec;
	dataRangeVec.push_back(0);
	dataRangeVec.push_back(1);
	codecInfo.SetProperty(pIOPropDataRange, propTypeUInt8, dataRangeVec.data(), static_cast<int>(dataRangeVec.size()));

	val = 8;
	codecInfo.SetProperty(pIOPropBitsPerSample, propTypeUInt32, &val, 1);

	const uint32_t temp = 0;
	codecInfo.SetProperty(pIOPropTemporalReordering, propTypeUInt32, &temp, 1);

	const uint8_t fieldSupport = (fieldProgressive | fieldTop | fieldBottom);
	codecInfo.SetProperty(pIOPropFieldOrder, propTypeUInt8, &fieldSupport, 1);

	std::vector<std::string> containerVec;
	containerVec.push_back("mp4");
	containerVec.push_back("mov");
	std::string valStrings;
	for (size_t i = 0; i < containerVec.size(); ++i) {
		valStrings.append(containerVec[i]);
		if (i < (containerVec.size() - 1)) {
			valStrings.append(1, '\0');
		}
	}

	codecInfo.SetProperty(pIOPropContainerList, propTypeString, valStrings.c_str(), static_cast<int>(valStrings.size()));

	if (!p_pList->Append(&codecInfo)) {
		return errFail;
	}

	return errNone;
}

X264EncoderHigh::X264EncoderHigh() : X264Encoder(2, X264_CSP_NV12)
{
	g_Log(logLevelInfo, "X264 Plugin :: X264EncoderHigh :: ()");

}

X264EncoderHigh::~X264EncoderHigh()
{
}