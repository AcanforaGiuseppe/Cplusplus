#define CLOVE_SUITE_NAME OpcodesTests
#include "clove-unit.h"
#include "Emulator.h"

chipotto::Emulator emulatorOpcodesTests;

uint8_t GetColorAtCoords(uint8_t* pixels, int pitch, int x, int y)
{
	int pixel_index = x * 4 + pitch * y;
	return pixels[pixel_index];
}

void SetColorAtCoords(uint8_t* pixels, int pitch, int x, int y, uint8_t color)
{
	int pixel_index = x * 4 + pitch * y;
	pixels[pixel_index + 0] = color;
	pixels[pixel_index + 1] = color;
	pixels[pixel_index + 2] = color;
	pixels[pixel_index + 3] = color;
}

CLOVE_SUITE_SETUP_ONCE()
{
	emulatorOpcodesTests.FixedRandomNumber = true;
	emulatorOpcodesTests.LoadFromFile(".\\TICTAC");
	std::cout << std::hex;
}

CLOVE_TEST(Opcode0NotSupportedOpcode)
{
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode0(0x07EE);
	CLOVE_INT_EQ(2, static_cast<int>(status));
}

CLOVE_TEST(Opcode0RETStackOverflow)
{
	emulatorOpcodesTests.SetStackPointerValue(0xFE);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode0(0x00EE);
	CLOVE_INT_EQ(3, static_cast<int>(status));
}

CLOVE_TEST(Opcode0RETSuccessful)
{
	emulatorOpcodesTests.SetStackPointerValue(0x1);
	emulatorOpcodesTests.SetStackValueAtIndex(0x1, 0x200);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode0(0x00EE);
	uint8_t sp = emulatorOpcodesTests.GetStackPointerValue();
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue();
	CLOVE_INT_EQ(0, static_cast<int>(status));
	CLOVE_CHAR_EQ(0x0, sp);
	CLOVE_UINT_EQ(0x200, pc);
}

CLOVE_TEST(Opcode0CLSSuccessful)
{
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode0(0x00E0);
	SDL_Texture* text = emulatorOpcodesTests.GetScreenTexture();
	uint8_t* pixels = nullptr;
	int pitch;
	int result = SDL_LockTexture(text, nullptr, reinterpret_cast<void**>(&pixels), &pitch);

	for (size_t i = 0; i < pitch * emulatorOpcodesTests.GetScreenTextureHeight(); i++)
		CLOVE_CHAR_EQ(0, pixels[i]);

	SDL_UnlockTexture(text);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode1Successful)
{
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode1(0x125E);
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue() + 2;
	CLOVE_UINT_EQ(0x25E, pc);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode2StackOverflow)
{
	emulatorOpcodesTests.SetStackPointerValue(0xF);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode2(0x225E);
	CLOVE_INT_EQ(3, static_cast<int>(status));
}

CLOVE_TEST(Opcode2Successful)
{
	emulatorOpcodesTests.SetStackPointerValue(0x3);
	emulatorOpcodesTests.SetProgramCounterValue(0x200);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode2(0x225E);
	uint8_t sp = emulatorOpcodesTests.GetStackPointerValue();
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue();
	uint16_t stack_value = 0;
	emulatorOpcodesTests.GetStackValueAtIndex(sp, stack_value);
	CLOVE_CHAR_EQ(0x4, sp);
	CLOVE_UINT_EQ(0x25E, pc);
	CLOVE_UINT_EQ(0x200, stack_value);
	CLOVE_INT_EQ(1, static_cast<int>(status));
}

