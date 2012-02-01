#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Common.h"

namespace JustRenderIt
{

  enum MouseButton
  {
    MOUSE_LEFT = 0,
    MOUSE_MIDDLE,
    MOUSE_RIGHT,
    MOUSE_NONE,

    MOUSE_BUTTON_COUNT,
  };

  /** Camera base class. How to control:<br>
      <strong>RB</strong>: rotate,<br>
      <strong>MB</strong>: slide,<br>
      <strong>MB + RB</strong>: pull or push<br>
  **/
  class DLL_DECLARE Camera
  {
  public:
    Camera(void);
    ~Camera(void);

    void LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up = glm::vec3(0,1,0));
    void Rotate(float yaw, float pitch, float roll);
    void MoveTo(float x, float y, float z);
    void MoveBy(float x, float y, float z);
    virtual void Look() = 0;

    /// Callback function for mouse movement event.
    bool OnMouseMove(const int x, const int y);
    /// Callback function for mouse click event.
    bool OnMouseClick(const int x, const int y, const MouseButton button, const bool pressed);
    /// Should be called before every frame get rendered.
    void Update();
    /// Set how fast the camera rotate.
    void SetRotateSpeed(float speed){ m_rotateSpeed = speed; };
    /// Set how fast the camera move.
    void SetTranslateSpeed(float speed){ m_translateSpeed = speed; };
    /// Set camera to the default position.
    void Reset();

    glm::mat4 GetMatrixModelView();
    glm::mat4 GetMatrixProjection();
    glm::mat4 GetMatrixViewModelProjection();

    /// Get the position of the target.
    glm::vec3 GetCenter(){ return m_center; };
    /// Get the position of the camera.
    glm::vec3 GetEye(){ return m_eye; };
    /// Get the direction of up.
    glm::vec3 GetUp(){ return m_up; };

    glm::vec2 GetViewportSize(){ return m_viewportSize; };
    void SetViewportSize(glm::vec2 size){ m_viewportSize = size; };

    void SetClipping(float nnear, float ffar)
    {
      m_near = nnear;
      m_far = ffar;
    }

    void GetClipping(float& nnear, float& ffar)
    {
      nnear = m_near;
      ffar = m_far;
    }

    float GetFov(){ return m_fov; };
    void SetFov(float fov){ m_fov = fov; };
  protected:
    glm::vec2 m_viewportSize;
    float m_fov, m_near, m_far;

    glm::vec3 m_eye, m_center, m_up;
    glm::vec3 m_viewForward, m_viewUp, m_viewRight;
    glm::mat4 m_worldView, m_projection, m_worldViewProjection;

    bool    m_buttonPressed[MOUSE_BUTTON_COUNT];

    int m_crtX, m_crtY;
    int m_lastX, m_lastY; 

    float m_rotateSpeed, m_translateSpeed;
  protected:
    void UpdateViewDirections();
  };

  /// Specialized camera for OpenGL.
  class DLL_DECLARE OpenGLCamera: public Camera
  {
  public:
    virtual void Look();
  };

}

#endif
