#pragma once
#include "tickable.h"

namespace game
{
	class Actor : public ITickable
	{
	public:
		void Tick(const float delta_time) override;
	};
}