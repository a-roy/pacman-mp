//! \file
//! Game class declaration

#include <vector>
#include "Player.h"
#include "Field.h"

class Game
{
public:
	Field GameField;
	std::vector<Player> Players;
	int CurrentFrame;

	Game(Field f, std::vector<Player> p);
	void update();
	bool move(Player *p, Player::Direction d);
};
