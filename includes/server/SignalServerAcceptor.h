/*****************************************************************//**
 * \file   SignalServerAcceptor.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#include "core/TcpAcceptor.h"


class SignalServerAcceptor
	: public TcpAcceptor
{

public:
	SignalServerAcceptor(
		boost::asio::io_context& ioContext,
		const tcp::endpoint& endpoint );

protected:
	virtual void Accept() override;

};

typedef std::shared_ptr< SignalServerAcceptor > SignalServerAcceptorPtr;
