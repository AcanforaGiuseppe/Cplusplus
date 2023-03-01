#include "window.h"
#include <iostream>
#include <vector>
#include "actor.h"
#include "aiv_math.h"
using tickable_vector = std::vector<std::shared_ptr<game::ITickable>>;

class MyGame : public game::ITickable
{
protected:
	tickable_vector tickables;

public:
	void Tick(const float delta_time) override
	{
		//std::cout << "Test String " << 5 << std::endl;
		// Iterating like that causes the creation of a copy of "tickable"
		//for (auto tickable : tickables)

		for (auto& tickable : tickables) // For iterating the instance, use the & after the type of the variable
			tickable->Tick(delta_time);
	}

	void AddTickable(std::shared_ptr<game::ITickable> tickable)
	{
		tickables.push_back(tickable);
	}
};

std::ostream& operator<<(std::ostream& stream, const aiv_math::Vector3 vector3)
{
	stream << vector3.x << "," << vector3.y << "," << vector3.z;
	return stream;
}

class CallMe
{
public:
	CallMe() = default;
	~CallMe() = default;

	void operator() ()
	{
		std::cout << "Hello" << std::endl;
	}

private:

};

class GameLoop
{
public:
	GameLoop() = default;
	~GameLoop() = default;

	void Update(const float delta_time)
	{
		std::cout << delta_time << std::endl;
	}

private:

};

// Functor
class GameLoopBinder
{
public:
	GameLoop& loop;
	GameLoopBinder() = default;

	GameLoopBinder(GameLoop& in_loop) : loop(in_loop)
	{ }

	void operator() (const float delta_time)
	{
		loop.Update(delta_time);
	}

	~GameLoopBinder() = default;

private:

};


template<typename Func, typename Context>
class AivBinder
{
public:
	Func func;
	Context* context;
	AivBinder() = delete;

	AivBinder(Func in_func, Context* in_context)
	{
		func = in_func;
		context = in_context;
	}

	void operator() (const float in_value)
	{
		std::invoke(func, context, in_value);
		//(context->*func)(in_value);
	}

	~AivBinder() = default;

private:

};

template<typename Func, typename Context>
AivBinder<Func, Context> AivBind(Func in_func, Context* in_context)
{
	return AivBinder<Func, Context>(in_func, in_context);
}

int main(int argc, char** argv)
{
	game::Window window("Window", 512, 512);
	auto my_game = std::make_shared<MyGame>();
	//std::unique_ptr<MyGame> my_game2 = my_game;
	auto actor_00 = std::make_shared<game::Actor>();
	my_game->AddTickable(actor_00);
	//window.Run(my_game);

	GameLoop game_loop;

	//GameLoopBinder game_binder(game_loop);

	auto bind = AivBind(&GameLoop::Update, &game_loop);

	window.Run(bind);

	/*int a = 5;
	window.Run([a](const float delta_time)
		{
			std::cout << delta_time << a << std::endl;
		});*/

	aiv_math::Vector2 vector(3, 2);
	aiv_math::Vector2 vector1(5, 1);
	auto vector2 = vector + vector1 * vector;
	aiv_math::Vector3 vector4 = static_cast<aiv_math::Vector3>(vector1);
	std::cout << vector4 << std::endl;

	std::vector<std::string> names;
	names.push_back("Name_00");
	names.push_back("Name_01");
	std::vector<std::string>& names2 = names;

	CallMe call;
	call();
	return 0;
}