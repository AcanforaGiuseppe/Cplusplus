#include <iostream>
#include <conio.h>
#include <chrono>
#include <cmath>

struct Position
{
	int X;
	int Y;
};

static bool IsPositionValid(Position Cell, int Rows, int Cols)
{
	if (Cell.X < 0)
		return false;

	if (Cell.Y < 0)
		return false;

	if (Cell.X >= Cols)
		return false;

	if (Cell.Y >= Rows)
		return false;

	return true;
}

static bool HasFoundArtifact(Position Player, Position Artifact)
{
	if (Player.X != Artifact.X)
		return false;

	if (Player.Y != Artifact.Y)
		return false;

	return true;
}

static Position RandPosition(int MaxX, int MaxY)
{
	int MinX = 0;
	int RangeX = MaxX - MinX + 1;
	int X = std::rand() % RangeX + MinX;

	int MinY = 0;
	int RangeY = MaxY - MinY + 1;
	int Y = std::rand() % RangeY + MinY;

	return { X, Y };
}

static int Distance(Position P1, Position P2)
{
	int DistX = abs(P1.X - P2.X);
	int DistY = abs(P1.Y - P2.Y);

	return DistX + DistY;
}

class IndianaGame
{
public:
	void MovePlayer(const Position& Direction, const std::string& MessageValidMove)
	{
		bool CanMove = false;
		Position NewPos = Player;

		// TODO: operator+ on Position to improve

		NewPos.X += Direction.X;
		NewPos.Y += Direction.Y;

		if (CanMove = IsPositionValid(NewPos, Rows, Cols))
		{
			Player = NewPos;
			std::cout << MessageValidMove << "\n";
		}

		HasWon = EvaluateGameScenarios(CanMove);
	}

	void Quit()
	{
		ExitGame = true;
	}

	bool IsRunning()
	{
		return !HasWon && !ExitGame;
	}

private:
	bool EvaluateGameScenarios(bool CanMove)
	{
		if (HasFoundArtifact(Player, Artifact))
		{
			std::cout << "Good! You found it!" << "\n";
			return true;
		}

		if (CanMove)
		{
			int NewDistance = Distance(Player, Artifact);

			if (CurrentDistance >= NewDistance)
			{
				CurrentDistance = NewDistance;
				std::cout << "You're getting closer..." << "\n";
			}
			else
			{
				std::cout << "You're going away!" << "\n";
			}
		}
		else //(!CanMove)
		{
			std::cout << "You can't go there!" << "\n";
		}

		return false;
	}

public:
	Position Player;
	Position Artifact;
	int Rows;
	int Cols;
	int CurrentDistance;
	bool HasWon;

private:
	bool ExitGame;
};

// Random map genaration
// Input management (console)
// Output management (console)
// Game Rule: check player if reach artifact
// Game Rule: Player movement 
// Actions: Up, Down, Right, Left, Exit, Invalid
// Game Loop
// Entry Point

class ICommand
{
public:
	virtual void Execute() = 0;
};

class MoveUpCommand : public ICommand
{
public:
	MoveUpCommand(IndianaGame& InGame) : Game(InGame) { }

	void Execute() override
	{
		Game.MovePlayer({ 0, -1 }, "You're moved upwards!");
	}

private:
	IndianaGame& Game;
};

class MoveDownCommand : public ICommand
{
public:
	MoveDownCommand(IndianaGame& InGame) : Game(InGame) { }

	void Execute() override
	{
		Game.MovePlayer({ 0, 1 }, "You're moved downwards!");
	}

private:
	IndianaGame& Game;
};

class MoveLeftCommand : public ICommand
{
public:
	MoveLeftCommand(IndianaGame& InGame) : Game(InGame) { }

	void Execute() override
	{
		Game.MovePlayer({ -1, 0 }, "You're moved leftwards!");
	}

private:
	IndianaGame& Game;
};

class MoveRightCommand : public ICommand
{
public:
	MoveRightCommand(IndianaGame& InGame) : Game(InGame) { }

	void Execute() override
	{
		Game.MovePlayer({ 1, 0 }, "You're moved rightwards!");
	}

private:
	IndianaGame& Game;
};

class ExitCommand : public ICommand
{
public:
	ExitCommand(IndianaGame& InGame) : Game(InGame) { }

	void Execute() override
	{
		Game.Quit();
	}

private:
	IndianaGame& Game;
};

class InvalidCommand : public ICommand
{
public:
	InvalidCommand() { }

	void Execute() override
	{
		std::cout << "Invalid Command!\n";
	}

private:
	IndianaGame& Game;
};

#include <map>

class CommandHandler
{
public:
	CommandHandler(ICommand* InDefault) : DefaultCmd(InDefault) { }

	void BindCmd(char Key, ICommand* Value)
	{
		Commands[Key] = Value;
	}

	ICommand* FindCmd(char Key)
	{
		auto Iter = Commands.find(Key);

		if (Iter != Commands.end())
			return Iter->second;

		return DefaultCmd;
	}

private:
	ICommand* DefaultCmd;
	std::map<char, ICommand*> Commands;
};

int main()
{
	const int Cols = 4;
	const int Rows = 5;

	std::srand(time(NULL));

	IndianaGame Game;
	Game.Player = RandPosition(Cols - 1, Rows - 1);
	Game.Artifact = RandPosition(Cols - 1, Rows - 1);
	Game.CurrentDistance = Distance(Game.Player, Game.Artifact);
	Game.HasWon = false;

	MoveUpCommand MoveUp{ Game };
	MoveDownCommand MoveDown{ Game };
	MoveLeftCommand MoveLeft{ Game };
	MoveRightCommand MoveRight{ Game };
	ExitCommand Exit{ Game };
	InvalidCommand Invalid;

	CommandHandler Handler{ &Invalid };
	Handler.BindCmd('w', &MoveUp);
	Handler.BindCmd('s', &MoveDown);
	Handler.BindCmd('a', &MoveLeft);
	Handler.BindCmd('d', &MoveRight);
	Handler.BindCmd('q', &Exit);

	while (Game.IsRunning())
	{
		printf("You're here (%d, %d). Where do you want to go?\n", Game.Player.X, Game.Player.Y);

		int Key = _getch(); // No portable

		auto* Selected = Handler.FindCmd((char)Key);
		Selected->Execute();
	}
}