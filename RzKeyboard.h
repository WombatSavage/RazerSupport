#pragma once
#include "RzSDK.h"

#ifndef _CHROMASDKIMPL_H_
#define _CHROMASDKIMPL_H_

#pragma once

#ifndef DLL_COMPILED
#define DLL_INTERNAL __declspec( dllexport )
#endif 

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

// Define all Colours you want
const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF PURPLE = RGB(128, 0, 128);
const COLORREF CYAN = RGB(00, 255, 255);
const COLORREF ORANGE = RGB(255, 165, 00);
const COLORREF PINK = RGB(255, 192, 203);
const COLORREF GREY = RGB(125, 125, 125);
//You dont have to define your colors as COLORREFs, just use the RGB(xxx,xxx,xxx) function like above

enum RazerDeviceType
{
	all_devices = 0,
	keyboard = 1,
	mousePad,
	mouse,
	headset,
	keypad
};

//Class of your Chroma Implementation
class RazerKeyboard
{

public:
	RazerKeyboard();
	~RazerKeyboard();
	bool Start();

	bool isDeviceConnected(RZDEVICEID DeviceId);
	void ResetEffects(RazerDeviceType deviceType);

	// Update creates the data to send down to the keyboard, etc.
	bool keyboardUpdate();

	// Render actually sets the data onto the devices.
	bool keyboardRender();

	// This is an "immediate mode" key color set.
	// ONLY call this after Render or Render will overwrite any changes
	// that you make here!  And the next render call will overwrite these too!
	bool forceKeycolor(ChromaSDK::Keyboard::RZKEY key, COLORREF keyColor);

private:
	HMODULE m_RazerSDKModule;
	
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE m_keyboardEffect = {};
};

#endif
