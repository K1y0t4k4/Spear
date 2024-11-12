#pragma once

#ifndef _OBFUSCATION_H_
#define _OBFUSCATION_H_

#include <cwchar>
namespace spear
{
    using KeyType = unsigned long long;
    using Size    = unsigned long long;
    constexpr KeyType GenerateKey(KeyType seed)
    {
        KeyType key = seed;
        key ^= (key >> 33);
        key *= 0x114514114514a0c0ull; // This string of numbers has no real meaning.
        key ^= (key >> 33);
        key *= 0x15975316fc6513caull; // Same as above.
        key ^= (key >> 33);
        key |= 0x1010101010101010ull;

        return key;
    }

    template<typename T>
    constexpr void Cipher(T* string, Size size, KeyType key)
    {
        for (Size i=0; i<size; ++i)
        {
            string[i] ^= T(key >> ((i % 8) * 8));
        }
    }

    template<typename T, Size N, KeyType Key>
    class Encrypt
    {
    private:
        T mBuffer[N];
    public:
        constexpr Encrypt(const T* string) : mBuffer()
        {
            for (Size i=0; i<N; ++i)
                mBuffer[i] = string[i];
            Cipher<T>(mBuffer, N, Key);
        }

        constexpr const T* GetBuffer() const
        {
            return mBuffer;
        }
    };

    template<typename T, Size N, KeyType Key>
    class Decrypt
    {
    private:
        T mBuffer[N];
        bool mEncrypted;
    public:
        Decrypt(const Encrypt<T, N, Key>& encrypt) : mBuffer(), mEncrypted(true)
        {
            for (Size i=0; i<N; ++i)
                mBuffer[i] = encrypt.GetBuffer()[i];
        }

        ~Decrypt()
        {
            for (Size i=0; i<N; ++i)
                mBuffer[i] = 0;
        }

        operator T* ()
        {
            if (mEncrypted)
                Cipher<T>(mBuffer, N, Key);
            return mBuffer;
        }
    };
}

#define KEY spear::GenerateKey(__LINE__)
#define OBF(string) \
    []() -> spear::Decrypt<char, sizeof(string)/sizeof(string[0]), KEY>& \
    { \
        constexpr spear::Size n = sizeof(string)/sizeof(string[0]); \
        constexpr spear::Encrypt encrypt = spear::Encrypt<char, n, KEY>(string); \
        static spear::Decrypt decrypt = spear::Decrypt<char, n, KEY>(encrypt); \
        return decrypt; \
    }()

#define WOBF(string) \
    []() -> spear::Decrypt<wchar_t, sizeof(string)/sizeof(string[0]), KEY>& \
    { \
        constexpr spear::Size n = sizeof(string)/sizeof(string[0]); \
        constexpr spear::Encrypt encrypt = spear::Encrypt<wchar_t, n, KEY>(string); \
        static spear::Decrypt decrypt = spear::Decrypt<wchar_t, n, KEY>(encrypt); \
        return decrypt; \
    }()

#endif