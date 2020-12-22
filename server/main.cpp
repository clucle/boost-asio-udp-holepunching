/*****************************************************************//**
 * \file   main.cpp
 * \brief  
 * 
 * \author wjden
 * \date   December 2020
 *********************************************************************/

#define BOOST_ALL_NO_LIB

#include <cstdlib>
#include <iostream>
#include <map>
#include <boost/bind.hpp>
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

class server
{
public:
	server( boost::asio::io_service& io_service, short port )
		: io_service_( io_service ),
		socket_( io_service, udp::endpoint( udp::v4(), port ) )
	{
		std::cout << "HI" << '\n';
		socket_.async_receive_from(
			boost::asio::buffer( data_, max_length ), sender_endpoint_,
			boost::bind( &server::handle_receive_from, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred ) );
	}

	void handle_receive_from( const boost::system::error_code& error,
		size_t bytes_recvd )
	{
		if ( !error && bytes_recvd > 0 )
		{
			int id = stoi( std::string( data_, data_ + bytes_recvd ) );

			if ( send_endpoint_map_.find( id ) == send_endpoint_map_.end() )
			{
				send_endpoint_map_[ id ] = sender_endpoint_;
			}
			else
			{
				std::string user1_ip = sender_endpoint_.address().to_string();
				unsigned short user1_port = sender_endpoint_.port();
				std::string user1_addr = user1_ip + ":" + std::to_string( user1_port );

				std::string user2_ip = send_endpoint_map_[ id ].address().to_string();
				unsigned short user2_port = send_endpoint_map_[ id ].port();
				std::string user2_addr = user1_ip + ":" + std::to_string( user2_port );

				socket_.async_send_to(
					boost::asio::buffer( user2_addr, user2_addr.size() ), sender_endpoint_,
					boost::bind( &server::handle_send_to, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred ) );

				socket_.async_send_to(
					boost::asio::buffer( user1_addr, user1_addr.size() ), send_endpoint_map_[ id ],
					boost::bind( &server::handle_send_to, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred ) );

				send_endpoint_map_.erase( id );
			}
		}
		std::cout << "Hi\n";

		/*
		boost::asio::io_service io_service2;
		std::string user1_ip = sender_endpoint_.address().to_string();
		std::string user1_port = std::to_string( sender_endpoint_.port() );
		std::cout << user1_ip << ' ' << user1_port << '\n';
		UDPClient client( io_service2, user1_ip, user1_port );
		client.send( "hi" );
		*/
		std::string user2_addr = "Hi2";
		socket_.async_send_to(
			boost::asio::buffer( user2_addr, user2_addr.size() ), sender_endpoint_,
			boost::bind( &server::handle_send_to, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred ) );
		/*
		socket_.async_receive_from(
			boost::asio::buffer( data_, max_length ), sender_endpoint_,
			boost::bind( &server::handle_receive_from, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred ) );
		*/
	}

	void handle_send_to( const boost::system::error_code& error, size_t bytes_sent )
	{
		if ( error )
		{
			throw error;
		}
		std::cout << "Send Bytes :" << bytes_sent << '\n';
	}

private:
	boost::asio::io_service& io_service_;
	udp::socket socket_;
	udp::endpoint sender_endpoint_;
	std::map< int, udp::endpoint > send_endpoint_map_;
	enum { max_length = 1024 };
	char data_[ max_length ];
};

int main( int argc, char* argv[] )
{
	try
	{
		const short port = 12111;
		boost::asio::io_service io_service;
		server s( io_service, port );

		io_service.run();
	}
	catch ( std::exception& e )
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}