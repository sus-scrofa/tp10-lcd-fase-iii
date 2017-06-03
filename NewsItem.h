#pragma once

#include <string>

class NewsItem
{
public:
	//NewsItem();
	//~NewsItem();

private:
	std::string title;
	std::string pubDate;

	friend class UserInfo;
	friend void data(void *userData, const XML_Char *s, int len);
	friend void startTag(void *userData, const XML_Char *name, const XML_Char **atts);
	friend void endTag(void *userData, const XML_Char *name);
};
