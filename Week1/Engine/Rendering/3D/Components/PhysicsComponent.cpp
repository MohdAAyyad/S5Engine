#include "PhysicsComponent.h"
#include "../GameObject.h"
#include <iostream>

PhysicsComponent::PhysicsComponent()
{
	ownerObj = nullptr;
}
PhysicsComponent::~PhysicsComponent()
{
	ownerObj = nullptr;
}

void PhysicsComponent::OnCreate(GameObject* gameObject_)
{
	ownerObj = gameObject_;
	std::cout << "On Create has been called on physicsComponent" << std::endl;
}

void PhysicsComponent::SetupScenario(int mass_, glm::vec3 force_, glm::vec3 rotationalVelocity_, glm::vec3 rotationAxis_, float angle_, float radius_)
{// set up any variables needed for the test scenario
	mass = mass_;
	force = force_;
	rotationalVelocity = rotationalVelocity_;
	rotationAxis = rotationAxis_;
	angle = angle_;
	radius = radius_;
}

void PhysicsComponent::Update(float deltaTime_)
{
	if (ownerObj)
	{
		// linear motion
		// handle all acceleration here
		acceleration.x = force.x / mass;
		acceleration.y = force.y / mass;
		acceleration.z = force.z / mass;
		// update the velocity vector
		velocity += acceleration * deltaTime_;

		// this is the result that we want to have at the end
		glm::vec3 result;
		result = ownerObj->GetPosition() + (velocity * deltaTime_);
		ownerObj->SetPosition(result);

		//glm::vec3 rotationVec = q * conjugate * norm;
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 axisOfRoatation = glm::cross(up, rotationalVelocity);
		glm::vec3 rotationalMag = glm::normalize(rotationalVelocity) / radius;

		glm::vec3 angularVelocityVector = rotationalMag * glm::normalize(axisOfRoatation);
		glm::quat angularVelocityQuat(0, angularVelocityVector);

		//glm::cross(radius, rotationalVelocity) / (glm::length(radius) * glm::length(radius));
		glm::quat currentRot(0, ownerObj->GetRotation());

		glm::quat result2 = currentRot + (angularVelocityQuat * currentRot * (deltaTime_ / 2));
		glm::vec3 finalResult = result2 * ownerObj->GetRotation();
		//owner->GetRotation() + rotationalVelocity * q * (deltaTime_ / 2.0f);

		ownerObj->SetRotation(finalResult);
	}
}