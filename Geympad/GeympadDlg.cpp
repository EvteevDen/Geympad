
// GeympadDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Geympad.h"
#include "GeympadDlg.h"
#include "afxdialogex.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CGeympadDlg

void MainFunc();

CGeympadDlg::CGeympadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GEYMPAD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGeympadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CGeympadDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений CGeympadDlg
//void MainFunc();


int perfectTransform(int in)
{
	if (in > pow(2, 15))
	{
		return in - pow(2, 15);
	}
	else
	{
		return in + pow(2, 15);
	}
}

void CGeympadDlg::ReadData(libusb_device_handle* handle, DS4State& state)
{
	unsigned char data[64];
	int received = 0;
	int r = libusb_bulk_transfer(handle, BULK_EP_OUT, data, 64, &received, 0);	//Передача данных
	if (r != 0)
	{
		return;
	}

	//cout << hex << (int)data[5] << endl;
	state.cross = data[5] & 0x20;
	state.triangle = data[5] & 0x80;
	state.square = data[5] & 0x10;
	state.circle = data[5] & 0x40;

	state.krestovina = (DPADState)(data[5]);

	state.left_stick_axis_x = (int)data[1];
	state.left_stick_axis_y = (int)data[2];
	state.right_stick_axis_x = (int)data[3];
	state.right_stick_axis_y = (int)data[4];

	state.battery_Level = (int)data[12];

	state.L1 = data[6] & 1;
	state.R1 = data[6] & 2;
	state.L2 = (int)data[8];
	state.R2 = (int)data[9];

	state.left_stick_click = data[6] & 0x40;
	state.right_stick_click = data[6] & 0x80;

	state.GyroX = data[14] << 8 | data[13];
	state.GyroX = perfectTransform(state.GyroX);
	state.GyroY = data[16] << 8 | data[15];
	state.GyroY = perfectTransform(state.GyroY);
	state.GyroZ = data[18] << 8 | data[17];
	state.GyroZ = perfectTransform(state.GyroZ);


	state.AccelX = data[20] << 8 | data[19];
	state.AccelX = perfectTransform(state.AccelX);
	state.AccelY = data[22] << 8 | data[21];
	state.AccelY = perfectTransform(state.AccelY);
	state.AccelZ = data[24] << 8 | data[23];
	state.AccelZ = perfectTransform(state.AccelZ);

	state.Typeface = (int)data[30] & 0x7B;
	state.bluetooth = (int)data[30] & 0x08;
	state.headphones = (int)data[30] & 0x3B;

	state.T_PAD = (int)data[34];
	state.T_PAD1 = (int)data[35] & 0x80;
	if (state.T_PAD1 == 0) state.t_pad1 = "Touch number:";
	else state.t_pad1 = "You let go of your finger:";
	state.T_PAD2 = (int)data[39] & 0x80;

	state.Koord_finger1 = (int)data[40];
	state.Koord_finger2 = (int)data[41];
	state.Koord_finger3 = (int)data[42];

	int C1 = state.Koord_finger2 & state.lowMask1;
	int D1 = state.Koord_finger2 & state.lowMask2;

	state.xx = (D1 << 8) | state.Koord_finger1;
	state.yy = (state.Koord_finger3 << 4) | (C1 >> 4);

	state.Koord_finger11 = (int)data[36];
	state.Koord_finger21 = (int)data[37];
	state.Koord_finger31 = (int)data[38];

	int C2 = state.Koord_finger21 & state.lowMask1;
	int D2 = state.Koord_finger21 & state.lowMask2;

	state.x = (D2 << 8) | state.Koord_finger11;
	state.y = (state.Koord_finger31 << 4) | (C2 >> 4);

	state.Share = (int)data[6] & 16;
	state.opt = (int)data[6] & 32;
	state.dpad = (DPADState)(data[5] & 0xF);
	state.ps_button = data[7] & 0x1;
	state.counter = data[7] >> 2;
	state.tpad_click = data[7] & 0x2;
}

void CGeympadDlg::procedure_update_ds4_state(CGeympadDlg* item)
{
	while (1)
	{
		item->ReadData(item->handle, item->buf);
	}
}

