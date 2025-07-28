#pragma once
#pragma comment(lib, "libusb-1.0.lib")
#include <iostream>
#include <iomanip>
#include "libusb.h"
#include <chrono>
#include <thread>
#include "DS4ControllerSource.h"
using namespace std;

enum DPADState
{
	N,
	NE,
	E,
	SE,
	S,
	SW,
	W,
	NW
};

struct DS4State
{
	bool ps_button;
	int counter;
	bool tpad_click;
	bool up, down, left, right;
	bool cross, triangle, square, circle;
	int battery_Level;
	int left_stick_axis_x;
	int left_stick_axis_y;
	int right_stick_axis_x;
	int right_stick_axis_y;
	bool left_stick_click;
	bool right_stick_click;
	int L2;
	bool L1;
	bool R1;
	string Hammers;
	int R2;
	int GyroX;
	int GyroY;
	int GyroZ;

	int AccelX;
	int AccelY;
	int AccelZ;
	int Typeface;
	int headphones;
	int bluetooth;

	int T_PAD;
	int T_PAD1;
	string t_pad1;
	int T_PAD2;
	int Koord_finger1, Koord_finger2, Koord_finger3;
	int Koord_finger11, Koord_finger21, Koord_finger31;
	int xx = 0, yy = 0, x = 0, y = 0;
	const unsigned int lowMask1 = 0xF0;
	const unsigned int lowMask2 = 0x0F;

	int krestovina;
	int opt;
	int Share;
	DPADState dpad;

	int rumble_l = 0,
		rumble_r = 0,
		r = 0,
		g = 0,
		b = 50,
		crc = 0,
		volume = 50,
		flash_bright = 150,
		flash_dark = 150;

	void print()
	{
		cout << "Заряд: " << (int)(battery_Level / 255.0 * 100) << "%" << endl;
		cout << "PS Button: " << (ps_button ? "нажата" : "не нажата") << endl;
		cout << "Счетчик: " << counter << endl; //!!!!!!!!!!!!!!!!!!!
		cout << "T-Pad: " << (tpad_click ? "нажат" : "не нажат") << endl;
		cout << endl;
		cout << "Share: " << (Share ? "нажата" : "не нажата") << "\t" << "Options: " << (opt ? "нажата" : "не нажата") << endl;
		cout << endl;
		cout << "Крестик: " << (cross ? "нажат" : "не нажат") << endl;
		cout << "Треугольник: " << (triangle ? "нажат" : "не нажат") << endl;
		cout << "Квадрат: " << (square ? "нажат" : "не нажат") << endl;
		cout << "Круг: " << (circle ? "нажат" : "не нажат") << endl;
		cout << endl;
		cout << "L1: " << ((L1 ? "нажат" : "не нажат")) << "\t" << "R1: " << ((R1 ? "нажат" : "не нажат")) << endl;
		cout << "L2: " << L2 << "\t" "R2: " << R2 << endl;
		cout << "Левый стик X: " << dec << left_stick_axis_x << "\t"
			<< "Левый стик Y: " << left_stick_axis_y << endl;
		cout << "Правый стик X: " << right_stick_axis_x << "\t"
			<< "Правый стик Y: " << right_stick_axis_y << "\t" << endl;
		cout << "Нажатие левый стик: " << '\t' << (left_stick_click ? "нажат" : "не нажат") << endl;
		cout << "Нажатие правый стик: " << '\t' << (right_stick_click ? "нажат" : "не нажат") << endl;
		cout << endl;
		switch ((krestovina & 0xF))
		{
		case 0: {cout << "Крестовина: Вверх" << endl; break; }
		case 1: {cout << "Крестовина: Вверх + Вправо" << endl; break; }
		case 2: {cout << "Крестовина: Вправо" << endl; break; }
		case 3: {cout << "Крестовина: Вниз + Вправо" << endl; break; }
		case 4: {cout << "Крестовина: Вниз" << endl; break; }
		case 5: {cout << "Крестовина: Вниз + Влево" << endl; break; }
		case 6: {cout << "Крестовина: Влево" << endl; break; }
		case 7: {cout << "Крестовина: Вверх + Влево" << endl; break; }
		default: {cout << "Крестовина: не нажата" << endl; break; }
		}
		cout << endl;
		cout << "GyroX:" << "\t" << GyroX << endl;
		cout << "GyroY:" << "\t" << GyroY << endl;
		cout << "GyroZ:" << "\t" << GyroZ << endl;
		cout << endl;
		cout << "AccelX:" << "\t" << AccelX << endl;
		cout << "AccelY:" << "\t" << AccelY << endl;
		cout << "AccelZ:" << "\t" << AccelZ << endl;
		cout << endl;
		cout << "Гарнитура: " << (Typeface ? "не подключена" : "подключена") << "\t"
			<< "Наушники: " << (headphones ? "не подключены" : "подключены") << "\t"
			<< "Вluetooth: " << (bluetooth ? "не подключен" : "подключен") << endl;
		cout << endl;
		if (!(T_PAD1))cout << "Палец 1 касается" << endl;
		else cout << "Палец 1 не касается" << endl;
		if (!(T_PAD2))cout << "палец 2 касается" << endl;
		else cout << "Палец 2 не касается" << endl;
		cout << endl;
		cout << "Координаты пальца 1:" << "\t" << Koord_finger11 <<
			"\t" << Koord_finger21 << "\t" << Koord_finger31 << endl;
		cout << "Координаты пальца 2:" << "\t" << Koord_finger1 <<
			"\t" << Koord_finger2 << "\t" << Koord_finger3 << endl;
	}

