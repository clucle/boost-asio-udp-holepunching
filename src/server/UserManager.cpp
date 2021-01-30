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
	m_userMap[ userId ] = user;

	return user;
}
