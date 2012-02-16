#include "Common.h"
#include "Camera.h"
#include "App/AppBase.h"
#include "Utils/Logger.h"

using namespace JustRenderIt;

Camera::Camera(void)
{
  Reset();

  for(int i=0; i<MOUSE_BUTTON_COUNT; i++)
    m_buttonPressed[i] = false;

  m_crtX = m_crtY = m_lastX = m_lastY = -1;
  m_translateSpeed = 0.05f;
  m_rotateSpeed = 8.f;
  m_viewportSize = glm::vec2(512, 512);
  m_near = 0.1f;
  m_far = 1000;
  m_fov = 45.0f;
}

Camera::~Camera(void)
{

}

void Camera::UpdateViewDirections()
{
  m_viewForward = m_center - m_eye;
  m_viewForward = glm::normalize(m_viewForward);

  m_viewRight = glm::cross( m_up, m_viewForward );
  m_viewRight = glm::normalize(m_viewRight);

  m_viewUp = glm::cross( m_viewForward, m_viewRight );
  m_viewUp = glm::normalize(m_viewUp);
}

void Camera::Rotate(float _yaw, float _pitch, float _roll)
{
  float yaw = DEGREESTORADIANS(_yaw);
  float pitch = DEGREESTORADIANS(_pitch);
  //  float roll = DEGREESTORADIANS(_roll);

  glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), pitch, m_viewRight);
  glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), yaw, m_up);

  glm::mat4 r = rotateX * rotateY;

  glm::vec4 tmpViewDirection;
  tmpViewDirection = glm::vec4(m_viewForward, 1.0);
  tmpViewDirection = r * tmpViewDirection;
  m_viewForward = glm::normalize(glm::vec3(tmpViewDirection.xyz));

  m_center = m_eye + m_viewForward;
}

void Camera::LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
  m_eye = pos;
  m_center = target;
  m_up = up;
}

void Camera::MoveTo(float x, float y, float z)
{
  glm::vec3 dest = glm::vec3(x, y, z);
  glm::vec3 v = dest - m_eye;
  m_eye = dest;
  m_center += v;
}

void Camera::MoveBy(float x, float y, float z)
{
  glm::vec3 m_viewForward = m_eye - m_center;
  m_viewForward = glm::normalize(m_viewForward);
  glm::vec3 m_viewRight = glm::cross(m_up, m_viewForward );
  m_viewRight = glm::normalize(m_viewRight);

  glm::vec3 moveXYZ = m_viewForward * z + m_viewRight * x
    + glm::vec3( 0, y, 0 );
  m_eye += moveXYZ;
  m_center += moveXYZ;
  m_up += glm::vec3( 0, y, 0 );
}

bool Camera::OnMouseMove(const int x, const int y)
{
  m_crtX = x;
  m_crtY = y;
  return true;
}

bool Camera::OnMouseClick(const int x, const int y,
                          const MouseButton button, const bool pressed)
{
  m_crtX = x;
  m_crtY = y;
  m_buttonPressed[button] = pressed;
  if(pressed)
  {
    m_lastX = x;
    m_lastY = y;
  }

  Logger::InstanceRef().Write(LogLower,
    "Camera::OnMouseClick: (%d, %d) %s", x, y,
    pressed ? "pressed down" : "release");
  return true;
}

void Camera::Update()
{
  int dx = m_crtX - m_lastX;
  int dy = m_crtY - m_lastY;

  m_lastX = m_crtX;
  m_lastY = m_crtY;

  UpdateViewDirections();
  if(m_buttonPressed[MOUSE_MIDDLE] &&
    m_buttonPressed[MOUSE_RIGHT])
  {
    glm::vec3 dir = m_viewForward * m_translateSpeed * (float)dy;

    m_center += dir;
    m_eye += dir;

    Logger::InstanceRef().Write(LogLower,
      "New camera pos: (%f, %f, %f), center: (%f, %f, %f)",
      m_eye.x, m_eye.y, m_eye.z,
      m_center.x, m_center.y, m_center.z);
  }
  else if(m_buttonPressed[MOUSE_MIDDLE])
  {
    glm::vec3 dir1 = m_viewRight * m_translateSpeed * (float)dx;
    glm::vec3 dir2 = m_viewUp * m_translateSpeed * (float)dy;

    m_center -= dir1;
    m_center -= dir2;
    m_eye -= dir1;
    m_eye -= dir2;

    Logger::InstanceRef().Write(LogLower,
      "New camera pos: (%f, %f, %f), center: (%f, %f, %f)",
      m_eye.x, m_eye.y, m_eye.z,
      m_center.x, m_center.y, m_center.z);
  }
  else if(m_buttonPressed[MOUSE_RIGHT])
  {
    Rotate( -dx * m_rotateSpeed, dy * m_rotateSpeed, 0);

    Logger::InstanceRef().Write(LogLower,
      "New camera pos: (%f, %f, %f), center: (%f, %f, %f)",
      m_eye.x, m_eye.y, m_eye.z,
      m_center.x, m_center.y, m_center.z);
  }
}

const glm::vec3 defaultCameraEyePos = glm::vec3(5.f, 5.f, 5.0f);
const glm::vec3 defaultCameraCenter = glm::vec3(0.f, 0.f, 0.f);
const glm::vec3 defaultCameraUp = glm::vec3(0.f, 1.0f, 0.f);

void Camera::Reset()
{
  m_eye = defaultCameraEyePos;
  m_center = defaultCameraCenter;
  m_up = defaultCameraUp;
}

glm::mat4 Camera::GetMatrixProjection()
{
  m_projection = glm::perspective<float>(
    m_fov, m_viewportSize.x/m_viewportSize.y,
    m_near, m_far);
  return m_projection;
}

glm::mat4 Camera::GetMatrixModelView()
{
  m_worldView = glm::lookAt(m_eye, m_center, m_up);
  return m_worldView;
}

glm::mat4 JustRenderIt::Camera::GetMatrixViewModelProjection()
{
  return GetMatrixProjection() * GetMatrixModelView();
}
