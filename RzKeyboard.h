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

	// You can use this to grab the current array set on the keyboard
	// and modify it.
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE& getKeyboardEffect() 
	{
		return m_keyboardEffect;
	};

	// This function clears the array below to the passed in COLORREF value
	// By default, this will just clear the keyboard to blank
	bool keyboardClear(COLORREF keyColor = RGB(0,0,0));

	// This will set the m_keyboardEffect to the keyboardEffect passed in
	bool keyboardUpdate(const ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE& keyboardEffect);

	// Render actually sets the data onto the devices.
	// -----------------------------------------------------------------
	// NOTE!!!!
	// Treating a Razer device as something that can be rendered to
	// at some arbtrary framerate will almost certainly not work.
	//
	// It is best to render _ONLY_ if you need to change keyboard state.
	// 
	// For animations, you can use the Razer SDK functionality for this.
	// (This is not _yet_ implemented here)
	// 
	// OR create an animation by setting key values and rendering "realtime"
	// Again, note that "realtime" is probably around 20 fps to a razer device!
	// 
	// You have been warned.
	//--------------------------------------------------------------------------
	bool keyboardRender();

	// This is an "immediate mode" key color set.
	// ONLY call this after Render or Render will overwrite any changes
	// that you make here!  And the next render call will overwrite these too!
	// "Permanent Changes" are stored in the m_keyboardEffect below
	bool forceKeycolor(ChromaSDK::Keyboard::RZKEY key, COLORREF keyColor);

private:
	HMODULE m_RazerSDKModule;
	
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE m_keyboardEffect = {};
};

#endif
