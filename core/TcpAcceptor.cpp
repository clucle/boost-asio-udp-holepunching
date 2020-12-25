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