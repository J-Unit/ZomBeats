#include "util.h"

#pragma once

#define AWARENESS_DRAIN 0.015


class GameState;
class AudioController;

class AIController
{
public:
	AIController();
	~AIController();

	void update(GameState *s, AudioController *audio);
};

