#include "configuration.h"

#include <spdlog/spdlog.h>

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>

using nlohmann::json;
namespace fs = std::filesystem;
using Point = b2Vec2;

namespace zombie {

	namespace {

		const std::string SettingsPath = "settings/settings.json";
		const std::string MapsPath = "settings/maps.json";
		const std::string MissilesPath = "settings/missiles";
		const std::string UnitsPath = "settings/units";
		const std::string CarsPath = "settings/cars";
		const std::string WeaponsPath = "settings/weapons";

	}

	namespace {
		
		// Takes a string as input and returns the point.
		// The string "POINT (x y)" the input should be defined
		// as "POINT (...)".
		Point loadPoint(std::string line) {
			std::stringstream stream(line);

			std::string word;
			stream >> word;
			assert(word == "POINT");

			char chr;
			stream >> chr;
			assert(chr == '(');
			Point point;
			stream >> point.x;
			stream >> point.y;
			return point;
		}
	
		// Takes a string as input and returns the points.
		// The string "POLYGON ((x1 y1, x2 y2, ...))" the input should be defined
		// as "POLYGON ((...))". The last point is assumed to be the same as the first, therefore
		// the last point will not be includeded in the returned vector.
		std::vector<Point> loadPolygon(std::string line) {
			std::stringstream stream(line);
			std::string word;

			stream >> word;
			assert(word == "POLYGON");

			char paranthes;
			stream >> paranthes;
			assert(paranthes == '(');
			stream >> paranthes;
			assert(paranthes == '(');

			Point point;
			std::vector<Point> points;

			while (stream >> point.x && stream >> point.y) {
				points.push_back(point);
				char comma;
				if (!(stream >> comma)) {
					break;
				}
			}
			points.pop_back();
			return points;
		}

		bool isJsonFile(const std::string name) {
			try {
				static const std::regex jsonRegex(R"(.*\.[Jj][Ss][Oo][Nn])");
				static const std::regex schemaRegex(R"(.*\.schema\.[Jj][Ss][Oo][Nn])");
				std::smatch match;
				if (std::regex_match(name, match, jsonRegex) && !std::regex_match(name, match, schemaRegex)) {
					return true;
				}
			} catch (const std::regex_error& error) {
				spdlog::critical("[zombie::Configuration] Programming error: {}", error.what());
				std::exit(1);
			}
			return false;
		}
	
	}

	Configuration::Configuration()
		: textureAtlas_{2048, 2048, []() {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}} {
		spdlog::info("[Configuration] Current path: {}", fs::current_path().string());
		
		spdlog::info("[Configuration] Load: {}", SettingsPath);
		if (std::ifstream file{SettingsPath}; file) {
			file >> settings_;
		} else {
			spdlog::critical("[Configuration] Failed to load: {}", SettingsPath);
		}
		
		auto map = settings_["settings"]["map"].get<std::string>();
		spdlog::info("[Configuration] Load: {}", map);
		if (std::ifstream file{map}; file) {
			file >> rootMap_;
		} else {
			spdlog::critical("[Configuration] Fail to load: {}", map);
		}

		loadAllWeaponProperties();
		loadAllMissileProperties();
		loadAllUnitProperties();
	}

	void Configuration::loadAllWeaponProperties() {
		for (const auto& fileName : fs::directory_iterator{WeaponsPath}) {
			if (fileName.is_regular_file() && isJsonFile(fileName.path().string())) {
				spdlog::info("[Configuration] Load: {}", fileName.path().string());
				if (std::ifstream file{fileName.path()}; file) {
					nlohmann::json weaponJson;
					file >> weaponJson;
					weaponPropertiesMap_[weaponJson["name"].get<std::string>()] = loadWeaponProperties(weaponJson);
				} else {
					spdlog::critical("[Configuration] Fail to load: {}", fileName.path().string());
				}
			}
		}
	}

	void Configuration::loadAllMissileProperties() {
		for (const auto& fileName : fs::directory_iterator{MissilesPath}) {
			if (fileName.is_regular_file() && isJsonFile(fileName.path().string())) {
				spdlog::info("[Configuration] Load: {}", fileName.path().string());
				if (std::ifstream file{fileName.path()}; file) {
					nlohmann::json missileJson;
					file >> missileJson;
					missilePropertiesMap_[missileJson["name"].get<std::string>()] = loadMissileProperties(missileJson);
				} else {
					spdlog::critical("[Configuration] Fail to load: {}", fileName.path().string());
				}
			}
		}
	}

