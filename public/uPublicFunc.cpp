
#include "uPublicFunc.h"
#include "stdafx.h"
#include <Windows.h>
void OutputDebug(TCHAR * OutPutStr,...)
{
	TCHAR StrBuff[4096] = {0};
	va_list vlArgs;
	va_start(vlArgs,OutPutStr);
	_vsnprintf_s(StrBuff,sizeof(StrBuff)-1,4096,OutPutStr,vlArgs);
	va_end(vlArgs);
	OutputDebugString(StrBuff);
}