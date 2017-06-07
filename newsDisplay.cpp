#include "newsDisplay.h"
#include <algorithm>

NewsDisplay::NewsDisplay(UserData * data)
{
	valid = true;
	scrollPos = 0;

	this->itemL = new std::list<NewsItem>(data->getItemList());
	if (itemL->size())
	{
		currNews = this->itemL->begin();
		fTitle = std::string(CHARXROW, ' ') + data->getChannelName() + " - " + currNews->getTitle();
		fDate = currNews->getFormattedPubDate();
	}
	else	//si la lista esta vacia, es imposible mostrar noticias
	{
		valid = false;
	}

	this->channelName = data->getChannelName();

	display = new myDisplay;
	if (!display->lcdInitOk())	
	{
		valid = false;
	}
}

NewsDisplay::~NewsDisplay()
{
	delete display;
	delete itemL;
}

bool NewsDisplay::updateDisplay()
{
	showNews();

	if (scrollPos < (fTitle.size()))	//si hay mas caracteres del titulo para mostrar, mover a la posicion siguiente
	{
		scrollPos++;
	}
	else if(currNews != --(itemL->end()))	//si se termino el titulo actual y no era el ultimo (end() "apunta" past-the-last
	{
		nextNews();
	}
	else	//si se termino el titulo actual y era el ultimo
	{
		return true;
	}
	return false;
}

bool NewsDisplay::showNews()
{	
	if(scrollPos > fTitle.size())
	{
		return false;	//indicar que no hay caracteres del titulo para mostrar
	}
	if (scrollPos == 0)
	{
		display->lcdClear();
		(*display) << fDate;
	}
	//Si hay caracteres para mostrar, borrar display, escribir fecha, posicionar cursor en el inicio de la segunda linea
	display->lcdSetCursorPosition({ SECOND_ROW, 0 });
 	display->lcdClearToEOL();
	//iterar por los 16 o menos caracteres que deben ser impresos y mandarselos al display
	std::string::iterator it = fTitle.begin();

	std::for_each(it + scrollPos,
		it + std::min<unsigned int>(scrollPos + CHARXROW, fTitle.size()),
		[this](const char& titleChar) { (*display) << titleChar; });	//por CHARXROW caracteres, imprimirolos en el display
	
	return true;
}

void NewsDisplay::nextNews()
{
	//currNews, por ser un iterador bidireccional, no tiene definido los operadores '<' o '>' 
	//ni permite escribir 'currNews + 1', por lo que se implementa el siguiente metodo para 
	//controlar que currNews no se exceda de los elementos de la lista. 
	if (!(currNews == itemL->end()))	//si currNews es distinto de end(), es menor.
	{
		currNews++;
		if (currNews == itemL->end())	//Si ya se leyeron todas las noticias
		{
			fDate = std::string("");
			fTitle = std::string(CHARXROW, ' ') + std::string("Ultima noticia");
		}
		else	//Si quedan noticias por leer
		{
			fDate = currNews->getFormattedPubDate();
			fTitle = std::string(CHARXROW, ' ') + channelName + " - " + currNews->getTitle();
		}
	}

	scrollPos = 0;
}

void NewsDisplay::prevNews()
{
	if (currNews != itemL->begin())
	{
		currNews--;
		fDate = currNews->getFormattedPubDate();
		fTitle = std::string(CHARXROW, ' ') + channelName + " - " + currNews->getTitle();
	}
	scrollPos = 0;
}

void NewsDisplay::restartNews()
{
	scrollPos = 0;
}

bool NewsDisplay::isValid()
{
	return valid;
}
