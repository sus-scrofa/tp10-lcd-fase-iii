#pragma once
#include"ParseXml.h"
#include <string>

class NewsItem		
{
public:
	NewsItem();
	~NewsItem();

	std::string getTitle();
	std::string getPubDate();

private:
	std::string title;		//titulo y pubdate, vienen del parser tan como figuraban en el rss
	std::string pubDate;

	//para simplificar el codigo de los callbacks del parser
	friend void startTag(void *userData, const XML_Char *name, const XML_Char **atts);
	friend void data(void *userData, const XML_Char *s, int len);
	friend void endTag(void *userData, const XML_Char *name);
};
