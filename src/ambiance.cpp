/*
 * ambience.cpp

 *
 *  Created on: Jun 13, 2015
 *      Author: brandon
 */

#include "ambiance.h"
#include "log.h"
#include "localplayer.h"
#include "map.h"
#include "clientmap.h"

Ambiance::Ambiance(ISoundManager *sound, ClientEnvironment &e):
	m_sound(sound),
	m_env(e)
{
	verbosestream << "[AMBIANCE] Loading sound files...";
	m_sound->loadSoundFile("bats","client/sounds/bats.ogg");
	m_sound->loadSoundFile("bird","client/sounds/bird.ogg");
	m_sound->loadSoundFile("bird2","client/sounds/bird2.ogg");
	m_sound->loadSoundFile("bird3","client/sounds/bird3.ogg");
	m_sound->loadSoundFile("bird4","client/sounds/bird4.ogg");
	m_sound->loadSoundFile("birdsong","client/sounds/birdsong.ogg");
	m_sound->loadSoundFile("coyote","client/sounds/coyote.ogg");
	m_sound->loadSoundFile("crickets","client/sounds/crickets.ogg");
	m_sound->loadSoundFile("crow","client/sounds/crow.ogg");
	m_sound->loadSoundFile("desert","client/sounds/desert.ogg");
	m_sound->loadSoundFile("drip_a","client/sounds/drippingwater_drip_a.ogg");
	m_sound->loadSoundFile("drip_b","client/sounds/drippingwater_drip_b.ogg");
	m_sound->loadSoundFile("drip_c","client/sounds/drippingwater_drip_c.ogg");
	m_sound->loadSoundFile("drip_d","client/sounds/drippingwater_drip_d.ogg");
	m_sound->loadSoundFile("drip_e","client/sounds/drippingwater_drip_e.ogg");
	m_sound->loadSoundFile("drowning_gasp","client/sounds/drowning_gasp.ogg");
	m_sound->loadSoundFile("lake_waves_calm","client/sounds/lake_waves_2_calm.ogg");
	m_sound->loadSoundFile("lake_waves_variety","client/sounds/lake_waves_2_variety.ogg");
	m_sound->loadSoundFile("owl","client/sounds/owl.ogg");
	m_sound->loadSoundFile("rattlesnake","client/sounds/rattlesnake.ogg");
	m_sound->loadSoundFile("rumble","client/sounds/rumble.ogg");
	m_sound->loadSoundFile("scuba1","client/sounds/scuba1bubbles.ogg");
	m_sound->loadSoundFile("scuba2","client/sounds/scuba1calm.ogg");
	m_sound->loadSoundFile("scuba3","client/sounds/scuba1calm2.ogg");
	m_sound->loadSoundFile("scuba4","client/sounds/scuba1interestingbubbles.ogg");
	m_sound->loadSoundFile("scuba5","client/sounds/scuba1tubulentbubbles.ogg");
	m_sound->loadSoundFile("splash","client/sounds/splash.ogg");
	m_sound->loadSoundFile("swimming1","client/sounds/water_swimming_splashing.ogg");
	m_sound->loadSoundFile("swimming2","client/sounds/water_swimming_splashing_breath.ogg");

verbosestream << "done" << std::endl;

verbosestream << "[AMBIANCE] Setting up environments...";
	a_env[ENV_UNKNOWN] = { "Unknown", 0, 0, "", "", 0 };

	// setup the various environment's sounds and frequencies
	a_env[ENV_CAVE] = { "Cave", 400, 6, "", "", 5.75 };
	a_env[ENV_CAVE].sounds[1] = {"bats",.4};
	a_env[ENV_CAVE].sounds[2] = {"drip_a",.8};
	a_env[ENV_CAVE].sounds[3] = {"drip_b",.8};
	a_env[ENV_CAVE].sounds[4] = {"drip_c",.6};
	a_env[ENV_CAVE].sounds[5] = {"drip_d",.4};
	a_env[ENV_CAVE].sounds[6] = {"drip_e",.5};

	a_env[ENV_UNDERWATER] = { "Underwater", 1001, 5, "splash", "drowning_gasp", 0 };
	a_env[ENV_UNDERWATER].sounds[1] = {"scuba1",1};
	a_env[ENV_UNDERWATER].sounds[2] = {"scuba2",1};
	a_env[ENV_UNDERWATER].sounds[3] = {"scuba3",1};
	a_env[ENV_UNDERWATER].sounds[4] = {"scuba4",1};
	a_env[ENV_UNDERWATER].sounds[5] = {"scuba5",1};

	a_env[ENV_INWATER] = { "Standing in Water", 1001, 2, "splash", "splash", 0 };
	a_env[ENV_INWATER].sounds[1] = {"swimming1",.6};
	a_env[ENV_INWATER].sounds[2] = {"swimming2",.6};

	a_env[ENV_MORNING] = { "Morning", 300, 3, "", "", 7 };
	a_env[ENV_MORNING].sounds[1] = {"bird2",.5};
	a_env[ENV_MORNING].sounds[2] = {"bird4",.5};
	a_env[ENV_MORNING].sounds[3] = {"bird3",.5};

	a_env[ENV_DAY] = { "Day", 400, 5, "", "", 6 };
	a_env[ENV_DAY].sounds[1] = {"bird2",.5};
	a_env[ENV_DAY].sounds[2] = {"bird4",.5};
	a_env[ENV_DAY].sounds[3] = {"bird3",.5};
	a_env[ENV_DAY].sounds[4] = {"bird",.5};
	a_env[ENV_DAY].sounds[5] = {"birdsong",.5};

	a_env[ENV_DUSK] = { "Dusk", 1001, 1, "", "", 0 };
	a_env[ENV_DUSK].sounds[1] = {"crickets",.5};

	a_env[ENV_NIGHT] = { "Night", 600, 1, "", "", 3 };
	a_env[ENV_NIGHT].sounds[1] = {"crickets",.7};

verbosestream << "done" << std::endl;
}
void Ambiance::doAmbiance(float dtime){
	static bool newEnv = false;
	if ( startDelay > 3 ) {	// delay ambiance 3 seconds from launch of game
		lastPlay += dtime;
		//verbosestream << "[AMBIANCE] doAmbiance()" << std::endl;
		int env_result = readEnvironment();
		if ( env_result != currentEnv ){
			lastEnv = currentEnv;
			currentEnv = env_result;
			stopEnvironment(lastEnv);	// change of enviroment stop all sounds that might be playing
			if ( !a_env[currentEnv].on_start.empty() ){
				playSound(a_env[currentEnv].on_start, 1);
			}
			newEnv = true;
			verbosestream << "[AMBIANCE] readEnvironment Results: " << a_env[currentEnv].name << std::endl;
		} else {
			newEnv = false;
		}

		if ( m_sounds_playing.rbegin() != m_sounds_playing.rend() ){
			if ( m_sound->soundExists(m_sounds_playing.rbegin()->second) ){
				// if the last sound isn't doesn't playing don't start another one
				//verbosestream << "[AMBIANCE] dont start a new sound" << std::endl;
				return;
			} else {
				m_sounds_it it = m_sounds_playing.find(m_sounds_playing.rbegin()->first);
				m_sounds_playing.erase(it);
			}
		}

		if ( newEnv == true || lastPlay > a_env[currentEnv].next_sound_delay ){
			srand (time(NULL));
			int freq = rand() % 1000;
			//verbosestream << "[AMBIANCE] Freq Random Number: " << to_string(freq) << " :: " << to_string(a_env[currentEnv].frequency) << std::endl;
			if ( freq <= a_env[currentEnv].frequency ){
				int s = ( rand() % a_env[currentEnv].sound_count ) + 1;
				verbosestream << "[AMBIANCE] Sound selected: " << to_string(s) << " " << a_env[currentEnv].sounds[s].name << std::endl;
				playSound(a_env[currentEnv].sounds[s].name, a_env[currentEnv].sounds[s].gain);
			}
		} else {
			//verbosestream << "[AMBIANCE] No Play" << to_string(lastPlay) <<std::endl;
		}
	} else {
		startDelay += dtime;
	}
}

