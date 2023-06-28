#include "Emulator.h"

chipotto::Emulator::Emulator()
{
	KeyboardMap[SDLK_1] = 0x0;
	KeyboardMap[SDLK_2] = 0x1;
	KeyboardMap[SDLK_3] = 0x2;
	KeyboardMap[SDLK_4] = 0x3;
	KeyboardMap[SDLK_q] = 0x4;
	KeyboardMap[SDLK_w] = 0x5;
	KeyboardMap[SDLK_e] = 0x6;
	KeyboardMap[SDLK_r] = 0x7;
	KeyboardMap[SDLK_a] = 0x8;
	KeyboardMap[SDLK_s] = 0x9;
	KeyboardMap[SDLK_d] = 0xA;
	KeyboardMap[SDLK_f] = 0xB;
	KeyboardMap[SDLK_z] = 0xC;
	KeyboardMap[SDLK_x] = 0xD;
	KeyboardMap[SDLK_c] = 0xE;
	KeyboardMap[SDLK_v] = 0xF;

	for (const auto& pair : KeyboardMap)
		KeyboardValuesMap[pair.second] = static_cast<SDL_Scancode>(pair.first);

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
	Opcodes[0xA] = std::bind(&Emulator::OpcodeA, this, std::placeholders::_1);
	Opcodes[0xB] = std::bind(&Emulator::OpcodeB, this, std::placeholders::_1);
	Opcodes[0xC] = std::bind(&Emulator::OpcodeC, this, std::placeholders::_1);
	Opcodes[0xD] = std::bind(&Emulator::OpcodeD, this, std::placeholders::_1);
	Opcodes[0xE] = std::bind(&Emulator::OpcodeE, this, std::placeholders::_1);
	Opcodes[0xF] = std::bind(&Emulator::OpcodeF, this, std::placeholders::_1);

	MemoryMapping[0x00] = 0xF0;
	MemoryMapping[0x01] = 0x90;
	MemoryMapping[0x02] = 0x90;
	MemoryMapping[0x03] = 0x90;
	MemoryMapping[0x04] = 0xF0;

	MemoryMapping[0x05] = 0x20;
	MemoryMapping[0x06] = 0x60;
	MemoryMapping[0x07] = 0x20;
	MemoryMapping[0x08] = 0x20;
	MemoryMapping[0x09] = 0x70;

	MemoryMapping[0x0A] = 0xF0;
	MemoryMapping[0x0B] = 0x10;
	MemoryMapping[0x0C] = 0xF0;
	MemoryMapping[0x0D] = 0x80;
	MemoryMapping[0x0E] = 0xF0;

	MemoryMapping[0x0F] = 0xF0;
	MemoryMapping[0x10] = 0x10;
	MemoryMapping[0x11] = 0xF0;
	MemoryMapping[0x12] = 0x10;
	MemoryMapping[0x13] = 0xF0;

	MemoryMapping[0x14] = 0x90;
	MemoryMapping[0x15] = 0x90;
	MemoryMapping[0x16] = 0xF0;
	MemoryMapping[0x17] = 0x10;
	MemoryMapping[0x18] = 0x10;

	MemoryMapping[0x19] = 0xF0;
	MemoryMapping[0x1A] = 0x80;
	MemoryMapping[0x1B] = 0xF0;
	MemoryMapping[0x1C] = 0x10;
	MemoryMapping[0x1D] = 0xF0;

	MemoryMapping[0x1E] = 0xF0;
	MemoryMapping[0x1F] = 0x80;
	MemoryMapping[0x20] = 0xF0;
	MemoryMapping[0x21] = 0x90;
	MemoryMapping[0x22] = 0xF0;

	MemoryMapping[0x23] = 0xF0;
	MemoryMapping[0x24] = 0x10;
	MemoryMapping[0x25] = 0x20;
	MemoryMapping[0x26] = 0x40;
	MemoryMapping[0x27] = 0x40;

	MemoryMapping[0x28] = 0xF0;
	MemoryMapping[0x29] = 0x90;
	MemoryMapping[0x2A] = 0xF0;
	MemoryMapping[0x2B] = 0x90;
	MemoryMapping[0x2C] = 0xF0;

	MemoryMapping[0x2D] = 0xF0;
	MemoryMapping[0x2E] = 0x90;
	MemoryMapping[0x2F] = 0xF0;
	MemoryMapping[0x30] = 0x10;
	MemoryMapping[0x31] = 0xF0;

	MemoryMapping[0x32] = 0xF0;
	MemoryMapping[0x33] = 0x90;
	MemoryMapping[0x34] = 0xF0;
	MemoryMapping[0x35] = 0x90;
	MemoryMapping[0x36] = 0x90;

	MemoryMapping[0x37] = 0xE0;
	MemoryMapping[0x38] = 0x90;
	MemoryMapping[0x39] = 0xE0;
	MemoryMapping[0x3A] = 0x90;
	MemoryMapping[0x3B] = 0xE0;

	MemoryMapping[0x3C] = 0xF0;
	MemoryMapping[0x3D] = 0x80;
	MemoryMapping[0x3E] = 0x80;
	MemoryMapping[0x3F] = 0x80;
	MemoryMapping[0x40] = 0xF0;

	MemoryMapping[0x41] = 0xE0;
	MemoryMapping[0x42] = 0x90;
	MemoryMapping[0x43] = 0x90;
	MemoryMapping[0x44] = 0x90;
	MemoryMapping[0x45] = 0xE0;

	MemoryMapping[0x46] = 0xF0;
	MemoryMapping[0x47] = 0x80;
	MemoryMapping[0x48] = 0xF0;
	MemoryMapping[0x49] = 0x80;
	MemoryMapping[0x4A] = 0xF0;

	MemoryMapping[0x4B] = 0xF0;
	MemoryMapping[0x4C] = 0x80;
	MemoryMapping[0x4D] = 0xF0;
	MemoryMapping[0x4E] = 0x80;
	MemoryMapping[0x4F] = 0x80;

	Window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * 10, height * 10, 0);

	if (!Window)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return;
	}

	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!Renderer)
	{
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		SDL_DestroyWindow(Window);
		return;
	}

	Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);

	if (!Texture)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
		SDL_DestroyRenderer(Renderer);
		SDL_DestroyWindow(Window);
		return;
	}
}

