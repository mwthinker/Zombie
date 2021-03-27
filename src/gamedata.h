#ifndef ZOMBIE_GAMEDATA_H
#define ZOMBIE_GAMEDATA_H

//#include "animation.h"
//#include "Weapons/explosion.h"
#include "Physic/Moving/unitproperties.h"
#include "Weapons/weaponproperties.h"
#include "Weapons/missileproperties.h"
#include "Physic/Stationary/buildingproperties.h"

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

	class GameData {
	public:
		static GameData& getInstance() {
			static GameData instance;
			return instance;
		}

		GameData(const GameData&) = delete;
		GameData& operator=(const GameData&) = delete;

		void save();

		const sdl::Font& loadFont(std::string file, int fontSize);
		sdl::Sound loadSound(std::string file);
		sdl::Music loadMusic(std::string file);
		sdl::Sprite loadSprite(std::string file);

		const sdl::Font& getDefaultFont(int size);

		void bindTextureFromAtlas() const;

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

		sdl::Sprite getTreeImage();

		sdl::Sprite getBuildingWallImage();

		float getSettingsImpulseThreshold();
		float getSettingsTimeStep();

		float getSettingsInnerSpawnRadius();
		float getSettingsOuterSpawnRadius();

		int getSettingsUnitLevel();
		int getSettingsUnitLimit();

		std::string getSettingsMap();

		sdl::Sound getMenuSoundChoice();
		sdl::Sound getMenuSoundHighlited();
		sdl::Sprite getMenuBackgroundImage();

		sdl::Sprite getWaterSeeFloorImage();

		sdl::Sprite getRoadIntersection();
		sdl::Sprite getRoadStraight0();
		sdl::Sprite getRoadStraight90();
		sdl::Sprite getRoadTurn0();
		sdl::Sprite getRoadTurn90();
		sdl::Sprite getRoadTurn180();
		sdl::Sprite getRoadTurn270();
		sdl::Sprite getRoadTurnIntersection0();
		sdl::Sprite getRoadTurnIntersection90();
		sdl::Sprite getRoadTurnIntersection180();
		sdl::Sprite getRoadTurntersection270();

		//ExplosionProperties getExplosionProperties();

		UnitProperties getHumanProperties();
		UnitProperties getZombieProperties();

		MissileProperties getMissileProperties();

		MapProperties loadMapProperties();

	private:
		GameData();
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
