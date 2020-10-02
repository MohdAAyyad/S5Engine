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

void PhysicsComponent::OnCreate(GameObject* obj_)
{
	ownerObj = obj_;
	std::cout << "On Create has been called on physicsComponent" << std::endl;
}

void PhysicsComponent::Update(float deltaTime_)
{

}