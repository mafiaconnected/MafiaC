/*
    Ported from reMafia's C_chunk.h/.cpp (same author, MafiaOrbitCam/Vendors/reMafia).
    Used by C_Actor::SaveData/LoadData and several actor types' save-game code.
*/

#ifndef _CCHUNK_H_
#define _CCHUNK_H_

namespace MafiaSDK
{
    namespace C_Chunk_Enum
    {
        enum FunctionAddresses
        {
            Read = 0x00404D80
        };
    };

    class C_Chunk
    {
    public:
        template<typename T>
        bool Read(T* target)
        {
            return Read((void*)target, sizeof(T));
        }

        bool Read(void* buffer, size_t bufferSize)
        {
            unsigned long funcAddress = C_Chunk_Enum::FunctionAddresses::Read;
            bool result = false;

            __asm
            {
                push bufferSize
                push buffer
                mov ecx, this
                call funcAddress
                mov result, al
            }

            return result;
        }

    private:
        PADDING(C_Chunk, _pad0, 0x4);
        unsigned long fileBegin;
        unsigned long fileEnd;
        PADDING(C_Chunk, _pad1, 0x4);
        bool isOpen;
        PADDING(C_Chunk, _pad2, 0x3);
        unsigned long fileHandle;
        bool isWritable;
        PADDING(C_Chunk, _pad3, 0x3);
        unsigned long currentOffset;
    };
};

#endif
