#include <iostream>
#include <fstream>
#include <direct.h>
#include "Warrior.h"
#include "Action.h"
#include "GameManager.h"
#include <ctime>
using namespace std;

void Run3vs3Simulation(GameManager gameManager);
void WinsPerLevel(GameManager gameManager);
void RunPveSimulation(GameManager gameManager);

void AddCharacterStatsToFile(Warrior warrior, std::ofstream& logFile);

int main() {

	GameManager gameManager;

	Warrior Greenmor("Greenmor", 25.0f, 3.0f, 4.0f, 3.0f, 3.0f, 2.0f, &gameManager);
	Warrior Brianna("Brianna", 29.0f, 2.0f, 6.0f, 4.0f, 1.0f, 1.0f, &gameManager);
	Warrior Elishah("Elishah", 28.0f, 8.0f, 2.0f, 3.0f, 6.0f, 5.0f, &gameManager);

	Warrior guard_1("Guard_1", 26.0f, 3.0f, 5.0f, 4.0f, 1.0f, 2.0f, &gameManager);
	Warrior guard_2("Guard_2", 26.0f, 3.0f, 5.0f, 4.0f, 1.0f, 2.0f, &gameManager);
	Warrior wizard("Wizard", 24.0f, 6.0f, 2.0f, 2.0f, 5.0f, 4.0f, &gameManager);

	Warrior Fornter("Fornter", 400.0f, 25.0f, 45.0f, 45.0f, 25.0f, 45.0f, &gameManager);
	Warrior GreatWizard("GreatWizard", 400.0f, 50.0f, 45.0f, 50.0f, 50.0f, 60.0f, &gameManager);

	// 3 vs 3 =====================================================================================

	//gameManager._heroes = { Greenmor, Brianna, Elishah };
	//gameManager._villains = { guard_1, guard_2, wizard };

	//Run3vs3Simulation(gameManager);

	// Main vs Enemy ==============================================================================

	gameManager._heroes = { Greenmor,Brianna,Elishah };
	gameManager._villains = { wizard,wizard,guard_1,guard_2,guard_2,guard_2,wizard,wizard,guard_1,guard_2};
	WinsPerLevel(gameManager);

	// PVE ========================================================================================
	//RunPveSimulation(gameManager);

	return 0;
}

void Run3vs3Simulation(GameManager gameManager)
{
	std::ofstream logFile;
	logFile.open("../LogFiles/3vs3.txt");

	srand(time(NULL));

	size_t simulations_amount = 20;
	size_t max_rounds = 100;
	float rounds_average = 0;
	float heroes_wins_average = 0.0f;
	float villains_wins_average = 0.0f;

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
				//gameManager.aliveHeroes[i].ChooseAction(ActionStrategy::RANDOM);
				gameManager.aliveHeroes[i].ChooseAction(ActionStrategy::ATTACK_TENDENCY);
				gameManager.aliveHeroes[i].ChooseEnemy(ChooseTargetStrategy::OPTIMIZED);
				gameManager.aliveHeroes[i].ExecuteAction();
			}

			gameManager.CheckAliveWarriors();

			for (size_t i = 0; i < gameManager.aliveVillains.size(); i++)
			{
				//gameManager.aliveVillains[i].ChooseAction(ActionStrategy::RANDOM);
				gameManager.aliveVillains[i].ChooseAction(ActionStrategy::ATTACK_TENDENCY);
				gameManager.aliveVillains[i].ChooseEnemy(ChooseTargetStrategy::OPTIMIZED);
				gameManager.aliveVillains[i].ExecuteAction();
			}

			winner = gameManager.CheckWinner();

			std::cout << "" << std::endl;
		}

		if (winner == 0) {
			heroes_wins_average++;
			cout << "Heroes win" << endl;
			logFile << "Heroes";
		}
		else if (winner == 1)
		{
			villains_wins_average++;
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
	heroes_wins_average /= simulations_amount;
	villains_wins_average /= simulations_amount;

	cout << endl << "Rounds Average: " << rounds_average << endl << endl;
	logFile << "Rounds Average: " << rounds_average << endl << endl;

	cout << "Heroes Wins Average: " << heroes_wins_average << endl;
	logFile << "Heroes Wins Average: " << heroes_wins_average << endl;

	cout << "Villains Wins Average: " << villains_wins_average << endl << endl;
	logFile << "Villains Wins Average: " << villains_wins_average << endl << endl;

	logFile << "Character | health | energy | attack | defense | magical attack | magical defense \n";
	AddCharacterStatsToFile(gameManager._heroes[0], logFile);
	AddCharacterStatsToFile(gameManager._heroes[1], logFile);
	AddCharacterStatsToFile(gameManager._heroes[2], logFile);
	AddCharacterStatsToFile(gameManager._villains[0], logFile);
	AddCharacterStatsToFile(gameManager._villains[1], logFile);
	AddCharacterStatsToFile(gameManager._villains[2], logFile);

	logFile.close();
}

