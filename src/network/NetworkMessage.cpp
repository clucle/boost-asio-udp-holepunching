/**
 * MinigameServer - a simple flip card game server
 * Copyright (C) 2020  clucle <wjdenwls123@gmail.com>
 * Github : http://github.com/clucle
 */


#define _CRT_SECURE_NO_WARNINGS

#include "NetworkMessage.h"

/**
 * @brief Constructor
 * 패킷 정보를 초기화
 * @param tcp::socket socket
 * @return void
 */
NetworkMessage::NetworkMessage()
	: m_data(), m_bodyLength( 0 ), m_readPosition( 0 )
{
}

/**
 * @brief 패킷전체를 읽기 전용으로 리턴
 * @return const char* : 헤더를 가리키는 포인터
 */
const char* NetworkMessage::GetData() const
{
	return m_data;
}

/**
 * @brief 패킷전체을 리턴
 * @return char* : 헤더를 가리키는 포인터
 */
char* NetworkMessage::GetData()
{
	return m_data;
}

/**
 * @brief 패킷전체 길이
 * @return size_t : 헤더 길이 + 패킷 바디 길이
 */
size_t NetworkMessage::GetLength() const
{
	size_t length = HEADER_LENGTH + m_bodyLength;
	return length;
}

/**
 * @brief 헤더를 제외한 패킷 읽기 전용으로 리턴
 * 실제 데이터에 접근 할 때 사용한다
 * @return const char* : 바디를 가리키는 포인터
 */
const char* NetworkMessage::GetBody() const
{
	const char* body = m_data + HEADER_LENGTH;
	return body;
}

/**
 * @brief 헤더를 제외한 패킷 리턴
 * 실제 데이터에 접근 할 때 사용한다
 * @return char* : 바디를 가리키는 포인터
 */
char* NetworkMessage::GetBody()
{
	char* body = m_data + HEADER_LENGTH;
	return body;
}

/**
 * @brief 패킷 바디의 길이
 * @return size_t : 패킷 바디 길이
 */
size_t NetworkMessage::GetBodyLength() const
{
	return m_bodyLength;
}

/**
 * @brief 받아온 패킷에서 정보를 얻어냄
 * 현재는 패킷 길이를 받고, 패킷 길이가 valid 한지 체크
 * @return bool : 패킷 디코딩의 성공 여부
 */
bool NetworkMessage::DecodeHeader()
{
	// 패킷 길이를 받아올 char 배열
	char header[ HEADER_LENGTH + 1 ] = "";
	strncat( header, m_data, HEADER_LENGTH );
	// bodyLength 를 파싱
	m_bodyLength = atoi( header );

	bool success = true;
	if ( m_bodyLength > MAX_BODY_LENGTH )
	{
		// bodyLength 가 너무 길어서 invalid 하다면 추가 할 수 없음
		m_bodyLength = 0;
		success = false;
	}
	return success;
}

/**
 * @brief 정보를 갖고 패킷을 만들어냄
 * 현재는 패킷 길이를 헤더에 추가해줌
 * @return void
 */
void NetworkMessage::EncodeHeader()
{
	// 패킷 길이를 입력할 char 배열
	char header[ HEADER_LENGTH + 1 ] = "";
	sprintf( header, "%4d", static_cast<int>( m_bodyLength ) );
	// 패킷 헤더에 길이를 붙여넣음
	memcpy( m_data, header, HEADER_LENGTH );
}

/**
 * @brief 패킷 읽을 준비
 * @return void
 */
void NetworkMessage::ReadyToRead()
{
	m_readPosition = 0;
}

/**
 * @brief 한 바이트만 읽음
 * @return Byte : 얻어낸 바이트
 */
UInt8 NetworkMessage::ReadByte()
{
	if ( !_CanRead( 1 ) )
	{
		return 0;
	}
	return m_data[ HEADER_LENGTH + m_readPosition++ ];
}


/**
 * @brief 스트링 길이를 얻고, 스트링을 얻어냄
 * @param uint16_t stringLen : 글자수를 읽을 필요 없다면 미리 정해줌
 * @return std::string : 얻어낸 스트링
 */
std::string NetworkMessage::ReadString( uint16_t stringLen )
{
	if ( stringLen == 0 )
	{
		stringLen = Read<uint16_t>();
	}

	if ( !_CanRead( stringLen ) )
	{
		return std::string();
	}

	char* body = reinterpret_cast<char*>( m_data ) + HEADER_LENGTH + m_readPosition; //does not break strict aliasing
	m_readPosition += stringLen;
	return std::string( body, stringLen );
}

/**
 * @brief 패킷에 바이트 변수를 추가시킴
 * param uint8 value : 패킷에 추가할 바이트 값
 * @return bool : 추가에 성공 했는지 여부
 */
bool NetworkMessage::AppendByte( UInt8 value )
{
	if ( !_CanAdd( 1 ) )
	{
		// Byte 를 추가할 자리가 모자라면 실패
		return false;
	}
	// 끝 자리에 1 byte를 추가하고 length를 1 추가
	m_data[ HEADER_LENGTH + m_bodyLength++ ] = value;
	return true;
}

/**
 * @brief 패킷에 스트링 변수를 추가시킴
 * @param cosnt std::string& value : 패킷에 추가할 스트링 레퍼런스
 * @return bool : 추가에 성공 했는지 여부
 */
bool NetworkMessage::AppendString( const std::string& value )
{
	// string 의 길이
	size_t stringLength = value.length();
	// string은 몇글자인지 추가할 바이트수가 추가적으로 필요
	if ( !_CanAdd( stringLength + PREFIX_BYTE_FOR_STRING ) )
	{
		return false;
	}
	// 스트링 앞에 글자수를 추가해줌
	Append<UInt16>( static_cast<UInt16>( stringLength ) );
	// 끝자리에 스트링 추가하고 length를 스트링 길이만큼 추가
	memcpy( m_data + HEADER_LENGTH + m_bodyLength, value.c_str(), stringLength );
	m_bodyLength += stringLength;

	return true;
}

/**
 * @brief 패킷 버퍼에 내용을 추가 할 수 있는지 확인
 * @param size_t size : 추가할 길이
 * @return bool : 추가에 성공 했는지 여부
 */
bool NetworkMessage::_CanAdd( size_t size ) const
{
	// 내용을 추가해서 MAX_BODY_LENGTH 보다 작다면 추가 할 수 있음
	return ( m_bodyLength + size ) < MAX_BODY_LENGTH;
}

bool NetworkMessage::_CanRead( size_t size ) const
{
	if ( ( m_readPosition + size ) > m_bodyLength )
	{
		return false;
	}
	return true;
}
