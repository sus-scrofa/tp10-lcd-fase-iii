#pragma once

#include <string>
#include <list>
#include "NewsItem.h"
#include "ParseXml.h"

typedef enum states {WAITING_CHANNEL, IN_CHANNEL, IN_CH_IN_OTHER, IN_CH_IN_TITLE, 
					IN_ITEM, IN_IT_IN_TITLE, IN_PUB_DATE, IN_IT_IN_OTHER, FINISHED} state_t;
					//NO CAMBIAR ESTE ORDEN!! ES IMPORTANTE!!

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
	state_t whereAmI;
	const char * waitingForEndTag;	//la end tag a la que estoy esperando 

	friend void data(void *userData, const XML_Char *s, int len);
	friend void startTag(void *userData, const XML_Char *name, const XML_Char **atts);
	friend void endTag(void *userData, const XML_Char *name);
};
