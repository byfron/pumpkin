#include "Camera.hpp"
#include "InputManager.hpp"
#include <common/bgfx_utils.h>
#include <common/entry/input.h>
#include <common/entry/cmd.h>
#include <common/common.h>
#include <limits>

#define FLT_MIN std::numeric_limits<float>::min()

uint8_t Camera::m_keys;

// static Camera* s_camera;

// Camera* getCamera() {
// 	return s_camera;
// }

// void createCamera() {
// 	s_camera = new Camera();
// }

// void destroyCamera() {
// 	delete s_camera;
// }

static void cmd(const void* _userData)
{
	cmdExec( (const char*)_userData);
}

static const InputBinding s_camBindings[] =
{
	{ entry::Key::KeyW,             entry::Modifier::None, 0, cmd, "move up"  },
	{ entry::Key::GamepadUp,        entry::Modifier::None, 0, cmd, "move up"  },
	{ entry::Key::KeyA,             entry::Modifier::None, 0, cmd, "move left"     },
	{ entry::Key::GamepadLeft,      entry::Modifier::None, 0, cmd, "move left"     },
	{ entry::Key::KeyS,             entry::Modifier::None, 0, cmd, "move down" },
	{ entry::Key::GamepadDown,      entry::Modifier::None, 0, cmd, "move down" },
	{ entry::Key::KeyD,             entry::Modifier::None, 0, cmd, "move right"    },
	{ entry::Key::GamepadRight,     entry::Modifier::None, 0, cmd, "move right"    },
	{ entry::Key::KeyQ,             entry::Modifier::None, 0, cmd, "rotate left"    },
	{ entry::Key::KeyE,             entry::Modifier::None, 0, cmd, "rotate right"    },

	INPUT_BINDING_END
};


