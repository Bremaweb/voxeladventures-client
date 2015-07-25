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
#include "nodedef.h"

class Ambiance
{
public:
	Ambiance(ISoundManager *sound, ClientEnvironment &e);
	void doAmbiance(float dtime, u32 tod);
private:
	bool playSound(std::string name, float gain, bool fade = false);
	void doFades(float dtime);
	bool nodesInRange(v3s16 pos, short searchRange, short searchAbove, short searchBelow, std::string nodeName, short accuracy, int count);
	std::string getNodeName(v3s16 pos);
	int getNodeId(v3s16 pos);
	void stopEnvironment(int env);
	int readEnvironment();
	void fadeSound(int soundid, float step, float gain);


	u32 m_timeOfDay;

	ISoundManager *m_sound;
	ClientEnvironment m_env;

	int m_background_sound;
	std::map<std::string, int> m_sounds_playing;
	typedef std::map<std::string, int>::iterator m_sounds_it;

	struct fade_status {
		fade_status() {}
		fade_status(float step, float current_gain, float target_gain):
			step(step),
			current_gain(current_gain),
			target_gain(target_gain){}
		float step;
		float current_gain;
		float target_gain;
	};

	std::map<int, fade_status> m_sounds_fading;		// 0 = step, 1 = current gain, 2 = goal gain
	typedef std::map<int, fade_status>::iterator m_fading_it;

	v3f lastpos;
	bool ascending;
	bool descending;
	bool moving;
	bool underwater;

	float startDelay;
	float envDelay;
	float tickDelay;

	int currentEnv;
	int lastEnv;
	float lastPlay;
	int TOD;
	int lastTOD;

	struct env_sound {
		env_sound() {}
		env_sound(std::string name,
				float gain):
			name(name),
			gain(gain) {}
		std::string name;
		float gain;
	};

	struct ambiance_environment {
		ambiance_environment() {}
		ambiance_environment(std::string name,
				int frequency,
				int sound_count,
				std::string on_start,
				std::string on_stop,
				std::string background_sound,
				float next_sound_delay):
			name(name),
			frequency(frequency),
			sound_count(sound_count),
			on_start(on_start),
			on_stop(on_stop),
			background_sound(background_sound),
			next_sound_delay(next_sound_delay) {}
		std::string name;
		int frequency;
		int sound_count;
		std::string on_start;
		std::string on_stop;
		std::string background_sound;
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
		ENV_PLAINS,
		ENV_PLAINS_NIGHT,
		ENV_FOREST,
		ENV_FOREST_NIGHT,
		ENV_SNOW,
		ENV_SNOW_NIGHT,
		ENV_OCEAN,
		ENV_DESERT,
		ENV_DESERT_NIGHT,
		ENV_JUNGLE,
		ENV_JUNGLE_NIGHT
	};

	enum time_of_day {
		MORNING,
		DAY,
		DUSK,
		NIGHT
	};

	ambiance_environment a_env[15];

};

#endif /* SRC_AMBIANCE_H_ */
