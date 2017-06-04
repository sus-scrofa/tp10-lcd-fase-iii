#include "ParseXml.h"
#include "UserData.h"

void startTag(void * userData, const XML_Char * name, const XML_Char ** atts)
{
	UserData * ud = (UserData *)userData;
	if (ud->isValid == false)	//si ya habia habido error, no hago nada
		return;

	switch (ud->whereAmI)
	{
	case WAITING_CHANNEL:	//aca solo le doy bola a channel
		if (strcmp(name, "channel") == 0) {
			ud->whereAmI = IN_CHANNEL;
			ud->waitingForEndTag = name;
		}
		break;

	case IN_CHANNEL:
		ud->waitingForEndTag = name;

		if (strcmp(name, "item") == 0) {
			ud->whereAmI = IN_ITEM;
			ud->currentItem = new NewsItem;	//en este nuevo objeto voy a guardar lo que me vaya llegando de item
		}
		else if (strcmp(name, "title") == 0) {
			ud->whereAmI = IN_CH_IN_TITLE; //la proxima data va a ser channelTitle
		}
		else { //para no tomar title de algo interno como el title del channel
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
		else { //para no tomar title de algo interno como el title del item
			ud->whereAmI = IN_IT_IN_OTHER;
		}
		break;
	}
}

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

void endTag(void * userData, const XML_Char * name)
{
	UserData * ud = (UserData *)userData;
	if (ud->isValid == false || ud->waitingForEndTag == NULL) {
		ud->isValid = false;
		return;
	}
	if (strcmp(ud->waitingForEndTag, name) == 0) {// solo si llego el endtag que estaba esperando
		switch (ud->whereAmI) {  //(puede que no haya registrado el startTag porque no era relevante)
		case IN_CHANNEL:
			ud->waitingForEndTag = NULL; //una vez que llega el endtag de channel ya tengo todo lo que necesito
			ud->whereAmI = FINISHED;
			break;

		case IN_ITEM:
			ud->itemList.push_back(*(ud->currentItem));	//ya llego toda la data de este item, lo guardo
			delete ud->currentItem;
			ud->currentItem = NULL;
	
		// ES ADREDE QUE NO HAY BREAK PARA ESTE CASO,
		// para item hay que hacer lo mismo que para otros elementos internos de channel pero un par de cosas mas tambien
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
