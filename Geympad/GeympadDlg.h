
// GeympadDlg.h: файл заголовка
//

#pragma once
#include <thread>
#include "DS4State.h"

using namespace std;

// Диалоговое окно CGeympadDlg
class CGeympadDlg : public CDialogEx
{
// Создание
public:
	CGeympadDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GEYMPAD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;
	UINT Timer;

	DS4State state;
	DS4State buf;
	libusb_device_handle* handle = NULL;


	HANDLE thr1;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void DrawFont();
	void Start();

	static void procedure_update_ds4_state(CGeympadDlg * item);
	void ReadData(libusb_device_handle* handle, DS4State& state);

	thread* thread_worker;

	//void MainFunc();
	DECLARE_MESSAGE_MAP()
	CRect wnd_rect;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);


};
