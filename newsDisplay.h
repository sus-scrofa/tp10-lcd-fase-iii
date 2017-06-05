#pragma once

#include <string>
#include <list>

#include "myDisplay.h"
#include "basicLCD.h"
#include "UserData.h"

class NewsDisplay
{
public:
	NewsDisplay(UserData * data);
	~NewsDisplay();			//
	bool updateDisplay();	//Actualiza scrollpos. Llama a showNews(). De ser necesario, aumenta currNews.
	void nextNews();		//de no estar en la ultima noticia, mueve el iterador de lista a la proxima noticia. Si se esta en la ultima noticia, mueve el iterador hacia un elemento dummy que indica el final de la lista. Si se esta en el dummy, no realiza cambios en el iterador. ScrollPos cambia a 0.
	void prevNews();		//mueve el iterador a la noticia anterior. Si esta apuntando a la primer noticia, no se realizan cambios. ScrollPos cambia a 0.
	void restartNews();		//No modifica el iterador. ScrollPos cambia a 0.
	bool isValid();			//Indica si hubo error en el constructor.

private:
	bool showNews();				//Se llama en cada evento de timer, muestra en display la fecha de la noticia actual en la primer linea, y la porcion del titulo correspondiente en la segunda linea
	basicLCD * display;				//Contiene un objeto del tipo myDisplay
	unsigned int scrollPos;			//Que caracter del titular es el que se esta mostrando en la columna 0
	std::list<NewsItem> * itemL;	//
	std::list<NewsItem>::iterator currNews;
	std::string channelName;		//Nombre del canal del que se pasan las noticias
	std::string fTitle;				//Title de la noticia actual con el diguiente formato: string(CHARXROW, ' ') +  "channel name" + "title".
	bool valid;						//indica si hubieron errores en el constructor
};