int cmdRotate(CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
{
	if (_argc > 1)
	{
		if (0 == strcmp(_argv[1], "left")) {
			Camera::setKeyState(CAMERA_KEY_ROTATE_LEFT, true);
			return 0;
		}
		else if (0 == strcmp(_argv[1], "right")) {
			Camera::setKeyState(CAMERA_KEY_ROTATE_RIGHT, true);
			return 0;
		}
	}
}

int cmdMove(CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
{
	if (_argc > 1)
	{
		if (0 == strcmp(_argv[1], "left") )
		{
			Camera::setKeyState(CAMERA_KEY_LEFT, true);
			return 0;
		}
		else if (0 == strcmp(_argv[1], "right") )
		{
			Camera::setKeyState(CAMERA_KEY_RIGHT, true);
			return 0;
		}
		else if (0 == strcmp(_argv[1], "up") )
		{
			Camera::setKeyState(CAMERA_KEY_UP, true);
			return 0;
		}
		else if (0 == strcmp(_argv[1], "down") )
		{
			Camera::setKeyState(CAMERA_KEY_DOWN, true);
			return 0;
		}
	}

	return 1;
}

Camera::Camera()
{
}

Camera::~Camera() {
	inputRemoveBindings("camBindings");
}

void Camera::init() {
	
	reset();
	cmdAdd("move", cmdMove);
	cmdAdd("rotate", cmdRotate);

	inputAddBindings("camBindings", s_camBindings);

}

void Camera::mtxLookAt(float* _outViewMtx)
{
	//bx::mtxLookAt(_outViewMtx, m_pos.curr, m_target.curr);
	bx::mtxLookAt(_outViewMtx, m_eye.data(), m_at.data(), m_cam_up.data());
}

void Camera::orbit(float _dx, float _dy)
{
	m_orbit[0] += _dx;
	m_orbit[1] += _dy;
}

void Camera::dolly(float _dz)
{
	const float cnear = 0.01f;
	const float cfar  = 10.0f;

	const float toTarget[3] =
		{
			m_target.dest[0] - m_pos.dest[0],
			m_target.dest[1] - m_pos.dest[1],
			m_target.dest[2] - m_pos.dest[2],
		};
	const float toTargetLen = bx::vec3Length(toTarget);
	const float invToTargetLen = 1.0f/(toTargetLen+FLT_MIN);
	const float toTargetNorm[3] =
		{
			toTarget[0]*invToTargetLen,
			toTarget[1]*invToTargetLen,
			toTarget[2]*invToTargetLen,
		};

	float delta = toTargetLen*_dz;
	float newLen = toTargetLen + delta;
	if ( (cnear < newLen || _dz < 0.0f)
	     &&   (newLen < cfar  || _dz > 0.0f) )
	{
		m_pos.dest[0] += toTargetNorm[0]*delta;
		m_pos.dest[1] += toTargetNorm[1]*delta;
		m_pos.dest[2] += toTargetNorm[2]*delta;
	}
}


void Camera::reset()
{
	m_target.curr[0] = 0.0f;
	m_target.curr[1] = 0.0f;
	m_target.curr[2] = 0.0f;
	m_target.dest[0] = 2.0f;
	m_target.dest[1] = 2.0f;
	m_target.dest[2] = 0.0f;
	
	m_pos.curr[0] =  0.0f;
	m_pos.curr[1] =  0.0f;
	m_pos.curr[2] = -2.0f;
	m_pos.dest[0] =  1.5f;
	m_pos.dest[1] =  1.5f;
	m_pos.dest[2] = -5.0f;

	m_cam_forward_dir = Eigen::Vector3f(0.0, 1.0, 0.0);
	m_cam_right_dir = Eigen::Vector3f(-1.0, 0.0, 0.0);
	
	m_orbit[0] = 0.0f;
	m_orbit[1] = 0.0f;

	m_eye[0] =   0.0f;
	m_eye[1] =   0.0f;
	m_eye[2] = -4.0f;
	m_at[0]  =   1.5f;
	m_at[1]  =   1.5f;
	m_at[2]  =  -2.5f;
	m_cam_up[0]  =   0.0f;
	m_cam_up[1]  =   1.0f;
	m_cam_up[2]  =   0.0f;
	m_horizontalAngle = 0.01f;
	m_verticalAngle = 1.2f;
	m_gamepadSpeed = 0.04f;
	m_moveSpeed = 5.0f;
	m_rotateSpeed = 3.0f;
	m_keys = 0;

	m_dir_angle = 0.0f;

	// compute initial camera direction
	m_cam_direction(0) = m_cam_forward_dir(0);
	m_cam_direction(1) = m_cam_forward_dir(1) * cosf(m_verticalAngle) -
		m_cam_forward_dir(2)*sinf(m_verticalAngle);
	m_cam_direction(2) = m_cam_forward_dir(1) * sinf(m_verticalAngle) +
		m_cam_forward_dir(2)*cosf(m_verticalAngle);
}

void Camera::setKeyState(uint8_t _key, bool _down)
{
	m_keys &= ~_key;
	m_keys |= _down ? _key : 0;
}

void Camera::consumeOrbit(float _amount)
{
	float consume[2];
	consume[0] = m_orbit[0]*_amount;
	consume[1] = m_orbit[1]*_amount;
	m_orbit[0] -= consume[0];
	m_orbit[1] -= consume[1];

	const float toPos[3] =
		{
			m_pos.curr[0] - m_target.curr[0],
			m_pos.curr[1] - m_target.curr[1],
			m_pos.curr[2] - m_target.curr[2],
		};
	const float toPosLen = bx::vec3Length(toPos);
	const float invToPosLen = 1.0f/(toPosLen+FLT_MIN);
	const float toPosNorm[3] =
		{
			toPos[0]*invToPosLen,
			toPos[1]*invToPosLen,
			toPos[2]*invToPosLen,
		};

	float ll[2];
	latLongFromVec(ll[0], ll[1], toPosNorm);
	ll[0] += consume[0];
	ll[1] -= consume[1];
	ll[1] = bx::fclamp(ll[1], 0.02f, 0.98f);

	float tmp[3];
	vecFromLatLong(tmp, ll[0], ll[1]);

	float diff[3];
	diff[0] = (tmp[0]-toPosNorm[0])*toPosLen;
	diff[1] = (tmp[1]-toPosNorm[1])*toPosLen;
	diff[2] = (tmp[2]-toPosNorm[2])*toPosLen;

	m_pos.curr[0] += diff[0];
	m_pos.curr[1] += diff[1];
	m_pos.curr[2] += diff[2];
	m_pos.dest[0] += diff[0];
	m_pos.dest[1] += diff[1];
	m_pos.dest[2] += diff[2];
}

float Camera::getPitch() {

	Eigen::Vector3f a = m_cam_forward_dir/m_cam_forward_dir.norm();
	Eigen::Vector3f b = m_cam_direction/m_cam_direction.norm();
	
	return acosf(a.dot(b));
}

void Camera::update(float _dt)
{

	entry::GamepadHandle handle = { 0 };
	m_horizontalAngle += m_gamepadSpeed * inputGetGamepadAxis(handle, entry::GamepadAxis::RightX)/32768.0f;
	m_verticalAngle   -= m_gamepadSpeed * inputGetGamepadAxis(handle, entry::GamepadAxis::RightY)/32768.0f;
	const int32_t gpx = inputGetGamepadAxis(handle, entry::GamepadAxis::LeftX);
	const int32_t gpy = inputGetGamepadAxis(handle, entry::GamepadAxis::LeftY);
	m_keys |= gpx < -16834 ? CAMERA_KEY_LEFT     : 0;
	m_keys |= gpx >  16834 ? CAMERA_KEY_RIGHT    : 0;
	m_keys |= gpy < -16834 ? CAMERA_KEY_UP  : 0;
	m_keys |= gpy >  16834 ? CAMERA_KEY_DOWN : 0;
	
	
	if (m_keys & CAMERA_KEY_ROTATE_LEFT)
	{
		m_dir_angle = m_rotateSpeed * _dt;
		Eigen::Vector3f axis = Eigen::Vector3f(0.0, 0.0, 1.0);
			
		//rotate direction vector wrt the up vector
		Eigen::Matrix3f rot =  Eigen::AngleAxisf(m_dir_angle, axis).toRotationMatrix();
		
		m_cam_direction = rot * m_cam_direction;
		m_cam_forward_dir = rot * m_cam_forward_dir;
		m_cam_right_dir = rot * m_cam_right_dir;

		m_eye = m_at - m_cam_direction*5;
		
		setKeyState(CAMERA_KEY_ROTATE_LEFT, false);
	}
	
	if (m_keys & CAMERA_KEY_ROTATE_RIGHT)
	{

		m_dir_angle = -m_rotateSpeed * _dt;
		Eigen::Vector3f axis = Eigen::Vector3f(0.0, 0.0, 1.0);
			
		//rotate direction vector wrt the up vector
		Eigen::Matrix3f rot =  Eigen::AngleAxisf(m_dir_angle, axis).toRotationMatrix();
		
		m_cam_direction = rot * m_cam_direction;
		m_cam_forward_dir = rot * m_cam_forward_dir;
		m_cam_right_dir = rot * m_cam_right_dir;

		m_eye = m_at - m_cam_direction*5;

		setKeyState(CAMERA_KEY_ROTATE_RIGHT, false);		
	}

	if (m_keys & CAMERA_KEY_LEFT)
	{
		m_eye += m_cam_right_dir*m_moveSpeed*_dt;
		setKeyState(CAMERA_KEY_LEFT, false);
	}

	if (m_keys & CAMERA_KEY_RIGHT)
	{       
		m_eye -= m_cam_right_dir*m_moveSpeed*_dt;		
		setKeyState(CAMERA_KEY_RIGHT, false);
	}

	if (m_keys & CAMERA_KEY_UP)
	{
		m_eye += m_cam_forward_dir * m_moveSpeed * _dt;		
		setKeyState(CAMERA_KEY_UP, false);
	}

	if (m_keys & CAMERA_KEY_DOWN)
	{
		m_eye -= m_cam_forward_dir * m_moveSpeed * _dt;		
		setKeyState(CAMERA_KEY_DOWN, false);
	}

	m_at = m_eye + m_cam_direction*5;
	m_cam_up = m_cam_right_dir.cross(m_cam_direction);	
}

void Camera::vecFromLatLong(float _vec[3], float _u, float _v)
{
	const float phi   = _u * 2.0f*bx::pi;
	const float theta = _v * bx::pi;

	const float st = bx::fsin(theta);
	const float sp = bx::fsin(phi);
	const float ct = bx::fcos(theta);
	const float cp = bx::fcos(phi);

	_vec[0] = -st*sp;
	_vec[1] = ct;
	_vec[2] = -st*cp;
}

void Camera::latLongFromVec(float& _u, float& _v, const float _vec[3])
{
	const float phi = atan2f(_vec[0], _vec[2]);
	const float theta = acosf(_vec[1]);

	_u = (bx::pi + phi)*bx::invPi*0.5f;
	_v = theta*bx::invPi;
}
