#ifndef ZOMBIE_ACTIONHANDLER_H
#define ZOMBIE_ACTIONHANDLER_H

namespace zombie {

	class Unit;
	class Car;

	class ActionHandler {
	public:
		virtual ~ActionHandler() = default;

		virtual void unitEvent(Unit* unit, int eventType) = 0;

		virtual void carEvent(Car* unit, int eventType) = 0;
	};

}

#endif
