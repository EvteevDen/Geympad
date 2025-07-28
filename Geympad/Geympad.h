
// Geympad.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CGeympadApp:
// Сведения о реализации этого класса: Geympad.cpp
//

class CGeympadApp : public CWinApp
{
private:
	Gdiplus::GdiplusStartupInput input;
	ULONG_PTR token;
public:
	CGeympadApp();

// Переопределение
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CGeympadApp theApp;
