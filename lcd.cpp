#include "lcd.h"
#include "definitionsLCD.h"

#include <thread>
#include <chrono>
#define SIZE_BUFFER 1

#define SET_BIT_MODE_MASK	0XFF
#define SET_BIT_MODE_MODE	1


#define IR 0X00
#define DR 0X02

FT_STATUS lcdWriteNibble(FT_HANDLE * h, BYTE value, BYTE rs);
FT_STATUS lcdWriteByte(FT_HANDLE * h, BYTE value, BYTE rs);


FT_HANDLE * initLCD(int iDevice)
{
	FT_HANDLE * h = new FT_HANDLE;
	
	if (FT_Open(iDevice, h) == FT_OK)
	{
		if (FT_SetBitMode(*h, SET_BIT_MODE_MASK, SET_BIT_MODE_MODE) == FT_OK)
		{
			if (lcdWriteNibble(h, 0x03, IR) == FT_OK)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				if (lcdWriteNibble(h, 0x03, IR) == FT_OK)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					if (lcdWriteNibble(h, 0x03, IR) == FT_OK)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
						if (lcdWriteNibble(h, 0x02, IR) == FT_OK)//modo 4 bits;
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(1));
							if (lcdWriteByte(h, LCD_FUNCSET | LCD_F_2LINE, IR) == FT_OK)
							{
								if (lcdWriteByte(h,LCD_CONTROL|LCD_F_DISPLAY_ON|LCD_F_CURSOR_ON|LCD_F_BLINK_ON,IR)==FT_OK)
								{
									if (lcdWriteByte(h, LCD_CLEAR, IR) == FT_OK)
									{
										if (lcdWriteByte(h, LCD_ENTSET|LCD_F_INC, IR) == FT_OK)
										{
											return h; //salio todo bien, devuelvo el handler
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	delete h;	//hubo error: devuelvo null, borro el handler
	return NULL;
}

FT_STATUS lcdWriteNibble(FT_HANDLE * h, BYTE value,BYTE rs)
{
	FT_STATUS status = FT_OK;
	DWORD byteSent = 0;
	char buffer[SIZE_BUFFER];
	
	buffer[0] = ((value << 4) & 0xF0);//guardo en el buffer en su parte alta el nibble enviado a traves de value, y borro el resto del nible

	if (rs != IR && rs != DR)
	{
		return ++status; //devolver un error cualquiera
	}

	buffer[0] |= rs;//pongo en modo correspondiente el bit rs;
	if ((status = FT_Write(*h, buffer, SIZE_BUFFER, &byteSent)) == FT_OK)//enable en 0
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		buffer[0] |= LCD_E_ON;//enable en 1
		if ((status = FT_Write(*h, buffer, SIZE_BUFFER, &byteSent)) == FT_OK)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			buffer[0] &= LCD_E_OFF;//enable en 0
			if ((status = FT_Write(*h, buffer, SIZE_BUFFER, &byteSent)) == FT_OK)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
	}
	return status;
}

FT_STATUS lcdWriteByte(FT_HANDLE * h, BYTE value, BYTE rs)
{
	FT_STATUS status = lcdWriteNibble(h, value >> 4, rs); //primero la parte alta
	if (status == FT_OK)
	{
		status = lcdWriteNibble(h, value, rs);	//si se mando bien la alta, mando la baja
	}

	return status;
}

FT_STATUS lcdWriteIR(FT_HANDLE * deviceHandler, BYTE valor)
{
	return lcdWriteByte(deviceHandler, valor, IR);
}

FT_STATUS lcdWriteDR(FT_HANDLE * deviceHandler, BYTE valor)
{
	return lcdWriteByte(deviceHandler, valor, DR);
}

void closeLcd(FT_HANDLE * deviceHandler)
{
	lcdWriteIR(deviceHandler, LCD_CONTROL | LCD_F_DISPLAY_OFF);	//apago el display
	FT_Close(deviceHandler);									
	delete deviceHandler;
}
