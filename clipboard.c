#include "includes.h"

void CopyToClipboard(DBRow* document, int line)
{
    #if (defined (_WIN32) || defined (_WIN64))
        HGLOBAL memblock = GlobalAlloc(GMEM_MOVEABLE, strlen(document[line].password)+1);   //+1 because GlobalAlloc() needs to allocate a space for the null terminator
        memcpy(GlobalLock(memblock), document[line].password, strlen(document[line].password));
        GlobalUnlock(memblock);

        OpenClipboard(0);
        EmptyClipboard();
        SetClipboardData(CF_TEXT, memblock);
        CloseClipboard();

        printf("Password copied to clipboard !\n");
    #elif (defined (LINUX) || defined (__linux__))
        printf("Not implemented yet\n");
    #elif (defined (__APPLE__))
        printf("Will likely never be implemented\n");   //Seriously, blame clang
    #endif
}
