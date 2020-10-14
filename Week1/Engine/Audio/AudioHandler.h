#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H
#include <string>
#include <map>
#include <memory>
#include <fmod.hpp>
#include <glm/glm.hpp>
#include "../Core/Debugger.h"
#include "AudioSource.h"

class AudioHandler
{
private:
	static std::unique_ptr<AudioHandler> audioHandler;
	friend std::default_delete<AudioHandler>;
	FMOD::System* audioSystem;
	static std::map<std::string, FMOD::Sound*> sounds;
	static std::map<int, FMOD::Channel*> chanels;

	int chanelAmount;

	FMOD_VECTOR VecToFMODVec(glm::vec3 input_);

	FMOD::Sound* GetSound(std::string soundName_);
	void LoadSound(std::string soundName_, bool loop_, bool is3D_, bool howToPlay_);
	int PlaySound(std::string soundName_, glm::vec3 position_, glm::vec3 vel_, float volume);
	void UpdateChannelPositionVelocity(int channelID_, glm::vec3 pos_, glm::vec3 vel_);
	bool IsPlaying(int channelID_);

	// set the audio source class
	friend class AudioSource;

public:
	AudioHandler();
	~AudioHandler();
	AudioHandler(const AudioHandler&) = delete; //Disable the copy constructor
	AudioHandler(AudioHandler&&) = delete; //Disable the move constructor
	AudioHandler& operator=(const AudioHandler&) = delete; //Disable operator = for copy and move
	AudioHandler& operator=(AudioHandler&&) = delete;


	static AudioHandler* GetInstance();

	bool Initialise(glm::vec3 position_, glm::vec3 vel_, glm::vec3 forward_, glm::vec3 up_);
	void Update(float deltaTime_);


	void OnDestroy();


};
#endif

