#ifndef ZOMBIE_GAMEDATA_H
#define ZOMBIE_GAMEDATA_H

#include "physics/moving/unitproperties.h"
#include "physics/weapons/weaponproperties.h"
#include "physics/weapons/missileproperties.h"
#include "textureview.h"

//#include <sdl/sound.h>
//#include <sdl/sprite.h>
#include <sdl/color.h>
#include <sdl/util.h>
#include <sdl/imageatlas.h>
#include <sdl/gpu/gpuutil.h>


//#include <sdl/font.h>
//#include <sdl/music.h>
//#include <sdl/textureatlas.h>

#include <nlohmann/json.hpp>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_audio.h>

#include <map>
#include <vector>

namespace sdl {

}

namespace zombie {

	using SdlTtfFontPtr = sdl::SdlUniquePtr<TTF_Font, TTF_CloseFont>;

	inline SdlTtfFontPtr createSdlTtfFontPtr(const std::string& fileName, float ptsize) {
		return sdl::makeSdlUnique<TTF_Font, TTF_CloseFont>(TTF_OpenFont(fileName.c_str(), ptsize));
	}

	class Configuration {
	public:
		static Configuration& getInstance() {
			static Configuration instance;
			return instance;
		}

		Configuration(const Configuration&) = delete;
		Configuration& operator=(const Configuration&) = delete;

		void init(SDL_GPUDevice* gpuDevice);
		void close();

		void save();

		TTF_Font* loadFont(const std::string& file, float fontSize);
		//sdl::Sound loadSound(const std::string& file);
		//sdl::Music loadMusic(const std::string& file);
		TextureView loadSprite(const std::string& file);

		TTF_Font* getDefaultFont(float size);

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

		//sdl::Music getMusicTrack();

		TextureView getTreeImage();

		TextureView getBuildingWallImage();

		float getSettingsImpulseThreshold();
		float getSettingsTimeStep();

		float getSettingsInnerSpawnRadius();
		float getSettingsOuterSpawnRadius();

		int getSettingsUnitLevel();
		int getSettingsUnitLimit();

		std::string getSettingsMap();

		//sdl::Sound getMenuSoundChoice();
		//sdl::Sound getMenuSoundHighlited();
		TextureView getMenuBackgroundImage();
		TextureView getWaterSeeFloorImage();

		TextureView getRoadIntersection();
		TextureView getRoadStraight0();
		TextureView getRoadStraight90();
		TextureView getRoadTurn0();
		TextureView getRoadTurn90();
		TextureView getRoadTurn180();
		TextureView getRoadTurn270();
		TextureView getRoadTurnIntersection0();
		TextureView getRoadTurnIntersection90();
		TextureView getRoadTurnIntersection180();
		TextureView getRoadTurntersection270();

		//ExplosionProperties getExplosionProperties();

		UnitProperties getHumanProperties();
		UnitProperties getZombieProperties();

		MissileProperties getMissileProperties();

		ImTextureID getTextureAtlasBinding() {
			return ImTextureID(&atlasBinding_);
		}

		SDL_GPUTextureSamplerBinding* getTextureSamplerBinding() {
			return &atlasBinding_;
		}

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
		
		std::map<std::string, SdlTtfFontPtr> fonts_;
		//std::map<std::string, sdl::Sound> sounds_;
		//std::map<std::string, sdl::Music> musics_;
		std::map<std::string, WeaponProperties> weaponPropertiesMap_;
		std::map<std::string, MissileProperties> missilePropertiesMap_;
		std::map<std::string, UnitProperties> unitPropertiesMap_;

		nlohmann::json settings_;
		nlohmann::json rootMap_;

		TextureView menuBackgroundImage_;
		TextureView treeImage_;
		TextureView buildingWallImage_;
		TextureView waterSeeFloorImage_;
		TextureView roadIntersection_;
		TextureView roadStraight0_;
		TextureView roadStraight90_;
		TextureView roadTurn0_;
		TextureView roadTurn90_;
		TextureView roadTurn180_;
		TextureView roadTurn270_;
		TextureView roadTurnIntersection0_;
		TextureView roadTurnIntersection90_;
		TextureView roadTurnIntersection180_;
		TextureView roadTurnIntersection270_;

		sdl::gpu::GpuSampler sampler_;
		sdl::gpu::GpuTexture atlasTexture_;
		SDL_GPUTextureSamplerBinding atlasBinding_;
	};

}

#endif