CLOVE_TEST(Opcode3SkipInstruction)
{
	emulatorOpcodesTests.SetProgramCounterValue(0x200);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x5, 0xEE);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode3(0x35EE);
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue();
	CLOVE_UINT_EQ(0x200 + 2, pc);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode3NotSkipInstruction)
{
	emulatorOpcodesTests.SetProgramCounterValue(0x200);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x5, 0xEE);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode3(0x3529);
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue();
	CLOVE_UINT_EQ(0x200, pc);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode4SkipInstruction)
{
	emulatorOpcodesTests.SetProgramCounterValue(0x200);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xD, 0xAB);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode4(0x4DEE);
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue();
	CLOVE_UINT_EQ(0x200 + 2, pc);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode4NotSkipInstruction)
{
	emulatorOpcodesTests.SetProgramCounterValue(0x200);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xD, 0xAB);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode4(0x4DAB);
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue();
	CLOVE_UINT_EQ(0x200, pc);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode5SkipInstruction)
{
	emulatorOpcodesTests.SetProgramCounterValue(0x200);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x1, 0xAB);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x3, 0xAB);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode5(0x5130);
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue();
	CLOVE_UINT_EQ(0x200 + 2, pc);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode5NotSkipInstruction)
{
	emulatorOpcodesTests.SetProgramCounterValue(0x200);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x1, 0xAB);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x3, 0xBA);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode5(0x5130);
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue();
	CLOVE_UINT_EQ(0x200, pc);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode6Successful)
{
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode6(0x6730);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x7, register_value);
	CLOVE_CHAR_EQ(0x30, register_value);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode7SuccessfulFromRegisterValueZero)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x2, 0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode7(0x726B);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x2, register_value);
	CLOVE_CHAR_EQ(0x6B, register_value);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode7SuccessfulFromRegisterValueNotZero)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x2, 0x3);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode7(0x7202);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x2, register_value);
	CLOVE_CHAR_EQ(0x5, register_value);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode7SuccessfulOverflow)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x2, 0xFF);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode7(0x7201);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x2, register_value);
	CLOVE_CHAR_EQ(0x0, register_value);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8NotSupportedOpcode)
{
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x8AA9);
	CLOVE_INT_EQ(2, static_cast<int>(status));
}

CLOVE_TEST(Opcode8LDSuccessful)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xD, 0x47);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x8AD0);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xD, register_value);
	CLOVE_CHAR_EQ(0x47, register_value);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8ORSuccessful)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x4, 0xF0);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x5, 0x0F);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x8451);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x4, register_value);
	CLOVE_CHAR_EQ(0xFF, register_value);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8ANDSuccessful)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x4, 0xF0);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x5, 0x0F);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x8452);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x4, register_value);
	CLOVE_CHAR_EQ(0x00, register_value);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8XORSuccessful)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x4, 0xC3);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x5, 0xFF);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x8453);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x4, register_value);
	CLOVE_CHAR_EQ(0x3C, register_value);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8ADDNotOverflow)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x7, 0x3);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x8, 0x6);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x1);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x8784);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x7, register_value);
	uint8_t carry = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, carry);
	CLOVE_CHAR_EQ(0x9, register_value);
	CLOVE_CHAR_EQ(0x0, carry);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8ADDOverflow)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x7, 0xFF);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x8, 0x2);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x8784);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x7, register_value);
	uint8_t carry = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, carry);
	CLOVE_CHAR_EQ(0x1, register_value);
	CLOVE_CHAR_EQ(0x1, carry);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8SUBNotOverflow)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xE, 0x9);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xD, 0x4);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x8ED5);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xE, register_value);
	uint8_t positive = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, positive);
	CLOVE_CHAR_EQ(0x5, register_value);
	CLOVE_CHAR_EQ(0x1, positive);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8SUBOverflow)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xE, 0x4);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xD, 0x5);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x1);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x8ED5);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xE, register_value);
	uint8_t positive = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, positive);
	CLOVE_CHAR_EQ(0xFF, register_value);
	CLOVE_CHAR_EQ(0x0, positive);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8SHRNotCarryBit)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x9, 0x1E);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x1);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x89D6);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x9, register_value);
	uint8_t carry = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, carry);
	CLOVE_CHAR_EQ(0xF, register_value);
	CLOVE_CHAR_EQ(0x0, carry);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8SHRCarryBit)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x9, 0x1F);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x89D6);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x9, register_value);
	uint8_t carry = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, carry);
	CLOVE_CHAR_EQ(0xF, register_value);
	CLOVE_CHAR_EQ(0x1, carry);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8SUBNNotOverflow)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 0x4);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x1, 0x9);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x8017);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x0, register_value);
	uint8_t positive = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, positive);
	CLOVE_CHAR_EQ(0x5, register_value);
	CLOVE_CHAR_EQ(0x1, positive);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8SUBNOverflow)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 0x5);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x1, 0x4);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x1);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x8017);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x0, register_value);
	uint8_t positive = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, positive);
	CLOVE_CHAR_EQ(0xFF, register_value);
	CLOVE_CHAR_EQ(0x0, positive);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8SHLNotCarryBit)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x9, 0x78);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x1);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x89AE);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x9, register_value);
	uint8_t carry = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, carry);
	CLOVE_CHAR_EQ(0xF0, register_value);
	CLOVE_CHAR_EQ(0x0, carry);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode8SHLCarryBit)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x9, 0xF8);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode8(0x89AE);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x9, register_value);
	uint8_t carry = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, carry);
	CLOVE_CHAR_EQ(0xF0, register_value);
	CLOVE_CHAR_EQ(0x1, carry);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode9SkipInstruction)
{
	emulatorOpcodesTests.SetProgramCounterValue(0x200);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x1, 0xAB);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x3, 0xBA);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode9(0x9130);
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue();
	CLOVE_UINT_EQ(0x200 + 2, pc);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(Opcode9NotSkipInstruction)
{
	emulatorOpcodesTests.SetProgramCounterValue(0x200);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x1, 0xAB);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x3, 0xAB);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.Opcode9(0x9130);
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue();
	CLOVE_UINT_EQ(0x200, pc);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(OpcodeASuccessful)
{
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeA(0xA264);
	uint16_t I = emulatorOpcodesTests.GetIRegisterValue();
	CLOVE_UINT_EQ(0x264, I);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(OpcodeBSuccessful)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 0x05);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeB(0xB264);
	uint16_t pc = emulatorOpcodesTests.GetProgramCounterValue() + 2;
	CLOVE_UINT_EQ(0x264 + 0x05, pc);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(OpcodeCSuccessfulWithNullifyingMask)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeC(0xC000);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x0, register_value);
	CLOVE_UINT_EQ(0, register_value);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(OpcodeCSuccessfulWithPassthroughMask)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeC(0xC0FF);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x0, register_value);
	CLOVE_UINT_EQ(195, register_value);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(OpcodeCSuccessfulWithCustomMask)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeC(0xC0F0);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x0, register_value);
	CLOVE_UINT_EQ(0xC0, register_value);
	CLOVE_INT_EQ(0, static_cast<int>(status));
}

