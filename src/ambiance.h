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
	bool playSound(std::string name, float gain);
	bool nodesInRange(v3s16 pos, short searchRange, short searchAbove, short searchBelow, std::string nodeName, short accuracy, int count);
	std::string getNodeName(v3s16 pos);
	int getNodeId(v3s16 pos);
	void stopEnvironment(int env);
	int readEnvironment();


	u32 m_timeOfDay;

	ISoundManager *m_sound;
	ClientEnvironment m_env;

	int m_background_sound;
	std::map<std::string, int> m_sounds_playing;
	typedef std::map<std::string, int>::iterator m_sounds_it;

	v3f lastpos;
	bool ascending;
	bool descending;
	bool moving;
	bool underwater;

	float startDelay;
	float envDelay;
	float tickDelay;

	int currentEnv = 0;
	int lastEnv;
	float lastPlay;
	int TOD;
	int lastTOD;

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

	const std::string DIRT = "default:dirt";//m_env.getGameDef()->getNodeDefManager()->getId("default:dirt");
	const std::string STONE = "default:stone";//m_env.getGameDef()->getNodeDefManager()->getId("default:stone");
	const std::string GRASS = "default:dirt_with_grass";//m_env.getGameDef()->getNodeDefManager()->getId("default:dirt_with_grass");
	const std::string TREE = "default:tree";//m_env.getGameDef()->getNodeDefManager()->getId("default:tree");
	const std::string LEAVES = "default:leaves";//m_env.getGameDef()->getNodeDefManager()->getId("default:leaves");
	const std::string WATER = "default:water_source";//m_env.getGameDef()->getNodeDefManager()->getId("default:water_source");
	const std::string WATER_FLOWING = "default:water_flowing";//m_env.getGameDef()->getNodeDefManager()->getId("default:water_flowing");
	const std::string SNOW = "default:snow";
	const std::string DIRT_SNOW = "default:dirt_with_snow";
	const std::string SAND = "default:sand";
	const std::string DESERT_STONE = "default:desert_stone";
	const std::string DESERT_SAND = "default:desert_sand";
	const std::string JUNGLE_TREE = "default:jungletree";
	const std::string JUNGLE_LEAVES = "default:jungleleaves";
	const std::string JUNGLE_GRASS = "default:junglegrass";

};

#endif /* SRC_AMBIANCE_H_ */
