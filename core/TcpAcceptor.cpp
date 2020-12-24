/*****************************************************************//**
 * \file   TcpAcceptor.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include "TcpAcceptor.h"
#include "TcpConnector.h"

TcpAcceptor::TcpAcceptor(
    boost::asio::io_context& ioContext,
    const tcp::endpoint& endpoint ) :
    m_acceptor( ioContext, endpoint )
{
}

void TcpAcceptor::Accept()
{
    m_acceptor.async_accept(
        [this]( boost::system::error_code ec, tcp::socket socket )
        {
            if ( !ec )
            {
                std::make_shared< TcpConnector >( std::move( socket ) )->OnAccept();
            }
            Accept();
        } );
}