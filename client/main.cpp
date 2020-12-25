/*****************************************************************//**
 * \file   main.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <boost/thread.hpp>

#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>


#include <cstdio>
#include <cstring>

class chat_message
{
public:
    enum { header_length = 4 };
    enum { max_body_length = 512 };

    chat_message()
        : body_length_( 0 )
    {
    }

    const char* data() const
    {
        return data_;
    }

    char* data()
    {
        return data_;
    }

    size_t length() const
    {
        return header_length + body_length_;
    }

    const char* body() const
    {
        return data_ + header_length;
    }

    char* body()
    {
        return data_ + header_length;
    }

    size_t body_length() const
    {
        return body_length_;
    }

    void body_length( size_t new_length )
    {
        body_length_ = new_length;
        if ( body_length_ > max_body_length )
            body_length_ = max_body_length;
    }

    bool decode_header()
    {
        using namespace std; // For strncat and atoi.
        char header[ header_length + 1 ] = "";
        strncat( header, data_, header_length );
        body_length_ = atoi( header );
        if ( body_length_ > max_body_length )
        {
            body_length_ = 0;
            return false;
        }
        return true;
    }

    void encode_header()
    {
        using namespace std; // For sprintf and memcpy.
        char header[ header_length + 1 ] = "";
        sprintf( header, "%4d", static_cast<int>( body_length_ ) );
        memcpy( data_, header, header_length );
    }

private:
    char data_[ header_length + max_body_length ];
    size_t body_length_;
};

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
    chat_client( boost::asio::io_service& io_service,
        tcp::resolver::iterator endpoint_iterator )
        : io_service_( io_service ),
        socket_( io_service )
    {
        boost::asio::async_connect( socket_, endpoint_iterator,
            boost::bind( &chat_client::handle_connect, this,
                boost::asio::placeholders::error ) );
    }

    void write( const chat_message& msg )
    {
        io_service_.post( boost::bind( &chat_client::do_write, this, msg ) );
    }

    void close()
    {
        io_service_.post( boost::bind( &chat_client::do_close, this ) );
    }

private:

    void handle_connect( const boost::system::error_code& error )
    {
        if ( !error )
        {
            boost::asio::async_read( socket_,
                boost::asio::buffer( read_msg_.data(), chat_message::header_length ),
                boost::bind( &chat_client::handle_read_header, this,
                    boost::asio::placeholders::error ) );
        }
    }

    void handle_read_header( const boost::system::error_code& error )
    {
        if ( !error && read_msg_.decode_header() )
        {
            boost::asio::async_read( socket_,
                boost::asio::buffer( read_msg_.body(), read_msg_.body_length() ),
                boost::bind( &chat_client::handle_read_body, this,
                    boost::asio::placeholders::error ) );
        }
        else
        {
            do_close();
        }
    }

    void handle_read_body( const boost::system::error_code& error )
    {
        if ( !error )
        {
            std::cout.write( read_msg_.body(), read_msg_.body_length() );
            std::cout << "\n";
            boost::asio::async_read( socket_,
                boost::asio::buffer( read_msg_.data(), chat_message::header_length ),
                boost::bind( &chat_client::handle_read_header, this,
                    boost::asio::placeholders::error ) );
        }
        else
        {
            do_close();
        }
    }

    void do_write( chat_message msg )
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back( msg );
        if ( !write_in_progress )
        {
            boost::asio::async_write( socket_,
                boost::asio::buffer( write_msgs_.front().data(),
                    write_msgs_.front().length() ),
                boost::bind( &chat_client::handle_write, this,
                    boost::asio::placeholders::error ) );
        }
    }

    void handle_write( const boost::system::error_code& error )
    {
        if ( !error )
        {
            write_msgs_.pop_front();
            if ( !write_msgs_.empty() )
            {
                boost::asio::async_write( socket_,
                    boost::asio::buffer( write_msgs_.front().data(),
                        write_msgs_.front().length() ),
                    boost::bind( &chat_client::handle_write, this,
                        boost::asio::placeholders::error ) );
            }
        }
        else
        {
            do_close();
        }
    }

    void do_close()
    {
        socket_.close();
    }

private:
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

int main( int argc, char* argv[] )
{
    try
    {

        boost::asio::io_service io_service;

        tcp::resolver resolver( io_service );
        tcp::resolver::query query( "192.168.0.29","12111" );
        tcp::resolver::iterator iterator = resolver.resolve( query );

        chat_client c( io_service, iterator );

        boost::thread t( boost::bind( &boost::asio::io_service::run, &io_service ) );

        char line[ chat_message::max_body_length + 1 ];
        while ( std::cin.getline( line, chat_message::max_body_length + 1 ) )
        {
            using namespace std; // For strlen and memcpy.
            chat_message msg;
            msg.body_length( strlen( line ) );
            memcpy( msg.body(), line, msg.body_length() );
            msg.encode_header();
            c.write( msg );
        }

        c.close();
        t.join();
    }
    catch ( std::exception& e )
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}