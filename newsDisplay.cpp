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
		itemL->push_back(NewsItem(std::string(CHARXROW, ' '), std::string("Ultimo titular")));	//Elemento que se muestra cuando ya pasaron todas las noticias del feed
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
		std::cout << "Usted esta informado" << std::endl;
		return true;
	}
	return false;
}

bool NewsDisplay::showNews()
{
#ifdef DEBUG
	system("cls");
#endif // DEBUG


	if(scrollPos > fTitle.size())
	{
		return false;	//indicar que no hay caracteres del titulo para mostrar
	}

	//Si hay caracteres para mostrar, borrar display, escribir fecha, posicionar cursor en el inicio de la segunda linea
	display->lcdClear();
	(*display) << currNews->getFormattedPubDate();
	display->lcdSetCursorPosition({ ROWSXLINE, 0 });

	//iterar por los 16 o menos caracteres que deben ser impresos y mandarselos al display
	std::string::iterator it = fTitle.begin();
	std::for_each(it + scrollPos,
		it + std::min(scrollPos + CHARXROW, fTitle.size()),
		[this](const char& titleChar) { (*display) << titleChar; });	//por CHARXROW caracteres, imprimirolos en el display

#ifdef DEBUG
	((myDisplay *)display)->printDDRAM();
#endif // DEBUG

	return true;
}

void NewsDisplay::nextNews()
{
	//currNews, por ser un iterador bidireccional, no tiene definido los operadores '<' o '>' 
	//ni permite escribir 'currNews + 1', por lo que se implementa el siguiente metodo para 
	//controlar que currNews no se exceda de los elementos de la lista. Se prefiere este metodo 
	//por sobre hacer:
	//		if(curr != (--itemL.end()) {currNews++}
	//ya que la implementacion usada en todos los casos menos el ultimo tiene solo un incremento,
	//mientras que la otra opcion tiene un incremento y un decremento

	if (++currNews == itemL->end())	
	{
		currNews--;			//desincrementar si el incremento anterior se "paso" del ultimo item (end() es past-the-end)
	}
	fTitle = std::string(CHARXROW, ' ') + channelName + " - " + currNews->getTitle();
	scrollPos = 0;
}

void NewsDisplay::prevNews()
{
	if (currNews != itemL->begin())
	{
		currNews--;
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
