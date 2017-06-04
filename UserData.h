#pragma once

#include <string>
#include <list>
#include "NewsItem.h"
#include "ParseXml.h"

typedef enum states {WAITING_CHANNEL, IN_CHANNEL, IN_CH_IN_OTHER, IN_CH_IN_TITLE, 
					IN_ITEM, IN_IT_IN_TITLE, IN_PUB_DATE, IN_IT_IN_OTHER, FINISHED} state_t;
//indicadores de en que tag del rss nos encontramos

class UserData 
{
public:
	UserData();
	~UserData();

	bool parsedOK();
	std::string getChannelName();
	std::list<NewsItem>& getItemList();

private:
	std::string channelName;
	std::list<NewsItem> itemList;
	NewsItem * currentItem;

	bool isValid;
	state_t whereAmI;				//el tag en el que estoy (si es relevante)
	const char * waitingForEndTag;	//el nombre de la end tag a la que estoy esperando 

	friend void startTag(void *userData, const XML_Char *name, const XML_Char **atts);
	friend void data(void *userData, const XML_Char *s, int len);
	friend void endTag(void *userData, const XML_Char *name);
};
