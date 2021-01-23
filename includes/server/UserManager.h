/*****************************************************************//**
 * \file   UserManager.h
 * \brief  
 * 
 * \author wjden
 * \date   January 2021
 *********************************************************************/


#pragma once


#include "core/Singleton.h"
#include "core/types.h"
#include "User.h"


class UserManager
	: public Singleton< UserManager >
{
public:
	typedef boost::asio::ip::tcp tcp;

	typedef std::map< UInt32, UserPtr > UserMap;

public:
	UserPtr AddUser( tcp::socket socket );

private:
	UserMap m_userMap;

	AtomicUInt32 m_userId;
};
