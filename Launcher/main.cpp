#include "stdafx.h"
#include <iostream>
#include <string>

const char *secret_key[]={ // Секретные ключи. Используется для проверки. После них добавляется pid родителя процесса, строка шифруется в md5 и отправляется клиенту.
	"950fbf144b0fa016c28ccc969230f966", //patcher-tor запуск клиента onion
	"28e7492ffa7871667b162d0d9ad92d8a", //patcher-tor без запуска клиента
	"3b2810dd87e024f19db7a32330dd2ada", //patcher-client запуск клиента onion
	"e4c717b4f0e4d7b72d9d9b5af6cd7cd5", //patcher-client запуск клиента
	"f21fe851192d5567392c592bc5c1bb2d", //tor-client запуск клиента onion
	NULL
};
DWORD dw;
char first_str[4] = "/C "; // Стандартный аргумент при запуске.
char arg_name[11] = "--sig_key "; // аргумент --sig_key, после него идет цифровая подпись.
char str_arg[128]; // В этой строке будут аргументы, которые будут переданы ChildProcess.exe. str_arg+arg_name+md5(secret_key:my_pid)
char work_dir[256]; // Сюда будет записан путь до текущей(рабочей) директории
char work_dir_postfix[16] = "\\Data"; // Если клиент находится в поддиректории то указать ее тут, иначе оставить пустые кавычки
char child_process[256]; // Сюда будет записан путь до ChildProcess.exe, который должен находиться в текущей(рабочей) директории
char tor[256]; // Сюда будет записан путь до secure, который должен находиться в текущей(рабочей) директории
char my_pid[64]; //Тут будет pid этой программы
char str_to_md5[64]; // Строка которая будет зашифрована в md5. Будет содержать один из ключей(secret_key) и pid этой программы(my_pid).

HWND hMainDlg;
HINSTANCE hInst;
HICON HiconDLG;

LPSTR str_argb(int type_key){
	_snprintf(my_pid, sizeof my_pid, "%i", GetCurrentProcessId()); //из DWORD GetCurrentProcessId() в char my_pid. GetCurrentProcessId() - из <windows.h>
	strcat(strcpy(str_arg, first_str), arg_name); // В строку с аргументами str_arg записываем стандартный аргумент "/C " и первый аргумент "--sig_key " 
	strcat(strcpy(str_to_md5, secret_key[type_key]), ":"); // В строку которая будет обработана функцией md5() записываем один из secret_key, а после него двоеточие. sposob в #define."
	strcat(str_to_md5, my_pid); // Дописываем свой pid после двоеточия в строку которая будет обработана функцией md5()
	// strcat(str_arg, md5(string(str_to_md5)).c_str()); // Добавляем к строке с аргументами(str_arg) результат выполнения функции md5().
	strcat(str_arg, md5(str_to_md5).c_str()); // Добавляем к строке с аргументами(str_arg) результат выполнения функции md5().
	return const_cast<LPSTR>(str_arg);
}

bool tor_ckeck_run(){ // Проверка запущен ли tor
	tagPROCESSENTRY32 proc;
	HANDLE hProcessSnap;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL,0);
	proc.dwSize = sizeof( tagPROCESSENTRY32 );
	while (Process32Next(hProcessSnap, &proc)){
		if(!strcmp("secure", proc.szExeFile)){
			return true;
		}
	}
	return false;
}

