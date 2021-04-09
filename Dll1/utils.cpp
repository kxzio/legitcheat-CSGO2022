#include "utils.h"
#include "datamap.h"

#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))

c_utils* utils = new c_utils();

unsigned int c_utils::find_in_data_map(datamap_t* map, const char* name)
{
    while (map)
    {
        for (auto i = 0; i < map->dataNumFields; ++i)
        {
            if (!map->dataDesc[i].fieldName)
                continue;

            if (!strcmp(name, map->dataDesc[i].fieldName))
                return map->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];

            if (map->dataDesc[i].fieldType == FIELD_EMBEDDED)
            {
                if (map->dataDesc[i].td)
                {
                    unsigned int offset;

                    if (offset = find_in_data_map(map->dataDesc[i].td, name))
                        return offset;
                }
            }
        }

        map = map->baseMap;
    }

    return 0;
}

uintptr_t c_utils::find_pattern(const char* szModule, const char* szSignature)
{
    const char* pat = szSignature;
    DWORD firstMatch = 0;
    DWORD rangeStart = (DWORD)GetModuleHandleA(szModule);
    MODULEINFO miModInfo;
    GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
    DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
    for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
    {
        if (!*pat)
            return firstMatch;

        if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GET_BYTE(pat))
        {
            if (!firstMatch)
                firstMatch = pCur;

            if (!pat[2])
                return firstMatch;

            if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
                pat += 3;

            else
                pat += 2;
        }
        else
        {
            pat = szSignature;
            firstMatch = 0;
        }
    }
    return 0u;
}