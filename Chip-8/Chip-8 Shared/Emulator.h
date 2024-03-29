#pragma once
#include <cstdint>
#include <array>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <iostream>
#include <fstream>
#define SDL_MAIN_HANDLED
#include "SDL.h"

namespace chipotto
{
	enum class OpcodeStatus
	{
		IncrementPC,
		NotIncrementPC,
		NotImplemented,
		StackOverflow,
		WaitForKeyboard,
		Error
	};

	class Emulator
	{
	public:
		Emulator();
		~Emulator() = default;
		Emulator(const Emulator& other) = delete;
		Emulator& operator=(const Emulator& other) = delete;
		Emulator(Emulator&& other) = delete;

		bool LoadFromFile(std::filesystem::path Path);
		bool Tick();
		bool IsValid() const;
		uint8_t GetRandomNumber(uint8_t mask);
		bool GetRegisterValueAtIndex(uint8_t register_index, uint8_t& return_value) const;
		bool SetRegisterValueAtIndex(uint8_t register_index, uint8_t new_value);
		bool GetMemoryMappingValueAtIndex(uint16_t memory_index, uint8_t& return_value) const;
		bool SetMemoryMappingValueAtIndex(uint16_t memory_index, uint8_t new_value);
		bool GetStackValueAtIndex(uint8_t stack_index, uint16_t& return_value) const;
		bool SetStackValueAtIndex(uint8_t stack_index, uint16_t new_value);
		uint16_t GetIRegisterValue() const;
		void SetIRegisterValue(uint16_t new_value);
		uint16_t GetProgramCounterValue() const;
		void SetProgramCounterValue(uint16_t new_value);
		uint8_t GetStackPointerValue() const;
		void SetStackPointerValue(uint8_t new_value);
		uint8_t GetDelayTimerValue();
		void SetDelayTimerValue(uint8_t new_value);
		uint8_t GetSoundTimerValue();
		void SetSoundTimerValue(uint8_t new_value);
		SDL_Texture* GetScreenTexture();
		int GetScreenTextureWidth();
		int GetScreenTextureHeight();
		bool FixedRandomNumber = false;

		OpcodeStatus Opcode0(const uint16_t opcode);
		OpcodeStatus Opcode1(const uint16_t opcode);
		OpcodeStatus Opcode2(const uint16_t opcode);
		OpcodeStatus Opcode3(const uint16_t opcode);
		OpcodeStatus Opcode4(const uint16_t opcode);
		OpcodeStatus Opcode5(const uint16_t opcode);
		OpcodeStatus Opcode6(const uint16_t opcode);
		OpcodeStatus Opcode7(const uint16_t opcode);
		OpcodeStatus Opcode8(const uint16_t opcode);
		OpcodeStatus Opcode9(const uint16_t opcode);
		OpcodeStatus OpcodeA(const uint16_t opcode);
		OpcodeStatus OpcodeB(const uint16_t opcode);
		OpcodeStatus OpcodeC(const uint16_t opcode);
		OpcodeStatus OpcodeD(const uint16_t opcode);
		OpcodeStatus OpcodeE(const uint16_t opcode);
		OpcodeStatus OpcodeF(const uint16_t opcode);

	private:
		std::array<uint8_t, 0x1000> MemoryMapping;
		std::array<uint8_t, 0x10> Registers;
		std::array<uint16_t, 0x10> Stack;
		std::array<std::function<OpcodeStatus(const uint16_t)>, 0x10> Opcodes;
		std::unordered_map<SDL_Keycode, uint8_t> KeyboardMap;
		std::array<SDL_Scancode, 0x10> KeyboardValuesMap;
		uint16_t I = 0x0;
		uint8_t DelayTimer = 0x0;
		uint8_t SoundTimer = 0x0;
		uint16_t PC = 0x200;
		uint8_t SP = 0xFF;
		bool Suspended = false;
		uint8_t WaitForKeyboardRegister_Index = 0;
		uint64_t DeltaTimerTicks = 0;
		SDL_Window* Window = nullptr;
		SDL_Renderer* Renderer = nullptr;
		SDL_Texture* Texture = nullptr;
		int width = 64;
		int height = 32;
	};
}