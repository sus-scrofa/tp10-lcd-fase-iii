#pragma once

#include "expat.h"

void data(void *userData, const XML_Char *s, int len);
void startTag(void *userData, const XML_Char *name, const XML_Char **atts);
void endTag(void *userData, const XML_Char *name);