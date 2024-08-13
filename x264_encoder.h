#ifndef X264ENCODER__X264_ENCODER_H_
#define X264ENCODER__X264_ENCODER_H_

#include <memory>

#include "wrapper/plugin_api.h"
#include "uisettings_controller.h"
#include "x264.h"

using namespace IOPlugin;

struct x264_t;
struct x264_param_t;

struct X264APIDeleter {
	void operator()(x264_t* pContext) {
		if (pContext != nullptr) {
			x264_encoder_close(pContext);
		}
	}
};

class X264Encoder : public IPluginCodecRef
{
public:
	static const uint8_t s_UUID[];

public:
	X264Encoder();
	virtual ~X264Encoder();

	static StatusCode s_RegisterCodecs(HostListRef* p_pList);
	static StatusCode s_GetEncoderSettings(HostPropertyCollectionRef* p_pValues, HostListRef* p_pSettingsList);

	virtual bool IsNeedNextPass() override
	{
		return (m_IsMultiPass && (m_PassesDone < 2));
	}

	virtual bool IsAcceptingFrame(int64_t p_PTS) override
	{
		// accepts every frame in multipass, PTS is the frame number in track fps
		// return false after all passes finished
		return (m_IsMultiPass && (m_PassesDone < 3));
	}

protected:
	X264Encoder(int iProfile, int iColorModel);
	virtual void DoFlush() override final;
	virtual StatusCode DoInit(HostPropertyCollectionRef* p_pProps) override final;
	virtual StatusCode DoOpen(HostBufferRef* p_pBuff) override final;
	virtual StatusCode DoProcess(HostBufferRef* p_pBuff) override;
	int GetProfile();
	int GetColorModel();


protected:
	std::unique_ptr<x264_t, X264APIDeleter> m_pContext;
	std::unique_ptr<UISettingsController> m_pSettings;
	HostCodecConfigCommon m_CommonProps;
	bool m_IsMultiPass;
	uint32_t m_PassesDone;
	StatusCode m_Error;

private:
	void SetupContext(bool p_IsFinalPass);

private:
	std::string m_TmpFileName;
	int m_ColorModel;
	int m_Profile;
	uint32_t m_BFrames;
};

#endif // X264ENCODER__X264_ENCODER_H_