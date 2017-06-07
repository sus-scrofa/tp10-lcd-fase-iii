#pragma once
#define FTD2XX_EXPORTS
#include"ftd2xx.h"

//	init LCD:
//	se debe llamar a esta rutina antes de empezar a usar el display. recibe el numero de device
//al que se quiere conectar, y devuelve un puntero al handler que permitira luego llamar a WriteIR,
//WriteDr y close. Si hubo algun error, ese puntero se devuelve en null.
FT_HANDLE * initLCD(int iDevice);


//	lcdWriteIR / lcdWriteDR:
//	escriben el byte recibido en el registro de instrucciones o de datos del lcd respectivamente. reciben el 
//handler que devolvio la funcion init y el byte que se quiere escribir.
//	devuelven los codigos de error establecidos en ftd2xx.h (FT_OK == 0 es que salio todo bien, el resto error)
FT_STATUS lcdWriteIR(FT_HANDLE * deviceHandler, BYTE valor);
FT_STATUS lcdWriteDR(FT_HANDLE * deviceHandler, BYTE valor);

//	closeLcd:
//	libera los recursos usados para el handler y apaga el display
void closeLcd(FT_HANDLE * deviceHandler);