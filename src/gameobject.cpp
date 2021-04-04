#include "gameobject.h"
#include "Physic/Moving/unit.h"

#include "inputkeyboard.h"

#include <sdl/graphic.h>

namespace zombie {

	class RenderMessage : public Message {
	public:
		RenderMessage(int id, sdl::Graphic& graphic)
			: Message{id}
			, graphic_{graphic} {
		}

		sdl::Graphic& getGraphic() const {
			return graphic_;
		}

	private:
		sdl::Graphic& graphic_;
	};

	class RenderUnit : public Component {
	public:
		RenderUnit(const UnitProperties& unitProperties)
			: unitProperties_{unitProperties} {
		}
	
		void draw(sdl::Graphic& graphic) override {
			graphic.addCircle({position_.x, position_.y}, unitProperties_.radius, sdl::Color{1.f, 1.f, 1.f});
		}

	private:
		UnitProperties unitProperties_;
		b2Vec2 position_;
	};

	class ZombieFactory {
	public:


	};

	class DeviceComponent {
	public:


	};

	namespace factory {

		GameObjectPtr createUnitGameObject(SceneManager& sceneManager, DevicePtr device) {
			auto unitGameObject = sceneManager.CreateObject();

			UnitProperties unitProperties;

			//auto renderComponent = std::make_shared<RenderUnit>(unitProperties);
			auto renderComponent = unitGameObject->AddComponent<RenderUnit>(unitProperties);

			//auto unit = std::make_shared<Unit>(sceneManager, unitProperties, nullptr);
			//unitGameObject->AddComponent(renderComponent);

			return unitGameObject;
		}

	}

	void test() {
		SceneManager sceneManager;

		//auto gameObject = factory::createUnitGameObject(sceneManager);

		//sceneManager.sendMessage

	}

}
