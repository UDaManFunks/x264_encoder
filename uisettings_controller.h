#ifndef X264ENCODER__UISETTINGS_CONTROLLER_H_
#define X264ENCODER__UISETTINGS_CONTROLLER_H_

#include "wrapper/plugin_api.h"
#include "x264.h"

#include <memory>

namespace IOPlugin {

	class UISettingsController final
	{
	public:
		UISettingsController();
		UISettingsController(const HostCodecConfigCommon& p_CommonProps);
		~UISettingsController();
		void Load(IPropertyProvider* p_pValues);
		StatusCode Render(HostListRef* p_pSettingsList);

	private:
		void InitDefaults();
		StatusCode RenderGeneral(HostListRef* p_pSettingsList);
		StatusCode RenderQuality(HostListRef* p_pSettingsList);

	public:
		int32_t GetNumPasses()
		{
			return m_NumPasses;
		}

		const char* GetEncPreset() const
		{
			return x264_preset_names[m_EncPreset];
		}

		const char* GetTune() const
		{

			if (m_Tune > 0) {
				return x264_tune_names[(m_Tune - 1)];
			}

			return NULL;
		}

		int32_t GetQualityMode() const
		{
			return (m_NumPasses == 2) ? X264_RC_ABR : m_QualityMode;
		}

		int32_t GetQP() const
		{
			return std::max<int>(0, m_QP);
		}

		int32_t GetBitRate() const
		{
			return m_BitRate;
		}

		const std::string& GetMarkerColor() const
		{
			return m_MarkerColor;
		}

	private:
		HostCodecConfigCommon m_CommonProps;

		std::string m_MarkerColor;
		std::string m_TmpFileNameIn;
		std::string m_TmpFileNameOut;

		int32_t m_EncPreset;
		int32_t m_Tune;
		int32_t m_NumPasses;
		int32_t m_QualityMode;
		int32_t m_QP;
		int32_t m_BitRate;
	};

}

#endif // PRORES__UISETINGS_CONTROLLER_H_