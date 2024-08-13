#include "x264_encoder_h422.h"

#include <assert.h>
#include <vector>

const uint8_t X264EncoderH422::s_UUID[] = { 0x6b, 0x88, 0xe8, 0x41, 0xd8, 0xe4, 0x41, 0x4b, 0x87, 0x9e, 0xa4, 0x80, 0xfc, 0x90, 0xda, 0xbd };

StatusCode X264EncoderH422::s_GetEncoderSettings(HostPropertyCollectionRef* p_pValues, HostListRef* p_pSettingsList)
{
	HostCodecConfigCommon commonProps;
	commonProps.Load(p_pValues);

	UISettingsController settings(commonProps);
	settings.Load(p_pValues);

	return settings.Render(p_pSettingsList);
}

StatusCode X264EncoderH422::s_RegisterCodecs(HostListRef* p_pList)
{
	// add x264 encoder
	HostPropertyCollectionRef codecInfo;
	if (!codecInfo.IsValid()) {
		return errAlloc;
	}

	codecInfo.SetProperty(pIOPropUUID, propTypeUInt8, X264EncoderH422::s_UUID, 16);

	const char* pCodecName = "Auto";
	codecInfo.SetProperty(pIOPropName, propTypeString, pCodecName, static_cast<int>(strlen(pCodecName)));

	const char* pCodecGroup = "X264 High 4:2:2";
	codecInfo.SetProperty(pIOPropGroup, propTypeString, pCodecGroup, static_cast<int>(strlen(pCodecGroup)));

	uint32_t val = 'avc1';
	codecInfo.SetProperty(pIOPropFourCC, propTypeUInt32, &val, 1);

	val = mediaVideo;
	codecInfo.SetProperty(pIOPropMediaType, propTypeUInt32, &val, 1);

	val = dirEncode;
	codecInfo.SetProperty(pIOPropCodecDirection, propTypeUInt32, &val, 1);

	val = clrUYVY;
	codecInfo.SetProperty(pIOPropColorModel, propTypeUInt32, &val, 1);

	// Optionally enable both Data Ranges, Video will be default for "Auto" thus "0" value goes first
	std::vector<uint8_t> dataRangeVec;
	dataRangeVec.push_back(0);
	dataRangeVec.push_back(1);
	codecInfo.SetProperty(pIOPropDataRange, propTypeUInt8, dataRangeVec.data(), static_cast<int>(dataRangeVec.size()));

	uint8_t hSampling = 2;
	uint8_t vSampling = 1;
	codecInfo.SetProperty(pIOPropHSubsampling, propTypeUInt8, &hSampling, 1);
	codecInfo.SetProperty(pIOPropVSubsampling, propTypeUInt8, &vSampling, 1);

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

X264EncoderH422::X264EncoderH422() : X264Encoder(4, X264_CSP_UYVY)
{
	g_Log(logLevelInfo, "X264 Plugin :: X264EncoderH422 :: ()");
}

X264EncoderH422::~X264EncoderH422()
{
}

StatusCode X264EncoderH422::DoProcess(HostBufferRef* p_pBuff)
{
	assert(m_ColorModel == X264_CSP_UYVY);

	if (m_Error != errNone) {
		return m_Error;
	}

	const int numDelayedFrames = x264_encoder_delayed_frames(m_pContext);
	if (((p_pBuff == NULL) || !p_pBuff->IsValid()) && (numDelayedFrames == 0)) {
		return errMoreData;
	}

	x264_picture_t outPic;
	x264_picture_init(&outPic);
	x264_nal_t* pNals = 0;
	int numNals = 0;
	int bytes = 0;
	int64_t pts = -1;

	if ((p_pBuff == NULL) || !p_pBuff->IsValid()) {
		bytes = x264_encoder_encode(m_pContext, &pNals, &numNals, 0, &outPic);
	} else {
		char* pBuf = NULL;
		size_t bufSize = 0;
		if (!p_pBuff->LockBuffer(&pBuf, &bufSize)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to lock the buffer");
			return errFail;
		}

		if (pBuf == NULL || bufSize == 0) {
			g_Log(logLevelError, "X264 Plugin :: No data to encode");
			p_pBuff->UnlockBuffer();
			return errUnsupported;
		}

		uint32_t width = 0;
		uint32_t height = 0;
		if (!p_pBuff->GetUINT32(pIOPropWidth, width) || !p_pBuff->GetUINT32(pIOPropHeight, height)) {
			g_Log(logLevelError, "X264 Plugin :: Width/Height not set when encoding the frame");
			return errNoParam;
		}

		if (!p_pBuff->GetINT64(pIOPropPTS, pts)) {
			g_Log(logLevelError, "X264 Plugin :: PTS not set when encoding the frame");
			return errNoParam;
		}

		x264_picture_t inPic;
		x264_picture_init(&inPic);
		inPic.i_pts = pts;
		inPic.img.plane[1] = 0;
		inPic.img.plane[2] = 0;
		inPic.img.plane[3] = 0;
		inPic.img.i_csp = m_ColorModel;

		// harcoded clrUYVY -> X264_CSP_UYVY

		inPic.img.i_plane = 1;
		inPic.img.i_stride[0] = width * 2;
		inPic.img.plane[0] = reinterpret_cast<uint8_t*>(const_cast<char*>(pBuf));
		bytes = x264_encoder_encode(m_pContext, &pNals, &numNals, &inPic, &outPic);
		p_pBuff->UnlockBuffer();

	}

	if (bytes < 0) {
		return errFail;
	} else if (bytes == 0) {
		return errMoreData;
	} else if (m_IsMultiPass && (m_PassesDone == 0)) {
		return errNone;
	}

	HostBufferRef outBuf(false);
	if (!outBuf.IsValid() || !outBuf.Resize(bytes)) {
		return errAlloc;
	}

	char* pOutBuf = NULL;
	size_t outBufSize = 0;
	if (!outBuf.LockBuffer(&pOutBuf, &outBufSize)) {
		return errAlloc;
	}

	memcpy(pOutBuf, pNals[0].p_payload, bytes);

	int64_t ts = outPic.i_pts;
	outBuf.SetProperty(pIOPropPTS, propTypeInt64, &ts, 1);

	ts = outPic.i_dts;
	outBuf.SetProperty(pIOPropDTS, propTypeInt64, &ts, 1);

	uint8_t isKeyFrame = IS_X264_TYPE_I(outPic.i_type) ? 1 : 0;
	outBuf.SetProperty(pIOPropIsKeyFrame, propTypeUInt8, &isKeyFrame, 1);

	return m_pCallback->SendOutput(&outBuf);
}