#pragma once

#include <ostream>
#include <string>
#include <string_view>

namespace Theater
{
    template<size_t Size>
    class String
    {
    public:

        char m_CharArray[Size];

    public:

        String()
        {
            std::memset(m_CharArray, 0, sizeof(char) * Size);
        }

        explicit String(std::string_view str)
        {
            Copy(str);
        }

        String(const String& other)
        {
            *this = other;
        }

        String& operator = (const String& other)
        {
            if (this != &other)
            {
                Copy(other.GetData());
            }
            return *this;
        }

        String& operator = (std::string_view str)
        {
            Copy(str);

            return *this;
        }

        constexpr bool operator == (std::string_view str)
        {
            return std::strncmp(m_CharArray, str.data(), str.size()) == 0;
        }

    public:

        void Copy(std::string_view str, size_t off = 0ull)
        {
            if (off > str.size() || off == 0ull) 
                off = str.size();  
            
            if (off > Size - 1) 
                off = Size - 1;  
            
            std::memset(m_CharArray, 0, sizeof(char) * Size);
            std::strncpy(m_CharArray, str.data(), off);
            
            m_CharArray[Size - 1] = '\0';
        }

        void Clear()
        {
            std::memset(m_CharArray, 0, sizeof(char) * Size);
        }

        constexpr operator std::string_view() const { return m_CharArray; }

        friend std::ostream& operator<<(std::ostream& os, const String& str)
        {
            for (size_t i = 0ull; i < Size && str.m_CharArray[i] != '\0'; i++)
                os << str.m_CharArray[i];

            return os;
        }

        constexpr bool IsEmpty() const { return m_CharArray[0] == '\0'; }

        const char* GetData() const { return m_CharArray; }
        std::string GetStdString() const { return std::string(m_CharArray); }

        constexpr size_t GetArraySize() const { return Size; }
        constexpr size_t GetUsableSize() const { return Size - 1; }
    };

    using Str512 = String<512>;
    using Str256 = String<256>;
    using Str128 = String<128>;
    using Str64 = String<64>;
    using Str32 = String<32>;
    using Str16 = String<16>;
}