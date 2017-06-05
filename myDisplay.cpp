#include "myDisplay.h"
#include "definitionsLCD.h"

#ifndef DEBUG
#include "lcd.h"
#else
FT_HANDLE * initLCD(int x) { return (FT_HANDLE *) new int;  }
void closeLcd(FT_HANDLE * deviceHandler) { delete ((int*)deviceHandler); }
#endif

#define MAX_S_LEN 32

#define	FIRST_ROW	0	//numero de fila de las dos que se ven en el lcd
#define	SECOND_ROW	4

myDisplay::myDisplay()
{
	cadd = 1;
	handler = NULL;

	for (unsigned int i = 0; i < 10 && handler == NULL; i++) {//pruebo 10 puertos hasta que uno funcione
		handler = initLCD(i);
	}

	if (handler == NULL) {
		lastError = FT_OK + 1;	//no podemos saber que error hubo en la inicializacion
	}
	else {
		lastError = FT_OK;
	}
#ifdef DEBUG
	for (int i = 0; i < CHARXROW*ROWSXLINE * 2 + 1; i++)
	{
		DDRAM[i] = 'x';
	}
#endif
}

myDisplay::~myDisplay()
{
	closeLcd(handler);
}

bool myDisplay::lcdInitOk()
{
	return !bool(lastError);	//FT_OK == 0, el resto es error
}

FT_STATUS myDisplay::lcdGetError()
{
	return lastError;
}

bool myDisplay::lcdClear()
{
	lastError = lcdWriteIR(handler, LCD_CLEAR);
	//actualiza el cursor automaticamente, no es necesario llamar a lcdUpdateCursor()
#ifdef DEBUG
	for (int i = 0; i < CHARXROW*ROWSXLINE * 2 + 1; i++)
	{
		DDRAM[i] = ' ';
	}
#endif // DEBUG

	cadd = 1;
	return  !bool(lastError);
}

bool myDisplay::lcdClearToEOL()	
{
	int oldcadd = cadd;		//se debe mantener la direccion original del cursor
	do
	{	
		lastError = lcdWriteDR(handler, ' ');
	} while ((cadd % (CHARXROW + 1)) && lastError == FT_OK);

	cadd = oldcadd;			
	lcdUpdateCursor();		//como cadd volvio a su valor original, el cursor vuelve a la posicion que tenia al inicio
	return true;
}

basicLCD & myDisplay::operator<<(const unsigned char c)
{
	lcdWriteDR(handler, c);
	if (getCursorCol() == 0)	// si se cambio de columna, correr el cursor a la otra linea que se muestra
	{
		cursorPosition c = { FIRST_ROW,0 };
		if (getCursorRow() == FIRST_ROW + 1) //si estoy en la fila 1 tengo que ir a la 2da linea, si no a la primera
			c.row = SECOND_ROW;
		lastError = lcdSetCursorPosition(c);
	}

	return *this;
}

basicLCD & myDisplay::operator<<(const unsigned char * c)
{
	std::string sentence((const char*)(c));
	unsigned int len = sentence.size();
	if (len > MAX_S_LEN)	//si la longitud excede el limite
	{
		//solamente quedarse con los ultimos MAX_S_LEN caracteres
		sentence.erase(sentence.begin(), sentence.end() - CHARXROW * 2);	
		len = MAX_S_LEN;
	}
	for (unsigned int i = 0; i < len && lastError == FT_OK; i++)
	{
		lastError = lcdWriteDR(handler, sentence[i]);

		if (getCursorCol() == 0)	// si se cambio de columna, correr el cursor a la otra linea que se muestra
		{
			cursorPosition c = { FIRST_ROW,0 };
			if (getCursorRow() == FIRST_ROW + 1) //si estoy en la fila 1 tengo que ir a la 2da linea, si no a la primera
				c.row = SECOND_ROW;
			lcdSetCursorPosition(c);
		}
	}
	return *this;
}

