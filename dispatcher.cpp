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
	else if(ev->type == ALLEGRO_EVENT_KEY_UP)
	{
		switch (ev->keyboard.keycode)
		{
			case ALLEGRO_KEY_R:
				news->restartNews();
				break;
			case ALLEGRO_KEY_S:
				news->nextNews();
				break;
			case ALLEGRO_KEY_A:
				news->prevNews();
				break;
			case ALLEGRO_KEY_Q:
				endScrolling = true;
				break;
			case ALLEGRO_KEY_V:
				al_set_timer_speed(timer, std::max<double>(al_get_timer_speed(timer) - 0.1, MAX_SPEED));	//establecer una velocidad maxima
				break;
			case ALLEGRO_KEY_L:
				al_set_timer_speed(timer, std::min<double>(al_get_timer_speed(timer) + 0.1, MIN_SPEED));	//establecer una velocidad minima
				break;
			default:
				break;
		}
	}

	return endScrolling;
}
