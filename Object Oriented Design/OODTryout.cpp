#include <iostream>

/* SOLID */

class LoggerV1
{
public:
	void LogInfo() { }
	void LogWarning() { }
	void LogError() { }
};

class ILoggerV2
{
public:
	virtual void Write() = 0;
};

class InfoLoggerV2 : ILoggerV2
{
public:
	void Write()
	{
		// Info
	}
};

class WarnLoggerV2 : ILoggerV2
{
public:
	void Write()
	{
		// Warn
	}
};

class ErroLoggerV2 : ILoggerV2
{
public:
	void Write()
	{
		// Erro
	}
};


class LoggerV1_1
{
public:
	LoggerV1_1(ILoggerV2* info, ILoggerV2* warn, ILoggerV2* erro) : _info(info), _warn(warn), _erro(erro) { }

	void LogInfo() { _info->Write(); }
	void LogWarning() { _warn->Write(); }
	void LogError() { _erro->Write(); }

private:
	ILoggerV2* _info;
	ILoggerV2* _warn;
	ILoggerV2* _erro;
};

class Client
{
public:
	void DoSome(ILoggerV2* logger, int logType)
	{
		logger->Write();
		//if (logType == Info)
		//logger.LogInfo
	}
};

/* FACTORY */

class Player
{
public:
	Player(std::string name, int hp) { }
};


class IPlayerFactory
{
public:
	virtual Player* Create() = 0;
};

class ExcelPlayerFactory : IPlayerFactory
{
public:
	ExcelPlayerFactory(std::string excelFilePath) { }

	Player* Create()
	{
		//Parsing Excel
		return new Player("Excel", 10);
	}
};


class PropsPlayerFactory : IPlayerFactory
{
public:
	PropsPlayerFactory(std::string propsFilePath) { }

	Player* Create()
	{
		// Parsing props
		return new Player("Excel", 10);
	}
};

/* BUILDER */

enum Species
{
	ELF,
	HUMAN
};

class Avatar
{
public:
	Avatar(std::string name, Species species) { }
};

class AvatarBuilder
{
public:
	AvatarBuilder()
	{
		FinalName = "NoName";
		FinalSpecies = Species::ELF;
	}

	void SetName(std::string InName)
	{
		FinalName = InName;
	}

	void SetSpecies(int species)
	{
		if (species == 1)
		{
			FinalSpecies = Species::ELF;
		}
		else if (species == 2)
		{
			FinalSpecies = Species::HUMAN;
		}
	}

	Avatar* Result()
	{
		return new Avatar(FinalName, FinalSpecies);
	}

private:
	std::string FinalName;
	Species FinalSpecies;
};

/* SINGLETON */

//Singleton.h
class Singleton
{
public:
	static Singleton* GetInstance()
	{
		if (Instance == nullptr)
			Instance = new Singleton();

		return Instance;
	}

	void DoSome() {	}

private:
	Singleton() { }
	inline static Singleton* Instance = nullptr;
	//static Singleton* Instance; // With initialization in cpp file
};

//Singleton.cpp
//#include "Singleton.h"
//Singleton* Singleton::Instance = nullptr;

class SingletonV2
{
public:
	static SingletonV2* GetInstance()
	{
		static SingletonV2* Instance = new SingletonV2();
		return Instance;
	}

	void DoSome() {	}

private:
	SingletonV2() { }
};

class SingletonClient
{
public:
	void Execute()
	{
		// Do 1
		// Do 2

		auto* s = Singleton::GetInstance();
		s->DoSome();

		// Do 3
		// Do 4
	}
};

class SingletonClientV2
{
public:
	SingletonClientV2(Singleton* instance) : _instance(instance) { }

	void Execute()
	{
		// Do 1
		// Do 2

		_instance->DoSome();

		// Do 3
		// Do 4
	}
private:
	Singleton* _instance;
};

/* ADAPTER */

class MyCalc
{
public:
	void SetA(int a) { }
	void SetB(int b) { }
	int Sum() { /* a + b */ }
};

class MyCalcAdapter
{
public:
	MyCalcAdapter(MyCalc* calc) { _calc = calc; }

	int Sum(int a, int b)
	{
		_calc->SetA(a);
		_calc->SetB(b);
		return _calc->Sum();
	}

private:
	MyCalc* _calc;
};

