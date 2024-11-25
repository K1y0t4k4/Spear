#pragma once
#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <string>

// ? I don't know if using so many std::wstring is a good idea.
// * I am a beginner in C++.
struct _Instance
{
    struct
    {
        bool         proxy; // TODO: Proxy servers will be supported in the future :)
        bool         exit;
        bool         connected;
        std::wstring userAgent;
        
        struct
        {
            std::wstring server;
            unsigned int port;
        } Address;
    } Config;

    struct
    {
        std::wstring os;
        std::wstring hostName;
        std::wstring cwd;
        std::wstring uuid;
        unsigned int pid;
    } Info;
};

extern _Instance Instance;

#endif