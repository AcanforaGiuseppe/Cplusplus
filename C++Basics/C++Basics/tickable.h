#pragma once

namespace game
{
	class ITickable
	{
	public:
		virtual void Tick(const float delta_time) = 0;
	};
}