	void Configuration::loadAllUnitProperties() {
		for (const auto& fileName : fs::directory_iterator(UnitsPath)) {
			if (fileName.is_regular_file() && isJsonFile(fileName.path().string())) {
				spdlog::info("[Configuration] Load: {}", fileName.path().string());
				if (std::ifstream file{fileName.path()}; file) {
					nlohmann::json unitJson;
					file >> unitJson;
					unitPropertiesMap_[unitJson["name"].get<std::string>()] = loadUnitProperties(unitJson);
				} else {
					spdlog::critical("[Configuration] Fail to load: {}", fileName.path().string());
				}
			}
		}
	}

	void Configuration::save() {
		std::ofstream out{SettingsPath};
		//settings_ >> out;
	}

	const sdl::Font& Configuration::loadFont(const std::string& file, int fontSize) {
		auto size = fonts_.size();
		std::string key = file;
		key += fontSize;
		sdl::Font& font = fonts_[key];

		// Font not found?
		if (fonts_.size() > size) {
			font = sdl::Font{file, fontSize};
		}

		return font;
	}

	sdl::Sound Configuration::loadSound(const std::string& file) {
		auto size = sounds_.size();
		sdl::Sound& sound = sounds_[file];

		// Sound not found?
		if (sounds_.size() > size) {
			sound = sdl::Sound{file};
		}

		return sound;
	}

	sdl::Music Configuration::loadMusic(const std::string& file) {
		auto size = musics_.size();
		sdl::Music& music = musics_[file];

		// Music not found?
		if (musics_.size() > size) {
			music = sdl::Music{file};
		}

		return music;
	}

	sdl::TextureView Configuration::loadSprite(const std::string& file) {
		return textureAtlas_.add(file, 1).getTextureView();
	}

	const sdl::Font& Configuration::getDefaultFont(int size) {
		return loadFont(settings_["window"]["font"].get<std::string>(), size);
	}

	void Configuration::bindTextureFromAtlas() {
		textureAtlas_.bind();
	}

	int Configuration::getWindowPositionX() {
		return settings_["window"]["positionX"].get<int>();
	}

	int Configuration::getWindowPositionY() {
		return settings_["window"]["positionY"].get<int>();
	}

	void Configuration::setWindowPositionX(int x) {
		settings_["window"]["positionX"] = x;
	}

	void Configuration::setWindowPositionY(int y) {
		settings_["window"]["positionY"] = y;
	}

	int Configuration::getWindowWidth() {
		return settings_["window"]["width"].get<int>();
	}

	int Configuration::getWindowHeight() {
		return settings_["window"]["height"].get<int>();
	}

	void Configuration::setWindowWidth(int width) {
		settings_["window"]["width"] = width;
	}

	void Configuration::setWindowHeight(int height) {
		settings_["window"]["height"] = height;
	}

	bool Configuration::isWindowResizable() {
		return settings_["window"]["resizeable"].get<bool>();
	}

	void Configuration::setWindowResizable(bool resizeable) {
		settings_["window"]["resizeable"] = resizeable;
	}

	int Configuration::getWindowMinWidth() {
		return settings_["window"]["minWidth"].get<int>();
	}

	int Configuration::getWindowMinHeight() {
		return settings_["window"]["minHeight"].get<int>();
	}

	std::string Configuration::getWindowIcon() {
		return settings_["window"]["icon"].get<std::string>();
	}

	bool Configuration::isWindowBordered() {
		return settings_["window"]["border"].get<bool>();
	}

	void Configuration::setWindowBordered(bool border) {
		settings_["window"]["border"] = border;
	}

	bool Configuration::isWindowMaximized() {
		return settings_["window"]["maximized"].get<bool>();
	}

	void Configuration::setWindowMaximized(bool maximized) {
		settings_["window"]["maximized"] = maximized;
	}

	bool Configuration::isWindowVsync() {
		return settings_["window"]["vsync"].get<bool>();
	}

	void Configuration::setWindowVsync(bool activate) {
		settings_["window"]["vsync"] = activate;
	}