// type is 0 - normal sound, 1 start sound, 2 stop sound
bool Ambiance::playSound(std::string name, float gain){
	verbosestream << "[AMBIANCE] playSound(" << name << ")" << std::endl;
	std::map<std::string, int>::iterator i = m_sounds_playing.find(name);
	if ( i != m_sounds_playing.end() ){
		// check the sound buffers to see if it's still playing
		if ( m_sound->soundExists(i->second) ){
			// get out of here so we don't play it again
			verbosestream << "[AMBIANCE] " << name << " is already playing" << std::endl;
			return false;
		} else {
			// remove it from our playing list
			m_sounds_playing.erase(name);
		}
	}
	int id = m_sound->playSound(name, false, gain);
	m_sounds_playing[name] = id;
	lastPlay = 0;
	return true;
}

int Ambiance::readEnvironment(){
	//verbosestream << "[AMBIANCE] readEnvironment() ";
	ascending = false;
	descending = false;
	moving = false;
	underwater = false;

	LocalPlayer *p = m_env.getLocalPlayer();
	v3f pos = p->getPosition();
	v3s16 spos = p->getStandingNodePos();

	// DAY AND NIGHT ARE KIND OF CATCH ALLS
	// 19000 night starts
	// 5000 morning starts
	u32 t = m_env.getTimeOfDay();
	//verbosestream << t << std::endl;

	if ( pos.Y > ( lastpos.Y + .5 ) )
		ascending = true;

	if ( pos.Y < ( lastpos.Y - .5 ) )
		descending = true;

	if ( pos.X != lastpos.X || pos.Z != lastpos.Z )
		moving = true;

	if ( moving == false && currentEnv == ENV_INWATER )	// for in water only play sounds if they are moving otherwise return unknown
		return ENV_UNKNOWN;

	// if they haven't moved much don't check the environment
	//if ( ascending == false && descending == false && moving == false ){
	//	return currentEnv;
	//}
	// don't update lastpos until they have moved
	lastpos = pos;

	//verbosestream << p->getName() << " " << to_string(spos.X) << "," << to_string(spos.Y) << "," << to_string(spos.Z) << std::endl;

	// find the nodes at their body positions
	std::string lowerBodyNodeName = getNodeName(v3s16(spos.X, (spos.Y+1), spos.Z));
	std::string upperBodyNodeName = getNodeName(v3s16(spos.X, (spos.Y+2), spos.Z));
	std::string standingOnNodeName = getNodeName(v3s16(spos.X, spos.Y, spos.Z));
	std::string aboveHeadNodeName = getNodeName(v3s16(spos.X, (spos.Y+3), spos.Z));

	if ( ( upperBodyNodeName == "default:water_source" || upperBodyNodeName == "default:water_flowing" ) ){
		underwater = true;
		return ENV_UNDERWATER;
	}

	if ( moving == true && ( lowerBodyNodeName == "default:water_source" || lowerBodyNodeName == "default:water_flowing" ) ){
		return ENV_INWATER;
	}

	if ( spos.Y < -15 && nodesInRange(spos, 5, "default:stone") > 8 ){
		// see if they are surrounded by stone
		return ENV_CAVE;
	}


	// time based catchalls
	if ( t > 5000 && t < 8000 )
		return ENV_MORNING;

	if ( t > 8000 && t < 18500 )
		return ENV_DAY;

	if ( t > 18500 && t < 19500 )
		return ENV_DUSK;

	if ( t > 19500 || t < 5000 )
		return ENV_NIGHT;

	return ENV_UNKNOWN;
}

