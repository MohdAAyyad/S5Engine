#include "AudioSource.h"
#include "AudioHandler.h"
#include "../Rendering/3D/GameObject.h"
#include "../../Engine/Core/Debugger.h"
#include "../../Engine/Rendering/3D/Components/PhysicsComponent.h"

AudioSource::AudioSource(std::string soundName_, bool loop_, bool is3D_, bool howToPlay_)
{
	channelID = -1;
	sound = soundName_;
	AudioHandler::GetInstance()->LoadSound(sound, loop_, is3D_, howToPlay_);
	pos = glm::vec3(0, 0, 0);
}

AudioSource::~AudioSource()
{
}

void AudioSource::OnCreate(GameObject * obj_)
{
	ownerObj = obj_;
}

void AudioSource::Update(float deltaTime_)
{
	if (ownerObj)
	{
		AudioHandler::GetInstance()->UpdateChannelPositionVelocity(channelID, ownerObj->GetPosition(), ownerObj->GetComponent<PhysicsComponent*>()->GetVelocity());

	}
}

void AudioSource::PlaySound(std::string soundName_, glm::vec3 position_, glm::vec3 vel_, float volume_)
{
	channelID = AudioHandler::GetInstance()->PlaySound(soundName_, position_, vel_, volume_);
}

bool AudioSource::IsPlaying()
{
	return AudioHandler::GetInstance()->IsPlaying(channelID);
}

