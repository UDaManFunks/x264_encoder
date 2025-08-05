#include "uisettings_controller.h"

#include <assert.h>
#include <sstream>

const char* const  UISettingsController::s_ENCODER_LEVELS[] = { "-1", "4.0", "4.1", "4.2", "5", "5.1", "5.2", "6", "6.1", "6.2", 0 };

UISettingsController::UISettingsController() {
	InitDefaults();
}

UISettingsController::UISettingsController(const HostCodecConfigCommon& p_CommonProps)
	: m_CommonProps(p_CommonProps)
{
	InitDefaults();
}

UISettingsController::~UISettingsController() {

}

void UISettingsController::Load(IPropertyProvider* p_pValues)
{
	uint8_t val8 = 0;
	p_pValues->GetUINT8("x264_reset", val8);
	if (val8 != 0) {
		*this = UISettingsController();
		return;
	}

	p_pValues->GetINT32("x264_enc_preset", m_EncPreset);
	p_pValues->GetINT32("x264_tune", m_Tune);
	p_pValues->GetINT32("x264_num_passes", m_NumPasses);
	p_pValues->GetINT32("x264_q_mode", m_QualityMode);
	p_pValues->GetINT32("x264_qp", m_QP);
	p_pValues->GetINT32("x264_bitrate", m_BitRate);
	p_pValues->GetString("x264_enc_markers", m_MarkerColor);
	p_pValues->GetINT32("x264_level", m_Level);
	p_pValues->GetINT32("x264_kf_max", m_KFMax);
	p_pValues->GetINT32("x264_kf_min", m_KFMin);
	p_pValues->GetINT32("x264_bf", m_BF);
	p_pValues->GetINT32("x264_scene_detection", m_SceneDetection);
}

StatusCode UISettingsController::Render(HostListRef* p_pSettingsList)
{
	StatusCode err = RenderGeneral(p_pSettingsList);
	if (err != errNone) {
		return err;
	}

	{
		HostUIConfigEntryRef item("x264_separator");
		item.MakeSeparator();
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to add a separator entry");
			return errFail;
		}
	}

	err = RenderQuality(p_pSettingsList);
	if (err != errNone) {
		return err;
	}

	{
		HostUIConfigEntryRef item("x264_reset");
		item.MakeButton("Reset");
		item.SetTriggersUpdate(true);
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate the button entry");
			return errFail;
		}
	}

	return errNone;
}

void UISettingsController::InitDefaults()
{
	m_EncPreset = 4;
	m_Tune = 0;
	m_NumPasses = 1;
	m_Level = 0; 
	m_QualityMode = X264_RC_CRF;
	m_QP = 23;
	m_BitRate = 8000;
	m_KFMax = 250;
	m_KFMin = 25;
	m_BF = 3;
	m_SceneDetection = 40;
}

StatusCode UISettingsController::RenderGeneral(HostListRef* p_pSettingsList)
{
	if (0) {
		HostUIConfigEntryRef item("x264_lbl_general");
		item.MakeLabel("General Settings");

		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate general label entry");
			return errFail;
		}
	}

	// Markers selection
	if (m_CommonProps.GetContainer().size() >= 32) {
		// plugin container in string "plugin_UUID:container_UUID" or "container_UUID" with UUID 32 characters
		// or "mov", "mp4" etc if non-plugin container
		HostUIConfigEntryRef item("x264_enc_markers");
		item.MakeMarkerColorSelector("Chapter Marker", "Marker 1", m_MarkerColor);
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate encoder preset UI entry");
			assert(false);
			return errFail;
		}
	}


	// Level combobox
	{
		HostUIConfigEntryRef item("x264_level");

		std::vector<std::string> textsVec;
		std::vector<int> valuesVec;

		int32_t curVal = 0;
		const char* const* pLevels = s_ENCODER_LEVELS;
		while (*pLevels != 0) {
			if (curVal == 0) {
				textsVec.push_back("Auto");
			} else {
				textsVec.push_back(*pLevels);
			}
			++pLevels;

			valuesVec.push_back(curVal++);
		}

		item.MakeComboBox("Encoder Level", textsVec, valuesVec, m_Level);
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate encoder level UI entry");
			return errFail;
		}
	}

	// Preset combobox
	{
		HostUIConfigEntryRef item("x264_enc_preset");

		std::vector<std::string> textsVec;
		std::vector<int> valuesVec;

		int32_t curVal = 1;
		const char* const* pPresets = x264_preset_names;
		while (*pPresets != 0) {
			valuesVec.push_back(curVal++);
			textsVec.push_back(*pPresets);
			++pPresets;
		}

		item.MakeComboBox("Encoder Preset", textsVec, valuesVec, m_EncPreset);
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate encoder preset UI entry");
			return errFail;
		}
	}

	// Tune combobox
	{
		HostUIConfigEntryRef item("x264_tune");

		std::vector<std::string> textsVec;
		std::vector<int> valuesVec;

		int32_t curVal = 0;

		// default None
		valuesVec.push_back(curVal++ + 0);
		textsVec.push_back("none");

		const char* const* pPresets = x264_tune_names;
		while (*pPresets != 0) {
			valuesVec.push_back(curVal++);
			textsVec.push_back(*pPresets);
			++pPresets;
		}

		item.MakeComboBox("Encoder Tune", textsVec, valuesVec, m_Tune);
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate tune UI entry");
			return errFail;
		}
	}

	return errNone;
}

