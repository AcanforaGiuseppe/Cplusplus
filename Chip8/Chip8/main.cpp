#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <functional>

namespace chipotto
{
	enum class OpcodeStatus
	{
		IncreasePC,
		NotIncreasePC,
		StackOverflow,
		WaitForKeyboard,
		NotImplemented
	};

	class Emulator
	{
	public:
		Emulator()
		{
			Opcodes[0x0] = std::bind(&Emulator::Opcode0, this, std::placeholders::_1);
			Opcodes[0x1] = std::bind(&Emulator::Opcode1, this, std::placeholders::_1);
			Opcodes[0x2] = std::bind(&Emulator::Opcode2, this, std::placeholders::_1);
			Opcodes[0x3] = std::bind(&Emulator::Opcode3, this, std::placeholders::_1);
			Opcodes[0x4] = std::bind(&Emulator::Opcode4, this, std::placeholders::_1);
			Opcodes[0x5] = std::bind(&Emulator::Opcode5, this, std::placeholders::_1);
			Opcodes[0x6] = std::bind(&Emulator::Opcode6, this, std::placeholders::_1);
			Opcodes[0x7] = std::bind(&Emulator::Opcode7, this, std::placeholders::_1);
			Opcodes[0x8] = std::bind(&Emulator::Opcode8, this, std::placeholders::_1);
			Opcodes[0x9] = std::bind(&Emulator::Opcode9, this, std::placeholders::_1);
			Opcodes[0XA] = std::bind(&Emulator::OpcodeA, this, std::placeholders::_1);
			Opcodes[0XB] = std::bind(&Emulator::OpcodeB, this, std::placeholders::_1);
			Opcodes[0XC] = std::bind(&Emulator::OpcodeC, this, std::placeholders::_1);
			Opcodes[0XD] = std::bind(&Emulator::OpcodeD, this, std::placeholders::_1);
			Opcodes[0XE] = std::bind(&Emulator::OpcodeE, this, std::placeholders::_1);
			Opcodes[0XF] = std::bind(&Emulator::OpcodeF, this, std::placeholders::_1);
		};

		Emulator(const Emulator& Other) = delete;
		Emulator operator=(Emulator& Other) = delete;
		Emulator(Emulator&& Other) = delete;
		~Emulator() = default;

		bool LoadFromFile(std::filesystem::path Path)
		{
			std::ifstream File;
			File.open(Path, std::ios::binary);

			if (!File.is_open())
				return false;

			auto FileSize = std::filesystem::file_size(Path);
			File.read(reinterpret_cast<char*>(MemoryMapping.data() + PC), FileSize);
			File.close();

			return true;
		}

		bool Tick()
		{
			uint16_t Opcode = (static_cast<uint16_t>(MemoryMapping[PC]) << 8) | MemoryMapping[PC + 1];
			std::cout << std::hex << "0x" << PC << ":  0x" << Opcode << std::dec << std::endl << "\t";

			OpcodeStatus Status = Opcodes[Opcode >> 12](Opcode);

			if (Status == OpcodeStatus::WaitForKeyboard)
			{
				getchar();
				Status = OpcodeStatus::IncreasePC;
			}

			if (Status == OpcodeStatus::IncreasePC)
				PC += 2;

			return Status != OpcodeStatus::NotImplemented && Status != OpcodeStatus::StackOverflow;
		}

		OpcodeStatus Opcode0(const uint16_t opcode)
		{
			if ((opcode & 0xFF) == 0xE0)
			{
				// TODO Clear screen
				std::cout << "CLS" << std::endl;
				return OpcodeStatus::IncreasePC;
			}
			else if ((opcode & 0xFF) == 0xEE)
			{
				if (SP > 0xF && SP < 0xFF)
					return OpcodeStatus::StackOverflow;

				std::cout << "RET" << std::endl;
				PC = Stack[SP & 0xF];
				SP -= 1;
				return OpcodeStatus::IncreasePC;
			}
			return OpcodeStatus::NotImplemented;
		}

		OpcodeStatus Opcode1(const uint16_t opcode)
		{
			uint16_t Address = opcode & 0x0FFF;
			std::cout << std::hex << "JP 0x" << Address << std::dec << std::endl;
			PC = Address;
			return OpcodeStatus::NotIncreasePC;
		}

		OpcodeStatus Opcode2(const uint16_t opcode)
		{
			uint16_t Address = opcode & 0xFFF;
			std::cout << std::hex << "CALL 0x" << (int)Address << std::dec << std::endl;

			if (SP == 0xFF)
			{
				SP = 0;
			}
			else
			{
				if (SP < 0xF)
					SP += 1;
				else
					return OpcodeStatus::StackOverflow;
			}

			Stack[SP] = PC;
			PC = Address;
			return OpcodeStatus::NotIncreasePC;
		}

		OpcodeStatus Opcode3(const uint16_t opcode)
		{
			uint8_t RegisterXIndex = (opcode >> 8) & 0xF;
			uint8_t Value = opcode & 0xFF;
			std::cout << std::hex << "SE V" << (int)RegisterXIndex << ", 0x" << (int)Value << std::dec << std::endl;

			if (Registers[RegisterXIndex] == Value)
				PC += 2;

			return OpcodeStatus::IncreasePC;
		}

		OpcodeStatus Opcode4(const uint16_t opcode)
		{
			return OpcodeStatus::NotImplemented;
		}

		OpcodeStatus Opcode5(const uint16_t opcode)
		{
			return OpcodeStatus::NotImplemented;
		}

