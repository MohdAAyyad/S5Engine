#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H
#include "../Rendering/3D/Components/Component.h"
#include <string>
#include <fmod.hpp>
#include <glm/glm.hpp>

class AudioSource : public Component
{

private:
	std::string sound;
	int channelID;
	glm::vec3 pos;
public:
	AudioSource(std::string soundName_, bool loop_, bool is3D_, bool howToPlay_);
	~AudioSource();

	void OnCreate(class GameObject* obj_) override;
	void Update(float deltaTime_) override;
	void PlaySound(std::string soundName_, glm::vec3 position_, glm::vec3 vel_, float volume_);
	bool IsPlaying();


};
#endif

