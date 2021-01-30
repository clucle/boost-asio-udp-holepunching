/*****************************************************************//**
 * \file   UserManager.cpp
 * \brief  
 * 
 * \author wjden
 * \date   January 2021
 *********************************************************************/


#include "UserManager.h"


static UserManager gs_userManager;


UserPtr UserManager::AddUser( tcp::socket socket )
{
	UserPtr user = std::make_shared< User >( std::move( socket ) );
	UInt32 userId = m_userId.fetch_add( 1, std::memory_order_relaxed );
	user->SetId( userId );
	m_userMutex.lock();
	m_userMap[ userId ] = user;
	m_userMutex.unlock();

	return user;
}

void UserManager::Register( UInt32 id, UInt32 ip, UInt16 port )
{
	std::vector< std::pair< UInt32, UInt16 > > others;
	m_userMutex.lock();
	
	UserPtr user = m_userMap[ id ];
	if ( user )
	{
		user->SetRegistered( true );

		NetworkMessage msg;
		msg.Append( ProtocolId::ProtocolUserAddressNotify );
		msg.Append( user->GetIP() );
		msg.Append( user->GetPort() );
		msg.EncodeHeader();

		for ( auto iter : m_userMap )
		{
			UInt32 userId = iter.first;

			if ( userId == id )
				continue;

			UserPtr other = iter.second;
			if ( other->GetRegistered() )
			{
				// other 에게 보내기
				other->Write( msg );
				others.push_back( { other->GetIP(), other->GetPort() } );
			}
		}

		for ( auto other : others )
		{
			NetworkMessage msg;
			msg.Append( ProtocolId::ProtocolUserAddressNotify );
			msg.Append( other.first );
			msg.Append( other.second );
			msg.EncodeHeader();
			user->Write( msg );
		}
	}

	m_userMutex.unlock();
}
