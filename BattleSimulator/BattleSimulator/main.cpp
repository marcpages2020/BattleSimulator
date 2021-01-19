#include <iostream>
using namespace std;
#include <fstream>
#include <direct.h>

#include "Warrior.h"
#include "Action.h"
#include "GameManager.h"

void Run3vs3Simulation(GameManager gameManager);
void RunMainVsEnemiesSimulation(GameManager gameManager);
void RunPveSimulation(GameManager gameManager);

void AddCharacterStatsToFile(Warrior warrior, std::ofstream& logFile);

int main() {

	GameManager gameManager;

	Warrior Greenmor("Greenmor", 25.0f, 3.0f, 4.0f, 3.0f, 3.0f, 2.0f, &gameManager);
	Warrior Brianna("Brianna", 29.0f, 2.0f, 6.0f, 4.0f, 1.0f, 1.0f, &gameManager);
	Warrior Elishah("Elishah", 28.0f, 8.0f, 2.0f, 3.0f, 6.0f, 5.0f, &gameManager);

	Warrior guard_1("Guard_1", 30.0f, 3.0f, 5.0f, 4.0f, 1.0f, 2.0f, &gameManager);
	Warrior guard_2("Guard_2", 30.0f, 3.0f, 5.0f, 4.0f, 1.0f, 2.0f, &gameManager);
	Warrior wizard("Wizard", 48.0f, 6.0f, 2.0f, 2.0f, 5.0f, 4.0f, &gameManager);

	Warrior Fornter("Fornter", 1712.0f, 25.0f, 175.0f, 135.0f, 25.0f, 45.0f, &gameManager);
	Warrior GreatWizard("GreatWizard", 1528.0f, 210.0f, 45.0f, 60.0f, 165.0f, 155.0f, &gameManager);

	// 3 vs 3 =====================================================================================

	gameManager._heroes = { Greenmor, Brianna, Elishah };
	gameManager._villains = { guard_1, guard_2, wizard };

	Run3vs3Simulation(gameManager);

	// Main vs Enemy ==============================================================================

	//gameManager._heroes = { Greenmor };
	//gameManager._villains = { Fornter };
	//RunMainVsEnemiesSimulation(gameManager);

	// PVE ========================================================================================
	//RunPveSimulation(gameManager);

	return 0;
}

void Run3vs3Simulation(GameManager gameManager)
{
	std::ofstream logFile;
	logFile.open("../LogFiles/3vs3.txt");

	srand(time(NULL));

	size_t simulations_amount = 50;
	size_t max_rounds = 100;
	float rounds_average = 0;

	logFile << "Simulation | Winner | Rounds amount | Alive Heroes | Alive Villains |\n";

	for (size_t s = 1; s <= simulations_amount; s++)
	{
		logFile << s << " | ";
		std::cout << "Simulation: " << s << " ==================================================" << std::endl;

		gameManager.SetSimulation();

		int winner = -1;

		size_t rounds = 1;
		for (rounds; rounds < max_rounds && winner == -1; rounds++)
		{
			std::cout << "Round " << rounds << std::endl;

			for (size_t i = 0; i < gameManager.aliveHeroes.size(); i++)
			{
				gameManager.aliveHeroes[i].ChooseAction(ActionStrategy::RANDOM);
				//gameManager._heroes[i].ChooseAction(Strategy::ALWAYS_ATTACK);
				gameManager.aliveHeroes[i].ChooseEnemy(ChooseTargetStrategy::OPTIMIZED);
				gameManager.aliveHeroes[i].ExecuteAction();
			}

			gameManager.CheckAliveWarriors();

			for (size_t i = 0; i < gameManager.aliveVillains.size(); i++)
			{
				gameManager.aliveVillains[i].ChooseAction(ActionStrategy::RANDOM);
				//gameManager._villains[i].ChooseAction(Strategy::ALWAYS_ATTACK);
				gameManager.aliveVillains[i].ChooseEnemy(ChooseTargetStrategy::OPTIMIZED);
				gameManager.aliveVillains[i].ExecuteAction();
			}

			winner = gameManager.CheckWinner();

			std::cout << "" << std::endl;
		}

		if (winner == 0) {
			cout << "Heroes win" << endl;
			logFile << "Heroes";
		}
		else if (winner == 1)
		{
			cout << "Villains win" << endl;
			logFile << "Villains";
		}
		else {
			cout << "Draw" << endl;
			logFile << "Draw";
		}

		logFile << " | " << rounds;
		rounds_average += rounds;

		logFile << " | " << gameManager.aliveHeroes.size() << " | ";
		logFile << gameManager.aliveVillains.size() << endl;
	}


	logFile << "\n";
	rounds_average /= simulations_amount;

	logFile << "Rounds Average: " << rounds_average << endl << endl;

	logFile << "Character | health | energy | attack | defense | magical attack | magical defense \n";
	AddCharacterStatsToFile(gameManager._heroes[0], logFile);
	AddCharacterStatsToFile(gameManager._heroes[1], logFile);
	AddCharacterStatsToFile(gameManager._heroes[2], logFile);
	AddCharacterStatsToFile(gameManager._villains[0], logFile);
	AddCharacterStatsToFile(gameManager._villains[1], logFile);
	AddCharacterStatsToFile(gameManager._villains[2], logFile);

	logFile.close();
}

