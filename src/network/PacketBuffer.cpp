/*****************************************************************//**
 * \file   PacketBuffer.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#define _CRT_SECURE_NO_WARNINGS

#include "PacketBuffer.h"


PacketBuffer::PacketBuffer() :
	m_bodyLength( 0 ),
    m_data()
{
}

const char* PacketBuffer::GetData() const
{
    return m_data;
}

char* PacketBuffer::GetData()
{
    return m_data;
}

size_t PacketBuffer::GetLength() const
{
    return HEADER_LENGTH + m_bodyLength;
}

const char* PacketBuffer::GetBody() const
{
    return m_data + HEADER_LENGTH;
}

char* PacketBuffer::GetBody() 
{
    return m_data + HEADER_LENGTH;
}

size_t PacketBuffer::GetBodyLength() const
{
    return m_bodyLength;
}

void PacketBuffer::SetBodyLength( size_t length )
{
    m_bodyLength = length;
    if ( m_bodyLength > MAX_BODY_LENGTH )
        m_bodyLength = MAX_BODY_LENGTH;
}

bool PacketBuffer::DecodeHeader()
{
    using namespace std; // For strncat and atoi.
    char header[ HEADER_LENGTH + 1 ] = "";
    strncat( header, m_data, HEADER_LENGTH );
    m_bodyLength = atoi( header );
    if ( m_bodyLength > MAX_BODY_LENGTH )
    {
        m_bodyLength = 0;
        return false;
    }
    return true;
}

void PacketBuffer::EncodeHeader()
{
    using namespace std; // For sprintf and memcpy.
    char header[ HEADER_LENGTH + 1 ] = "";
    sprintf( header, "%4d", static_cast<int>( m_bodyLength ) );
    memcpy( m_data, header, HEADER_LENGTH );
}