BOOL CGeympadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	//MainFunc();
	
	libusb_context* context = NULL;
	int r = 0;

	r = libusb_init(&context);
	if (r < 0)
	{
		MessageBox(L"Something wrong", L"Error", MB_ICONERROR);
		return FALSE;
	}


	handle = libusb_open_device_with_vid_pid(context, VID, PID);	//Поиск устр
	if (handle == NULL)
	{
		MessageBox(L"Error in device opening!", L"Error", MB_ICONERROR);
		return FALSE;
	}



	r = libusb_claim_interface(handle, 3);	//Заявить интерфейс
	if (r < 0)
	{
		MessageBox(L"Cannot Claim Interface", L"Error", MB_ICONERROR);
		return FALSE;
	}

	libusb_set_auto_detach_kernel_driver(handle, 1);	//Автоотсоединение

	unsigned char data[64];

	GetDlgItem(IDC_MAINPIC)->GetClientRect(&wnd_rect);


	thread_worker = new thread(procedure_update_ds4_state, this);


	Timer = SetTimer(1, 10, NULL);

	Invalidate(0);


	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CGeympadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CGeympadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawFont();


		CDialogEx::OnPaint();
	}

}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CGeympadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGeympadDlg::DrawFont()
{
	
	CDC* dc = GetDlgItem(IDC_MAINPIC)->GetDC();
	HDC hdc = *dc;

	Gdiplus::Graphics gr(hdc);
	CRect obj_rect;
	GetDlgItem(IDC_MAINPIC)->GetClientRect(&obj_rect);
	//gr.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	//gr.Clear(Gdiplus::Color::White);
	Gdiplus::Image imgFont(L"Font.bmp");
	Gdiplus::Image imgPointerDown(L"pointer.bmp");
	Gdiplus::Image imgPointerUp(L"pointer2.bmp");
	Gdiplus::Image imgPointerRight(L"pointer3.bmp");
	Gdiplus::Image imgPointerLeft(L"pointer1.bmp");
	Gdiplus::Image imgBattary(L"battary.bmp");
	Gdiplus::Pen pen(Gdiplus::Color::Red, 8);
	
	Gdiplus::Bitmap myBitmap(imgFont.GetWidth(), imgFont.GetHeight(), &gr);

	//int xPading = 0;
	//int yPading = 0;
	Gdiplus::Graphics grr(&myBitmap);
	grr.DrawImage(&imgFont, 0, 0);

	Gdiplus::FontFamily FF(L"Courier");
	Gdiplus::Font font(&FF, 14, Gdiplus::FontStyle::FontStyleUnderline , Gdiplus::UnitPixel);
	Gdiplus::SolidBrush brush(Gdiplus::Color::Black);

	grr.DrawImage(&imgBattary, 0, 0, 50, 50);

	//Аккум
	CString str;
	str.Format(L"%i", (int)buf.battery_Level);
	str += L"%";
	Gdiplus::PointF strPoint(9, 18);
	grr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

	//R2 L2
	if(buf.R2>0)
		grr.DrawImage(&imgPointerDown, 405, 0, 120, 30);
	if (buf.L2 > 0)
		grr.DrawImage(&imgPointerDown, 75, 0, 120, 30);

	//R1 L1
	if (buf.R1)
		grr.DrawImage(&imgPointerDown, 460, 0, 20, 30);
	if (buf.L1)
		grr.DrawImage(&imgPointerDown, 120, 0, 20, 30);

	//PS Button
	if(buf.ps_button)
		grr.DrawEllipse(&pen, 285, 187, 30, 30);

	//Share Opt
	if(buf.Share)
		grr.DrawImage(&imgPointerDown, 173, 40, 30, 40);
	if (buf.opt)
		grr.DrawImage(&imgPointerDown, 398, 40, 30, 40);

	//Buttons
	if(buf.triangle)
		grr.DrawEllipse(&pen, 454, 65, 35, 35);
	if(buf.cross)
		grr.DrawEllipse(&pen, 454, 145, 35, 35);
	if(buf.circle)
		grr.DrawEllipse(&pen, 495, 106, 35, 35);
	if(buf.square)
		grr.DrawEllipse(&pen, 414, 106, 35, 35);

	//TrakPad
	if (buf.tpad_click)
		grr.DrawRectangle(&pen, 210, 50, 180, 90);

	if (buf.t_pad1=="Touch number:")
		grr.DrawEllipse(&pen, 210 + buf.x * 180 / 2000, 50 + buf.y * 90 / 1000, 10, 10);

	//Krestovina
	if (buf.krestovina == 0)
		grr.DrawImage(&imgPointerUp, 109, 69, 40, 30);
	if (buf.krestovina == 2)
		grr.DrawImage(&imgPointerRight, 155, 105, 30, 40);
	if (buf.krestovina==4)
		grr.DrawImage(&imgPointerDown, 109, 155, 40, 30);
	if (buf.krestovina == 6)
		grr.DrawImage(&imgPointerLeft, 72, 105, 30, 40);

	//Left Stik
	if (buf.left_stick_axis_x < 60 && buf.left_stick_axis_y<60)
		grr.DrawArc(&pen, 180, 162, 70, 70, 180, 90);
	else if(buf.left_stick_axis_x < 180 && buf.left_stick_axis_y < 60)
		grr.DrawArc(&pen, 180, 162, 70, 70, 225, 90);
	if (buf.left_stick_axis_x > 180 && buf.left_stick_axis_y < 60)
		grr.DrawArc(&pen, 180, 162, 70, 70, 270, 90);
	else if(buf.left_stick_axis_x > 180 && buf.left_stick_axis_y < 180)
		grr.DrawArc(&pen, 180, 162, 70, 70, 315, 90);
	if (buf.left_stick_axis_x > 180 && buf.left_stick_axis_y > 180)
		grr.DrawArc(&pen, 180, 162, 70, 70, 0, 90);
	else if (buf.left_stick_axis_x > 60 && buf.left_stick_axis_y > 180)
		grr.DrawArc(&pen, 180, 162, 70, 70, 45, 90);
	if (buf.left_stick_axis_x < 60 && buf.left_stick_axis_y > 180)
		grr.DrawArc(&pen, 180, 162, 70, 70, 90, 90);
	else if(buf.left_stick_axis_x < 60 && buf.left_stick_axis_y > 60)
		grr.DrawArc(&pen, 180, 162, 70, 70, 135, 90);

	//Right Stik
	if (buf.right_stick_axis_x < 60 && buf.right_stick_axis_y < 60)
		grr.DrawArc(&pen, 354, 162, 70, 70, 180, 90);
	else if (buf.right_stick_axis_x < 180 && buf.right_stick_axis_y < 60)
		grr.DrawArc(&pen, 354, 162, 70, 70, 225, 90);
	if (buf.right_stick_axis_x > 180 && buf.right_stick_axis_y < 60)
		grr.DrawArc(&pen, 354, 162, 70, 70, 270, 90);
	else if (buf.right_stick_axis_x > 180 && buf.right_stick_axis_y < 180)
		grr.DrawArc(&pen, 354, 162, 70, 70, 315, 90);
	if (buf.right_stick_axis_x > 180 && buf.right_stick_axis_y > 180)
		grr.DrawArc(&pen, 354, 162, 70, 70, 0, 90);
	else if (buf.right_stick_axis_x > 60 && buf.right_stick_axis_y > 180)
		grr.DrawArc(&pen, 354, 162, 70, 70, 45, 90);
	if (buf.right_stick_axis_x < 60 && buf.right_stick_axis_y > 180)
		grr.DrawArc(&pen, 354, 162, 70, 70, 90, 90);
	else if (buf.right_stick_axis_x < 60 && buf.right_stick_axis_y > 60)
		grr.DrawArc(&pen, 354, 162, 70, 70, 135, 90);

	if (buf.left_stick_click)
		grr.DrawEllipse(&pen, 190, 172, 50, 50);
	if (buf.right_stick_click)
		grr.DrawEllipse(&pen, 364, 172, 50, 50);

	gr.DrawImage(&myBitmap, 0, 0);

}


bool IsInterestingDev(libusb_device* dev)
{
	libusb_device_descriptor desc;
	libusb_get_device_descriptor(dev, &desc);
	cout << desc.idProduct << endl;
	if (desc.idProduct == 2508)return true;
	//if (desc.idProduct == 1476)return true;
	//if (desc.idProduct == 3)return true;
	return false;
}









void CGeympadDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	//ReadData(handle, buf);

	//if (state != buf)
	InvalidateRect(wnd_rect,0);
	//	Invalidate(FALSE);
	//state = buf;




}
