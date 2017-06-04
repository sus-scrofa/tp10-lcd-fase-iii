#include "UserData.h"

UserData::UserData()
{
	currentItem = NULL;
	whereAmI = WAITING_CHANNEL;	//empiezo desde rss, y no hago nada hasta que llegue channel
	isValid = true;
}

UserData::~UserData()
{
	if (currentItem != NULL)	//por si hubo un error antes de que me llegue el endtag de un item
		delete currentItem;
}


bool UserData::parsedOK()
{
	return isValid;
}

std::string UserData::getChannelName()
{
	return channelName;
}

std::list<NewsItem>& UserData::getItemList()
{
	return itemList;
}
