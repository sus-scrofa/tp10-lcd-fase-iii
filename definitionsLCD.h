#pragma once

#define PORT_P0 0
#define PORT_P1 1

#define LCD_E		(1 << PORT_P0)
#define LCD_E_ON	LCD_E
#define LCD_E_OFF	(LCD_E ^ LCD_E)

#define LCD_RS		(1 << PORT_P1)
#define LCD_RS_DR	LCD_RS
#define LCD_RS_IR	(LCD_RS ^ LCD_RS)

//**********instrucciones al LCD**********//

#define LCD_CLEAR	       0b1	//clear display
#define LCD_RETURN	      0b10	//return home
#define LCD_ENTSET	     0b100	//Entry mode set
#define LCD_CONTROL	    0b1000	//Display on/off control
#define LCD_SHIFT	   0b10000	//Cursor or display shift
#define LCD_FUNCSET	  0b100000	//Funtion set
#define LCD_SETCGR	 0b1000000	//Set CGRAM address
#define LCD_SETDDR	0b10000000	//Set DDRAM address

//**********flags de instrucciones**********//

//Entry mode set
#define LCD_F_INC			0b10	//increment
#define LCD_F_DEC			0b00	//decrement
#define LCD_F_ACC_ON		0b1		//Accompanies display shift

//Display on/off control
#define LCD_F_DISPLAY_ON	0b100
#define LCD_F_DISPLAY_OFF	0b0
#define LCD_F_CURSOR_ON		0b10
#define LCD_F_CURSOR_OFF	0b0
#define LCD_F_BLINK_ON		0b1		//cursor blink
#define LCD_F_BLINK_OFF		0b0

//Cursor or display shift
#define LCD_F_DISPLAY_SHIFT	0b1000	
#define LCD_F_CURSOR_SHIFT	0b0
#define LCD_F_SHIFT_RIGHT	0b100
#define	LCD_F_SHIFT_LEFT	0b0

//Function set
#define LCD_F_8BIT			0b10000
#define LCD_F_4BIT			0b0	
#define LCD_F_2LINE			0b1000
#define LCD_F_1LINE			0b0
#define LCD_F_10DOTS		0b100
#define LCD_F_8DOTS			0b0

#define SET_ADDRESS(a,b)	(LCD_SETDDR + (a) + (b) * 0x40)

#define CHARXROW			0x10		//Ancho del display
#define ROWSXLINE			4			//filas de DDRAM asignadas a una linea del display

#define FIRST_ROW			0
#define SECOND_ROW			4
