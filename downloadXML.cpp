#include "downloadXML.h"

#include "cliente.h"
#include <cstdio>
#define SIZE_SEND 1000
#define SIZE_RECIVE 1000
#define OPEN_MODE "w+"
#define PORT_TO_CONNECT "80"

#define GET_COMAND(x) ("GET " + (x) + " HTTP/1.1")

downloadXML::downloadXML()
{
}

downloadXML::~downloadXML()
{
}

bool downloadXML::saveXMLtoFile(std::string filename, std::string linkOfXML)
{



	cliente * client = NULL;
	char buffSend[SIZE_SEND];
	char buffRecive[SIZE_RECIVE];

	int sizedata = 0;

	FILE * outputFile = fopen(filename.c_str(), OPEN_MODE);
	if (outputFile == NULL)
		return false;

	std::string linkData = getHost(linkOfXML);
	if (linkData.empty())
		return false;
//**********************CONECTANDOSE AL SERVIDOR**************************
	client = new cliente;
	client->ConectToServer(linkData.c_str(),PORT_TO_CONNECT);
//************************************************************************
	

//*********************SOLICITANDO INFORMACION AL SERVER******************
	
	
	
	if ((linkData = getUrl(linkOfXML)).empty())
		return false;
	linkData = GET_COMAND(linkData);
	strcpy(buffSend,linkData.c_str());
	sizedata = strlen(buffSend);
	buffSend[sizedata] = 0x0D;
	buffSend[sizedata + 1] = 0x0A;
	client->sendData(buffSend, sizedata + 2);
	
	if ((linkData = getHost(linkOfXML)).empty())
		return false;
	linkData = "Host: " + linkData;
	strcpy(buffSend, linkData.c_str());
	sizedata = strlen(buffSend);
	buffSend[sizedata] = 0x0D;
	buffSend[sizedata + 1] = 0x0A;
	client->sendData(buffSend, sizedata + 2);
	
	buffSend[0] = 0x0D;
	buffSend[1] = 0x0A;
	client->sendData(buffSend, 2);
//*************************************************************************	
	
	int i;//variable donde se almacenana los caracteres recividos del servidor, y tambien si hubo o no error
	do {
		i = client->receiveDataForServidor(buffRecive, SIZE_RECIVE);
		if (i != MY_EMPTY&&i != MY_ERROR)
		{
			for (int j = 0; j < i; j++)//guardo lo recivido en el archvio deseado.
			{
				_fputc_nolock(buffRecive[j], outputFile);
			}
		}
	} while (i != MY_ERROR);
			
	fclose(outputFile);//guardo todo en el archivo
	client->~cliente();//termino la coneccion con el servidor
	

	return 0;
		
}

std::string downloadXML::getHost(std::string link)
{
	size_t whereIsTheString = 0;
	if( ( (whereIsTheString=link.find("http")) != std::string::npos)&& whereIsTheString==0)
	{
		link=link.substr(7);
	if((whereIsTheString = link.find("/"))== std::string::npos)
		return std::string();
		return link.substr(0, whereIsTheString);
		
	}
	else if (((whereIsTheString = link.find("www")) != std::string::npos) && whereIsTheString ==0)
	{
		link = link.substr(4);
		if((whereIsTheString = link.find("/"))== std::string::npos)
			return std::string();
		return link.substr(0, whereIsTheString);

	}
	else
	{
		if ((whereIsTheString = link.find("/")) == std::string::npos)
			return std::string();
		return link.substr(0, whereIsTheString);
	}
	
	return std::string();
}

std::string downloadXML::getUrl(std::string link)
{
	size_t whereIsTheString = 0;
	if (((whereIsTheString = link.find("http")) != std::string::npos) && whereIsTheString == 0)
	{
		link = link.substr(7);
		if ((whereIsTheString = link.find("/")) == std::string::npos)
			return std::string();
		return link.substr(whereIsTheString, std::string::npos);

	}
	else if (((whereIsTheString = link.find("www")) != std::string::npos) && whereIsTheString == 0)
	{
		link = link.substr(4);
		if ((whereIsTheString = link.find("/")) == std::string::npos)
			return std::string();
		return link.substr(whereIsTheString , std::string::npos);

	}
	else
	{
		if ((whereIsTheString = link.find("/")) == std::string::npos)
			return std::string();
		return link.substr(whereIsTheString , std::string::npos);
	}

	return std::string();
}

