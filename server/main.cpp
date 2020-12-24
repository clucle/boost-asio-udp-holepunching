/*****************************************************************//**
 * \file   main.cpp
 * \brief  
 * 
 * \author wjden
 * \date   December 2020
 *********************************************************************/


#define BOOST_ALL_NO_LIB

#include <iostream>
#include "SignalServerAcceptor.h"


int main( int argc, char* argv[] )
{
	try
	{
		unsigned short port = 12111;
		using boost::asio::ip::tcp;
		boost::asio::io_context ioContext;
		tcp::endpoint endpoint( tcp::v4(), port );

		SignalServerAcceptor acceptor( ioContext, endpoint );

		ioContext.run();
	}
	catch ( std::exception& e )
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}