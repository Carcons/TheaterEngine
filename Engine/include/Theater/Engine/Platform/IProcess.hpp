#pragma once

#include <string_view>

#include "../EngineAPI.hpp"

namespace Theater
{
    class IProcess
    {
    public:
        virtual ~IProcess() {};

    public:

        virtual bool Start() = 0;
        virtual void SetCommand(std::string_view command) = 0;
        virtual std::string_view GetOutput() = 0;
        
    };

    ENGINE_API [[nodiscard]] IProcess* NewProcess(std::string_view command = {});
    ENGINE_API void DeleteProcess(IProcess* ptr);
}