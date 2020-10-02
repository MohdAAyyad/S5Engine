#include "Camera.h"
#include "../Core/CoreEngine.h" //Added here to avoid circular dependencies

Camera::Camera(): position(glm::vec3()), lights(std::vector<LightSource*>())
{

	fieldOfView = 45.0f;
	forward = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	worldUp = up;
	nearPlane = 2.0f; //Everything rendered between the near and far planes
	farPlane = 50.0f;
	yaw = -90.0f;
	pitch = 0.0f;
	
	lights.reserve(5);

	perspective = glm::perspective(fieldOfView, 
								   CoreEngine::GetInstance()->GetWindowSize().x/ CoreEngine::GetInstance()->GetWindowSize().y,
								   nearPlane,
								   farPlane);

	orthographic = glm::ortho(0.0f, CoreEngine::GetInstance()->GetWindowSize().x,
		           0.0f, CoreEngine::GetInstance()->GetWindowSize().y, -1.0f, 1.0f); //Check slides for pic reference. we're trying to create a perfect cube

	UpdateCameraVector();
}
Camera::~Camera()
{
	OnDestroy();
}

void Camera::OnDestroy()
{
	if (lights.size() > 0)
	{
		for (auto m : lights)
		{
			delete m;
			m = nullptr;
		}
	}
}
void Camera::SetPosition(glm::vec3 position_)
{
	position = position_;
	UpdateCameraVector();
}
void Camera::SetRotation(float yaw_, float pitch_)
{
	yaw = yaw_;
	pitch = pitch_;
	UpdateCameraVector();
}

glm::mat4 Camera::GetOrthographic() const
{
	//If you're gonna resize the window, make sure to recalcualte the orthographic first
	return orthographic;
}
glm::mat4 Camera::GetView() const
{
	return glm::lookAt(position, position + forward, up);
}
glm::mat4 Camera::GetPerspective() const
{
	//If you're gonna resize the window, make sure to recalcualte the perspective first
	return perspective;
}
void Camera::UpdateCameraVector()
{
	//Check slides for explanation but it's bascially looking at the forward vector from different persepctives to get is X and Z values. Y is just going along its axis, i.e. pitch
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward); //Always normalize your vectors

	right = glm::normalize(glm::cross(forward, worldUp));

	up = glm::normalize(glm::cross(right, forward));
}


void Camera::AddLightSource(LightSource* light_)
{
	lights.push_back(light_);
}
std::vector<LightSource*> Camera::GetLights()
{
	return lights;
}

glm::vec3 Camera::GetPosition()
{
	return position;
}

void Camera::ProcessMouseMovement(glm::vec2 offset_)
{
	offset_ *= 0.05f;

	yaw += offset_.x;
	pitch += offset_.y;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	if (yaw > 360.0f)
	{
		yaw -= 360.0f;
	}
	if (yaw < 0.0f)
	{
		yaw += 360.0f;
	}

	UpdateCameraVector();
}
void Camera::ProcessMouseZoom(int y_)
{
	if (y_ < 0 || y_ > 0)
	{
		position += static_cast<float>(y_) * (forward * 0.5f); //2.0f is the step size of the zoom //The way the zoom works is why we don't save a view matrix but rather return a new one everytime
		if (position.z < nearPlane + 2)
		{
			position -= static_cast<float>(y_) * (forward * 0.5f); //If we've reached the near or far plane, don't update the position at all
			position.z = nearPlane + 2;
		}
		if (position.z > farPlane - 2)
		{
			position -= static_cast<float>(y_) * (forward * 0.5f);
			position.z = farPlane - 2;
		}
	}

	UpdateCameraVector();

}

glm::vec2 Camera::GetClippingPlanes() const
{
	return glm::vec2(nearPlane, farPlane);
}