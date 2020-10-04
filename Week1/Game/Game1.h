#ifndef GAME1_H

#define GAME1_H

#include "../Engine/Core/CoreEngine.h"
#include "Scenes/GameScene.h"
#include "Scenes/StartScene.h"

class Game1 : public GameInterface //Terminology: GameInterface is called a static instance while Game1 is called a dynamic instance
{
public:
	Game1();
	virtual ~Game1(); //When inheriting, a rule of thumb is to make the destructors virtual for the parents and the children
					  //This allows you to garbage collect both the parents and the children. 
					  //Making the a function virtual binds the children and their parents into a Virtual Table (V table) which is technically a lookup table.
					  //In the case of destructor, when the child's is called, the compiler knows to call the parent's as well
					  
	virtual void Update(const float deltaTime_); //You don't really need the override keyword here cause these are abstract functions
	virtual void Render();
	virtual void Draw();
	virtual bool OnCreate();

private:
	int currentSceneNum;
	Scene* currentScene;
	void BuildScene();


};

#endif
