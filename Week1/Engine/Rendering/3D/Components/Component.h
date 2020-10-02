
#ifndef COMPONENT_H
#define COMPONENT_H


class Component
{
public:
	virtual void OnCreate(class GameObject* obj_) = 0;
	virtual void Update(float deltaTime_) = 0;

protected:

	GameObject* ownerObj;
};

#endif // !COMPONENT_H