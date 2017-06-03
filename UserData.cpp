#include "UserData.h"

UserData::UserData()
{
	currentItem = NULL;
	whereAmI = WAITING_CHANNEL;
	isValid = true;
}

UserData::~UserData()
{
	if (currentItem != NULL)
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
