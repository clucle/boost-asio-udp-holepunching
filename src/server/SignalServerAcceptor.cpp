/*****************************************************************//**
 * \file   SignalServerAcceptor.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include "SignalServerAcceptor.h"
#include "User.h"
#include "network/TcpConnector.h"


SignalServerAcceptor::SignalServerAcceptor(
	boost::asio::io_context& ioContext,
	const tcp::endpoint& endpoint ) :
	TcpAcceptor( ioContext, endpoint )
{
    std::cout << "Server Open " << endpoint.address().to_string() << ':' << endpoint.port() << '\n';
    Accept();
}

void SignalServerAcceptor::Accept()
{
    m_acceptor.async_accept(
        [this]( boost::system::error_code ec, tcp::socket socket )
        {
            if ( !ec )
            {
                std::cout << "accept user" << '\n';
                std::make_shared< User >( std::move( socket ) );
            }
            Accept();
        } );
}