CLOVE_TEST(OpcodeDDraw0SpriteAtCoords0x0)
{
	emulatorOpcodesTests.Opcode0(0x00E0);
	emulatorOpcodesTests.SetIRegisterValue(0x0);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x1);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeD(0xD005);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, register_value);
	CLOVE_CHAR_EQ(0x0, register_value);

	int width = emulatorOpcodesTests.GetScreenTextureWidth();
	int height = emulatorOpcodesTests.GetScreenTextureHeight();
	SDL_Texture* text = emulatorOpcodesTests.GetScreenTexture();
	uint8_t* pixels = nullptr;
	int pitch;
	int result = SDL_LockTexture(text, nullptr, reinterpret_cast<void**>(&pixels), &pitch);
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 0, 0));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 1, 0));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 2, 0));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 3, 0));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 0, 1));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 1, 1));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 2, 1));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 3, 1));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 0, 2));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 1, 2));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 2, 2));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 3, 2));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 0, 3));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 1, 3));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 2, 3));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 3, 3));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 0, 4));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 1, 4));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 2, 4));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 3, 4));
}

CLOVE_TEST(OpcodeDDraw0SpriteAtCoords10x10)
{
	emulatorOpcodesTests.Opcode0(0x00E0);
	emulatorOpcodesTests.SetIRegisterValue(0x0);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x1);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 10);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeD(0xD005);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, register_value);
	CLOVE_CHAR_EQ(0x0, register_value);

	int width = emulatorOpcodesTests.GetScreenTextureWidth();
	int height = emulatorOpcodesTests.GetScreenTextureHeight();
	SDL_Texture* text = emulatorOpcodesTests.GetScreenTexture();
	uint8_t* pixels = nullptr;
	int pitch;
	int result = SDL_LockTexture(text, nullptr, reinterpret_cast<void**>(&pixels), &pitch);
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 10, 10));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 11, 10));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 12, 10));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 13, 10));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 10, 11));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 11, 11));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 12, 11));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 13, 11));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 10, 12));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 11, 12));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 12, 12));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 13, 12));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 10, 13));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 11, 13));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 12, 13));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 13, 13));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 10, 14));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 11, 14));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 12, 14));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 13, 14));
}

CLOVE_TEST(OpcodeDDraw0SpriteAtCoords62x30)
{
	emulatorOpcodesTests.Opcode0(0x00E0);
	emulatorOpcodesTests.SetIRegisterValue(0x0);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x1);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 62);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x1, 30);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeD(0xD015);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, register_value);
	CLOVE_CHAR_EQ(0x0, register_value);

	int width = emulatorOpcodesTests.GetScreenTextureWidth();
	int height = emulatorOpcodesTests.GetScreenTextureHeight();
	SDL_Texture* text = emulatorOpcodesTests.GetScreenTexture();
	uint8_t* pixels = nullptr;
	int pitch;
	int result = SDL_LockTexture(text, nullptr, reinterpret_cast<void**>(&pixels), &pitch);
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 62, 30));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 63, 30));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 0, 30));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 1, 30));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 62, 31));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 63, 31));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 0, 31));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 1, 31));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 62, 0));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 63, 0));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 0, 0));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 1, 0));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 62, 1));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 63, 1));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 0, 1));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 1, 1));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 62, 2));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 63, 2));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 0, 2));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 1, 2));
}

