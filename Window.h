#pragma once
#include<Windows.h>


class Window
{
public:
	Window();

	bool init();
	bool broadcast();
	bool release();
	bool isRun();

	virtual void onCreate() ;
	virtual void onUpdate() ;
	virtual void onDestroy();

	~Window();


private:
	HWND m_hand;
	bool m_is_run;

};

