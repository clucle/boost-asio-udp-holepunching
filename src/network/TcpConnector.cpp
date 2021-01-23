/*****************************************************************//**
 * \file   TcpConnector.cpp
 * \brief  
 * 
 * \author clucle
 * \date   January 2021
 *********************************************************************/


#include "NetworkMessage.h"
#include "TcpConnector.h"


TcpConnector::TcpConnector( tcp::socket socket ) :
	m_socket( std::move( socket ) )
{
}

void TcpConnector::OnAccept()
{
    _ReadHeader();
}

void TcpConnector::Write( NetworkMessage& networkMessage )
{
    networkMessage.EncodeHeader();

    auto self( shared_from_this() );
    boost::asio::async_write( m_socket,
        boost::asio::buffer( networkMessage.GetData(), networkMessage.GetLength() ),
        [this, self]( boost::system::error_code ec, std::size_t /*length*/ ) {
            if ( ec )
            {
                std::cerr << "write fail" << '\n';
            }
        } );
}

void TcpConnector::SetReadCallback( std::function< void( NetworkMessage& ) > readCallback )
{
    m_readCallback = readCallback;
}

void TcpConnector::_ReadHeader()
{
    auto self( shared_from_this() );
    boost::asio::async_read( m_socket,
        boost::asio::buffer( buffer.GetData(), NetworkMessage::HEADER_LENGTH ),
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
    m_readCallback( buffer );

    _ReadHeader();
}
