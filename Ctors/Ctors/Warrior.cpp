#include <vector>
#include <memory>
#include <iostream>

class Warrior
{
public:
	Warrior()
	{
		std::cout << "unknown" << std::endl;
		name = "unknown";
	}

	Warrior(const std::string& in_name) : name(in_name)
	{
		std::cout << "Warrior() ctor for " << this << " [" << name << "]" << std::endl;
	}

	Warrior(const Warrior& other)
	{
		std::cout << "Warrior() copy ctor from" << &other << " to " << this << std::endl;
	}

	Warrior& operator=(const Warrior& other)
	{
		std::cout << "Warrior() assignment operator from" << &other << " to " << this << std::endl;
		return *this;
	}

	// Move constructors can not throw exceptions - so they'll be declared as "noexcept"
	Warrior(Warrior&& other) noexcept
	{
		std::cout << "Warrior() move operator from" << &other << " to " << this << std::endl;
	}

	~Warrior()
	{
		std::cout << "Warrior() ~dtor for " << this << " [" << name << "]" << std::endl;
	}

	std::string name;
};

void Attack(Warrior&& warrior0, Warrior&& warrior1)
{
	std::cout << &warrior0 << "Attack()" << &warrior1 << std::endl;
}

Warrior Temp(Warrior warrior)
{
	std::cout << "Temp " << &warrior << std::endl;
	return warrior;
}

int main(int argc, char** argv)
{
	// lvalue - rvalue

	Warrior warrior10 = Temp(Warrior("Temp Warrior"));
	Warrior warrior11 = Temp(warrior10);

	std::vector<std::shared_ptr<Warrior>> warriors(100);

	auto dummy000 = std::make_shared<Warrior>("dummy");
	auto dummy001 = std::make_shared<Warrior>("dummy");
	warriors.push_back(dummy000);
	warriors.push_back(dummy001);

	std::getchar();

	Warrior warrior("Base Warrior");
	Warrior warrior2 = warrior;
	Warrior warrior3("Warrior3");
	warrior3 = warrior2;
	Warrior& warrior4 = warrior;
	const Warrior& warrior5 = warrior4;

	std::getchar();

	Warrior warrior6("Warrior6");
	Warrior warrior7("Warrior7");
	//Warrior warrior8 = std::move(warrior7);

	std::getchar();

	//Attack(std::move(warrior6), std::move(warrior7));

	// Temp:
	Attack(Warrior("Temp Warrior0"), Warrior("Temp Warrior1"));

	std::getchar();

	return 0;
}