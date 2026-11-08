/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#if defined(_WIN32)
    #include <windows.h>
#endif

#include "gpu_inside.h"

void keep_dxgi(void)
{
#if defined(_WIN32)
    LoadLibraryW(L"dxgi.dll");
#endif
}
