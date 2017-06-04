#pragma once

#include "expat.h"

// callbacks para el XMLParser de expat

void startTag(void *userData, const XML_Char *name, const XML_Char **atts);
void data(void *userData, const XML_Char *s, int len);
void endTag(void *userData, const XML_Char *name);