CLOVE_TEST(OpcodeDDraw0SpriteWithCollision)
{
	emulatorOpcodesTests.Opcode0(0x00E0);
	int width = emulatorOpcodesTests.GetScreenTextureWidth();
	int height = emulatorOpcodesTests.GetScreenTextureHeight();
	SDL_Texture* text = emulatorOpcodesTests.GetScreenTexture();
	uint8_t* pixels = nullptr;
	int pitch;
	SDL_LockTexture(text, nullptr, reinterpret_cast<void**>(&pixels), &pitch);
	SetColorAtCoords(pixels, pitch, 0, 0, 0xFF);
	SDL_UnlockTexture(text);

	emulatorOpcodesTests.SetIRegisterValue(0x0);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0xF, 0x0);
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeD(0xD005);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0xF, register_value);
	CLOVE_CHAR_EQ(0x1, register_value);

	SDL_LockTexture(text, nullptr, reinterpret_cast<void**>(&pixels), &pitch);
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 0, 0));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 1, 0));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 2, 0));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 3, 0));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 0, 1));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 1, 1));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 2, 1));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 3, 1));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 0, 2));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 1, 2));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 2, 2));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 3, 2));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 0, 3));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 1, 3));
	CLOVE_UINT_EQ(0x00, GetColorAtCoords(pixels, pitch, 2, 3));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 3, 3));

	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 0, 4));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 1, 4));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 2, 4));
	CLOVE_UINT_EQ(0xFF, GetColorAtCoords(pixels, pitch, 3, 4));
}

CLOVE_TEST(OpcodeFNotSupportedOpcode)
{
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xF001);
	CLOVE_INT_EQ(2, static_cast<int>(status));
}

CLOVE_TEST(OpcodeFx07Successful)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x4, 0x0);
	emulatorOpcodesTests.SetDelayTimerValue(0xFF);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xF407);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x4, register_value);
	CLOVE_CHAR_EQ(0xFF, register_value);
}

CLOVE_TEST(OpcodeFx15Successful)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x4, 0xFF);
	emulatorOpcodesTests.SetDelayTimerValue(0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xF415);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint8_t delay_timer = emulatorOpcodesTests.GetDelayTimerValue();
	CLOVE_CHAR_EQ(0xFF, delay_timer);
}

CLOVE_TEST(OpcodeFx18Successful)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x4, 0xFF);
	emulatorOpcodesTests.SetSoundTimerValue(0x0);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xF418);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint8_t sound_timer = emulatorOpcodesTests.GetSoundTimerValue();
	CLOVE_CHAR_EQ(0xFF, sound_timer);
}

CLOVE_TEST(OpcodeFx1ESuccessful)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x4, 0x3);
	emulatorOpcodesTests.SetIRegisterValue(0x200);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xF41E);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint16_t I = emulatorOpcodesTests.GetIRegisterValue();
	CLOVE_UINT_EQ(0x203, I);
}

CLOVE_TEST(OpcodeFx29Successful)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x4, 0xA);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xF429);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint16_t I = emulatorOpcodesTests.GetIRegisterValue();
	CLOVE_UINT_EQ(0x32, I);
}

CLOVE_TEST(OpcodeFx33NumberLessThan10)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x4, 7);
	emulatorOpcodesTests.SetIRegisterValue(0x500);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xF433);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint16_t I = emulatorOpcodesTests.GetIRegisterValue();
	CLOVE_UINT_EQ(0x500, I);
	uint8_t memory_value = 0;
	emulatorOpcodesTests.GetMemoryMappingValueAtIndex(I, memory_value);
	CLOVE_UINT_EQ(0x0, memory_value);
	emulatorOpcodesTests.GetMemoryMappingValueAtIndex(I + 1, memory_value);
	CLOVE_UINT_EQ(0x0, memory_value);
	emulatorOpcodesTests.GetMemoryMappingValueAtIndex(I + 2, memory_value);
	CLOVE_UINT_EQ(0x7, memory_value);
}