bool chipotto::Emulator::LoadFromFile(std::filesystem::path Path)
{
	std::ifstream file;
	file.open(Path, std::ios::binary);

	if (!file.is_open())
		return false;

	auto file_size = std::filesystem::file_size(Path);

	file.read(reinterpret_cast<char*>(MemoryMapping.data() + PC), file_size);
	file.close();
}

bool chipotto::Emulator::Tick()
{
	uint64_t tick = SDL_GetTicks64();

	if (DelayTimer > 0 && tick >= DeltaTimerTicks)
	{
		DelayTimer--;
		DeltaTimerTicks = 17 + SDL_GetTicks64();
	}

	if (SoundTimer > 0 && tick >= DeltaTimerTicks)
	{
		SoundTimer--;
		DeltaTimerTicks = 17 + SDL_GetTicks64();
	}

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			SDL_Keycode key = event.key.keysym.sym;

			if (KeyboardMap.contains(key))
			{
				Registers[WaitForKeyboardRegister_Index] = KeyboardMap[key];
				Suspended = false;
				PC += 2;
			}
		}

		if (event.type == SDL_QUIT)
			return false;
	}
	SDL_PumpEvents();

	if (Suspended)
		return true;

	uint16_t opcode = MemoryMapping[PC + 1] + (static_cast<uint16_t>(MemoryMapping[PC]) << 8);
	std::cout << std::hex << "0x" << PC << ": 0x" << opcode << "  -->  ";

	OpcodeStatus status = Opcodes[opcode >> 12](opcode);

	std::cout << std::endl;

	if (status == OpcodeStatus::IncrementPC)
		PC += 2;

	return status != OpcodeStatus::NotImplemented && status != OpcodeStatus::StackOverflow && status != OpcodeStatus::Error;
}

bool chipotto::Emulator::IsValid() const
{
	if (!Window || !Renderer || !Texture)
		return false;

	return true;
}

uint8_t chipotto::Emulator::GetRandomNumber(uint8_t mask)
{
	if (FixedRandomNumber)
		return 195 & mask;
	else
		return (std::rand() % 256) & mask;
}

