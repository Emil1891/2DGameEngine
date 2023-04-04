#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

class GameManager
{
public:

	static int getEnemyCount() { return enemiesLeft; }
	static int getRoundNumber() { return roundNumber; }

	static void lowerEnemyCount(); 

	static void startNewRound(const bool newGame = false);

private:
	inline static const int startEnemyCount = 5;
	inline static int enemiesLeft = startEnemyCount;
	inline static int roundNumber = 0; 

	GameManager() {};
};

#endif