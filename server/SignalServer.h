/*****************************************************************//**
 * \file   SignalServer.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#define BOOST_ALL_NO_LIB

#include "core/TcpConnector.h"


class SignalServer :
	public TcpConnector,
	public std::enable_shared_from_this< SignalServer >
{
public:
	SignalServer( tcp::socket socket );

	void test() { ; };

protected:
	virtual void OnRead() override;
};

typedef std::shared_ptr< SignalServer > SignalServerPtr;