/* DECORATOR */

class ILogger
{
public:
	virtual void Info() = 0;
	virtual void Warn() = 0;
	virtual void Erro() = 0;
};

class ConsoleLogger : public ILogger
{
public:
	void Info() override { }
	void Warn() override { }
	void Erro() override { }
};

class SendEmailConsoleLogger : public ConsoleLogger
{
public:
	void Error()
	{
		// Send Email
		SendEmailConsoleLogger::Erro();
	}
};

class SendEmailDecoratorLogger : public ILogger
{
public:
	SendEmailDecoratorLogger(ILogger* logger) : _logger(logger) { }

	void Info() { _logger->Info(); }
	void Warn() { _logger->Warn(); }
	void Erro()
	{
		// Send Email
		_logger->Erro();
	}

private:
	ILogger* _logger;
};

/* Composite */

#include <vector>

class BoxCollider;
class CircleCollider;
class CompoundCollider;

class ICollider
{
public:
	virtual bool Compute(ICollider* Other) = 0;

	virtual bool Compute(BoxCollider* Other) = 0;

	virtual bool Compute(CircleCollider* Other) = 0;

	virtual bool Compute(CompoundCollider* Other) = 0;
};

class BoxCollider : public ICollider
{
public:
	bool Compute(ICollider* Other) override
	{
		return Other->Compute(this);
	}

	bool Compute(BoxCollider* Other) override
	{
		// Compute Box 2 Box collision
		return true;
	}

	bool Compute(CircleCollider* Other) override
	{
		// Compute Box 2 Circle collision
		return true;
	}

	bool Compute(CompoundCollider* Other) override
	{
		return Other->Compute(this);
	}
};

class CircleCollider : public ICollider
{
public:
	bool Compute(ICollider* Other) override { }

	bool Compute(BoxCollider* Other) override
	{
		return Other->Compute(this);
	}

	bool Compute(CircleCollider* Other) override
	{
		// Compute Circle 2 Circle collision
		return true;
	}

	bool Compute(CompoundCollider* Other) override
	{
		return Other->Compute(this);
	}
};

class CompoundCollider : public ICollider
{
public:
	bool Compute(ICollider* Other) override
	{
		for (ICollider* Each : colliders)
		{
			bool IsColliding = Each->Compute(Other);

			if (IsColliding)
				return true;
		}
		return false;
	}

	bool Compute(BoxCollider* Other) override
	{
		for (ICollider* Each : colliders)
		{
			bool IsColliding = Each->Compute(Other);

			if (IsColliding)
				return true;
		}
		return false;
	}

	bool Compute(CircleCollider* Other) override
	{
		for (ICollider* Each : colliders)
		{
			bool IsColliding = Each->Compute(Other);

			if (IsColliding)
				return true;
		}
		return false;
	}

	bool Compute(CompoundCollider* Other) override
	{
		for (ICollider* Each : colliders)
		{
			bool IsColliding = Each->Compute(Other);

			if (IsColliding)
				return true;
		}
		return false;
	}

	void Add(ICollider* collider)
	{
		colliders.push_back(collider);
	}

private:
	std::vector<ICollider*> colliders;
};

int main()
{
	auto* Factory = new ExcelPlayerFactory("path");
	// Client
	auto* Player = Factory->Create();

	// BUILDER RUN
	AvatarBuilder builder;
	builder.SetName("Giuseppe");
	builder.SetSpecies(2);
	auto* avatar = builder.Result();

	// SIGLETON
	// Client
	auto* s = Singleton::GetInstance();
	auto* sc = new SingletonClientV2(s);

	// Adapter
	MyCalc calc;
	calc.SetA(2);
	calc.SetB(3);
	int r = calc.Sum(); // 5
	MyCalcAdapter calcAdapter{ &calc };
	r = calcAdapter.Sum(2, 3); // 5;

	// Decorator
	auto* logger = new SendEmailDecoratorLogger(new ConsoleLogger());
	logger->Erro();

	// Composite
	BoxCollider* collider1 = new BoxCollider();
	CompoundCollider* collider2 = new CompoundCollider();
	collider2->Add(new CircleCollider());
	collider2->Add(new CircleCollider());
	bool isColliding = collider1->Compute(collider2);
}