	bool Configuration::isMusicOn() {
		return settings_["music"]["switch"].get<bool>();
	}

	void Configuration::setMusicOn(bool maximized) {
		settings_["music"]["music"] = maximized;
	}

	float Configuration::getMusicVolume() {
		return settings_["music"]["volume"].get<float>();
	}

	void Configuration::setMusicVolume(float volume) {
		settings_["music"]["volume"] = volume;
	}

	sdl::Music Configuration::getMusicTrack() {
		return loadMusic(settings_["music"]["track"].get<std::string>());
	}

	sdl::TextureView Configuration::getTreeImage() {
		return loadSprite(settings_["tree"].get<std::string>());
	}

	sdl::TextureView Configuration::getBuildingWallImage() {
		return loadSprite(settings_["buildings"]["wallImage"].get<std::string>());
	}

	float Configuration::getSettingsImpulseThreshold() {
		return settings_["settings"]["impulseThreshold"].get<float>();
	}

	float Configuration::getSettingsTimeStep() {
		return settings_["settings"]["timeStep"].get<float>();
	}

	float Configuration::getSettingsInnerSpawnRadius() {
		return settings_["settings"]["innerSpawnRadius"].get<float>();
	}

	float Configuration::getSettingsOuterSpawnRadius() {
		return settings_["settings"]["outerSpawnRadius"].get<float>();
	}

	int Configuration::getSettingsUnitLevel() {
		return settings_["settings"]["unitLevel"].get<int>();
	}

	int Configuration::getSettingsUnitLimit() {
		return settings_["settings"]["unitLimit"].get<int>();
	}

	std::string Configuration::getSettingsMap() {
		return settings_["settings"]["map"].get<std::string>();
	}

	sdl::Sound Configuration::getMenuSoundChoice() {
		return loadSound(settings_["menu"]["soundChoice"].get<std::string>());
	}

	sdl::Sound Configuration::getMenuSoundHighlited() {
		return loadSound(settings_["menu"]["soundHighlited"].get<std::string>());
	}

	sdl::TextureView Configuration::getMenuBackgroundImage() {
		return loadSprite(settings_["menu"]["backgroundImage"].get<std::string>());
	}

	sdl::TextureView Configuration::getWaterSeeFloorImage() {
		return loadSprite(settings_["water"]["seeFloorImage"].get<std::string>());
	}

	sdl::TextureView Configuration::getRoadIntersection() {
		return loadSprite(settings_["roads"]["intersection"].get<std::string>());
	}

	sdl::TextureView Configuration::getRoadStraight0() {
		return loadSprite(settings_["roads"]["straight0"].get<std::string>());
	}

	sdl::TextureView Configuration::getRoadStraight90() {
		return loadSprite(settings_["roads"]["straight90"].get<std::string>());
	}

	sdl::TextureView Configuration::getRoadTurn0() {
		return loadSprite(settings_["roads"]["turn0"].get<std::string>());
	}

	sdl::TextureView Configuration::getRoadTurn90() {
		return loadSprite(settings_["roads"]["turn90"].get<std::string>());
	}

	sdl::TextureView Configuration::getRoadTurn180() {
		return loadSprite(settings_["roads"]["turn180"].get<std::string>());
	}

	sdl::TextureView Configuration::getRoadTurn270() {
		return loadSprite(settings_["roads"]["turn270"].get<std::string>());
	}

	sdl::TextureView Configuration::getRoadTurnIntersection0() {
		return loadSprite(settings_["roads"]["tintersection0"].get<std::string>());
	}

	sdl::TextureView Configuration::getRoadTurnIntersection90() {
		return loadSprite(settings_["roads"]["tintersection90"].get<std::string>());
	}

	sdl::TextureView Configuration::getRoadTurnIntersection180() {
		return loadSprite(settings_["roads"]["tintersection180"].get<std::string>());
	}

	sdl::TextureView Configuration::getRoadTurntersection270() {
		return loadSprite(settings_["roads"]["tintersection270"].get<std::string>());
	}

