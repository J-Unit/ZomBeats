#pragma once
class GameState;

class AIController
{
public:
	AIController();
	~AIController();

	void update(GameState *s);
};

