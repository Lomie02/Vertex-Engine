#pragma once
#include "irrKlang-64bit-1.6.0/include/irrKlang.h"
#include "VertexComponent.h"
#include <irrKlang.h>
#include <iostream >

using namespace irrklang;

/// <summary>
/// Allows for audio to be played.
/// </summary>
class AudioSource
{
public:
	AudioSource();
	~AudioSource();

	/// <summary>
	/// Set the audio file that you want to play.
	/// </summary>
	/// <param name="Name of the audio file."></param>
	void AudioClip(std::string _path) { m_SoundToPlayFilepath = _path; }
	/// <summary>
	/// Assigning the sound engine that was created in the application.cpp. This is assigned by the asset manager so there is no need to ever call this.
	/// </summary>
	/// <param name="Sound Engine from application.cpp"></param>
	/// <summary>
	/// Plays the audio linked with this audio source object
	/// </summary>
	void Play();
	/// <summary>
	/// Puases the audio source.
	/// </summary>
	void Pause();
	/// <summary>
	/// Resumes the audio source
	/// </summary>
	void Resume();
	/// <summary>
	/// Stops the audio all together.
	/// </summary>
	void Stop();
	/// <summary>
	/// Sets the current volume of the audio source.
	/// </summary>
	/// <param name="_volume"></param>
	void SetVolume(float _volume);
	/// <summary>
	/// Should the audio loop
	/// </summary>
	/// <param name="_state"></param>
	void SetLoop(bool _state) { m_IsLoop = _state; }
	/// <summary>
	/// Set if the audio source should loop.
	/// </summary>
	/// <param name="_state"></param>
	void PlayOnAwake(bool _state) { m_PlayOnAwake = _state; }
	/// <summary>
	/// Check if the audio source has finished playing.
	/// </summary>
	/// <returns> True if the audio finished. False if it didnt.</returns>
	bool HasFinishedPlaying() { return m_Sound->isFinished(); }

private:
	float m_VolumeMaster = 20;
	int m_MaxVolume = 100;
	int m_MinVolume = 0;

	bool m_UseEffects = false;
	bool m_PlayOnAwake = false;
	std::string m_SoundToPlayFilepath = "";
	bool m_IsLoop = false;
	ISoundEngine* m_SoundEngine;
	ISound* m_Sound;
};
