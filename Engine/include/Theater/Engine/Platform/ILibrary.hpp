#pragma once

#include <string_view>
#include <functional>

#include "../EngineAPI.hpp"

namespace Theater
{
    class ILibrary
    {
    public:

        virtual ~ILibrary() {};

    public:

        virtual bool Load(std::string_view path) = 0;
        virtual bool IsLoaded() const = 0;
        virtual void Free() = 0;

        virtual void* GetLibNativePtr() const = 0;

    public:

        virtual void* LoadFunction(std::string_view fn) = 0;
    };

    ENGINE_API [[nodiscard]] ILibrary* NewLibrary();
    ENGINE_API void DeleteLibrary(ILibrary* ptr);

    template<typename Signature>
    inline std::function<Signature> LibFnCast(void* fn)
    {
        return reinterpret_cast<Signature*>(fn);
    }

}

#define THT_LoadLibFn(LibPtr, Signature, Fn) Theater::LibFnCast<Signature>(LibPtr->LoadFunction(Fn));