StatusCode UISettingsController::RenderQuality(HostListRef* p_pSettingsList)
{
	if (0) {
		HostUIConfigEntryRef item("x264_lbl_quality");
		item.MakeLabel("Quality Settings");

		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate quality label entry");
			return errFail;
		}
	}

	{
		HostUIConfigEntryRef item("x264_num_passes");

		std::vector<std::string> textsVec;
		std::vector<int> valuesVec;

		textsVec.push_back("1-Pass");
		valuesVec.push_back(1);
		textsVec.push_back("2-Pass");
		valuesVec.push_back(2);

		item.MakeComboBox("Passes", textsVec, valuesVec, m_NumPasses);
		item.SetTriggersUpdate(true);
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate passes UI entry");
			return errFail;
		}
	}

	if (m_NumPasses < 2) {
		HostUIConfigEntryRef item("x264_q_mode");

		std::vector<std::string> textsVec;
		std::vector<int> valuesVec;

		textsVec.push_back("Constant Rate Factor");
		valuesVec.push_back(X264_RC_CRF);

		textsVec.push_back("Average Bitrate");
		valuesVec.push_back(X264_RC_ABR);

		item.MakeRadioBox("Quality", textsVec, valuesVec, GetQualityMode());
		item.SetTriggersUpdate(true);

		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate quality UI entry");
			return errFail;
		}
	}

	{
		HostUIConfigEntryRef item("x264_qp");
		const char* pLabel = NULL;
		if (m_QP < 17) {
			pLabel = "(high)";
		} else if (m_QP < 34) {
			pLabel = "(medium)";
		} else {
			pLabel = "(low)";
		}
		item.MakeSlider("Factor", pLabel, m_QP, 1, 51, 25);
		item.SetTriggersUpdate(true);
		item.SetHidden((m_QualityMode == X264_RC_ABR) || (m_NumPasses > 1));
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate qp slider UI entry");
			return errFail;
		}
	}

	{
		HostUIConfigEntryRef item("x264_separator");
		item.MakeSeparator();
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to add a separator entry");
			return errFail;
		}
	}

	{
		HostUIConfigEntryRef item("x264_kf_max");
		const char* pLabel = NULL;
		if (m_KFMax == 250) {
			pLabel = "(default)";
		} else {
			pLabel = " ";
		}
		item.MakeSlider("Keyframes Max", pLabel, m_KFMax, 1, 300, 250);
		item.SetTriggersUpdate(true);
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate kf max slider UI entry");
			return errFail;
		}
	}

	{
		if (m_KFMin > m_KFMax)
		m_KFMin = m_KFMax;
		HostUIConfigEntryRef item("x264_kf_min");
		const char* pLabel = NULL;
		if (m_KFMin == 25) {
			pLabel = "(default)";
		} else {
			pLabel = " ";
		}
		item.MakeSlider("Keyframes Min", pLabel, m_KFMin, 1, 300, 25);
		item.SetTriggersUpdate(true);
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate kf min slider UI entry");
			return errFail;
		}
	}

	{
		if (m_BF > m_KFMax - 2) {
    		m_BF = std::max(0, m_KFMax - 2);
		}
		HostUIConfigEntryRef item("x264_bf");
		const char* pLabel = NULL;
		if (m_BF == 3) {
			pLabel = "(default)";
		} else {
			pLabel = " ";
		}
		item.MakeSlider("B-Frames", pLabel, m_BF, 0, 5, 3);
		item.SetTriggersUpdate(true);
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate bf slider UI entry");
			return errFail;
		}
	}

	{
		HostUIConfigEntryRef item("x264_scene_detection");
		const char* pLabel = NULL;
		if (m_SceneDetection == 40) {
			pLabel = "(default)";
		} else if (m_SceneDetection == 0) {
			pLabel = "(OFF)";
		} else {
			pLabel = " ";
		}
		item.MakeSlider("Scenecut Threshold", pLabel, m_SceneDetection, 0, 100, 40);
		item.SetTriggersUpdate(true);
		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate scene detection slider UI entry");
			return errFail;
		}
	}

	{
		HostUIConfigEntryRef item("x264_bitrate");
		item.MakeSlider("Bit Rate", "Kbps", m_BitRate, 100, 100000, 8000, 1);
		item.SetHidden((m_QualityMode != X264_RC_ABR) && (m_NumPasses < 2));

		if (!item.IsSuccess() || !p_pSettingsList->Append(&item)) {
			g_Log(logLevelError, "X264 Plugin :: Failed to populate bitrate slider UI entry");
			return errFail;
		}
	}

	return errNone;
}
