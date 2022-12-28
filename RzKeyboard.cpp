#include "RzKeyboard.h"

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif

//----------------------------------------------------
// Razer Chroma interface functions

using namespace ChromaSDK;
using namespace std;

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEHEADSETEFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYPADEFFECT)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);

namespace RazerSDK
{
	INIT Init = nullptr;
	UNINIT UnInit = nullptr;
	CREATEEFFECT CreateEffect = nullptr;
	CREATEKEYBOARDEFFECT CreateKeyboardEffect = nullptr;
	CREATEMOUSEEFFECT CreateMouseEffect = nullptr;
	CREATEHEADSETEFFECT CreateHeadsetEffect = nullptr;
	CREATEMOUSEPADEFFECT CreateMousepadEffect = nullptr;
	CREATEKEYPADEFFECT CreateKeypadEffect = nullptr;
	SETEFFECT SetEffect = nullptr;
	DELETEEFFECT DeleteEffect = nullptr;
	QUERYDEVICE QueryDevice = nullptr;
};

//------------------------------------------------------------------


bool RazerKeyboard::isDeviceConnected(RZDEVICEID DeviceId)
{
	if (RazerSDK::QueryDevice != nullptr)
	{
		ChromaSDK::DEVICE_INFO_TYPE DeviceInfo = {};
		auto Result = RazerSDK::QueryDevice(DeviceId, DeviceInfo);

		return (DeviceInfo.Connected >= 1);
	}

	return false;
}


RazerKeyboard::RazerKeyboard() :
	m_RazerSDKModule(nullptr)
{
}

RazerKeyboard::~RazerKeyboard()
{

}

bool RazerKeyboard::Start()
{
	if (m_RazerSDKModule == nullptr)
	{
		// TODO: Why doesn't checking if signed work?  It is.  Why doesn't Windows trust it?
		m_RazerSDKModule = LoadLibraryEx(CHROMASDKDLL, nullptr, 0 /* LOAD_LIBRARY_REQUIRE_SIGNED_TARGET */ );
		if (m_RazerSDKModule == nullptr)
		{
			DWORD errorNumber = GetLastError();
			cout << "Unable to load DLL: " << std::hex << errorNumber << "\n";

			return false;
		}
	}

	if (RazerSDK::Init == nullptr)
	{
		auto Result = RZRESULT_INVALID;
		RazerSDK::Init = reinterpret_cast<INIT>(GetProcAddress(m_RazerSDKModule, "Init"));
		if (RazerSDK::Init != nullptr)
		{
			Result = RazerSDK::Init();
			if (Result == RZRESULT_SUCCESS)
			{
				RazerSDK::CreateEffect = reinterpret_cast<CREATEEFFECT>(GetProcAddress(m_RazerSDKModule, "CreateEffect"));
				RazerSDK::CreateKeyboardEffect = reinterpret_cast<CREATEKEYBOARDEFFECT>(GetProcAddress(m_RazerSDKModule, "CreateKeyboardEffect"));
				RazerSDK::CreateMouseEffect = reinterpret_cast<CREATEMOUSEEFFECT>(GetProcAddress(m_RazerSDKModule, "CreateMouseEffect"));
				RazerSDK::CreateHeadsetEffect = reinterpret_cast<CREATEHEADSETEFFECT>(GetProcAddress(m_RazerSDKModule, "CreateHeadsetEffect"));
				RazerSDK::CreateMousepadEffect = reinterpret_cast<CREATEMOUSEPADEFFECT>(GetProcAddress(m_RazerSDKModule, "CreateMousepadEffect"));
				RazerSDK::CreateKeypadEffect = reinterpret_cast<CREATEKEYPADEFFECT>(GetProcAddress(m_RazerSDKModule, "CreateKeypadEffect"));
				RazerSDK::SetEffect = reinterpret_cast<SETEFFECT>(GetProcAddress(m_RazerSDKModule, "SetEffect"));
				RazerSDK::DeleteEffect = reinterpret_cast<DELETEEFFECT>(GetProcAddress(m_RazerSDKModule, "DeleteEffect"));
				RazerSDK::QueryDevice = reinterpret_cast<QUERYDEVICE>(GetProcAddress(m_RazerSDKModule, "QueryDevice"));

				if ((RazerSDK::CreateEffect != nullptr) &&
					(RazerSDK::CreateKeyboardEffect != nullptr) &&
					(RazerSDK::CreateMouseEffect != nullptr) &&
					(RazerSDK::CreateHeadsetEffect != nullptr) &&
					(RazerSDK::CreateMousepadEffect != nullptr) &&
					(RazerSDK::CreateKeypadEffect != nullptr) &&
					(RazerSDK::SetEffect != nullptr) &&
					(RazerSDK::DeleteEffect != nullptr) &&
					(RazerSDK::QueryDevice != nullptr))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}

	// If get here, we have called this function more than once and it was successful before.
	return true;
}

void RazerKeyboard::ResetEffects(RazerDeviceType deviceType)
{
	switch (deviceType)
	{
	case RazerDeviceType::all_devices:
		if (RazerSDK::CreateKeyboardEffect != nullptr)
		{
			RazerSDK::CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, nullptr, nullptr);
		}

		if (RazerSDK::CreateMousepadEffect != nullptr)
		{
			RazerSDK::CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_NONE, nullptr, nullptr);
		}

		if (RazerSDK::CreateMouseEffect != nullptr)
		{
			RazerSDK::CreateMouseEffect(ChromaSDK::Mouse::CHROMA_NONE, nullptr, nullptr);
		}

		if (RazerSDK::CreateHeadsetEffect != nullptr)
		{
			RazerSDK::CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_NONE, nullptr, nullptr);
		}

		if (RazerSDK::CreateKeypadEffect != nullptr)
		{
			RazerSDK::CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_NONE, nullptr, nullptr);
		}
		break;

	case RazerDeviceType::keyboard:
		if (RazerSDK::CreateKeyboardEffect != nullptr)
		{
			RazerSDK::CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, nullptr, nullptr);
		}
		break;

	case RazerDeviceType::mousePad:
		if (RazerSDK::CreateMousepadEffect != nullptr)
		{
			RazerSDK::CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_NONE, nullptr, nullptr);
		}
		break;

	case RazerDeviceType::mouse:
		if (RazerSDK::CreateMouseEffect != nullptr)
		{
			RazerSDK::CreateMouseEffect(ChromaSDK::Mouse::CHROMA_NONE, nullptr, nullptr);
		}
		break;

	case RazerDeviceType::headset:
		if (RazerSDK::CreateHeadsetEffect != nullptr)
		{
			RazerSDK::CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_NONE, nullptr, nullptr);
		}
		break;

	case RazerDeviceType::keypad:
		if (RazerSDK::CreateKeypadEffect != nullptr)
		{
			RazerSDK::CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_NONE, nullptr, nullptr);
		}
		break;
	}
}

