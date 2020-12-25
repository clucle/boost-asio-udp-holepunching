/*****************************************************************//**
 * \file   TcpConnector.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include "TcpConnector.h"


TcpConnector::TcpConnector( tcp::socket socket ) :
	m_socket( std::move( socket ) )
{
}

void TcpConnector::OnAccept()
{
    _ReadHeader();
}

void TcpConnector::SetReadCallback( std::function< void( void ) > readCallback )
{
    m_readCallback = readCallback;
}

void TcpConnector::_ReadHeader()
{
    auto self( shared_from_this() );
    boost::asio::async_read( m_socket,
        boost::asio::buffer( buffer.GetData(), PacketBuffer::HEADER_LENGTH ),
        [this, self]( boost::system::error_code ec, std::size_t /*length*/ )
        {
            if ( !ec && buffer.DecodeHeader() )
            {
                _ReadBody();
            }
            else
            {
                // close user
            }
        } );
}

void TcpConnector::_ReadBody()
{
    auto self( shared_from_this() );
    boost::asio::async_read( m_socket,
        boost::asio::buffer( buffer.GetBody(), buffer.GetBodyLength() ),
        [this, self]( boost::system::error_code ec, std::size_t /*length*/ )
        {
            if ( !ec )
            {
                _OnRead();
            }
            else
            {
                // close user
            }
        } );
}

void TcpConnector::_OnRead()
{
    std::cout << buffer.GetBody() << '\n';

    m_readCallback();

    _ReadHeader();
}
