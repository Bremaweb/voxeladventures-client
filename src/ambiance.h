/*
 * ambience.h
 *
 *  Created on: Jun 13, 2015
 *      Author: brandon
 */
#ifndef SRC_AMBIANCE_H_
#define SRC_AMBIANCE_H_

#include "gamedef.h"
#include "environment.h"
#include "sound.h"
#include "settings.h"
#include "sound_openal.h"

class Ambiance
{
public:
	Ambiance(ISoundManager *sound, ClientEnvironment &e);
	void doAmbiance(float dtime);
private:
	bool playSound(std::string name, float gain);
	int nodesInRange(v3s16 pos, short searchRange, std::string nodeName);
	std::string getNodeName(v3s16 pos);
	void stopEnvironment(int env);

	ISoundManager *m_sound;
	ClientEnvironment m_env;

	std::map<std::string, int> m_sounds_playing;
	typedef std::map<std::string, int>::iterator m_sounds_it;

	v3f lastpos;
	bool ascending;
	bool descending;
	bool moving;
	bool underwater;

	float startDelay;
	float envDelay;

	int currentEnv = 0;
	int lastEnv;
	float lastPlay;

	struct env_sound {
		std::string name;
		float gain;
	};

	struct ambiance_environment {
		std::string name;
		int frequency;
		int sound_count;
		std::string on_start;
		std::string on_stop;
		float next_sound_delay;
		env_sound sounds[10];
		bool on_start_played;
		bool on_stop_played;
	};

	enum environment_ids {
		ENV_UNKNOWN,
		ENV_CAVE,
		ENV_UNDERWATER,
		ENV_INWATER,
		ENV_MORNING,
		ENV_DAY,
		ENV_DUSK,
		ENV_NIGHT
	};

	ambiance_environment a_env[8];

	int readEnvironment();
};

#endif /* SRC_AMBIANCE_H_ */
