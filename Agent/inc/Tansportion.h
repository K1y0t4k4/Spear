#pragma once
#ifndef _TANSPORTION_H_
#define _TANSPORTION_H_

#include <nlohmann/json.hpp>

namespace spear
{
    nlohmann::json HttpGet(const std::wstring& uri);
    bool HttpPost(const std::wstring& uri, const nlohmann::json& data);
}

#endif