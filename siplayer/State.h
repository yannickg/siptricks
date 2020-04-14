#pragma once

#include <string>
#include <map>
#include "Events.h"
#include "StateFactory.h"

namespace siptricks
{
	namespace fsm
	{
		class Actions;

		class State : public Events
		{
		public:
			typedef std::string Name;
			typedef std::map<StateFactory::FactoryState, State*> Map;

			virtual ~State();

			virtual Name getName() const = 0;
		protected:
			State();
		};
	}
}
