#pragma once
#ifndef _KEYWORD_H_
#define _KEYWORD_H_

#include <string>
#include "Obfuscation.h"

struct URI {
    std::wstring Send;
    std::wstring Recv;
};

struct METHOD
{
    const wchar_t* doGet;
    const wchar_t* doPost;
};

const METHOD Method = {(const wchar_t*)WOBF(L"GET"), (const wchar_t*)WOBF(L"POST")};
static const wchar_t* headerKeyString = (const wchar_t*)WOBF(L"Content-Type: application/json");

const URI uri{std::wstring(WOBF(L"/send")), std::wstring(WOBF(L"/recv"))};
static const char* closeKeyWord = OBF("close");
static const char* CmdKeyWord   = OBF("Cmd");
static const char* dataKeyWord  = OBF("Data");

#endif