CLOVE_TEST(OpcodeFx33NumberLessThan100)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x4, 48);
	emulatorOpcodesTests.SetIRegisterValue(0x500);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xF433);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint16_t I = emulatorOpcodesTests.GetIRegisterValue();
	CLOVE_UINT_EQ(0x500, I);
	uint8_t memory_value = 0;
	emulatorOpcodesTests.GetMemoryMappingValueAtIndex(I, memory_value);
	CLOVE_UINT_EQ(0x0, memory_value);
	emulatorOpcodesTests.GetMemoryMappingValueAtIndex(I + 1, memory_value);
	CLOVE_UINT_EQ(0x4, memory_value);
	emulatorOpcodesTests.GetMemoryMappingValueAtIndex(I + 2, memory_value);
	CLOVE_UINT_EQ(0x8, memory_value);
}

CLOVE_TEST(OpcodeFx33NumberGreaterThan100)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x4, 253);
	emulatorOpcodesTests.SetIRegisterValue(0x500);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xF433);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint16_t I = emulatorOpcodesTests.GetIRegisterValue();
	CLOVE_UINT_EQ(0x500, I);
	uint8_t memory_value = 0;
	emulatorOpcodesTests.GetMemoryMappingValueAtIndex(I + 0, memory_value);
	CLOVE_UINT_EQ(0x2, memory_value);
	emulatorOpcodesTests.GetMemoryMappingValueAtIndex(I + 1, memory_value);
	CLOVE_UINT_EQ(0x5, memory_value);
	emulatorOpcodesTests.GetMemoryMappingValueAtIndex(I + 2, memory_value);
	CLOVE_UINT_EQ(0x3, memory_value);
}

CLOVE_TEST(OpcodeFx55StoreRegistersFrom0To0)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 10);
	emulatorOpcodesTests.SetMemoryMappingValueAtIndex(0x800, 20);
	emulatorOpcodesTests.SetIRegisterValue(0x800);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xF055);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint16_t I = emulatorOpcodesTests.GetIRegisterValue();
	CLOVE_UINT_EQ(0x800, I);
	uint8_t memory_value = 0;
	emulatorOpcodesTests.GetMemoryMappingValueAtIndex(I, memory_value);
	CLOVE_UINT_EQ(10, memory_value);
}

CLOVE_TEST(OpcodeFx55StoreRegistersFrom0ToF)
{
	for (size_t i = 0; i < 0x10; i++)
	{
		emulatorOpcodesTests.SetRegisterValueAtIndex(0x0 + i, 10 + i);
		emulatorOpcodesTests.SetMemoryMappingValueAtIndex(0x800 + i, 9);
	}

	emulatorOpcodesTests.SetIRegisterValue(0x800);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xFF55);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint16_t I = emulatorOpcodesTests.GetIRegisterValue();
	CLOVE_UINT_EQ(0x800, I);
	uint8_t memory_value = 0;

	for (size_t i = 0; i < 16; i++)
	{
		emulatorOpcodesTests.GetMemoryMappingValueAtIndex(I + i, memory_value);
		CLOVE_UINT_EQ(10 + i, memory_value);
	}
}

CLOVE_TEST(OpcodeFx55ReadRegistersFrom0To0)
{
	emulatorOpcodesTests.SetRegisterValueAtIndex(0x0, 10);
	emulatorOpcodesTests.SetMemoryMappingValueAtIndex(0x800, 20);
	emulatorOpcodesTests.SetIRegisterValue(0x800);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xF065);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint16_t I = emulatorOpcodesTests.GetIRegisterValue();
	CLOVE_UINT_EQ(0x800, I);
	uint8_t register_value = 0;
	emulatorOpcodesTests.GetRegisterValueAtIndex(0x0, register_value);
	CLOVE_UINT_EQ(20, register_value);
}

CLOVE_TEST(OpcodeFx55ReadRegistersFrom0ToF)
{
	for (size_t i = 0; i < 0x10; i++)
	{
		emulatorOpcodesTests.SetRegisterValueAtIndex(i, 10);
		emulatorOpcodesTests.SetMemoryMappingValueAtIndex(0x800 + i, 20 + i);
	}

	emulatorOpcodesTests.SetIRegisterValue(0x800);
	chipotto::OpcodeStatus status = emulatorOpcodesTests.OpcodeF(0xFF65);
	CLOVE_INT_EQ(0, static_cast<int>(status));
	uint16_t I = emulatorOpcodesTests.GetIRegisterValue();
	CLOVE_UINT_EQ(0x800, I);
	uint8_t register_value = 0;

	for (size_t i = 0; i < 16; i++)
	{
		emulatorOpcodesTests.GetRegisterValueAtIndex(i, register_value);
		CLOVE_UINT_EQ(20 + i, register_value);
	}
}