#include <iostream>
#include <fstream>
#include <direct.h>

#include "Warrior.h"
#include "Action.h"
#include "GameManager.h"

void AddCharacterStatsToFile(Warrior warrior, std::ofstream& logFile);

void Run3vs3Simulation();
void RunMainVsEnemiesSimulation();

int main() {
	
	//Run3vs3Simulation();
	RunMainVsEnemiesSimulation();

	return 0;
}

void AddCharacterStatsToFile(Warrior warrior, std::ofstream& logFile)
{
	logFile << warrior.name << " | ";
	logFile << warrior._health << " | ";
	logFile << warrior._attack << " | ";
	logFile << warrior._defense << " | ";
	logFile << warrior._magicalAttack << " | ";
	logFile << warrior._magicalDefense << " \n";
}

void Run3vs3Simulation()
{
	GameManager gameManager;

	_mkdir("../LogFiles");
	std::ofstream logFile;
	logFile.open("../LogFiles/3vs3.txt");

	srand(time(NULL));

	size_t simulations_amount = 10;
	size_t max_rounds = 100;

	Warrior Greenmor("Greenmor", 50.0f, 4.0f, 3.0f, 3.0f, 2.0f, &gameManager);
	Warrior Brianna("Brianna", 58.0f, 6.0f, 4.0f, 1.0f, 1.0f, &gameManager);
	Warrior Elishah("Elishah", 56.0f, 2.0f, 3.0f, 6.0f, 5.0f, &gameManager);

	Warrior guard_1("guard_1", 53.0f, 5.0f, 4.0f, 1.0f, 2.0f, &gameManager);
	Warrior guard_2("guard_2", 53.0f, 5.0f, 4.0f, 1.0f, 2.0f, &gameManager);
	Warrior wizard("wizard", 48.0f, 2.0f, 2.0f, 5.0f, 4.0f, &gameManager);

	//Warrior Fornter("Fornter", 1712.0f, 175.0f, 135.0f, 25.0f, 45.0f);
	//Warrior GreatWizard("GreatWizard", 1528.0f, 45.0f, 60.0f, 165.0f, 155.0f);

	for (size_t s = 0; s < simulations_amount; s++)
	{
		logFile << "Simulation: " << s + 1 << ": ";
		std::cout << "Simulation: " << s + 1 << " ==================================================" << std::endl;

		gameManager._heroes = { Greenmor, Brianna, Elishah };
		gameManager._villains = { guard_1, guard_2, wizard };

		gameManager.SetSimulation();

		int winner = -1;

		for (size_t i = 0; i < max_rounds && winner == -1; i++)
		{
			std::cout << "Round " << i + 1 << std::endl;

			for (size_t i = 0; i < gameManager._heroes.size(); i++)
			{
				gameManager._heroes[i].ChooseAction(Strategy::RANDOM);
				gameManager._heroes[i].ExecuteAction();
			}

			for (size_t i = 0; i < gameManager._villains.size(); i++)
			{
				gameManager._villains[i].ChooseAction(Strategy::RANDOM);
				gameManager._villains[i].ExecuteAction();
			}

			winner = gameManager.CheckWinner();

			std::cout << "" << std::endl;
		}

		if (winner == 0) {
			logFile << "Heroes won \n";
		}
		else if (winner == 1)
		{
			logFile << "Villains won \n";
		}
		else {
			logFile << "Match \n";
		}
	}

	logFile << "\n";

	logFile << "Character | " << "health |" << "attack |" << "defense |" << "magical attack |" << "magical defense \n";
	AddCharacterStatsToFile(Greenmor, logFile);
	AddCharacterStatsToFile(Brianna, logFile);
	AddCharacterStatsToFile(Elishah, logFile);
	AddCharacterStatsToFile(guard_1, logFile);
	AddCharacterStatsToFile(guard_2, logFile);
	AddCharacterStatsToFile(wizard, logFile);

	logFile.close();
}

void RunMainVsEnemiesSimulation()
{
	GameManager gameManager;

	_mkdir("../LogFiles");
	std::ofstream logFile;
	logFile.open("../LogFiles/mainVsEnemies.txt");

	srand(time(NULL));

	size_t max_levels = 100;
	size_t max_rounds = 100;

	Warrior Greenmor("Greenmor", 50.0f, 4.0f, 3.0f, 3.0f, 2.0f, &gameManager);
	Warrior Fornter("Fornter", 1712.0f, 175.0f, 135.0f, 25.0f, 45.0f, &gameManager);

	for (size_t l = 1; l <= max_levels; l++)
	{
		logFile << "Level " << l << ": ";
		std::cout << "Level " << l << " ==================================================" << std::endl;

		if ((l % 5) == 0)
		{
			std::cout << "Level Increased!" << std::endl;
			Greenmor.IncreaseStats(80.0f, 80.0f, 70.0f, 70.0f, 100.0f);
		}

		std::cout << "Greenmor Stats: " << "HP " << Greenmor._health << " | " << "ATCK " << Greenmor._attack << " | " << "DEF " << Greenmor._defense 
			<< " | " <<	"M_ATCK " << Greenmor._magicalAttack << " | " << "M_DEF " << Greenmor._magicalDefense << "\n"; 

		gameManager._heroes = { Greenmor};
		gameManager._villains = { Fornter };

		gameManager.SetSimulation();

		int winner = -1;

		for (size_t i = 0; i < max_rounds && winner == -1; i++)
		{
			std::cout << "Round " << i + 1 << std::endl;

			for (size_t i = 0; i < gameManager._heroes.size(); i++)
			{
				gameManager._heroes[i].ChooseAction(Strategy::RANDOM);
				gameManager._heroes[i].ExecuteAction();
			}

			for (size_t i = 0; i < gameManager._villains.size(); i++)
			{
				gameManager._villains[i].ChooseAction(Strategy::RANDOM);
				gameManager._villains[i].ExecuteAction();
			}

			winner = gameManager.CheckWinner();

			std::cout << "" << std::endl;
		}

		if (winner == 0) {
			logFile << "Heroes won \n";
		}
		else if (winner == 1)
		{
			logFile << "Villains won \n";
		}
		else {
			logFile << "Match \n";
		}
	}

	logFile << "\n";

	logFile << "Character | " << "health |" << "attack |" << "defense |" << "magical attack |" << "magical defense \n";
	AddCharacterStatsToFile(Greenmor, logFile);
}
