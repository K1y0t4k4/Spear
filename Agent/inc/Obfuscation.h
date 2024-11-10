#pragma once

#ifndef _OBFUSCATION_H_
#define _OBFUSCATION_H_

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

    constexpr void Cipher(char* string, Size size, KeyType key)
    {
        for (Size i=0; i<size; ++i)
        {
            string[i] ^= char(key >> ((i % 8) * 8));
        }
    }

    template<Size N, KeyType Key>
    class Encrypt
    {
    private:
        char mBuffer[N];
    public:
        constexpr Encrypt(const char* string) : mBuffer()
        {
            for (Size i=0; i<N; ++i)
                mBuffer[i] = string[i];
            Cipher(mBuffer, N, Key);
        }

        constexpr const char* GetBuffer() const
        {
            return mBuffer;
        }
    };

    template<Size N, KeyType Key>
    class Decrypt
    {
    private:
        char mBuffer[N];
        bool mEncrypted;
    public:
        Decrypt(const Encrypt<N, Key>& encrypt) : mBuffer(), mEncrypted(true)
        {
            for (Size i=0; i<N; ++i)
                mBuffer[i] = encrypt.GetBuffer()[i];
        }

        ~Decrypt()
        {
            for (Size i=0; i<N; ++i)
                mBuffer[i] = 0;
        }

        operator char* ()
        {
            if (mEncrypted)
                Cipher(mBuffer, N, Key);
            return mBuffer;
        }
    };
}

#define KEY spear::GenerateKey(__LINE__)
#define OBF(string) \
    []() -> spear::Decrypt<sizeof(string)/sizeof(string[0]), KEY>& \
    { \
        constexpr spear::Size n = sizeof(string)/sizeof(string[0]); \
        constexpr spear::Encrypt encrypt = spear::Encrypt<n, KEY>(string); \
        static spear::Decrypt decrypt = spear::Decrypt<n, KEY>(encrypt); \
        return decrypt; \
    }()

#endif