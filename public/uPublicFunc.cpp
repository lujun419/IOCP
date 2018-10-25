
#include "uPublicFunc.h"
#include "stdafx.h"
#include <Windows.h>
void OutputDebug(char * OutPutStr,...)
{
	char StrBuff[4096] = {0};
	va_list vlArgs;
	va_start(vlArgs,OutPutStr);
	char a = va_arg(vlArgs,char);
	_vsnprintf_s(StrBuff,sizeof(StrBuff)-1,4096,OutPutStr,vlArgs);
	va_end(vlArgs);
	OutputDebugString(StrBuff);
}