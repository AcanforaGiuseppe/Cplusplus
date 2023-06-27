#include <iostream>
#include "Window.h"
#include "PlatformScene.h"
#include "ServiceLocator.h"

int main()
{
	std::cout << "It' Working" << std::endl;

	Window *Win = new Window(800, 600, "OpenGL Window");

	float FixedDeltaTime = 0.016f;
	Physics2D Physics{glm::vec2{0, -9.81f}, FixedDeltaTime};

	ServiceLocator &Locator = ServiceLocator::GetInstance();
	Locator.Physics = &Physics;

	float TimeElapsed = 0.f;
	float FixedTimeElapsed = 0.f;

	PlatformScene Scene{*Win};

	Scene.Start();
	// Till Window is not in closing
	while (Win->IsOpened())
	{
		float LoopTime = Win->GetDeltaTime();

		// Update fps 1 sec
		TimeElapsed += LoopTime;

		if (TimeElapsed >= 1.f)
		{
			TimeElapsed -= 1.f;
			int Fps = 1.f / LoopTime;
			char Title[50];
			sprintf_s(Title, sizeof(Title), "OpenGL App | DeltaTime: %f - FPS: %d", LoopTime, Fps);
			Win->SetTitle(Title);
		}

		FixedTimeElapsed += LoopTime;

		if (FixedTimeElapsed >= FixedDeltaTime)
		{
			FixedTimeElapsed -= FixedDeltaTime;
			Scene.FixedUpdate();
			Physics.Step();
		}

		Scene.Update();
		Win->Update();
	}

	Scene.Destroy();
	delete Win;
	return 0;
}