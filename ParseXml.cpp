#include "ParseXml.h"
#include "UserData.h"

void data(void * userData, const XML_Char * s, int len)
{
	UserData * ud = (UserData *)userData;
	if (ud->isValid == false) 
		return;

	switch (ud->whereAmI) {
	case IN_CH_IN_TITLE:
		ud->channelName.append(s, len); //si esta vacia escribo lo que me mandan, si no lo agrego
		break;

	case IN_IT_IN_TITLE:
		ud->currentItem->title.append(s, len);
		break;

	case IN_PUB_DATE:
		ud->currentItem->pubDate.append(s, len);
		break;
	}
}

void startTag(void * userData, const XML_Char * name, const XML_Char ** atts)
{
	UserData * ud = (UserData *)userData;
	if (ud->isValid == false)
		return;

	switch (ud->whereAmI)
	{
	case WAITING_CHANNEL:
		if (strcmp(name, "channel") == 0) {
			ud->whereAmI = IN_CHANNEL;
			ud->waitingForEndTag = name;
		}

	case IN_CHANNEL:
		ud->waitingForEndTag = name;

		if (strcmp(name, "item") == 0) {
			ud->whereAmI = IN_ITEM;
			ud->currentItem = new NewsItem;
		}
		else if (strcmp(name, "title")) {
			ud->whereAmI = IN_CH_IN_TITLE;
		}
		else {
			ud->whereAmI = IN_CH_IN_OTHER;
		}
		break;

	case IN_ITEM:
		ud->waitingForEndTag = name;
		if (strcmp(name, "title") == 0) {
			ud->whereAmI = IN_IT_IN_TITLE;
		}
		else if (strcmp(name, "pubDate") == 0) {
			ud->whereAmI = IN_PUB_DATE;
		}
		else {
			ud->whereAmI = IN_IT_IN_OTHER;
		}
	}
}

void endTag(void * userData, const XML_Char * name)
{
	UserData * ud = (UserData *)userData;
	if (ud->isValid == false || ud->waitingForEndTag == NULL) {
		ud->isValid = false;
		return;
	}
	if (strcmp(ud->waitingForEndTag, name) == 0) {
		switch (ud->whereAmI) { // llego el endtag que estaba esperando!
		case IN_CHANNEL:
			ud->waitingForEndTag = NULL;
			ud->whereAmI = FINISHED;
			break;

		case IN_ITEM:
			ud->itemList.push_back(*(ud->currentItem));
			delete ud->currentItem;
			ud->currentItem = NULL;
	
		// ES ADREDE QUE NO HAY BREAK PARA ESTE CASO,
		// hay que hacer lo mismo que para otros elementos en channel pero un par de cosas mas tambien
		case IN_CH_IN_OTHER: case IN_CH_IN_TITLE:
			ud->waitingForEndTag = "channel";
			ud->whereAmI = IN_CHANNEL;
			break;

		case IN_IT_IN_TITLE: case IN_PUB_DATE: case IN_IT_IN_OTHER:
			ud->waitingForEndTag = "item";
			ud->whereAmI = IN_ITEM;
			break;
		}
	}
}