chipotto::OpcodeStatus chipotto::Emulator::Opcode0(const uint16_t opcode)
{
	if ((opcode & 0xFFF) == 0x0E0)
	{
		std::cout << "CLS";
		uint8_t* pixels = nullptr;
		int pitch;
		int result = SDL_LockTexture(Texture, nullptr, reinterpret_cast<void**>(&pixels), &pitch);
		memset(pixels, 0, pitch * height);
		SDL_UnlockTexture(Texture);
		SDL_RenderCopy(Renderer, Texture, nullptr, nullptr);
		SDL_RenderPresent(Renderer);
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xFFF) == 0x0EE)
	{
		if (SP > 0xF && SP < 0xFF)
			return OpcodeStatus::StackOverflow;

		std::cout << "RET";
		PC = Stack[SP & 0xF];
		SP -= 1;
		return OpcodeStatus::IncrementPC;
	}
	return OpcodeStatus::NotImplemented;
}

chipotto::OpcodeStatus chipotto::Emulator::Opcode1(const uint16_t opcode)
{
	uint16_t address = opcode & 0x0FFF;
	std::cout << "JP 0x" << (int)address;
	PC = address - 2;
	return OpcodeStatus::IncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::Opcode2(const uint16_t opcode)
{
	uint16_t address = opcode & 0xFFF;
	std::cout << "CALL 0x" << (int)address;

	if (SP > 0xF)
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
	PC = address;
	return OpcodeStatus::NotIncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::Opcode3(const uint16_t opcode)
{
	uint8_t register_index = (opcode >> 8) & 0xF;
	uint8_t value = opcode & 0xFF;
	std::cout << "SE V" << (int)register_index << ", 0x" << (int)value;

	if (Registers[register_index] == value)
		PC += 2;

	return OpcodeStatus::IncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::Opcode4(const uint16_t opcode)
{
	uint8_t register_index = (opcode >> 8) & 0xF;
	uint8_t value = opcode & 0xFF;
	std::cout << "SNE V" << (int)register_index << ", 0x" << (int)value;

	if (Registers[register_index] != value)
		PC += 2;

	return OpcodeStatus::IncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::Opcode5(const uint16_t opcode)
{
	uint8_t registerX_index = (opcode >> 8) & 0xF;
	uint8_t registerY_index = (opcode >> 4) & 0xF;
	std::cout << "SE V" << (int)registerX_index << ", V" << (int)registerY_index;

	if (Registers[registerX_index] == Registers[registerY_index])
		PC += 2;

	return OpcodeStatus::IncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::Opcode6(const uint16_t opcode)
{
	uint8_t register_index = (opcode >> 8) & 0xF;
	uint8_t register_value = opcode & 0xFF;
	Registers[register_index] = register_value;
	std::cout << "LD V" << (int)register_index << ", 0x" << (int)register_value;
	return OpcodeStatus::IncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::Opcode7(const uint16_t opcode)
{
	uint8_t register_index = (opcode >> 8) & 0xF;
	uint8_t value = opcode & 0xFF;
	std::cout << "ADD V" << (int)register_index << ", 0x" << (int)value;
	Registers[register_index] += value;
	return OpcodeStatus::IncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::Opcode8(const uint16_t opcode)
{
	if ((opcode & 0xF) == 0x0)
	{
		uint8_t registerX_index = (opcode >> 8) & 0xF;
		uint8_t registerY_index = (opcode >> 4) & 0xF;
		Registers[registerX_index] = Registers[registerY_index];
		std::cout << "LD V" << (int)registerX_index << ", V" << (int)registerY_index;
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xF) == 0x1)
	{
		uint8_t registerX_index = (opcode >> 8) & 0xF;
		uint8_t registerY_index = (opcode >> 4) & 0xF;
		Registers[registerX_index] |= Registers[registerY_index];
		std::cout << "OR V" << (int)registerX_index << ", V" << (int)registerY_index;
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xF) == 0x2)
	{
		uint8_t registerX_index = (opcode >> 8) & 0xF;
		uint8_t registerY_index = (opcode >> 4) & 0xF;
		Registers[registerX_index] &= Registers[registerY_index];
		std::cout << "AND V" << (int)registerX_index << ", V" << (int)registerY_index;
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xF) == 0x3)
	{
		uint8_t registerX_index = (opcode >> 8) & 0xF;
		uint8_t registerY_index = (opcode >> 4) & 0xF;
		Registers[registerX_index] ^= Registers[registerY_index];
		std::cout << "XOR V" << (int)registerX_index << ", V" << (int)registerY_index;
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xF) == 0x4)
	{
		uint8_t registerX_index = (opcode >> 8) & 0xF;
		uint8_t registerY_index = (opcode >> 4) & 0xF;
		int result = static_cast<int>(Registers[registerX_index]) + Registers[registerY_index];

		if (result > 255)
			Registers[0xF] = 1;
		else
			Registers[0xF] = 0;

		Registers[registerX_index] += Registers[registerY_index];
		std::cout << "ADD V" << (int)registerX_index << ", V" << (int)registerY_index;
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xF) == 0x5)
	{
		uint8_t registerX_index = (opcode >> 8) & 0xF;
		uint8_t registerY_index = (opcode >> 4) & 0xF;

		if (Registers[registerX_index] > Registers[registerY_index])
			Registers[0xF] = 1;
		else
			Registers[0xF] = 0;

		Registers[registerX_index] -= Registers[registerY_index];
		std::cout << "SUB V" << (int)registerX_index << ", V" << (int)registerY_index;
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xF) == 0x6)
	{
		uint8_t registerX_index = (opcode >> 8) & 0xF;
		uint8_t registerY_index = (opcode >> 4) & 0xF;
		Registers[0xF] = Registers[registerX_index] & 0x1;
		Registers[registerX_index] >>= 1;
		std::cout << "SHR V" << (int)registerX_index << "{, V" << (int)registerY_index << "}";
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xF) == 0x7)
	{
		uint8_t registerX_index = (opcode >> 8) & 0xF;
		uint8_t registerY_index = (opcode >> 4) & 0xF;

		if (Registers[registerX_index] < Registers[registerY_index])
			Registers[0xF] = 1;
		else
			Registers[0xF] = 0;

		Registers[registerX_index] = Registers[registerY_index] - Registers[registerX_index];
		std::cout << "SUBN V" << (int)registerX_index << ", V" << (int)registerY_index;
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xF) == 0xE)
	{
		uint8_t registerX_index = (opcode >> 8) & 0xF;
		uint8_t registerY_index = (opcode >> 4) & 0xF;
		Registers[0xF] = Registers[registerX_index] >> 7;
		Registers[registerX_index] <<= 1;
		std::cout << "SHL V" << (int)registerX_index << "{, V" << (int)registerY_index << "}";
		return OpcodeStatus::IncrementPC;
	}
	else
	{
		return OpcodeStatus::NotImplemented;
	}
}

