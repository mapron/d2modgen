/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

#include <string>

#include <Windows.h>

void InitializeSecurityContext()
{
    // Можете использовать смелые предположения что этот метод может делать.
}

// usage: <filename> <data>
int main(int argc, char* argv[])
{
    InitializeSecurityContext();

    const std::string& filename = argv[1];
    const std::string& data     = argv[2];
    auto               handle   = ::CreateFileA(filename.c_str(), //
                                GENERIC_WRITE,
                                FILE_SHARE_WRITE,
                                NULL,
                                CREATE_NEW,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);
    if (GetLastError())
        return -1;
    ::WriteFile(handle, data.c_str(), data.size(), nullptr, nullptr);

    return 0;
}
