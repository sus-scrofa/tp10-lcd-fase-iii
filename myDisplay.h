#pragma once

#include "basicLCD.h"
#include <iostream>
#include <string>

#include "definitionsLCD.h"

#ifdef DEBUG
typedef int FT_HANDLE;
typedef int FT_STATUS;
typedef char BYTE;
#define FT_OK 0
#endif

class myDisplay : public basicLCD
{
public:
	myDisplay();
	~myDisplay();
	bool lcdInitOk();
	FT_STATUS lcdGetError();
	bool lcdClear();
	bool lcdClearToEOL();
	basicLCD & operator<<(const unsigned char c);
	basicLCD & operator<<(const unsigned char * c);
	basicLCD & operator<<(std::string s);
	bool lcdMoveCursorUp();
	bool lcdMoveCursorDown();
	bool lcdMoveCursorRight();
	bool lcdMoveCursorLeft();
	bool lcdSetCursorPosition(const cursorPosition pos);
	cursorPosition lcdGetCursorPosition();

#ifdef DEBUG
	BYTE DDRAM[CHARXROW * ROWSXLINE * 2 + 1];
	void printDDRAM();
	FT_STATUS lcdWriteIR(FT_HANDLE * handler, BYTE by);
	FT_STATUS lcdWriteDR(FT_HANDLE * handler, BYTE by);
#endif // DEBUG

protected:
	void lcdUpdateCursor();
	int cadd;
	int getCursorRow();
	int getCursorCol();

private:
	FT_HANDLE * handler;
	FT_STATUS lastError;
};

