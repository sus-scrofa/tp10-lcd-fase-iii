#include "NewsItem.h"
#include <cstdio>

NewsItem::NewsItem()
{
	;
}

NewsItem::~NewsItem()
{
	;
}

std::string NewsItem::getTitle()
{
	return title;
}

std::string NewsItem::getPubDate()
{
	return pubDate;
}

std::string NewsItem::getFormattedPubDate()
{
	std::string pd;

	if (pubDate.size() != 0) {

		size_t pos = pubDate.find_first_of(',');
		if (pos == std::string::npos) {
			pos = 0;	//si no hay una coma, el dia esta al principio
		}
		else {
			pos+=2; //apunto a despues de la coma (ademas hay un espacio)
		}

		pd = pubDate.substr(pos, 2);
		pd.append("/");	//hasta ahora tengo "DD/"
		pos += 3;		//apunto a donde empieza el mes

		char * name[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
		char month[] = { pubDate[pos], pubDate[pos + 1], pubDate[pos + 2] };
		unsigned int n = 0;

		while (n < 12) {
			if (strncmp(name[n], month, 3) == 0) {
				snprintf(month, 3, "%02u", n+1);	//guardo el numero de mes en formato string
				month[2] = '/';
				pd.append(month, 3);				//ahora tengo DD/MM/

				n = 12;								//salgo del loop
			}
			n++;
		}

		pos += 4;							//apunto a donde empieza el anio
		pd.append(pubDate.substr(pos, 4));	//obtengo los cuatro digitos
		pd.append(" - ");					//ahora tengo "DD/MM/YYYY - "
	
		pd += pubDate.substr(pos + 5, 5); //copio la hora

	}

	return pd;
}

