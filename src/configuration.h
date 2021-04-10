#ifndef ZOMBIE_GAMEDATA_H
#define ZOMBIE_GAMEDATA_H

//#include "animation.h"
//#include "weapons/explosion.h"
#include "physics/moving/unitproperties.h"
#include "physics/weapons/weaponproperties.h"
#include "physics/weapons/missileproperties.h"
#include "physics/Stationary/buildingproperties.h"

#include <sdl/sound.h>
#include <sdl/sprite.h>
#include <sdl/color.h>
#include <sdl/font.h>
#include <sdl/music.h>
#include <sdl/textureatlas.h>

#include <nlohmann/json.hpp>

#include <map>
#include <vector>

namespace zombie {

	class Configuration {
	public:
		static Configuration& getInstance() {
			static Configuration instance;
			return instance;
		}

		Configuration(const Configuration&) = delete;
		Configuration& operator=(const Configuration&) = delete;

		void save();

		const sdl::Font& loadFont(std::string file, int fontSize);
		sdl::Sound loadSound(std::string file);
		sdl::Music loadMusic(std::string file);
		sdl::TextureView loadSprite(std::string file);

		const sdl::Font& getDefaultFont(int size);

		void bindTextureFromAtlas();

		int getWindowPositionX();
		int getWindowPositionY();

		void setWindowPositionX(int x);
		void setWindowPositionY(int y);

		int getWindowWidth();
		int getWindowHeight();

		void setWindowWidth(int width);
		void setWindowHeight(int height);

		bool isWindowResizable();
		void setWindowResizable(bool resizeable);

		int getWindowMinWidth();
		int getWindowMinHeight();
		std::string getWindowIcon();
		bool isWindowBordered();
		void setWindowBordered(bool border);

		bool isWindowMaximized();
		void setWindowMaximized(bool maximized);

		bool isWindowVsync();
		void setWindowVsync(bool activate);

		bool isMusicOn();
		void setMusicOn(bool activate);

		float getMusicVolume();
		void setMusicVolume(float volume);

		sdl::Music getMusicTrack();

		sdl::TextureView getTreeImage();

		sdl::TextureView getBuildingWallImage();

		float getSettingsImpulseThreshold();
		float getSettingsTimeStep();

		float getSettingsInnerSpawnRadius();
		float getSettingsOuterSpawnRadius();

		int getSettingsUnitLevel();
		int getSettingsUnitLimit();

		std::string getSettingsMap();

		sdl::Sound getMenuSoundChoice();
		sdl::Sound getMenuSoundHighlited();
		sdl::TextureView getMenuBackgroundImage();

		sdl::TextureView getWaterSeeFloorImage();

		sdl::TextureView getRoadIntersection();
		sdl::TextureView getRoadStraight0();
		sdl::TextureView getRoadStraight90();
		sdl::TextureView getRoadTurn0();
		sdl::TextureView getRoadTurn90();
		sdl::TextureView getRoadTurn180();
		sdl::TextureView getRoadTurn270();
		sdl::TextureView getRoadTurnIntersection0();
		sdl::TextureView getRoadTurnIntersection90();
		sdl::TextureView getRoadTurnIntersection180();
		sdl::TextureView getRoadTurntersection270();

		//ExplosionProperties getExplosionProperties();

		UnitProperties getHumanProperties();
		UnitProperties getZombieProperties();

		MissileProperties getMissileProperties();

		MapProperties loadMapProperties();

	private:
		Configuration();
		void loadAllWeaponProperties();
		void loadAllMissileProperties();
		void loadAllUnitProperties();

		//Animation loadAnimation(const nlohmann::json& animationTag);

		UnitProperties loadUnitProperties(const nlohmann::json& unitTag);
		
		WeaponProperties loadWeaponProperties(const nlohmann::json& unitTag);
		WeaponProperties loadWeaponProperties(std::string weaponName);
		
		MissileProperties loadMissileProperties(const nlohmann::json& unitTag);
		MissileProperties loadMissileProperties(std::string weaponName);
		
		sdl::TextureAtlas textureAtlas_;
		std::map<std::string, sdl::Sound> sounds_;
		std::map<std::string, sdl::Font> fonts_;
		std::map<std::string, sdl::Music> musics_;
		std::map<std::string, WeaponProperties> weaponPropertiesMap_;
		std::map<std::string, MissileProperties> missilePropertiesMap_;
		std::map<std::string, UnitProperties> unitPropertiesMap_;

		nlohmann::json settings_;
		nlohmann::json rootMap_;
	};

}

#endif
