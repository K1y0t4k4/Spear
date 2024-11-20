#pragma once
#ifndef _TANSPORTION_H_
#define _TANSPORTION_H_

#include <string>
#include <exception>
#include <nlohmann/json.hpp>
#include "API.h"

namespace spear
{
    class TransporterException : public std::exception
    {
    private:
        std::string message;
    public:
        TransporterException(const std::string& msg) : message(msg) {}
        const char* what() const noexcept override
        {
            return message.c_str();
        } 
    };

    nlohmann::json HttpGet(const std::wstring& uri);
    bool HttpPost(const std::wstring& uri, const nlohmann::json& data);

    class Transporter
    {
    private:
        HINTERNET hSession, hConnect, hRequest;

    public:
        static Transporter& GetInstance()
        {
            static Transporter transporter;
            return transporter;
        }
        nlohmann::json HttpGet(const std::wstring& uri);
        bool HttpPost(const std::wstring& uri, const nlohmann::json& data);

    private:
        Transporter(const Transporter&) = delete;
        Transporter();
        void CloseAll()
        {
            if (hRequest) Win32::WinHttpCloseHandle(hRequest); hRequest = NULL;
            if (hConnect) Win32::WinHttpCloseHandle(hConnect); hConnect = NULL;
            if (hSession) Win32::WinHttpCloseHandle(hSession); hSession = NULL;
        }
        ~Transporter()
        {
            CloseAll();
        }
    };
}

#endif