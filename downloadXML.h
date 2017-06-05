#pragma once
#include <iostream>
class downloadXML
{
public:
	downloadXML ();
	~downloadXML();
	//saveXMLtoFile
	//filename: nombre del archivo que se creara con el xml mas la estension
	//linkOfXML: link del xml que se desea descargar
	//crea un archivo con el nombre del "filename", con el contenido del link
	//devuelve true si se creo correctamente
	//IMPORTANTE: esta funcion es bloqueante, y detiene la ejecucion por 2 segundos.
	bool saveXMLtoFile(std::string filename,std::string linkOfXML);

private:
	std::string getHost(std::string link);
	std::string getUrl(std::string link);
};