	DS4State operator = (DS4State& rl)
	{
		ps_button = rl.ps_button;
		counter = rl.counter;
		tpad_click = rl.tpad_click;
		Share = rl.Share;
		opt = rl.opt;
		cross = rl.cross;
		triangle = rl.triangle;
		square = rl.square;
		circle = rl.circle;
		left_stick_click = rl.left_stick_click;
		right_stick_click = rl.right_stick_click;
		dpad = rl.dpad;
		krestovina = rl.krestovina;
		L2 = rl.L2;
		R2 = rl.R2;
		battery_Level = rl.battery_Level;
		left_stick_axis_x = rl.left_stick_axis_x;
		left_stick_axis_y = rl.left_stick_axis_y;
		right_stick_axis_x = rl.right_stick_axis_x;
		right_stick_axis_y = rl.right_stick_axis_y;
		Hammers = rl.Hammers;
		GyroX = rl.GyroX;
		GyroY = rl.GyroY;
		GyroZ = rl.GyroZ;
		AccelX = rl.AccelX;
		AccelY = rl.AccelY;
		AccelZ = rl.AccelZ;
		Typeface = rl.Typeface;
		headphones = rl.headphones;
		bluetooth = rl.bluetooth;
		T_PAD = rl.T_PAD;
		t_pad1 = rl.t_pad1;
		T_PAD2 = rl.T_PAD2;
		Koord_finger1 = rl.Koord_finger1;
		Koord_finger2 = rl.Koord_finger2;
		Koord_finger3 = rl.Koord_finger3;
		Koord_finger11 = rl.Koord_finger11;
		Koord_finger21 = rl.Koord_finger21;
		Koord_finger31 = rl.Koord_finger31;
		return *this;
	}

	bool operator != (DS4State& rl)
	{
		if (ps_button != rl.ps_button)
		{
			return true;
		}

		//!!!!!!!!!!!!!!!!!!!
		/*if (counter != rl.counter)
		{
			return true;
		}*/


		if (tpad_click != rl.tpad_click)
		{
			return true;
		}

		if (Share != rl.Share)
		{
			return true;
		}
		if (opt != rl.opt)
		{
			return true;
		}

		if (krestovina != rl.krestovina)
		{
			return true;
		}
		if (cross != rl.cross)
		{
			return true;
		}
		if (triangle != rl.triangle)
		{
			return true;
		}
		if (square != rl.square)
		{
			return true;
		}
		if (circle != rl.circle)
		{
			return true;
		}
		if (left_stick_click != rl.left_stick_click)
		{
			return true;
		}
		if (right_stick_click != rl.right_stick_click)
		{
			return true;
		}
		if (L2 != rl.L2)
		{
			return true;
		}
		if (R2 != rl.R2)
		{
			return true;
		}
		if (L1 != rl.L1)
		{
			return true;
		}
		if (R1 != rl.R1)
		{
			return true;
		}
		if (dpad != rl.dpad)
		{
			return true;
		}
		//if (battery_Level != rl.battery_Level)
		//{
		//	return true;
		//}
		if (abs(left_stick_axis_x - rl.left_stick_axis_x) > 5)
		{
			return true;
		}
		if (abs(left_stick_axis_y - rl.left_stick_axis_y) > 5)
		{
			return true;
		}
		if (abs(right_stick_axis_x - rl.right_stick_axis_x) > 5)
		{
			return true;
		}
		if (abs(right_stick_axis_y - rl.right_stick_axis_y) > 5)
		{
			return true;
		}
		if (Hammers != rl.Hammers)
		{
			return true;
		}
		if (abs(GyroX - rl.GyroX) > 3000)
		{
			return true;
		}
		if (abs(GyroY - rl.GyroY) > 3000)
		{
			return true;
		}
		if (abs(GyroZ - rl.GyroZ) > 3000)
		{
			return true;
		}
		if (abs(AccelX - rl.AccelX) > 3000)
		{
			return true;
		}
		if (abs(AccelY - rl.AccelY) > 3000)
		{
			return true;
		}
		if (abs(AccelZ - rl.AccelZ) > 3000)
		{
			return true;
		}
		if (Typeface != rl.Typeface)
		{
			return true;
		}
		if (headphones != rl.headphones)
		{
			return true;
		}
		if (bluetooth != rl.bluetooth)
		{
			return true;
		}
		if (T_PAD != rl.T_PAD)
		{
			return true;
		}
		if (t_pad1 != rl.t_pad1)
		{
			return true;
		}
		if (T_PAD2 != rl.T_PAD2)
		{
			return true;
		}
		if (Koord_finger1 != rl.Koord_finger1)
		{
			return true;
		}
		if (Koord_finger2 != rl.Koord_finger2)
		{
			return true;
		}
		if (Koord_finger3 != rl.Koord_finger3)
		{
			return true;
		}
		if (Koord_finger11 != rl.Koord_finger11)
		{
			return true;
		}
		if (Koord_finger21 != rl.Koord_finger21)
		{
			return true;
		}
		if (Koord_finger31 != rl.Koord_finger31)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};