#include "StdAfx.h"

#include <stdio.h>
#include <time.h>
#include <winsock.h>
#include <imagehlp.h>

FILE * fException;

/*
static char __msg[4000], __cmsg[4000];
static int __idx;
CLZObject __l;
*/
/*
typedef BOOL
(CALLBACK *PENUMLOADED_MODULES_CALLBACK)(
    __in PCSTR ModuleName,
    __in ULONG ModuleBase,
    __in ULONG ModuleSize,
    __in_opt PVOID UserContext
    );
*/
#if _MSC_VER >= 1400
BOOL CALLBACK EnumerateLoadedModulesProc(PCSTR ModuleName, ULONG ModuleBase, ULONG ModuleSize, PVOID UserContext)
#else
BOOL CALLBACK EnumerateLoadedModulesProc(PSTR ModuleName, ULONG ModuleBase, ULONG ModuleSize, PVOID UserContext)
#endif
{
	DWORD offset = *((DWORD*)UserContext);
	
	if (offset >= ModuleBase && offset <= ModuleBase + ModuleSize)
	{
		fprintf(fException, "%s", ModuleName);
		//__idx += sprintf(__msg+__idx, "%s", ModuleName);
		return FALSE;
	}
	else
		return TRUE;
}

LONG __stdcall EterExceptionFilter(_EXCEPTION_POINTERS* pExceptionInfo)
{
	HANDLE		hProcess	= GetCurrentProcess();
	HANDLE		hThread		= GetCurrentThread();
	
	fException = fopen("ErrorLog.txt", "wt");
	if (fException)
	{
		char module_name[256];
		time_t module_time;
		
		HMODULE hModule = GetModuleHandle(NULL);
		
		GetModuleFileName(hModule, module_name, sizeof(module_name));
		module_time = (time_t)GetTimestampForLoadedLibrary(hModule);
		
		fprintf(fException, "Имя процесса: %s\n", module_name);
		fprintf(fException, "Time Stamp: 0x%08x - %s\n", module_time, ctime(&module_time));
		fprintf(fException, "\n");
		fprintf(fException, "Тип исключения: 0x%08x\n", pExceptionInfo->ExceptionRecord->ExceptionCode);
		fprintf(fException, "\n");

		/*
		{
			__idx+=sprintf(__msg+__idx,"Module Name: %s\n", module_name);
			__idx+=sprintf(__msg+__idx, "Time Stamp: 0x%08x - %s\n", module_time, ctime(&module_time));
			__idx+=sprintf(__msg+__idx, "\n");
			__idx+=sprintf(__msg+__idx, "Exception Type: 0x%08x\n", pExceptionInfo->ExceptionRecord->ExceptionCode);
			__idx+=sprintf(__msg+__idx, "\n");
		}
		*/
		
		CONTEXT&	context		= *pExceptionInfo->ContextRecord;
		
		fprintf(fException, "eax: 0x%08x\tebx: 0x%08x\n", context.Eax, context.Ebx);
		fprintf(fException, "ecx: 0x%08x\tedx: 0x%08x\n", context.Ecx, context.Edx);
		fprintf(fException, "esi: 0x%08x\tedi: 0x%08x\n", context.Esi, context.Edi);
		fprintf(fException, "ebp: 0x%08x\tesp: 0x%08x\n", context.Ebp, context.Esp);
		fprintf(fException, "\n");
		/*
		{
			__idx+=sprintf(__msg+__idx, "eax: 0x%08x\tebx: 0x%08x\n", context.Eax, context.Ebx);
			__idx+=sprintf(__msg+__idx, "ecx: 0x%08x\tedx: 0x%08x\n", context.Ecx, context.Edx);
			__idx+=sprintf(__msg+__idx, "esi: 0x%08x\tedi: 0x%08x\n", context.Esi, context.Edi);
			__idx+=sprintf(__msg+__idx, "ebp: 0x%08x\tesp: 0x%08x\n", context.Ebp, context.Esp);
			__idx+=sprintf(__msg+__idx, "\n");
		}
		*/
		
		STACKFRAME stackFrame = {0,};
		stackFrame.AddrPC.Offset	= context.Eip;
		stackFrame.AddrPC.Mode		= AddrModeFlat;
		stackFrame.AddrStack.Offset	= context.Esp;
		stackFrame.AddrStack.Mode	= AddrModeFlat;
		stackFrame.AddrFrame.Offset	= context.Ebp;
		stackFrame.AddrFrame.Mode	= AddrModeFlat;
		
		for (int i=0; i < 512 && stackFrame.AddrPC.Offset; ++i)
		{
			if (StackWalk(IMAGE_FILE_MACHINE_I386, hProcess, hThread, &stackFrame, &context, NULL, NULL, NULL, NULL) != FALSE)
			{
				fprintf(fException, "0x%08x\t", stackFrame.AddrPC.Offset);
				//__idx+=sprintf(__msg+__idx, "0x%08x\t", stackFrame.AddrPC.Offset);
				EnumerateLoadedModules(hProcess, (PENUMLOADED_MODULES_CALLBACK) EnumerateLoadedModulesProc, &stackFrame.AddrPC.Offset);
				fprintf(fException, "\n");

				//__idx+=sprintf(__msg+__idx,  "\n");
			}
			else
			{
				break;
			}
		} 
		
		fprintf(fException, "\n");
		

		fflush(fException);

		fclose(fException);
		fException = NULL;
		
		

	}
	
	return EXCEPTION_EXECUTE_HANDLER;
}

void SetEterExceptionHandler()
{
	SetUnhandledExceptionFilter(EterExceptionFilter);
}