	/*
	ExplosionProperties Configuration::getExplosionProperties() {
		float timeDelay = settings_["explosion"]["timeDelay"].get<float>();
		float speed = settings_["explosion"]["speed"].get<float>();
		sdl::Sound sound = loadSound(settings_["explosion"]["sound"].get<std::string>());
		Animation animation = loadAnimation(settings_["explosion"]["animation"]);
		animation.setLooping(false);
		return ExplosionProperties(animation, sound, timeDelay);
	}
	*/

	UnitProperties Configuration::getHumanProperties() {
		return unitPropertiesMap_[std::string("human")];
	}

	UnitProperties Configuration::getZombieProperties() {
		return unitPropertiesMap_[std::string("zombie")];
	}

	/*
	Animation Configuration::loadAnimation(const json& animationTag) {
		float defaultDeltaTime = 1.f;
		try {
			defaultDeltaTime = animationTag.at("deltaTime").get<float>();
		} catch (nlohmann::detail::out_of_range) {
			// Do nothing.
		}

		float defaultBodyWidth = 1.f;
		try {
			defaultBodyWidth = animationTag.at("bodyWidth").get<float>();
		} catch (nlohmann::detail::out_of_range) {
			// Do nothing.
		}

		Animation animation;
		try {
			for (const auto& child : animationTag.at("frames")) {
				float bodyWidth = defaultBodyWidth;
				try {
					bodyWidth = child.at("bodyWidth").get<float>();
				} catch (nlohmann::detail::out_of_range) {
					// Do nothing.
				}
			
				float deltaTime = defaultDeltaTime;
				try {
					deltaTime = child.at("time").get<float>();
				} catch (nlohmann::detail::out_of_range) {
					// Do nothing.
				}

				auto n = child["image"].get<std::string>();
				animation.add(loadSprite(child["image"].get<std::string>()), bodyWidth, deltaTime);
			}
		} catch (nlohmann::detail::out_of_range) {
			// Do nothing.
		}

		return animation;
	}
	*/

	MissileProperties Configuration::getMissileProperties() {
		try {
			return missilePropertiesMap_.at(std::string("missile"));
		} catch (const std::out_of_range&) {
			return MissileProperties();
		}
	}

	UnitProperties Configuration::loadUnitProperties(const json& unitTag) {
		UnitProperties unitProperties;
		//unitProperties.hitSound = loadSound(unitTag["hitSound"].get<std::string>());
		//unitProperties.dieSound = loadSound(unitTag["dieSound"].get<std::string>());
		//unitProperties.moveAnimation_ = loadAnimation(unitTag["moveAnimation"]);
		//unitProperties.dieAnimation_ = loadAnimation(unitTag["dieAnimation"]);
		//unitProperties.injuredAnimation_ = loadAnimation(unitTag["injuredAnimation"]);
		unitProperties.grip = Position(unitTag["moveImageGripX"].get<float>(), unitTag["moveImageGripY"].get<float>());
		unitProperties.mass = unitTag["mass"].get<float>();
		unitProperties.radius = unitTag["radius"].get<float>();
		unitProperties.life = unitTag["life"].get<float>();
		unitProperties.walkingSpeed = unitTag["walkingSpeed"].get<float>();
		unitProperties.runningSpeed = unitTag["runningSpeed"].get<float>();
		//unitProperties.stamina = unitTag["stamina"].get<float>();
		unitProperties.weaponProperties = loadWeaponProperties(unitTag["weapon"].get<std::string>());
		return unitProperties;
	}

	WeaponProperties Configuration::loadWeaponProperties(std::string weaponName) {
		try {
			return weaponPropertiesMap_.at(weaponName);
		} catch (const std::out_of_range&) {
			return WeaponProperties();
		}
	}