size_t currentRow = 0;
size_t currentCol = 0;
byte currentClr = 252;
bool countDn = true;

bool RazerKeyboard::keyboardUpdate() 
{
	// The keyboard effect is initialized as a 2 dimensional matrix/array
	// rows start from zero -----BUT----- cols start from ONE (1)
	// So Esc at top left is [0][1]
	// 
	// Source: http://developer.razerzone.com/chroma/razer-chroma-led-profiles/
	// Take the super keyboard as standard, so your programm will work with every keyboard out of the box 

	m_keyboardEffect.Color[currentRow][currentCol] = BLUE;

	currentCol++;
	if(currentCol >= ChromaSDK::Keyboard::MAX_COLUMN)
	{ 
		currentCol = 0;
		currentRow++;
		if (currentRow >= ChromaSDK::Keyboard::MAX_ROW)
			currentRow = 0;
	}

	COLORREF keyColor = RGB(currentClr, currentClr, currentClr);
	m_keyboardEffect.Color[2][10] = keyColor;
	if (countDn == true)
		currentClr-=3;
	else
		currentClr+=3;

	if ((countDn == true) && (currentClr == 0))
		countDn = false;
	else if ((countDn == false) && (currentClr == 252))
		countDn = true;

	return true;
}

bool RazerKeyboard::forceKeycolor(ChromaSDK::Keyboard::RZKEY key, COLORREF keyColor)
{
	Keyboard::CUSTOM_KEY_EFFECT_TYPE keyboardEffectKey = {};
	keyboardEffectKey.Key[HIBYTE(key)][LOBYTE(key)] = 0x01000000 | keyColor;

	if (RazerSDK::CreateKeyboardEffect != nullptr)
	{
		RZRESULT keyboardResult = RazerSDK::CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM_KEY, &keyboardEffectKey, nullptr);
		return keyboardResult == S_OK;
	}

	return false;
}

bool RazerKeyboard::keyboardRender()
{
	if (RazerSDK::CreateKeyboardEffect != nullptr)
	{
		RZRESULT keyboardResult = RazerSDK::CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &m_keyboardEffect, nullptr);
		return keyboardResult == S_OK;
	}

	return false;
}