basicLCD & myDisplay::operator<<(std::string s)
{
	std::string sentence(s);	//copiar el string recibido para poder modificarlo
	unsigned int len = sentence.size();
	
	if (len > MAX_S_LEN)	//si la longitud excede el limite
	{
		//solamente quedarse con los ultimos MAX_S_LEN caracteres
		sentence.erase(sentence.begin(), sentence.end() - CHARXROW * 2);
		len = MAX_S_LEN;
	}
	
	for (unsigned int i = 0; i < len && lastError == FT_OK; i++)
	{
		lastError = lcdWriteDR(handler, sentence[i]);
		if (getCursorCol() == 0)	// si se cambio de columna, correr el cursor a la otra linea que se muestra
		{
			cursorPosition c = { FIRST_ROW,0 };
			if (getCursorRow() == FIRST_ROW + 1) //si estoy en la fila 1 tengo que ir a la 2da linea, si no a la primera
				c.row = SECOND_ROW;
			lcdSetCursorPosition(c);
		}
	}
	return *this;
}

bool myDisplay::lcdMoveCursorUp()
{
	bool valid = false;
	//Mover solamente si no se encuentra en la primera fila
	if (getCursorRow())	
	{
		if (!(cadd %= CHARXROW))	//si se encuentra en la ultima columna, cadd va a ser 0 cuando en realidad deberia ser CHARXROW
		{
			cadd = CHARXROW;
		}
		lcdUpdateCursor();
		valid =  true;
	}

	return valid;
}

bool myDisplay::lcdMoveCursorDown()
{
	bool valid = false;
	//Mover solamente si no se encuentra en la ultima fila
	if (getCursorRow() != ROWSXLINE)
	{
		if (!(cadd %= CHARXROW))	
		{
			cadd += CHARXROW;		// si se encuentra en la ultima columna, cadd va a ser 0 cuando en realidad deberia ser CHARXROW
		}
		cadd += CHARXROW*ROWSXLINE;
		lcdUpdateCursor();
		valid = true;
	}
	
	return valid;
}

bool myDisplay::lcdMoveCursorRight()
{
	bool valid = false;
	//Mover solamente si no se encuentra en la ultima columna
	if (getCursorCol() < CHARXROW - 1)
	{
		cadd++;
		lcdUpdateCursor();
		valid = true;
	}

	return valid;
}

bool myDisplay::lcdMoveCursorLeft()
{	
	//Mover solamente si no se encuentra en la primer columna
	bool valid = false;

	if (getCursorCol())	
	{
		cadd--;
		lcdUpdateCursor();
		valid = true;
	}

	return valid;
}

bool myDisplay::lcdSetCursorPosition(const cursorPosition pos)
{
	bool valid = false;

	if (pos.column<CHARXROW && pos.column>=0 && (pos.row==4 || pos.row==0))
	//solo se puede mover el cursor a las posiciones que se muestran
	{
		cadd = pos.column + pos.row*CHARXROW + 1;
		lcdUpdateCursor();

		valid = true;
	}
	
	return valid;
}

cursorPosition myDisplay::lcdGetCursorPosition()
{
	return { cadd / CHARXROW , cadd % CHARXROW };
}

void myDisplay::lcdUpdateCursor()
{
	lastError = lcdWriteIR(handler, LCD_SETDDR | cadd - 1);	
}

int myDisplay::getCursorRow()
{
	return (int)((cadd - 1) / CHARXROW);
}

int myDisplay::getCursorCol()
{
	return ((cadd - 1) % 0x10);
}

#ifdef DEBUG
void myDisplay::printDDRAM()
{

	for (int i = 1; i <= CHARXROW*ROWSXLINE * 2; i++)
	{
		std::cout << "| " << ((i == cadd) ? (char)2 : (char)DDRAM[i]) << ' ';

		if (!(i % CHARXROW))
		{
			std::cout << '|' << std::endl;
		}
	}
	std::cout << std::endl;
}
FT_STATUS myDisplay::lcdWriteIR(FT_HANDLE * handler, BYTE by)
{
//	printf("\nIR: %X\n", by);
	return FT_OK;
}
FT_STATUS myDisplay::lcdWriteDR(FT_HANDLE * handler, BYTE by)
{
	DDRAM[cadd] = by;
	if (cadd < (CHARXROW*ROWSXLINE * 2))
	{
		cadd++;
	}
	return FT_OK;
}
#endif // DEBUG
