#pragma once

#include <allegro5\allegro5.h>
#include "newsDisplay.h"

class dispatcher
{
public:
	dispatcher(NewsDisplay * news, ALLEGRO_TIMER * timer);
	bool dispatch(ALLEGRO_EVENT * ev);
private:
	NewsDisplay * news;
	ALLEGRO_TIMER * timer;
};

