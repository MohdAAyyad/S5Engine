#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};
	virtual bool OnCreate() = 0;
	virtual void Update(const float deltaTime_) = 0;
	virtual void Render() = 0;
	virtual void Draw() = 0; //For GUI objects
	virtual void LateUpdate(const float deltaTime_) = 0;

};

#endif // !SCENE_H

