#include <iostream>
#include <cstdio>

#include "expat.h"


#include"NewsItem.h"
#include"ParseXml.h"
#include"UserData.h"

#define MIN_ARGS 1
#define SIZE_BUFFER_XMLX 100000
int main(int argc, char * argv[])
{
	
	if (argc < MIN_ARGS)//si no se enviaron suficientes argumento salgo
		return 1;
	char bufferXmlx[SIZE_BUFFER_XMLX];//buffer donde se almacena temporalemnte la partes del archivo xmlx
	UserData _data;//donde se almacena la informacion del archivo xmlx
	XML_Parser parser = XML_ParserCreate(NULL);//creo el parser
	
	FILE * xmlx_rss_file = fopen(argv[1], "r");//abro el archivo xmlx
	
	if (xmlx_rss_file == NULL)
		return 1;
		
	int endParsing = false;//cuando sea true, parse todo el xmlx
	int dataRead = 0;//cantidad de boytes que se parsean

	XML_SetUserData(parser, &_data);//seteo la user data para que sea modificada por el parser/handlers
	XML_SetElementHandler(parser, startTag, endTag);//seteo los handler para el principio y fin  de ...
	XML_SetCharacterDataHandler(parser, data);//seteo el handler que se encarga de analizar la data

	do//recorro todo el xmlx
	{
		dataRead = fread(bufferXmlx, 1, SIZE_BUFFER_XMLX, xmlx_rss_file);//leo SIZE_BUFFER_XMLX bytes del archivo
		endParsing = dataRead < SIZE_BUFFER_XMLX;//evaluo si ya no queda nada mas para leer
		if (XML_Parse(parser, bufferXmlx, dataRead, endParsing) == XML_STATUS_ERROR)//parseo
		{
			fprintf(stderr,"%s at line %"  "u\n",XML_ErrorString(XML_GetErrorCode(parser)),XML_GetCurrentLineNumber(parser));
			//return 1
		}


	} while (!endParsing);
	std::cout << _data.getChannelName();
	fclose(xmlx_rss_file);
	XML_ParserFree(parser);
	return 0;
}