int Ambiance::nodesInRange(v3s16 pos, short searchRange, std::string nodeName){
	//verbosestream << "[AMBIANCE] nodesInRange(pos, " << to_string(searchRange) << ", " << nodeName << ")" << std::endl;
	int nodeCount = 0;
	bool valid_pos;
	// set positions for node checks
	v3s16 p1 = v3s16((pos.X - searchRange), (pos.Y - searchRange), (pos.Z - searchRange));
	v3s16 p2 = v3s16((pos.X + searchRange), (pos.Y + searchRange), (pos.Z + searchRange));
	//verbosestream << "[AMBIANCE] Searching from " << to_string(p1.X) << ", " << to_string(p1.Y) << ", " << to_string(p1.Z) << " to " << to_string(p2.X) << ", " << to_string(p2.Y) << ", " << to_string(p2.Z) << std::endl;
	for ( int y = p1.Y; y < p2.Y; y++ ){
		for ( int x = p1.X; x < p2.X; x++ ){
			for ( int z = p1.Z; z < p2.Z; z++ ){
				//verbosestream << "[AMBIANCE] Position " << to_string(x) << ", " << to_string(y) << ", " << to_string(z) << std::endl;
				MapNode n = m_env.getMap().getNodeNoEx(v3s16(x,y,z), &valid_pos);
				ContentFeatures f = m_env.getGameDef()->getNodeDefManager()->get(n);
				//verbosestream << f.name << std::endl;
				if ( valid_pos == true ){
					if ( f.name == nodeName )
						nodeCount++;
				} else {
					verbosestream << "[AMBIANCE] Invalid position " << to_string(x) << ", " << to_string(y) << ", " << to_string(z) << std::endl;
				}
			}
		}
	}
	//verbosestream << "[AMBIANCE] Node Count: " << to_string(nodeCount) << std::endl;
	return nodeCount;
}

std::string Ambiance::getNodeName(v3s16 pos){
	MapNode n = m_env.getMap().getNodeNoEx(pos);
	ContentFeatures f = m_env.getGameDef()->getNodeDefManager()->get(n);
	return f.name;
}

void Ambiance::stopEnvironment(int env){
	// not going to bother seeing if the sounds playing are part of a certain environment just yet, just stop all sounds
	for ( m_sounds_it i = m_sounds_playing.begin(); i != m_sounds_playing.end(); i++ ){
		m_sound->stopSound(i->second);
	}
	if ( !a_env[env].on_stop.empty() ){
		playSound(a_env[env].on_stop,1);
	}
}
