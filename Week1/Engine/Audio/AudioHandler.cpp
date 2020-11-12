#include "AudioHandler.h"

// redefintion of our instance variable
std::unique_ptr<AudioHandler> AudioHandler::audioHandler = nullptr;
std::map<std::string, FMOD::Sound*> AudioHandler::sounds = std::map<std::string, FMOD::Sound*>();
std::map<int, FMOD::Channel*> AudioHandler::chanels = std::map<int, FMOD::Channel*>();


FMOD_VECTOR AudioHandler::VecToFMODVec(glm::vec3 input_)
{
	FMOD_VECTOR result;

	result.x = input_.x;
	result.y = input_.y;
	result.z = input_.z;

	return result;
}

FMOD::Sound* AudioHandler::GetSound(std::string soundName_)
{
	// loop through and look for sounds 
	if (sounds.find(soundName_) != sounds.end())
	{
		return sounds[soundName_];
	}

	return nullptr;
}

void AudioHandler::LoadSound(std::string soundName_, bool loop_, bool is3D_, bool howToPlay_)
{
	// first step check to see if the sound you're trying to play hasn't been loaded already

	if (GetSound(soundName_) == nullptr)
	{
		//sound->setMode();
		FMOD_MODE mode = FMOD_DEFAULT;
		if (loop_)
		{
			mode = mode | FMOD_LOOP_NORMAL;
		}
		else
		{
			mode = mode | FMOD_LOOP_OFF;
		}

		if (is3D_)
		{
			mode = mode | FMOD_3D;
		}
		else
		{
			mode = mode | FMOD_2D;
		}

		if (howToPlay_)
		{
			mode = mode | FMOD_CREATESTREAM;
		}
		else
		{
			mode = mode | FMOD_CREATESAMPLE;
		}
		//| loop_ | is3D_ | howToPlay_;
		
		FMOD::Sound* sound = nullptr;
		if (audioSystem)
		{
			if (audioSystem->createSound(soundName_.c_str(), mode, nullptr, &sound) == FMOD_OK)
			{
				// insert the two values into the map, not sure why the curly braces are needed
				sounds.insert({ soundName_, sound });
			}
		}
	}

}

int AudioHandler::PlaySound(std::string soundName_, glm::vec3 position_, glm::vec3 vel_ = glm::vec3(0,0,0), float volume = 1.0f)
{
	int chanelId = -1;
	if (GetSound(soundName_) == nullptr)
	{
		// i don't know what to laod for the last one
		LoadSound(soundName_, true, true, true);
	}

	FMOD::Channel* chanel = nullptr;
	if (audioSystem)
	{
		// set up the attributes
		audioSystem->playSound(GetSound(soundName_), nullptr, true, &chanel);
		chanel->set3DAttributes(&VecToFMODVec(position_), &VecToFMODVec(vel_));
		chanel->setVolume(volume);
		// play the sound
		chanel->setPaused(false);
		chanelId = chanelAmount;
		chanelAmount++;
		chanels.insert({ chanelId, chanel });
	}

	return chanelId;
}

void AudioHandler::UpdateChannelPositionVelocity(int channelID_, glm::vec3 pos_ = glm::vec3(0,0,0), glm::vec3 vel_ = glm::vec3(0,0,0))
{
	if (chanels.find(channelID_) != chanels.end())
	{
		chanels[channelID_]->set3DAttributes(&VecToFMODVec(pos_), &VecToFMODVec(vel_));
	}
}

bool AudioHandler::IsPlaying(int channelID_)
{
	bool result;
	if (chanels.find(channelID_) != chanels.end())
	{

		result = chanels[channelID_]->isPlaying(&result);
	}

	return result;
}

AudioHandler::AudioHandler()
{
	chanelAmount = 0;
}

AudioHandler::~AudioHandler()
{
	OnDestroy();
}

AudioHandler* AudioHandler::GetInstance()
{
	if (audioHandler.get() == nullptr)
	{
		audioHandler.reset(new AudioHandler);
	}

	return audioHandler.get();
}

bool AudioHandler::Initialise(glm::vec3 position_ = glm::vec3(0,0,0), glm::vec3 vel_ = glm::vec3(0, 0, 0), glm::vec3 forward_ = glm::vec3(0, 0, -1), glm::vec3 up_ = glm::vec3(0, 1, 0))
{
	FMOD::System_Create(&audioSystem);
	int drivers = 0;
	audioSystem->getNumDrivers(&drivers);
	if (drivers == 0)
	{
		// if there were no drivers found exit and go home
		return false;
	}
	//set up the audio system
	if (audioSystem->init(10, FMOD_INIT_NORMAL | FMOD_3D | FMOD_INIT_3D_RIGHTHANDED, nullptr) != FMOD_OK)
	{
		return false;
	}
	if (audioSystem->set3DListenerAttributes(0, &VecToFMODVec(position_), &VecToFMODVec(vel_), 
											 &VecToFMODVec(forward_), &VecToFMODVec(up_)) != FMOD_OK)
	{
		return false;
	}

	// if we got here than everything initialised successfully
	return true;
}

void AudioHandler::Update(float deltaTime_)
{
	if (audioSystem)
	{
		audioSystem->update();
	}
}

void AudioHandler::OnDestroy()
{
	if (sounds.size() > 0)
	{
		for (auto a : sounds)
		{
			// loop through all the pointers in the map and delete the sounds
			a.second->release();
		}
		// clear the rest of the map
		sounds.clear();
	}

	if (chanels.size() > 0)
	{
		for (auto c : chanels)
		{
			c.second->stop();
		}
		chanels.clear();
	}

	// finally get rid of the system
	audioSystem->release();
	audioSystem = nullptr;
}