		OpcodeStatus Opcode6(const uint16_t opcode)
		{
			uint8_t RegisterIndex = (opcode >> 8) & 0xF;
			uint8_t RegisterValue = opcode & 0xFF;
			std::cout << std::hex << "LD V" << (int)RegisterIndex << ", 0x" << (int)RegisterValue << std::dec << std::endl;
			Registers[RegisterIndex] = RegisterValue;
			return OpcodeStatus::IncreasePC;
		}

		OpcodeStatus Opcode7(const uint16_t opcode)
		{
			uint8_t RegisterXIndex = (opcode >> 8) & 0xF;
			uint8_t Value = opcode & 0xFF;
			std::cout << std::hex << "ADD V" << (int)RegisterXIndex << ", 0x" << (int)Value << std::dec << std::endl;
			Registers[RegisterXIndex] += Value;
			return OpcodeStatus::IncreasePC;
		}

		OpcodeStatus Opcode8(const uint16_t opcode)
		{
			if ((opcode & 0xF) == 0x0)
			{
				uint8_t RegisterXIndex = (opcode >> 8) & 0xF;
				uint8_t RegisterYIndex = (opcode >> 4) & 0xF;
				std::cout << std::hex << "LD V" << (int)RegisterXIndex << ", V" << (int)RegisterYIndex << std::dec << std::endl;
				Registers[RegisterXIndex] = Registers[RegisterYIndex];
				return OpcodeStatus::IncreasePC;
			}
			return OpcodeStatus::NotImplemented;
		}

		OpcodeStatus Opcode9(const uint16_t opcode)
		{
			return OpcodeStatus::NotImplemented;
		}

		OpcodeStatus OpcodeA(const uint16_t opcode)
		{
			uint16_t Value = (opcode & 0xFFF);
			std::cout << std::hex << "LD I, 0x" << (int)Value << std::dec << std::endl;
			I = Value;
			return OpcodeStatus::IncreasePC;
		}

		OpcodeStatus OpcodeB(const uint16_t opcode)
		{
			return OpcodeStatus::NotImplemented;
		}

		OpcodeStatus OpcodeC(const uint16_t opcode)
		{
			return OpcodeStatus::NotImplemented;
		}

		OpcodeStatus OpcodeD(const uint16_t opcode)
		{
			uint8_t RegisterXIndex = (opcode >> 8) & 0xF;
			uint8_t RegisterYIndex = (opcode >> 4) & 0xF;
			uint8_t ByteNumbers = opcode & 0xF;
			std::cout << std::hex << "DRW V" << (int)RegisterXIndex << ", V" << (int)RegisterYIndex << ", " << (int)ByteNumbers << std::dec << std::endl;
			// TODO Draw Sprite
			return OpcodeStatus::IncreasePC;
		}

		OpcodeStatus OpcodeE(const uint16_t opcode)
		{
			return OpcodeStatus::NotImplemented;
		}

		OpcodeStatus OpcodeF(const uint16_t opcode)
		{
			if ((opcode & 0xFF) == 0x0A)
			{
				uint8_t RegisterIndex = (opcode >> 8) & 0xF;
				std::cout << std::hex << "LD V" << (int)RegisterIndex << ", K" << std::dec << std::endl;
				return OpcodeStatus::WaitForKeyboard;
			}
			else if ((opcode & 0xFF) == 0x55)
			{
				uint8_t RegisterIndex = (opcode >> 8) & 0xF;
				std::cout << std::hex << "LD [I], V" << (int)RegisterIndex << std::dec << std::endl;

				for (uint8_t i = 0; i < RegisterIndex; ++i)
					MemoryMapping[I + i] = Registers[i];

				return OpcodeStatus::IncreasePC;
			}
			else if ((opcode & 0xFF) == 0x65)
			{
				uint8_t RegisterIndex = (opcode >> 8) & 0xF;
				std::cout << std::hex << "LD V" << (int)RegisterIndex << ", [I]" << std::dec << std::endl;

				for (uint8_t i = 0; i < RegisterIndex; ++i)
					Registers[i] = MemoryMapping[I + i];

				return OpcodeStatus::IncreasePC;
			}
			else if ((opcode & 0xFF) == 0x29)
			{
				uint8_t RegisterIndex = (opcode >> 8) & 0xF;
				std::cout << std::hex << "LD F, V" << (int)RegisterIndex << std::dec << std::endl;
				I = 5 * Registers[RegisterIndex];
				return OpcodeStatus::IncreasePC;
			}
			else if ((opcode & 0xFF) == 0x33)
			{
				uint8_t RegisterIndex = (opcode >> 8) & 0xF;
				std::cout << std::hex << "LD B, V" << (int)RegisterIndex << std::dec << std::endl;
				uint8_t RegisterValue = Registers[RegisterIndex];
				MemoryMapping[I] = RegisterValue / 100;
				MemoryMapping[I + 1] = (RegisterValue - (MemoryMapping[I] * 100)) / 10;
				MemoryMapping[I + 2] = RegisterValue % 10;
				return OpcodeStatus::IncreasePC;
			}
			return OpcodeStatus::NotImplemented;
		}

	private:
		std::array<uint8_t, 0x1000> MemoryMapping;
		std::array<uint8_t, 0x10> Registers;
		std::array<uint16_t, 0x10> Stack;
		std::array<std::function<OpcodeStatus(const uint16_t)>, 0x10> Opcodes;
		uint16_t I = 0x000;
		uint8_t DelayTimer = 0x00;
		uint8_t SoundTimer = 0x00;
		uint16_t PC = 0x200;
		uint8_t SP = 0xFF;
	};
}

int main(int argc, char** argv)
{
	::chipotto::Emulator emulator;
	emulator.LoadFromFile(".\\TICTAC");

	while (true)
	{
		if (!emulator.Tick())
			return -1;
	}
	return 0;
}