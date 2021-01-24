/*****************************************************************//**
 * \file   SignalServerProtocol.cpp
 * \brief  
 * 
 * \author wjden
 * \date   January 2021
 *********************************************************************/


#include "SignalServerProtocol.h"
#include "User.h"

SignalServerProtocol::SignalServerProtocol( UserPtr user )
{
	m_user = user;
}

void SignalServerProtocol::OnRecvMessage( NetworkMessage networkMessage )
{
	networkMessage.ReadyToRead();
	std::cout << networkMessage.ReadString() << '\n';

	UserPtr a;

	UserPtr user = m_user.lock();
	if ( user )
		user->Write( networkMessage );
}