bool case_mtd(int mtd){
	_getcwd(work_dir, 256); // Записываем текущую директорию в workdir
	strcat(work_dir, work_dir_postfix);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	if(mtd == 1){
		strcat(strcpy(child_process, work_dir), "\\FenixMt2.exe"); // Записываем в child_process текущую директорию и имя файла который будет запущен.
		if(CreateProcess(child_process,str_argb(3),NULL,NULL,FALSE,NULL,NULL,work_dir,&si,&pi)==TRUE){
			return true;
		}
	}
	else if(mtd == 2){
		if(tor_ckeck_run() == true){ // Если тор уже запущен
			strcat(strcpy(child_process, work_dir), "\\FenixMt2.exe"); // Записываем в child_process текущую директорию и имя файла который будет запущен.
			if(CreateProcess(child_process,str_argb(2),NULL,NULL,FALSE,NULL,NULL,work_dir,&si,&pi)==TRUE){
				return true;
			}
		}
		else{
			strcat(strcpy(tor, work_dir), "\\secure"); // Записываем в tor текущую директорию и имя файла который будет запущен.
			if(CreateProcess(tor,str_argb(0),NULL,NULL,FALSE,NULL,NULL,work_dir,&si,&pi)==TRUE){
				return true;
			}
		}
	}
	else if(mtd == 3){
		strcat(strcpy(tor, work_dir), "\\secure"); // Записываем в tor текущую директорию и имя файла который будет запущен.
		if(CreateProcess(tor,str_argb(1),NULL,NULL,FALSE,NULL,NULL,work_dir,&si,&pi)==TRUE){
			return true;
		}
	}
	return false;
}

BOOL CALLBACK MainDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM) {
	HDC hdc;
	PAINTSTRUCT ps;
	switch(uMsg) {
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_BUTTON:
					if (case_mtd(1) == true){
						DestroyWindow(hWnd);
						PostQuitMessage(0);
					}
					break;
				case IDC_BUTTON2:
					if (case_mtd(2) == true){
						DestroyWindow(hWnd);
						PostQuitMessage(0);
					}
					break;
				case IDC_BUTTON3:
					if (case_mtd(3) == true){
					}
					break;
			}	// switch(LOWORD(wParam))
			break;
		case WM_CLOSE:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
		case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
			DrawIconEx(hdc, 0, 0, HiconDLG, 128, 128, NULL, NULL, DI_NORMAL);
            EndPaint(hWnd, &ps);
            break;

	}
	
	return false;
}


std::string Utf8_to_cp1251(const char *str) // Преобразователь кодировки
{
	std::string res;
	int result_u, result_c;
	result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
	if (!result_u)
		return 0;
	wchar_t *ures = new wchar_t[result_u];

	if(!MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u))
	{
		delete[] ures;
		return 0;
	}

	result_c = WideCharToMultiByte(1251, 0, ures, -1, 0, 0, 0, 0);

	if(!result_c)
	{
		delete [] ures;
		return 0;
	}

	char *cres = new char[result_c];

	if(!WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, 0, 0))
	{
		delete[] cres;
		return 0;
	}
	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	hInst = hInstance;
	
	HiconDLG = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 128, 128, 0);

	hMainDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINDLG), (HWND)NULL, MainDlgProc);
	if(!hMainDlg) return false;
	
	HICON HiconDLGico = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetClassLong(hMainDlg, GCL_HICON, (LONG) HiconDLGico);

	// Переводит диалоговое окно на русский если язык операционной системы русский
	if (PRIMARYLANGID(GetUserDefaultLangID()) == LANG_RUSSIAN){
		SetWindowText(hMainDlg, Utf8_to_cp1251("Metin2 лаунчер").c_str());
		SetDlgItemText(hMainDlg, IDC_BUTTON, Utf8_to_cp1251("Запуск").c_str());
		SetDlgItemText(hMainDlg, IDC_BUTTON2, Utf8_to_cp1251("Запуск с AntiDDoS").c_str());
		SetDlgItemText(hMainDlg, IDC_BUTTON3, Utf8_to_cp1251("Запуск прокси сервера").c_str());
	}
	
	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		if (!IsWindow(hMainDlg) || !IsDialogMessage(hMainDlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}	// if (!IsWindow(hMainDlg) || !IsDialogMessage(hMainDlg, &msg))
	}	// while (GetMessage(&msg, NULL, NULL, NULL))
	
	return true;
	DestroyIcon(HiconDLGico);
	DestroyIcon(HiconDLG);
}