chipotto::OpcodeStatus chipotto::Emulator::Opcode9(const uint16_t opcode)
{
	uint8_t registerX_index = (opcode >> 8) & 0xF;
	uint8_t registerY_index = (opcode >> 4) & 0xF;
	std::cout << "SNE V" << (int)registerX_index << ", V" << (int)registerY_index;

	if (Registers[registerX_index] != Registers[registerY_index])
		PC += 2;

	return OpcodeStatus::IncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::OpcodeA(const uint16_t opcode)
{
	uint16_t value = (opcode & 0xFFF);
	std::cout << "LD I, 0x" << (int)value;
	I = value;
	return OpcodeStatus::IncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::OpcodeB(const uint16_t opcode)
{
	uint16_t address = opcode & 0x0FFF;
	std::cout << "JP V0, 0x" << address;
	PC = Registers[0x0] + address - 2;
	return OpcodeStatus::IncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::OpcodeC(const uint16_t opcode)
{
	uint8_t register_index = (opcode >> 8) & 0xF;
	uint8_t random_mask = opcode & 0xFF;
	std::cout << "RND V" << (int)register_index << ", 0x" << (int)random_mask;
	Registers[register_index] = GetRandomNumber(random_mask);
	return OpcodeStatus::IncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::OpcodeD(const uint16_t opcode)
{
	uint8_t registerX_index = (opcode >> 8) & 0xF;
	uint8_t registerY_index = (opcode >> 4) & 0xF;
	uint8_t sprite_height = opcode & 0xF;
	std::cout << "DRW V" << (int)registerX_index << ", V" << (int)registerY_index << ", " << (int)sprite_height;

	uint8_t x_coord = Registers[registerX_index] % width;
	uint8_t y_coord = Registers[registerY_index] % height;
	uint8_t* pixels = nullptr;
	int pitch;
	int result = SDL_LockTexture(Texture, nullptr, reinterpret_cast<void**>(&pixels), &pitch);

	if (result != 0)
	{
		SDL_Log("Failed to lock texture");
		return OpcodeStatus::Error;
	}

	Registers[0xF] = 0x0;

	for (int y = 0; y < sprite_height; ++y)
	{
		int final_y_coord = (y + y_coord) % height;
		uint8_t row_byte = MemoryMapping[I + y];
		for (int x = 0; x < 8; x++)
		{
			uint8_t pixel_color = (row_byte >> (7 - x)) & 0x1;
			uint8_t color = 0;

			if (pixel_color)
				color = 0xFF;

			int final_x_coord = (x + x_coord) % width;
			int pixel_index = final_x_coord * 4 + pitch * final_y_coord;
			uint8_t existing_pixel = pixels[pixel_index];
			color ^= existing_pixel;

			if (existing_pixel != 0 && color == 0)
				Registers[0xF] = 0x1;

			pixels[pixel_index + 0] = color;
			pixels[pixel_index + 1] = color;
			pixels[pixel_index + 2] = color;
			pixels[pixel_index + 3] = color;
		}
	}

	SDL_UnlockTexture(Texture);
	SDL_RenderCopy(Renderer, Texture, nullptr, nullptr);
	SDL_RenderPresent(Renderer);
	return OpcodeStatus::IncrementPC;
}

chipotto::OpcodeStatus chipotto::Emulator::OpcodeE(const uint16_t opcode)
{
	if ((opcode & 0xFF) == 0x9E)
	{
		uint8_t register_index = (opcode >> 8) & 0xF;
		std::cout << "SKP V" << (int)register_index;
		const uint8_t* keys_state = SDL_GetKeyboardState(nullptr);

		if (keys_state[KeyboardValuesMap[Registers[register_index]]] == 1)
			PC += 2;

		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xFF) == 0xA1)
	{
		uint8_t register_index = (opcode >> 8) & 0xF;
		std::cout << "SKNP V" << (int)register_index;
		const uint8_t* keys_state = SDL_GetKeyboardState(nullptr);

		if (keys_state[KeyboardValuesMap[Registers[register_index]]] == 0)
			PC += 2;

		return OpcodeStatus::IncrementPC;
	}
	return OpcodeStatus::NotImplemented;
}

chipotto::OpcodeStatus chipotto::Emulator::OpcodeF(const uint16_t opcode)
{
	if ((opcode & 0xFF) == 0x07)
	{
		uint8_t register_index = (opcode >> 8) & 0xF;
		std::cout << "LD V" << (int)register_index << ", DT";
		Registers[register_index] = DelayTimer;
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xFF) == 0x0A)
	{
		uint8_t register_index = (opcode >> 8) & 0xF;
		std::cout << "LD V" << (int)register_index << ", K";
		WaitForKeyboardRegister_Index = register_index;
		Suspended = true;
		return OpcodeStatus::WaitForKeyboard;
	}
	else if ((opcode & 0xFF) == 0x15)
	{
		uint8_t register_index = (opcode >> 8) & 0xF;
		std::cout << "LD DT, V" << (int)register_index;
		DelayTimer = Registers[register_index];
		DeltaTimerTicks = 17 + SDL_GetTicks64();
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xFF) == 0x18)
	{
		uint8_t register_index = (opcode >> 8) & 0xF;
		std::cout << "LD ST, V" << (int)register_index;
		SoundTimer = Registers[register_index];
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xFF) == 0x1E)
	{
		uint8_t register_index = (opcode >> 8) & 0xF;
		std::cout << "ADD I, V" << (int)register_index;
		I += Registers[register_index];
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xFF) == 0x29)
	{
		uint8_t register_index = (opcode >> 8) & 0xF;
		std::cout << "LD F, V" << (int)register_index;
		I = 5 * Registers[register_index];
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xFF) == 0x33)
	{
		uint8_t register_index = (opcode >> 8) & 0xF;
		uint8_t value = Registers[register_index];

		uint8_t hundreds = value / 100;
		uint8_t tens = (value - (hundreds * 100)) / 10;
		uint8_t ones = value % 10;
		MemoryMapping[I + 0] = hundreds;
		MemoryMapping[I + 1] = tens;
		MemoryMapping[I + 2] = ones;

		std::cout << "LD B, V" << (int)register_index;
		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xFF) == 0x55)
	{
		uint8_t register_index = (opcode >> 8) & 0xF;
		std::cout << "LD [I], V" << (int)register_index;

		for (uint8_t i = 0; i <= register_index; ++i)
			MemoryMapping[I + i] = Registers[i];

		return OpcodeStatus::IncrementPC;
	}
	else if ((opcode & 0xFF) == 0x65)
	{
		uint8_t register_index = (opcode >> 8) & 0xF;
		std::cout << "LD V" << (int)register_index << ", [I]";

		for (uint8_t i = 0; i <= register_index; ++i)
			Registers[i] = MemoryMapping[I + i];

		return OpcodeStatus::IncrementPC;
	}
	else
	{
		return OpcodeStatus::NotImplemented;
	}
}

