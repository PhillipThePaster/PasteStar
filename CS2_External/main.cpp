#define _CRT_SECURE_NO_WARNINGS
#include "Cheats.h"
#include "Offsets.h"

#include "Utils/Initial/Init.h"
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <stdio.h>

using namespace std;
/*
Contributors:
	Shinyaluvs,
	Nx0Ri,
	Skarbor,
	PedroGoncalves,
	KeysIsCool,
	Kenny,
	Cr1ppl3,
	Tairitsu,
	sh1pi,
	toepas,
	djsacred,
	tokinaa,
	faster_bbc,
	vsantos1,
	5mmod,
	gScream,
	Hazetick,
	styx,
	user1232,
	TaKaStuKi.sen
*/

namespace fs = filesystem;
bool otp = false;
string fileName;

void Exit()
{
	system("pause");
	exit(0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void Cheat()
{

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	//Gets a standard output device handle  
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);	//Set the text color to green  
	//ik this is cringy but i didnt want to remove it 
	cout << R"(                                                                                                                                                
*@@@***@@m                    @@             m@***@m@  @@                     
  @@   *@@m                   @@            m@@    *@  @@                     
  @@   m@@  m@*@@m   m@@*@@@@@@@@@   mm@*@@ *@@@m    @@@@@@  m@*@@m  *@@@m@@@ 
  @@@@@@@  @@   @@   @@   **  @@    m@*   @@  *@@@@@m  @@   @@   @@    @@* ** 
  @@        m@@@!@   *@@@@@m  @@    !@******      *@@  @@    m@@@!@    @!     
  @!       @!   !@        @@  @!    !@m    m@@     @@  @!   @!   !@    @!     
  @!        !!!!:!   *!   @!  !!    !!******!     *@!  !!    !!!!:!    !!      
  !!       !!   :!   !!   !!  !!    :!!     !!     !!  !!   !!   :!    !:     
:!:!:      :!: : !:  : :!:    ::: :  : : :: :!: : :!   ::: ::!: : !: : :::    
	)" << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	auto ProcessStatus = ProcessMgr.Attach("cs2.exe");

	char documentsPath[MAX_PATH];
	if (SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, documentsPath) != S_OK) {
		cerr << "[Info] Error: Failed to get the Documents folder path." << endl;
		Exit();
	}
	MenuConfig::path = documentsPath;
	MenuConfig::path += "\\PasteStar";

	switch (ProcessStatus) {
	case 1:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "[ERROR] Please launch the game first!" << endl;
		Exit();
	case 2:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "[ERROR] Failed to hook process, please run the cheat as Administrator (Right click PasteStar > Run as Adminstrator)." << endl;
		Exit();
	case 3:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "[ERROR] Failed to get module address." << endl;
		Exit();
	default:
		break;
	}

	if (!Offset::UpdateOffsets())
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "[ERROR] Failed to update offsets." << endl;
		Exit();
	}

	if (!gGame.InitAddress())
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "[ERROR] Failed to call InitAddress()." << endl;
		Exit();
	}

	cout << "[Game] Process ID: " << ProcessMgr.ProcessID << endl;
	cout << "[Game] Client Address: " << gGame.GetClientDLLAddress() << endl;

	if (fs::exists(MenuConfig::path))
	{
		cout << "[Info] Config folder connected: " << MenuConfig::path << endl;
	}
	else
	{
		if (fs::create_directory(MenuConfig::path))
		{
			cout << "[Info] Config folder created: " << MenuConfig::path << endl;
		}
		else
		{
			cerr << "[Info] Error: Failed to create the config directory." << endl;
			Exit();
		}
	}

	cout << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "Cheat running successfully!" << endl;
	cout << "Press [INS] to show or hide Menu." << endl;
	cout << "Have fun..." << endl << endl;
	cout << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	try
	{
		Gui.AttachAnotherWindow("Counter-Strike 2", "SDL_app", Cheats::Run);
	}
	catch (OSImGui::OSException& e)
	{
		try
		{
			// Perfect World version
			Gui.AttachAnotherWindow("反恐精英：全球攻势", "SDL_app", Cheats::Run);
		}
		catch (OSImGui::OSException& e)
		{
			cout << e.what() << endl;
		}
	}
}

int main()
{
	const char* tempPath = std::getenv("TMP");
	if (tempPath != nullptr)
	{
		fileName = std::string(tempPath) + "\\PasteStar";
		otp = Init::Verify::isVerified(fileName);
	}

	if (otp)
	{
		Cheat();
	}
	else
	{
		// OTP Window
		WNDCLASS wc = { 0 };
		const wchar_t CLASS_NAME[] = L"OTPInputClass";

		wc.lpfnWndProc = WndProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = CLASS_NAME;

		RegisterClass(&wc);

		HWND hwnd = CreateWindowEx(
			0, CLASS_NAME, L"Verify", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 400, 200,
			NULL, NULL, GetModuleHandle(NULL), NULL
		);

		if (hwnd == NULL) {
			return 0;
		}

		ShowWindow(hwnd, SW_SHOW);

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	std::ofstream outfile(fileName);
	outfile.close();
	ShowWindow(hwnd, SW_HIDE);
	system("cls");
	Cheat();

	return 0;
}
