#pragma once

#include "basicLCD.h"
#include "definitionsLCD.h"


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

protected:
	void lcdUpdateCursor();
	int cadd;
	int getCursorRow();
	int getCursorCol();
	void nextPosition();

private:
	FT_HANDLE * handler;
	FT_STATUS lastError;
};

