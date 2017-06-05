#include "dispatcher.h"
#include <algorithm>

#define MAX_SPEED 0.1
#define MIN_SPEED 1.0

dispatcher::dispatcher(NewsDisplay * news, ALLEGRO_TIMER * timer)
{
	this->news = news;
	this->timer = timer;
}

bool dispatcher::dispatch(ALLEGRO_EVENT * ev)
{
	bool endScrolling = false;

	if (ev->type == ALLEGRO_EVENT_TIMER)
	{
		endScrolling = news->updateDisplay();
	}
	else
	{
		switch (ev->keyboard.keycode)
		{
			case 'R':
				news->restartNews();
				break;
			case 'S':
				news->nextNews();
				break;
			case 'A':
				news->prevNews();
				break;
			case 'Q':
				endScrolling = true;
				break;
			case '+':
				al_set_timer_speed(timer, std::max(al_get_timer_speed(timer) - 0.1, MAX_SPEED));	//establecer una velocidad maxima
				break;
			case '-':
				al_set_timer_speed(timer, std::min(al_get_timer_speed(timer) + 0.1, MIN_SPEED));	//establecer una velocidad minima
				break;
			default:
				break;
		}
	}

	return endScrolling;
}
