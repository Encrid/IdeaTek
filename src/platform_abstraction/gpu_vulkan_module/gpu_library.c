/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#if defined(_WIN32)
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

#include <string.h>

#include "gpu_inside.h"

static const char MARK = 0;

void keep_dxgi(void)
{
#if defined(_WIN32)
    LoadLibraryW(L"dxgi.dll");
#endif
}

#if defined(_WIN32)

size_t module_file(char *out, size_t most)
{
    HMODULE module = nullptr;
    wchar_t wide[GPU_PATH];
    DWORD len;
    int bytes;

    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
        | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&MARK,
        &module);
    len = GetModuleFileNameW(module, wide, GPU_PATH);
    bytes = WideCharToMultiByte(CP_UTF8, 0, wide, (int)len, out,
        (int)most - 1, nullptr, nullptr);
    out[bytes] = '\0';
    return ((size_t)bytes);
}

PFN_vkGetInstanceProcAddr open_loader(const char *path)
{
    wchar_t wide[GPU_PATH];
    HMODULE library;

    MultiByteToWideChar(CP_UTF8, 0, path, -1, wide, GPU_PATH);
    library = LoadLibraryW(wide);
    if (!library)
        return (nullptr);
    return ((PFN_vkGetInstanceProcAddr)(void (*)(void))GetProcAddress(
        library, "vkGetInstanceProcAddr"));
}

#else

size_t module_file(char *out, size_t most)
{
    Dl_info info = {};
    const char *names[2] = { "", nullptr };

    dladdr(&MARK, &info);
    names[1] = info.dli_fname;
    strncpy(out, names[info.dli_fname != nullptr], most - 1);
    out[most - 1] = '\0';
    return (strlen(out));
}

PFN_vkGetInstanceProcAddr open_loader(const char *path)
{
    void *library = dlopen(path, RTLD_NOW | RTLD_LOCAL);

    if (!library)
        return (nullptr);
    return ((PFN_vkGetInstanceProcAddr)dlsym(library,
        "vkGetInstanceProcAddr"));
}

#endif