void RunMainVsEnemiesSimulation(GameManager gameManager)
{
	std::ofstream logFile;
	logFile.open("../LogFiles/mainVsEnemies.txt");

	srand(time(NULL));

	size_t max_levels = 100;
	size_t max_rounds = 100;

	for (size_t l = 1; l <= max_levels; l++)
	{
		logFile << "Level " << l << ": ";
		std::cout << "Level " << l << " ==================================================" << std::endl;

		if ((l % 5) == 0)
		{
			std::cout << "Level Increased!" << std::endl;
			gameManager._heroes[0].IncreaseStats(80.0f, 80.0f, 70.0f, 70.0f, 100.0f, 70.0f);
		}

		std::cout << "Greenmor Stats: " << "HP " << gameManager._heroes[0]._health << " | " << "ATCK " << gameManager._heroes[0]._attack 
			<< " | " << "DEF " << gameManager._heroes[0]._defense << " | " << "M_ATCK " << gameManager._heroes[0]._magicalAttack 
			<< " | " << "M_DEF " << gameManager._heroes[0]._magicalDefense << "\n";

		gameManager.SetSimulation();

		int winner = -1;

		for (size_t i = 0; i < max_rounds && winner == -1; i++)
		{
			std::cout << "Round " << i + 1 << std::endl;

			for (size_t i = 0; i < gameManager.aliveHeroes.size(); i++)
			{
				gameManager.aliveHeroes[0].ChooseAction(ActionStrategy::RANDOM);
				gameManager.aliveHeroes[0].ChooseEnemy(ChooseTargetStrategy::RANDOM);
				gameManager.aliveHeroes[0].ExecuteAction();
			}

			for (size_t i = 0; i < gameManager.aliveVillains.size(); i++)
			{
				gameManager.aliveVillains[0].ChooseAction(ActionStrategy::RANDOM);
				gameManager.aliveVillains[0].ChooseEnemy(ChooseTargetStrategy::RANDOM);
				gameManager.aliveVillains[0].ExecuteAction();
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
			logFile << "Draw \n";
		}
	}

	logFile << "\n";

	logFile << "Character | " << "health |" << " energy |" << " attack |" << " defense |" << " magical attack |" << " magical defense \n";
	AddCharacterStatsToFile(gameManager._heroes[0], logFile);
}

void RunPveSimulation(GameManager gameManager)
{
	std::ofstream logFile;
	logFile.open("../LogFiles/PVE.txt");

	srand(time(NULL));

	size_t simulations_amount = 10;
	size_t max_rounds = 100;

	bool quit = false;

	while (!quit)
	{
		gameManager.SetSimulation();

		int winner = -1;

		for (size_t i = 0; i < max_rounds && winner == -1; i++)
		{
			std::cout << "Round " << i + 1 << std::endl;

			//Show stats =================================================================
			for (size_t i = 0; i < gameManager.aliveHeroes.size(); i++) {
				gameManager.aliveHeroes[i].ShowStats();
			}

			cout << "-------------------------------------------------------------------" << endl;

			for (size_t i = 0; i < gameManager.aliveVillains.size(); i++) {
				gameManager.aliveVillains[i].ShowStats();
			}

			cout << endl;

			cout << "PLAYER TURN ===============================================================" << endl;

			//Take Action ================================================================
			for (size_t i = 0; i < gameManager.aliveHeroes.size(); i++)
			{
				gameManager.aliveHeroes[i].HandleInput();
				//gameManager._heroes[i].ChooseAction(Strategy::ALWAYS_ATTACK);
				gameManager.aliveHeroes[i].ExecuteAction();

				cout << endl;
			}

			cout << "ENEMIES TURN ===============================================================" << endl;

			for (size_t i = 0; i < gameManager.aliveVillains.size(); i++)
			{
				gameManager.aliveVillains[i].ChooseAction(ActionStrategy::RANDOM);
				//gameManager._villains[i].ChooseAction(Strategy::ALWAYS_ATTACK);
				gameManager.aliveVillains[i].ExecuteAction();
			}

			winner = gameManager.CheckWinner();

			std::cout << "" << std::endl;
			system("pause");
			system("cls");
		}

		if (winner == 0) {
			logFile << "Heroes won \n";
		}
		else if (winner == 1)
		{
			logFile << "Villains won \n";
		}
		else {
			logFile << "Draw \n";
		}
	}

	logFile << "\n";

	logFile << "Character | " << "health |" << " energy |" << " attack |" << " defense |" << " magical attack |" << " magical defense \n";
	AddCharacterStatsToFile(gameManager._heroes[0], logFile);
	AddCharacterStatsToFile(gameManager._heroes[1], logFile);
	AddCharacterStatsToFile(gameManager._heroes[2], logFile);
	AddCharacterStatsToFile(gameManager._villains[0], logFile);
	AddCharacterStatsToFile(gameManager._villains[1], logFile);
	AddCharacterStatsToFile(gameManager._villains[2], logFile);

	logFile.close();
}

void AddCharacterStatsToFile(Warrior warrior, std::ofstream& logFile)
{
	logFile << warrior.name << " | ";
	logFile << warrior._health << " | ";
	logFile << warrior._energy << " | ";
	logFile << warrior._attack << " | ";
	logFile << warrior._defense << " | ";
	logFile << warrior._magicalAttack << " | ";
	logFile << warrior._magicalDefense << " \n";
}