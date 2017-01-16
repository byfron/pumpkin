#ifndef INPUTMANAGER_H_HEADER_GUARD
#define INPUTMANAGER_H_HEADER_GUARD

#include <common/common.h>

struct Mouse
{
	Mouse()
	{
		m_dx = 0.0f;
		m_dy = 0.0f;
		m_prevMx = 0.0f;
		m_prevMx = 0.0f;
		m_scroll = 0;
		m_scrollPrev = 0;
	}

	void update(float _mx, float _my, int32_t _mz, uint32_t _width, uint32_t _height)
	{
		const float widthf  = float(int32_t(_width));
		const float heightf = float(int32_t(_height));

		// Delta movement.
		m_dx = float(_mx - m_prevMx)/widthf;
		m_dy = float(_my - m_prevMy)/heightf;

		m_prevMx = _mx;
		m_prevMy = _my;

		// Scroll.
		m_scroll = _mz - m_scrollPrev;
		m_scrollPrev = _mz;
	}

	float m_dx; // Screen space.
	float m_dy;
	float m_prevMx;
	float m_prevMy;
	int32_t m_scroll;
	int32_t m_scrollPrev;
};


class InputManager {

public:
	static bool processEvents(uint32_t & width, uint32_t & height) {
		m_mouse.update(float(m_mouseState.m_mx), float(m_mouseState.m_my), m_mouseState.m_mz, width, height);
		return entry::processEvents(width, height, m_debug, m_reset, &m_mouseState);
	}
	
	static uint32_t m_debug;
	static uint32_t m_reset;
	static entry::MouseState m_mouseState;
	static Mouse m_mouse;
};

#endif
