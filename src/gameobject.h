#ifndef ZOMBIE_GAMEOBJECT_H
#define ZOMBIE_GAMEOBJECT_H

#include "Graphic/graphic.h"
#include "device.h"


#include <memory>
#include <map>
#include <vector>

#include <fmt/printf.h>

namespace zombie {

	class Event {
	public:
	};

	class Message {
	public:

		int getDestinationId() const {
			return destinationId_;
		}
	
	protected:
		Message(int destinationId)
			: destinationId_{destinationId} {
		}

	private:
		int destinationId_;
	};
	using MessagePtr = std::shared_ptr<Message>;

	class MessageSender {
	public:
		virtual bool sendMessage(MessagePtr msg) = 0;
	};

	class Component {
	public:
		virtual ~Component() = default;
		virtual bool handleMessage(MessagePtr msg) {
			return false;
		};

		virtual void emitMessage(MessagePtr msg) {};

		virtual void update(double deltaTime) {
		}

		virtual void draw(graphic::Graphic& graphic) {
		}
	};
	using ComponentPtr = std::shared_ptr<Component>;

	class GameObject {
	public:
		GameObject(int id)
			: id_{id} {
		}

		int GetObjectID() const {
			return id_;
		}

		/*
		template <class T, class... Params>
		const CommandPtr& emplace(Params&&... params) {
			static_assert(std::is_base_of<Command, T>::value, "T must be derived from Command");
			return commands_.emplace(std::make_shared<T>(std::forward<Params>(params)...));
		}
		*/

		template <typename TComponent, typename... Args>
		std::shared_ptr<TComponent> AddComponent(Args&&... args)
		{
			auto component = std::make_shared<TComponent>(args...);
			components_.push_back(component);
			return component;
		}

		bool sendMessage(MessagePtr message) {
			return PassMessageToComponents(std::move(message));
		}

		void update(double deltaTime) {

		}

		void draw(graphic::Graphic& graphic) {

		}

	private:
		bool PassMessageToComponents(MessagePtr message) {
			bool messageHandled = false;
			
			for (const auto& component : components_) {
				messageHandled |= component->handleMessage(message);
			}

			return messageHandled;
		}

		int id_;
		std::vector<ComponentPtr> components_;
	};
	using GameObjectPtr = std::shared_ptr<GameObject>;

	class SceneManager : public MessageSender {
	public:
		// Returns true if the object or any components handled the message
		bool sendMessage(MessagePtr msg) override {
			auto it = gameObjects_.find(msg->getDestinationId());
			if (it != gameObjects_.end())
			{
				return it->second->sendMessage(std::move(msg));
			}
			return false;
		}

		void update(double deltaTime) {
			for (auto& [_, gameObject] : gameObjects_) {
				gameObject->update(deltaTime);
			}
		}

		void draw(graphic::Graphic& graphic) {
			for (auto& [_, gameObject] : gameObjects_) {
				gameObject->draw(graphic);
			}
		}

		GameObjectPtr CreateObject() {
			int id = nextID_++;
			auto gameObject = std::make_shared<GameObject>(id);
			gameObjects_[id] = gameObject;
			return std::move(gameObject);
		}

	private:
		std::map<int, GameObjectPtr> gameObjects_;
		int nextID_{1};
	};

	namespace factory {

		GameObjectPtr createUnitGameObject(SceneManager& sceneManager, DevicePtr device);

	}

}
#endif
