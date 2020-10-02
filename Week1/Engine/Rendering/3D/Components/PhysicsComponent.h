#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Component.h"

class PhysicsComponent :public Component
{
public:
	PhysicsComponent();
	~PhysicsComponent();
	void OnCreate(class GameObject* obj_) override;
	void Update(float deltaTime_) override;
};

#endif // !PHYSICSCOMPONENT_H

