

struct
player_camera
{
	glm::vec3 Position;
	glm::vec3 Velocity;
	glm::vec3 InputAxis;

	f32 Pitch{ 0 }; //up-down rotation
	f32 Yaw{ 0 }; //left-right rotation

	bool bSprint = false;
	bool bLocked;

	void
  ProcessInputEvent(u32 message, i64 params/* SDL_Event* ev*/)
  {
    if (message == WM_KEYDOWN)
    {
      switch (params)
      {
      case VK_UP:
        InputAxis.x += 1.f;
        break;
      case VK_DOWN:
        InputAxis.x -= 1.f;
        break;
      case VK_LEFT:
        InputAxis.y -= 1.f;
        break;
      case VK_RIGHT:
        InputAxis.y += 1.f;
        break;
      // case SDLK_q:
      //   inputAxis.z -= 1.f;
      //   break;

      // case SDLK_e:
      //   inputAxis.z += 1.f;
      //   break;
      case VK_LSHIFT:
        bSprint = true;
        break;
      }
    }
    else if (message == WM_KEYUP)
    {
      switch (params)
      {
      case VK_UP:
        InputAxis.x -= 1.f;
        break;
      case VK_DOWN:
        InputAxis.x += 1.f;
        break;
      case VK_LEFT:
        InputAxis.y += 1.f;
        break;
      case VK_RIGHT:
        InputAxis.y -= 1.f;
        break;
      // case SDLK_q:
      //   inputAxis.z += 1.f;
      //   break;

      // case SDLK_e:
      //   inputAxis.z -= 1.f;
      //   break;
      case VK_LSHIFT:
        bSprint = false;
        break;
      }
    }
    else if (message == WM_MOUSEMOVE)
    {
      if (!bLocked)
      {
        static int xPos = LOWORD(params);
        static int yPos = HIWORD(params);
        
        int xPosCurrent = LOWORD(params);
        int yPosCurrent = HIWORD(params);

        int xPosRelative = xPos - xPosCurrent;
        int yPosRelative = yPos - yPosCurrent;

        xPos = xPosCurrent;
        yPos = yPosCurrent;

        Pitch -= yPosRelative * 0.003f;
        Yaw -= xPosRelative * 0.003f;
      }
    }

    InputAxis = glm::clamp(InputAxis, {-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0});
  }

	void
  UpdateCamera(float deltaSeconds)
  {
    const float cam_vel = 0.001f + bSprint * 0.01f;
    glm::vec3 forward = {0, 0, cam_vel};
    glm::vec3 right = {cam_vel, 0, 0};
    glm::vec3 up = {0, cam_vel, 0};

    glm::mat4 cam_rot = GetRotationMatrix();

    forward = cam_rot * glm::vec4(forward, 0.f);
    right = cam_rot * glm::vec4(right, 0.f);

    Velocity = InputAxis.x * forward + InputAxis.y * right + InputAxis.z * up;

    Velocity *= 10 * deltaSeconds;

    Position += Velocity;
  }


	glm::mat4
  GetViewMatrix()
  {
    glm::vec3 camPos = Position;

    glm::mat4 cam_rot = (GetRotationMatrix());

    glm::mat4 view = glm::translate(glm::mat4{1}, camPos) * cam_rot;

    //we need to invert the camera matrix
    view = glm::inverse(view);

    return view;
  }

	glm::mat4
  GetProjectionMatrix(bool bReverse = true)
  {
    if (bReverse)
    {
      glm::mat4 pro = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 5000.0f, 0.1f);
      pro[1][1] *= -1;
      return pro;
    }
    else
    {
      glm::mat4 pro = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 5000.0f);
      pro[1][1] *= -1;
      return pro;
    }
  }

	glm::mat4
  GetRotationMatrix()
  {
    glm::mat4 yaw_rot = glm::rotate(glm::mat4{ 1 }, Yaw, { 0,-1,0 });
	  glm::mat4 pitch_rot = glm::rotate(glm::mat4{ yaw_rot }, Pitch, { -1,0,0 });
	  return pitch_rot;
  }
};