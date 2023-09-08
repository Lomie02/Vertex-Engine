#include "AudioSource.h"

AudioSource::AudioSource()
{
	m_SoundEngine = createIrrKlangDevice();
	m_IsLoop = false;
	m_PlayOnAwake = false;
}

AudioSource::~AudioSource()
{
	m_Sound->drop();
}

void AudioSource::Play()
{
	m_Sound = m_SoundEngine->play2D(m_SoundToPlayFilepath.c_str(), m_IsLoop, m_PlayOnAwake);
}

void AudioSource::Pause()
{
	m_Sound->setIsPaused(true);
}

void AudioSource::Resume()
{
	m_Sound->setIsPaused(false);
}

void AudioSource::Stop()
{
	m_Sound->stop();
}

void AudioSource::SetVolume(float _volume)
{
	m_Sound->setVolume(_volume);
}