	WeaponProperties Configuration::loadWeaponProperties(const json& unitTag) {
		WeaponProperties properties;
		properties.name = unitTag["name"].get<std::string>();
		properties.type = WeaponProperties::Type::Bullet;
		std::string s = unitTag["type"].get<std::string>();
		if (unitTag["type"].get<std::string>() == "BULLET") {
			properties.type = WeaponProperties::Type::Bullet;
		} else if (unitTag["type"].get<std::string>() == "MISSILE") {
			properties.type = WeaponProperties::Type::Missile;
		} else {
			throw 1;
		}
		properties.symbolImage = loadSprite(unitTag["symbolImage"].get<std::string>());
		properties.timeBetweenShots = unitTag["timeBetweenShots"].get<float>();
		properties.clipSize = unitTag["clipSize"].get<int>();
		properties.shootSound = loadSound(unitTag["shootSound"].get<std::string>());
		properties.reloadSound = loadSound(unitTag["reloadSound"].get<std::string>());
		//properties.moveAnimation_ = loadAnimation(unitTag["moveAnimation"]);
		properties.size = unitTag["size"].get<float>();
		properties.moveImageGrip = Position(unitTag["moveImageGripX"].get<float>(), unitTag["moveImageGripY"].get<float>());
		
		// Projectile properties.
		properties.damage = unitTag["projectile"]["damage"].get<float>();
		properties.range = unitTag["projectile"]["range"].get<float>();
		properties.laserSight = unitTag["laserSight"].get<bool>();
		switch (properties.type) {
			case WeaponProperties::Type::Bullet:
				break;
			case WeaponProperties::Type::Missile:
				properties.damageRadius = unitTag["projectile"]["damageRadius"].get<float>();
				properties.deathTime = unitTag["projectile"]["deathTime"].get<float>();
				properties.speed = unitTag["projectile"]["speed"].get<float>();
				properties.force = unitTag["projectile"]["force"].get<float>();
				properties.missileProperties = loadMissileProperties(std::string("missile"));
				break;
		}
		
		return properties;
	}

	MissileProperties Configuration::loadMissileProperties(std::string missileName) {
		for (const auto& child : settings_["missiles"]) {
			std::string name = child["name"].get<std::string>();
			if (name == missileName) {
				return loadMissileProperties(child);
			}
		}
		return MissileProperties();
	}

	MissileProperties Configuration::loadMissileProperties(const json& unitTag) {
		MissileProperties properties;
		properties.name = unitTag["name"].get<std::string>();
		properties.mass = unitTag["mass"].get<float>();
		properties.width = unitTag["width"].get<float>();
		properties.length = unitTag["length"].get<float>();
		//properties.animation_ = loadAnimation(unitTag["animation"]);
		properties.moveSound = loadSound(unitTag["moveSound"].get<std::string>());
		return properties;
	}

	MapProperties Configuration::loadMapProperties() {
		MapProperties properties;
		properties.name_ = rootMap_["name"].get<std::string>();

		for (const auto& child : rootMap_["objects"]) {
			std::string objectType = child["objectType"].get<std::string>();
			if (objectType == "building") {
				MapProperties::Object ob;
				ob.type_ = MapProperties::ObjectType::Building;
				ob.geom_ = loadPolygon(child["geom"].get<std::string>());
				properties.positions_.push_back(ob);
			} else if (objectType == "grass") {
				MapProperties::Object ob;
				ob.type_ = MapProperties::ObjectType::Grass;
				ob.geom_ = loadPolygon(child["geom"].get<std::string>());
				properties.positions_.push_back(ob);
			} else if (objectType == "tilepoint") {
				MapProperties::Object ob;
				ob.type_ = MapProperties::ObjectType::TilePoint;
				ob.geom_.push_back(loadPoint(child["geom"].get<std::string>()));
				properties.positions_.push_back(ob);
			} else if (objectType == "tree") {
				MapProperties::Object ob;
				ob.type_ = MapProperties::ObjectType::Tree;
				ob.geom_.push_back(loadPoint(child["geom"].get<std::string>()));
				properties.positions_.push_back(ob);
			} else if (objectType == "water") {
				MapProperties::Object ob;
				ob.type_ = MapProperties::ObjectType::Water;
				ob.geom_ = loadPolygon(child["geom"].get<std::string>());
				properties.positions_.push_back(ob);
			} else if (objectType == "spawningpoint") {
				MapProperties::Object ob;
				ob.type_ = MapProperties::ObjectType::SpawningPoint;
				ob.geom_.push_back(loadPoint(child["geom"].get<std::string>()));
				properties.positions_.push_back(ob);
			} else if (objectType == "car spawningpoint") {
				MapProperties::Object ob;
				ob.type_ = MapProperties::ObjectType::CarSpawningPoint;
				ob.geom_.push_back(loadPoint(child["geom"].get<std::string>()));
				properties.positions_.push_back(ob);
			}
		}
		return properties;
	}

}