bool chipotto::Emulator::GetRegisterValueAtIndex(uint8_t register_index, uint8_t& return_value) const
{
	if (register_index >= 16 || register_index < 0)
	{
		return_value = 0;
		return false;
	}

	return_value = Registers[register_index];
	return true;
}

bool chipotto::Emulator::SetRegisterValueAtIndex(uint8_t register_index, uint8_t new_value)
{
	if (register_index >= 16 || register_index < 0)
		return false;

	Registers[register_index] = new_value;
	return true;
}

bool chipotto::Emulator::GetMemoryMappingValueAtIndex(uint16_t memory_index, uint8_t& return_value) const
{
	if (memory_index > 0xFFF || memory_index < 0x0)
	{
		return_value = 0;
		return false;
	}

	return_value = MemoryMapping[memory_index];
	return true;
}

bool chipotto::Emulator::SetMemoryMappingValueAtIndex(uint16_t memory_index, uint8_t new_value)
{
	if (memory_index > 0xFFF || memory_index < 0x0)
		return false;

	MemoryMapping[memory_index] = new_value;
	return true;
}

bool chipotto::Emulator::GetStackValueAtIndex(uint8_t stack_index, uint16_t& return_value) const
{
	if (stack_index >= 16 || stack_index < 0)
	{
		return_value = 0;
		return false;
	}

	return_value = Stack[stack_index];
	return true;
}

