#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../FX/LightSource.h"
#include <vector>

class Camera
{
public:
	Camera();
	~Camera();
	void SetPosition(glm::vec3 position_);
	void SetRotation(float yaw_, float pitch_);

	glm::mat4 GetOrthographic() const;
	glm::mat4 GetView() const;
	glm::mat4 GetPerspective() const;

	void AddLightSource(LightSource* light_);
	std::vector<LightSource*> GetLights();
	glm::vec3 GetPosition();

	void ProcessMouseMovement(glm::vec2 offset_);
	void ProcessMouseZoom(int y_);

	glm::vec2 GetClippingPlanes() const;

private:
	void UpdateCameraVector();
	void OnDestroy();
	glm::vec3 position;
	glm::mat4 perspective;
	glm::mat4 orthographic;
	float fieldOfView;
	float yaw, pitch;
	float nearPlane;
	float farPlane;
	glm::vec3 forward, up, right, worldUp;
	std::vector<LightSource*> lights;

};

#endif
