#include "myDisplay.h"
#include "definitionsLCD.h"
#include "lcd.h"

#define MAX_S_LEN	CHARXROW*2

myDisplay::myDisplay()
{
	cadd = 1;
	handler = NULL;

	for (unsigned int i = 1; i < 10 && handler == NULL; i++) {//pruebo 10 puertos hasta que uno funcione
		handler = initLCD(i);
	}

	if (handler == NULL) {
		lastError = FT_OK + 1;	//no podemos saber que error hubo en la inicializacion
	}
	else {
		lastError = FT_OK;
	}
}

myDisplay::~myDisplay()
{
	if (handler != NULL)
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
	if (handler == NULL)
		return false;
	
	lastError = lcdWriteIR(handler, LCD_CLEAR);
	//actualiza el cursor automaticamente, no es necesario llamar a lcdUpdateCursor()
	cadd = 1;
	return  !bool(lastError);
}

bool myDisplay::lcdClearToEOL()	
{
	if (handler == NULL)
		return false;

	int oldcadd = cadd;		//se debe mantener la direccion original del cursor
	do
	{	
		lastError = lcdWriteDR(handler, ' ');
		cadd++;
	} while ((cadd % (CHARXROW + 1)) && lastError == FT_OK);

	cadd = oldcadd;			
	lcdUpdateCursor();		//como cadd volvio a su valor original, el cursor vuelve a la posicion que tenia al inicio

	return true;
}

basicLCD & myDisplay::operator<<(const unsigned char c)
{
	if (handler != NULL) {
		lastError = lcdWriteDR(handler, c);
		nextPosition();	
	}
	return *this;
}

basicLCD & myDisplay::operator<<(const unsigned char * c)
{
	if (handler != NULL && c != NULL) {
		std::string sentence((const char*)(c)); //hago lo mismo que para string
		*this << sentence;
	}
	return *this;
}

basicLCD & myDisplay::operator<<(std::string s)
{
	unsigned int len = s.size();
	
	if (len > MAX_S_LEN)	//si la longitud excede el limite
	{
		//solamente quedarse con los ultimos MAX_S_LEN caracteres
		s.erase(s.begin(), s.end() - CHARXROW * 2);
		len = MAX_S_LEN;
	}
	
	for (unsigned int i = 0; i < len && lastError == FT_OK; i++) 
	//escribo solo si no hubo error hasta ahora (notese que si habia error de antes no mando)
	{
		lastError = lcdWriteDR(handler, s[i]);
		nextPosition();
	}

	return *this;
}

bool myDisplay::lcdMoveCursorUp()
{
	bool valid = false;
	//Mover solamente si no se encuentra en la primera fila
	if (handler != NULL && getCursorRow() == SECOND_ROW)
	{
		cursorPosition c = { FIRST_ROW, getCursorCol() };
		lastError =!lcdSetCursorPosition(c);
		lcdUpdateCursor();
		valid =  !lastError;
	}

	return valid;
}

bool myDisplay::lcdMoveCursorDown()
{
	bool valid = false;
	//Mover solamente si se encuentra en la ultima fila
	if (handler != NULL && getCursorRow() == FIRST_ROW)
	{
		cursorPosition c = { SECOND_ROW, getCursorCol() };
		lastError = !lcdSetCursorPosition(c);
		lcdUpdateCursor();
		valid = !lastError;
	}
	
	return valid;
}

bool myDisplay::lcdMoveCursorRight()
{
	bool valid = false;
	//Mover solamente si no se encuentra en la ultima columna
	if (handler != NULL && getCursorCol() < CHARXROW - 1)
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

	if (handler != NULL && getCursorCol())
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

	if (handler != NULL && pos.column<CHARXROW && pos.column>=0 && (pos.row==SECOND_ROW || pos.row==FIRST_ROW))
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
	cursorPosition pos = { cadd / CHARXROW , cadd % CHARXROW };

	return pos;
}

void myDisplay::lcdUpdateCursor()
{
	if(handler != NULL)
		lastError = lcdWriteIR(handler, LCD_SETDDR | cadd - 1);
}

int myDisplay::getCursorRow()
{
	return (int)((cadd - 1) / CHARXROW);
}

int myDisplay::getCursorCol()
{
	return ((cadd - 1) % CHARXROW);
}

void myDisplay :: nextPosition()
{
	cadd++;
	if (getCursorCol() == 0)	// si se cambio de columna, correr el cursor a la otra linea que se muestra
	{
		cursorPosition c = { FIRST_ROW,0 };
		if (getCursorRow() == FIRST_ROW + 1) //si estoy en la fila 1 tengo que ir a la 2da linea, si no a la primera
			c.row = SECOND_ROW;
		lastError = !lcdSetCursorPosition(c);
	}
}