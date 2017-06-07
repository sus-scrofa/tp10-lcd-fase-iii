#include <iostream>
#include <string>
#include <list>

#include "expat.h"
#include <allegro5\allegro5.h>

#include "UserData.h"
#include "newsDisplay.h"
#include "dispatcher.h"
#include "newsItem.h"

#include "downloadXML.h"
bool initAllegro(ALLEGRO_EVENT_QUEUE ** evQtimer, ALLEGRO_EVENT_QUEUE ** evQkeyboard, ALLEGRO_TIMER ** timer, ALLEGRO_DISPLAY ** display);
void shutdownAllegro(ALLEGRO_EVENT_QUEUE ** evQtimer, ALLEGRO_EVENT_QUEUE ** evQkeyboard, ALLEGRO_TIMER ** timer, ALLEGRO_DISPLAY ** display);

#define MIN_ARGS 1
#define SIZE_BUFFER_XMLX 100000
int main(int argc, char * argv[])
{

	if (argc <= MIN_ARGS)//si no se enviaron suficientes argumento salgo
	{
		return 1;
	}

	downloadXML downloader;

	if (!downloader.saveXMLtoFile("tst.txt", argv[1]))
		return 1;

	char bufferXmlx[SIZE_BUFFER_XMLX];	//buffer donde se almacena temporalemnte la partes del archivo xmlx
	UserData _data;	//donde se almacena la informacion del archivo xmlx
	XML_Parser parser = XML_ParserCreate(NULL);	//creo el parser

	FILE * xmlx_rss_file = fopen("tst.txt", "r");	//abro el archivo xmlx

	if (xmlx_rss_file == NULL)
		return 1;

	int endParsing = false;	//cuando sea true, parse todo el xmlx
	int dataRead = 0;		//cantidad de boytes que se parsean

	XML_SetUserData(parser, &_data);	//seteo la user data para que sea modificada por el parser/handlers
	XML_SetElementHandler(parser, startTag, endTag);	//seteo los handler para el principio y fin  de ...
	XML_SetCharacterDataHandler(parser, data);	//seteo el handler que se encarga de analizar la data

	do	//recorro todo el xmlx
	{
		dataRead = fread(bufferXmlx, 1, SIZE_BUFFER_XMLX, xmlx_rss_file);//leo SIZE_BUFFER_XMLX bytes del archivo
		endParsing = dataRead < SIZE_BUFFER_XMLX;//evaluo si ya no queda nada mas para leer
		if (XML_Parse(parser, bufferXmlx, dataRead, endParsing) == XML_STATUS_ERROR)//parseo
		{
			fprintf(stderr, "%s at line %"  "u\n", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
			//return 1
		}


	} while (!endParsing);

	std::cout << _data.getChannelName();
	fclose(xmlx_rss_file);
	XML_ParserFree(parser);

	ALLEGRO_EVENT_QUEUE * evQtimer = nullptr;
	ALLEGRO_EVENT_QUEUE * evQkeyboard = nullptr;
	ALLEGRO_TIMER * timer = nullptr;
	ALLEGRO_DISPLAY * display = nullptr;
	ALLEGRO_EVENT ev;

	if(!initAllegro(&evQtimer, &evQkeyboard, &timer, &display))
	{
		std::cout << "Error inicializando Allegro" << std::endl;
		return -1;
	}

	NewsDisplay n(&_data);
	if (!n.isValid())
	{
		std::cout << "Error inicializando display de noticias" << std::endl;
		return -1;
	}
	dispatcher disp(&n, timer);
	bool endScrolling = false;

	al_start_timer(timer);

	do
	{
		if (al_get_next_event(evQkeyboard, &ev))
		{
			endScrolling = disp.dispatch(&ev);
		}
		else if(al_get_next_event(evQtimer, &ev))
		{
			endScrolling = disp.dispatch(&ev);
		}

	} while (!endScrolling);

	shutdownAllegro(&evQtimer, &evQkeyboard, &timer, &display);
	
	return 0;
}


bool initAllegro(ALLEGRO_EVENT_QUEUE ** evQtimer, ALLEGRO_EVENT_QUEUE ** evQkeyboard, ALLEGRO_TIMER ** timer, ALLEGRO_DISPLAY ** display)
{
	if (!al_init() || !al_install_keyboard() || !al_create_display(300, 300))	//por evaluacion lazy, solo se llama a al_install_keyboard() si al_init() fue exitoso
	{
		return false;
	}

	*evQtimer = al_create_event_queue();
	*evQkeyboard = al_create_event_queue();
	*timer = al_create_timer(0.1);
	al_register_event_source(*evQkeyboard, al_get_keyboard_event_source());
	al_register_event_source(*evQtimer, al_get_timer_event_source(*timer));
	return true;
}

void shutdownAllegro(ALLEGRO_EVENT_QUEUE ** evQtimer, ALLEGRO_EVENT_QUEUE ** evQkeyboard, ALLEGRO_TIMER ** timer, ALLEGRO_DISPLAY ** display)
{
	al_unregister_event_source(*evQkeyboard, al_get_keyboard_event_source());
	al_unregister_event_source(*evQtimer, al_get_timer_event_source(*timer));
	al_destroy_event_queue(*evQkeyboard);
	al_destroy_event_queue(*evQtimer);
	al_destroy_timer(*timer);
}