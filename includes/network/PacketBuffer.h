/*****************************************************************//**
 * \file   PacketBuffer.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>


class PacketBuffer
{
public:
    enum { HEADER_LENGTH = 4 };
    enum { MAX_BODY_LENGTH = 512 };
    
    PacketBuffer();

    const char* GetData() const;

    char* GetData();

    size_t GetLength() const;

    const char* GetBody() const;

    char* GetBody();

    size_t GetBodyLength() const;

    void SetBodyLength( size_t new_length );

    bool DecodeHeader();

    void EncodeHeader();

private:
    char m_data[ HEADER_LENGTH + MAX_BODY_LENGTH ];
    size_t m_bodyLength;
};
