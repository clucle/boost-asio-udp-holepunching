/*****************************************************************//**
 * \file   TcpClient.cpp
 * \brief
 *
 * \author clucle
 * \date   January 2021
 *********************************************************************/

#include "TcpClient.h"

TcpClient::TcpClient(
    boost::asio::io_context& ioContext,
    const tcp::resolver::results_type& endpoints )
    : m_ioContext( ioContext ),
    m_socket( ioContext )
{
    _Connect( endpoints );
}

void TcpClient::Write( const NetworkMessage& msg )
{
    boost::asio::post( m_ioContext,
        [this, msg]()
        {
            bool writeInProgress = !m_networkMessageDeque.empty();
            m_networkMessageDeque.push_back( msg );
            if ( !writeInProgress )
            {
                _Write();
            }
        } );
}

void TcpClient::Close()
{
    boost::asio::post( m_ioContext, [this]() { m_socket.close(); } );
}

void TcpClient::_Connect( const tcp::resolver::results_type& endpoints )
{
    boost::asio::async_connect( m_socket, endpoints,
        [this]( boost::system::error_code ec, tcp::endpoint )
        {
            if ( !ec )
            {
                _ReadHeader();
            }
        } );
}

void TcpClient::_ReadHeader()
{
    boost::asio::async_read( m_socket,
        boost::asio::buffer( m_networkMessage.GetData(), NetworkMessage::HEADER_LENGTH ),
        [this]( boost::system::error_code ec, std::size_t /*length*/ )
        {
            if ( !ec && m_networkMessage.DecodeHeader() )
            {
                _ReadBody();
            }
            else
            {
                m_socket.close();
            }
        } );
}

void TcpClient::_ReadBody()
{
    boost::asio::async_read( m_socket,
        boost::asio::buffer( m_networkMessage.GetBody(), m_networkMessage.GetBodyLength() ),
        [this]( boost::system::error_code ec, std::size_t /*length*/ )
        {
            if ( !ec )
            {
                m_readCallback( m_networkMessage );

                _ReadHeader();
            }
            else
            {
                m_socket.close();
            }
        } );
}

void TcpClient::_Write()
{
    boost::asio::async_write( m_socket,
        boost::asio::buffer( m_networkMessageDeque.front().GetData(),
            m_networkMessageDeque.front().GetLength() ),
        [this]( boost::system::error_code ec, std::size_t /*length*/ )
        {
            if ( !ec )
            {
                m_networkMessageDeque.pop_front();
                if ( !m_networkMessageDeque.empty() )
                {
                    _Write();
                }
            }
            else
            {
                m_socket.close();
            }
        } );
}