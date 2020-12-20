/*****************************************************************//**
 * \file   main.cpp
 * \brief
 *
 * \author clucle
 * \date   December 2020
 *********************************************************************/

#define BOOST_ALL_NO_LIB

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class UDPClient
{
public:
	UDPClient(
		boost::asio::io_service& io_service,
		const std::string& host,
		const std::string& port
	) : io_service_( io_service ), socket_( io_service, udp::endpoint( udp::v4(), 0 ) )
	{
		udp::resolver resolver( io_service_ );
		udp::resolver::query query( udp::v4(), host, port );
		udp::resolver::iterator iter = resolver.resolve( query );
		endpoint_ = *iter;
	}

	~UDPClient()
	{
		socket_.close();
	}

	void send( const std::string& msg )
	{
		socket_.send_to( boost::asio::buffer( msg, msg.size() ), endpoint_ );
	}

	void recv()
	{
		udp::endpoint sender_endpoint;
		size_t reply_length = socket_.receive_from(
			boost::asio::buffer( data_, max_length ), sender_endpoint );

		std::cout << "Reply is: ";
		std::cout.write( data_, reply_length );
		std::cout << "\n";
	}

private:
	boost::asio::io_service& io_service_;
	udp::socket socket_;
	udp::endpoint endpoint_;

	enum { max_length = 1024 };
	char data_[ max_length ];
};

int main()
{
	boost::asio::io_service io_service;
	UDPClient client( io_service, "localhost", "12111" );

	std::cout << "Input matching Id : ";
	int id;
	std::cin >> id;
	client.send( std::to_string( id ) );

	client.recv();
	std::cin >> id;
}