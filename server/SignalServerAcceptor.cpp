/*****************************************************************//**
 * \file   SignalServerAcceptor.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include "SignalServerAcceptor.h"
#include "SignalServer.h"
#include "core/TcpConnector.h"


SignalServerAcceptor::SignalServerAcceptor(
	boost::asio::io_context& ioContext,
	const tcp::endpoint& endpoint ) :
	TcpAcceptor( ioContext, endpoint )
{
    Accept();
}

void SignalServerAcceptor::Accept()
{
    m_acceptor.async_accept(
        [this]( boost::system::error_code ec, tcp::socket socket )
        {
            if ( !ec )
            {
                std::make_shared< SignalServer >( std::move( socket ) )->OnAccept();
            }
            Accept();
        } );
}