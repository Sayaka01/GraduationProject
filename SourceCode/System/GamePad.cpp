#include "Stdafx.h"

#pragma comment(lib, "Xinput.lib")

#include <windows.h>
#include <Xinput.h>
#include <math.h>
#include "GamePad.h"

// �X�V
void GamePad::Update(float elapsedTime)
{
	//�o�C�u���[�V����
	if (vibrationTimer > 0.0f)
	{
		vibrationTimer -= elapsedTime;
		if (vibrationTimer <= 0.0f)
		{
			vibrationTimer = 0.0f;
			XINPUT_VIBRATION vib;
			vib.wLeftMotorSpeed = static_cast<SHORT>(0.0f);
			vib.wRightMotorSpeed = static_cast<SHORT>(0.0f);
			XInputSetState(slot, &vib);
		}
	}

	axisLx = axisLy = 0.0f;
	axisRx = axisRy = 0.0f;
	triggerL = triggerR = 0.0f;

	GamePadButton newButtonState = 0;

	// �{�^�����擾
	XINPUT_STATE xinputState;
	if (XInputGetState(slot, &xinputState) == ERROR_SUCCESS)
	{
		//XINPUT_CAPABILITIES caps;
		//XInputGetCapabilities(m_slot, XINPUT_FLAG_GAMEPAD, &caps);
		XINPUT_GAMEPAD& pad = xinputState.Gamepad;

		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_UP)					newButtonState |= BTN_UP;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)				newButtonState |= BTN_RIGHT;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)				newButtonState |= BTN_DOWN;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)				newButtonState |= BTN_LEFT;
		if (pad.wButtons & XINPUT_GAMEPAD_A)						newButtonState |= BTN_A;
		if (pad.wButtons & XINPUT_GAMEPAD_B)						newButtonState |= BTN_B;
		if (pad.wButtons & XINPUT_GAMEPAD_X)						newButtonState |= BTN_X;
		if (pad.wButtons & XINPUT_GAMEPAD_Y)						newButtonState |= BTN_Y;
		if (pad.wButtons & XINPUT_GAMEPAD_START)					newButtonState |= BTN_START;
		if (pad.wButtons & XINPUT_GAMEPAD_BACK)						newButtonState |= BTN_BACK;
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)				newButtonState |= BTN_LEFT_THUMB;
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)				newButtonState |= BTN_RIGHT_THUMB;
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)			newButtonState |= BTN_LEFT_TRIGGER;
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)			newButtonState |= BTN_RIGHT_TRIGGER;
		if (pad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	newButtonState |= BTN_LEFT_SHOULDER;
		if (pad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	newButtonState |= BTN_RIGHT_SHOULDER;

		if ((pad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(pad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			pad.sThumbLX = 0;
			pad.sThumbLY = 0;
		}

		if ((pad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(pad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			pad.sThumbRX = 0;
			pad.sThumbRY = 0;
		}

		triggerL = static_cast<float>(pad.bLeftTrigger) / 255.0f;
		triggerR = static_cast<float>(pad.bRightTrigger) / 255.0f;
		axisLx = static_cast<float>(pad.sThumbLX) / static_cast<float>(0x8000);
		axisLy = static_cast<float>(pad.sThumbLY) / static_cast<float>(0x8000);
		axisRx = static_cast<float>(pad.sThumbRX) / static_cast<float>(0x8000);
		axisRy = static_cast<float>(pad.sThumbRY) / static_cast<float>(0x8000);
	}
	else
	{
#if 0
		// XInput�œ��͏�񂪎擾�o���Ȃ������ꍇ��WindowsAPI�Ŏ擾����
		JOYINFOEX joyInfo;
		joyInfo.dwSize = sizeof(JOYINFOEX);
		joyInfo.dwFlags = JOY_RETURNALL;	// �S�Ă̏����擾

		if (joyGetPosEx(slot, &joyInfo) == JOYERR_NOERROR)
		{
			// ���iID���`�F�b�N����PS4�R���g���[���[�����Ή�����
			static const WORD PS4_PID = 1476;

			JOYCAPS joy_caps;
			if (joyGetDevCaps(slot, &joy_caps, sizeof(JOYCAPS)) == JOYERR_NOERROR)
			{
				// �\���L�[
				if (joyInfo.dwPOV != 0xFFFF)
				{
					static const int povBit[8] =
					{
						BTN_UP,					// ��
						BTN_RIGHT | BTN_UP,		// �E��
						BTN_RIGHT,				// �E
						BTN_RIGHT | BTN_DOWN,	// �E��
						BTN_DOWN,				// ��
						BTN_LEFT | BTN_DOWN,	// ����
						BTN_LEFT,				// ��
						BTN_LEFT | BTN_UP		// ����
					};
					int angle = joyInfo.dwPOV / 4500;
					newButtonState |= povBit[angle];
				}
				if (joy_caps.wPid == PS4_PID)
				{
					// �{�^�����
					if (joyInfo.dwButtons & JOY_BUTTON1)  newButtonState |= BTN_Y;
					if (joyInfo.dwButtons & JOY_BUTTON2)  newButtonState |= BTN_B;
					if (joyInfo.dwButtons & JOY_BUTTON3)  newButtonState |= BTN_A;
					if (joyInfo.dwButtons & JOY_BUTTON4)  newButtonState |= BTN_X;
					if (joyInfo.dwButtons & JOY_BUTTON5)  newButtonState |= BTN_LEFT_TRIGGER;
					if (joyInfo.dwButtons & JOY_BUTTON6)  newButtonState |= BTN_RIGHT_TRIGGER;
					if (joyInfo.dwButtons & JOY_BUTTON7)  newButtonState |= BTN_LEFT_SHOULDER;
					if (joyInfo.dwButtons & JOY_BUTTON8)  newButtonState |= BTN_RIGHT_SHOULDER;
					if (joyInfo.dwButtons & JOY_BUTTON9)  newButtonState |= BTN_BACK;
					if (joyInfo.dwButtons & JOY_BUTTON10) newButtonState |= BTN_START;
					if (joyInfo.dwButtons & JOY_BUTTON11) newButtonState |= BTN_LEFT_THUMB;
					if (joyInfo.dwButtons & JOY_BUTTON12) newButtonState |= BTN_RIGHT_THUMB;
					//if (joyInfo.dwButtons & JOY_BUTTON13) newButtonState |= BTN_?;	// PS
					//if (joyInfo.dwButtons & JOY_BUTTON14) newButtonState |= BTN_?;	// Touch

					// ���X�e�B�b�N
					axisLx = static_cast<int>(joyInfo.dwXpos - 0x7FFF) / static_cast<float>(0x8000);
					axisLy = -static_cast<int>(joyInfo.dwYpos - 0x7FFF) / static_cast<float>(0x8000);

					// �E�X�e�B�b�N
					axisRx = static_cast<int>(joyInfo.dwZpos - 0x7FFF) / static_cast<float>(0x8000);
					axisRy = -static_cast<int>(joyInfo.dwRpos - 0x7FFF) / static_cast<float>(0x8000);

					// LR�g���K�[
					triggerL = static_cast<float>(joyInfo.dwVpos) / static_cast<float>(0xFFFF);
					triggerR = static_cast<float>(joyInfo.dwUpos) / static_cast<float>(0xFFFF);

					if (axisLx > -0.25f && axisLx < 0.25f) axisLx = 0.0f;
					if (axisRx > -0.25f && axisRx < 0.25f) axisRx = 0.0f;
				}
			}
		}
#endif
	}

	// �L�[�{�[�h�ŃG�~�����[�V����
	{
		float lx = 0.0f;
		float ly = 0.0f;
		float rx = 0.0f;
		float ry = 0.0f;
		if (GetAsyncKeyState('W') & 0x8000) ly = 1.0f;
		if (GetAsyncKeyState('A') & 0x8000) lx = -1.0f;
		if (GetAsyncKeyState('S') & 0x8000) ly = -1.0f;
		if (GetAsyncKeyState('D') & 0x8000) lx = 1.0f;

		if (GetAsyncKeyState('I') & 0x8000) ry = 1.0f;
		if (GetAsyncKeyState('J') & 0x8000) rx = -1.0f;
		if (GetAsyncKeyState('K') & 0x8000) ry = -1.0f;
		if (GetAsyncKeyState('L') & 0x8000) rx = 1.0f;

		if (GetAsyncKeyState('B') & 0x8000) newButtonState |= BTN_A;
		if (GetAsyncKeyState('C') & 0x8000) newButtonState |= BTN_B;
		if (GetAsyncKeyState('V') & 0x8000) newButtonState |= BTN_X;
		if (GetAsyncKeyState('N') & 0x8000) newButtonState |= BTN_Y;
		if (GetAsyncKeyState(VK_UP) & 0x8000)	newButtonState |= BTN_UP;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	newButtonState |= BTN_RIGHT;
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)	newButtonState |= BTN_DOWN;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)	newButtonState |= BTN_LEFT;
		if (GetAsyncKeyState('H') & 0x8000)	newButtonState |= BTN_RIGHT_TRIGGER;
		if (GetAsyncKeyState('G') & 0x8000)	newButtonState |= BTN_LEFT_TRIGGER;
		if (GetAsyncKeyState('Y') & 0x8000)	newButtonState |= BTN_RIGHT_SHOULDER;
		if (GetAsyncKeyState('T') & 0x8000)	newButtonState |= BTN_LEFT_SHOULDER;

#if 1
		if (newButtonState & BTN_UP)    ly = 1.0f;
		if (newButtonState & BTN_RIGHT) lx = 1.0f;
		if (newButtonState & BTN_DOWN)  ly = -1.0f;
		if (newButtonState & BTN_LEFT)  lx = -1.0f;
#endif

		if (lx >= 1.0f || lx <= -1.0f || ly >= 1.0f || ly <= -1.0)
		{
			float power = ::sqrtf(lx * lx + ly * ly);
			axisLx = lx / power;
			axisLy = ly / power;
		}

		if (rx >= 1.0f || rx <= -1.0f || ry >= 1.0f || ry <= -1.0)
		{
			float power = ::sqrtf(rx * rx + ry * ry);
			axisRx = rx / power;
			axisRy = ry / power;
		}
	}



	// �{�^�����̍X�V
	{
		buttonState[1] = buttonState[0];	// �X�C�b�`����
		buttonState[0] = newButtonState;

		buttonDown = ~buttonState[1] & newButtonState;	// �������u��
		buttonUp = ~newButtonState & buttonState[1];	// �������u��
	}
}

bool GamePad::SetVibration(float r, float l, float vibTimer)
{
	vibrationTimer = vibTimer;

	XINPUT_VIBRATION vib;
	constexpr float maxMotorSpeed = 65535.0f;
	vib.wLeftMotorSpeed = static_cast<WORD>(l * maxMotorSpeed);
	vib.wRightMotorSpeed = static_cast<WORD>(r * maxMotorSpeed);
	return XInputSetState(slot, &vib) == ERROR_SUCCESS;
}