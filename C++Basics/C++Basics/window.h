#pragma once
#include "tickable.h"
#include <memory>
#include <string>
#include <functional>

struct SDL_Window;
struct SDL_Renderer;

namespace game
{
	class Window
	{
	protected:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		bool failed = false;
		void StartFrame(bool running) const;
		void EndFrame() const;

	public:
		Window() = delete;
		Window(const std::string& title, const int width, const int height);
		Window(const Window& other) = delete;
		Window& operator=(const Window& other) = delete;
		~Window();
		bool IsFailed() const;
		void Run(ITickable& tickable) const;
		void Run(std::shared_ptr<ITickable> tickable) const;
		void Run(std::function<void(const float)> tick) const;
		void Run(std::unique_ptr<ITickable> tickable) const;
	};
}