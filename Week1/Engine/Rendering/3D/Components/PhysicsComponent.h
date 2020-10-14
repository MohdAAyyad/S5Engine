#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>

class PhysicsComponent :public Component
{
public:
	PhysicsComponent();
	~PhysicsComponent();
	//void OnCreate(class GameObject* obj_) override;
	//void Update(float deltaTime_) override;
	void OnCreate(class GameObject* gameObject_) override;
	void SetupScenario(int mass_, glm::vec3 force_, glm::vec3 rotationalVelocity_, glm::vec3 rotationAxis_, float angle_, float radius_);
	void Update(float deltaTime_) override;

	glm::vec3 GetVelocity();
	//void OnDestroy() override;
private:
	glm::vec3 acceleration;
	glm::vec3 velocity;
	glm::vec3 rotationalVelocity;
	glm::vec3 rotationalAcceleration;
	glm::vec3 rotationalForce;
	glm::vec3 rotationAxis;
	int mass;
	glm::vec3 force;
	float radius;
	float angle;
};

#endif // !PHYSICSCOMPONENT_H