void WinsPerLevel(GameManager gameManager)
{
	std::ofstream logFile;
	logFile.open("mainVsEnemies.txt");

	srand(time(NULL));

	size_t max_levels = 100;
	size_t max_simulations = 100;
	size_t max_rounds = 100;

	for (size_t l = 1; l <= max_levels; l++)
	{
		logFile << "Level " << l << ": ";
		std::cout << "Level " << l << " ==================================================" << std::endl;

		if ((l % 5) == 0)
		{
			std::cout << "Level Increased!" << std::endl;
			gameManager._heroes[0].IncreaseStats(80.0f, 80.0f, 70.0f, 70.0f, 100.0f, 70.0f);
			gameManager._heroes[1].IncreaseStats(100.0f, 100.0f, 45.0f, 40.0f, 95.0f, 40.0f);
			gameManager._heroes[2].IncreaseStats(60.0f, 65.0f, 100.0f, 100.0f, 85.0f, 100.0f);
		}

		std::cout << "Greenmor Stats: " << "HP " << gameManager._heroes[0]._currentHealth << " | " << "ATCK " << gameManager._heroes[0]._attack
			<< " | " << "DEF " << gameManager._heroes[0]._defense << " | " << "M_ATCK " << gameManager._heroes[0]._magicalAttack
			<< " | " << "M_DEF " << gameManager._heroes[0]._magicalDefense << "\n";
		std::cout << "Brianna Stats: " << "HP " << gameManager._heroes[1]._currentHealth << " | " << "ATCK " << gameManager._heroes[1]._attack
			<< " | " << "DEF " << gameManager._heroes[1]._defense << " | " << "M_ATCK " << gameManager._heroes[1]._magicalAttack
			<< " | " << "M_DEF " << gameManager._heroes[1]._magicalDefense << "\n";
		std::cout << "Elishah Stats: " << "HP " << gameManager._heroes[2]._currentHealth << " | " << "ATCK " << gameManager._heroes[2]._attack
			<< " | " << "DEF " << gameManager._heroes[2]._defense << " | " << "M_ATCK " << gameManager._heroes[2]._magicalAttack
			<< " | " << "M_DEF " << gameManager._heroes[2]._magicalDefense << "\n";
		float won_simulations = 0.0f;

		for (size_t simulation = 0; simulation < max_simulations; simulation++)
		{
			std::cout << "Simulation: " << simulation << " ==================================================" << std::endl;
			gameManager.SetSimulation();
			int winner = -1;

			for (size_t i = 0; i < max_rounds && winner == -1; i++)
			{
				std::cout << "Round " << i + 1 << std::endl;

				for (size_t j = 0; j < gameManager.aliveHeroes.size(); j++)
				{
					gameManager.aliveHeroes[j].ChooseAction(ActionStrategy::RANDOM);
					gameManager.aliveHeroes[j].ChooseEnemy(ChooseTargetStrategy::OPTIMIZED);
					gameManager.aliveHeroes[j].ExecuteAction();
				}

				for (size_t j = 0; j < gameManager.aliveVillains.size(); j++)
				{
					gameManager.aliveVillains[j].ChooseAction(ActionStrategy::RANDOM);
					gameManager.aliveVillains[j].ChooseEnemy(ChooseTargetStrategy::OPTIMIZED);
					gameManager.aliveVillains[j].ExecuteAction();

				}

				winner = gameManager.CheckWinner();

				std::cout << "" << std::endl;

				if (winner == 0) {
					//logFile << "Heroes won \n";
					won_simulations++;
				}
				else if (winner == 1)
				{
					//logFile << "Villains won \n";
				}
				else {
					//logFile << "Draw \n";
				}
			}
		}

		logFile << "Won simulations " << won_simulations / (float)max_simulations * 100.0f << endl;
	}

	logFile << "\n";

	logFile << "Character | " << "health |" << " energy |" << " attack |" << " defense |" << " magical attack |" << " magical defense \n";
	AddCharacterStatsToFile(gameManager._heroes[0], logFile);
	AddCharacterStatsToFile(gameManager._heroes[1], logFile);
	AddCharacterStatsToFile(gameManager._heroes[2], logFile);
	logFile.close();
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
	logFile << warrior._maxHealth << " | ";
	logFile << warrior._maxEnergy << " | ";
	logFile << warrior._attack << " | ";
	logFile << warrior._defense << " | ";
	logFile << warrior._magicalAttack << " | ";
	logFile << warrior._magicalDefense << " \n";
}