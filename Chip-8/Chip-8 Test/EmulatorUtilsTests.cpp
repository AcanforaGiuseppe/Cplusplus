#define CLOVE_SUITE_NAME EmulatorUtilsTest
#include "clove-unit.h"
#include "Emulator.h"

chipotto::Emulator emulatorUtils;

CLOVE_SUITE_SETUP_ONCE()
{
	emulatorUtils.LoadFromFile(".\\TICTAC");
}

CLOVE_TEST(GetRegisterValueOutOfBounds)
{
	uint8_t value = 100;
	bool result = emulatorUtils.GetRegisterValueAtIndex(17, value);
	CLOVE_CHAR_EQ(0, value);
	CLOVE_IS_FALSE(result);
}

CLOVE_TEST(GetRegisterValueInBounds)
{
	uint8_t value = 100;
	bool result = emulatorUtils.GetRegisterValueAtIndex(4, value);
	CLOVE_CHAR_EQ(0, value);
	CLOVE_IS_TRUE(result);
}

CLOVE_TEST(SetRegisterValueOutOfBounds)
{
	uint8_t new_value = 0x38;
	bool result = emulatorUtils.SetRegisterValueAtIndex(17, new_value);
	CLOVE_IS_FALSE(result);
}

CLOVE_TEST(SetRegisterValueInBounds)
{
	uint8_t new_value = 0x38;
	bool result = emulatorUtils.SetRegisterValueAtIndex(4, new_value);
	CLOVE_IS_TRUE(result);
	uint8_t return_value = 0;
	emulatorUtils.GetRegisterValueAtIndex(4, return_value);
	CLOVE_CHAR_EQ(0x38, return_value);
}

CLOVE_TEST(GetMemoryMappingValueOutOfBounds)
{
	uint8_t value = 100;
	bool result = emulatorUtils.GetMemoryMappingValueAtIndex(0x1000, value);
	CLOVE_CHAR_EQ(0, value);
	CLOVE_IS_FALSE(result);
}

CLOVE_TEST(GetMemoryMappingValueInBounds)
{
	uint8_t value = 100;
	bool result = emulatorUtils.GetMemoryMappingValueAtIndex(69, value);
	CLOVE_UINT_EQ(0xE0, value);
	CLOVE_IS_TRUE(result);
}

CLOVE_TEST(SetMemoryMappingValueOutOfBounds)
{
	uint8_t new_value = 100;
	bool result = emulatorUtils.SetMemoryMappingValueAtIndex(0x1000, new_value);
	CLOVE_IS_FALSE(result);
}

CLOVE_TEST(SetMemoryMappingValueInBounds)
{
	uint8_t new_value = 0xEA;
	bool result = emulatorUtils.SetMemoryMappingValueAtIndex(69, new_value);
	CLOVE_IS_TRUE(result);
	uint8_t return_value = 0;
	emulatorUtils.GetMemoryMappingValueAtIndex(69, return_value);
	CLOVE_CHAR_EQ(0xEA, return_value);
}