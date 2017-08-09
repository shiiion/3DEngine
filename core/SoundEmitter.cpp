#include "SoundEmitter.h"
#include "AudioManager.h"
#include <iostream>
#include "IEntity.h"

namespace ginkgo
{
	void SoundEmitter::onTick(float elapsedTime) {}

	void SoundEmitter::onTickEnd(float elapsedTime)
	{
		for (int i = 0; i < playingSounds.size(); i++)
		{
			if (!AudioManager::getAudioManager()->updateSound(playingSounds[i], getParent()->getPosition(), getParent()->getVelocity(), getParent()->getRotation()))
			{
				AudioManager::getAudioManager()->eraseSound(playingSounds[i]);

				playingSounds.erase(playingSounds.begin() + i);
			}
		}
	}

	IEntity* SoundEmitter::getParent()
	{
		return parent;
	}

	SoundEmitter::SoundEmitter(IEntity* entity, bool isMusic)
	{
		parent = entity;

		music = isMusic;
	}
	
	SoundEmitter::~SoundEmitter()
	{
		for (int i = 0; i < playingSounds.size(); i++)
		{
			AudioManager::getAudioManager()->eraseSound(playingSounds[i]);
		}
	}

	void SoundEmitter::loadSound(const std::string name, const std::string filepath)
	{
		AudioManager::getAudioManager()->addBuffer(name, filepath);
	}

	void SoundEmitter::emitSound(const std::string name, float volume)
	{
		playingSounds.emplace_back(AudioManager::getAudioManager()->playSound(name, music, volume, getParent()->getPosition(), getParent()->getVelocity(), getParent()->getRotation()));
	}

	void SoundEmitter::stopSounds()
	{
		for (int i = 0; i < playingSounds.size(); i++)
		{
			AudioManager::getAudioManager()->eraseSound(playingSounds[i]);
		}
	}

	void SoundEmitter::pauseMusic()
	{
		AudioManager::getAudioManager()->pauseSound(playingSounds[0]);
	}

	void SoundEmitter::resumeMusic()
	{
		if (music)
			AudioManager::getAudioManager()->resumeSound(playingSounds[0]);
	}

	ISoundEmitter* createSoundEmitter(IEntity* entity, bool isMusic)
	{
		return new SoundEmitter(entity, isMusic);
	}
}