bool chipotto::Emulator::SetStackValueAtIndex(uint8_t stack_index, uint16_t new_value)
{
	if (stack_index >= 16 || stack_index < 0)
		return false;

	Stack[stack_index] = new_value;
	return true;
}

uint16_t chipotto::Emulator::GetIRegisterValue() const
{
	return I;
}

void chipotto::Emulator::SetIRegisterValue(uint16_t new_value)
{
	I = new_value;
}

uint16_t chipotto::Emulator::GetProgramCounterValue() const
{
	return PC;
}

void chipotto::Emulator::SetProgramCounterValue(uint16_t new_value)
{
	PC = new_value;
}

uint8_t chipotto::Emulator::GetStackPointerValue() const
{
	return SP;
}

void chipotto::Emulator::SetStackPointerValue(uint8_t new_value)
{
	SP = new_value;
}

uint8_t chipotto::Emulator::GetDelayTimerValue()
{
	return DelayTimer;
}

void chipotto::Emulator::SetDelayTimerValue(uint8_t new_value)
{
	DelayTimer = new_value;
}

uint8_t chipotto::Emulator::GetSoundTimerValue()
{
	return SoundTimer;
}

void chipotto::Emulator::SetSoundTimerValue(uint8_t new_value)
{
	SoundTimer = new_value;
}

SDL_Texture* chipotto::Emulator::GetScreenTexture()
{
	return Texture;
}

int chipotto::Emulator::GetScreenTextureWidth()
{
	return width;
}

int chipotto::Emulator::GetScreenTextureHeight()
{
	return height;
}