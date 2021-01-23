/**
 * MinigameServer - a simple flip card game server
 * Copyright (C) 2020  clucle <wjdenwls123@gmail.com>
 * Github : http://github.com/clucle
 */

#ifndef NETWORK_MESSAGE_HPP
#define NETWORK_MESSAGE_HPP

#include <string>
#include <cstdio>

#include "core/types.h"

 /**
  * @file NetworkMessage.h
  * @class NetworkMessage
  * @author clucle (github.com/clucle)
  * @brief 패킷 자체애 대한 정보를 저장하는 클래스
  * 패킷을 쉽게 만들 수 있도록 함수를 제공하고
  * 만들어진 패킷을 읽을 수 있게 한다
  *
  */

class NetworkMessage
{
public:
	NetworkMessage();

	/* ---------  Packet Info --------- */
	// [HEADER_LENGTH][MAX_BODY_LENGTH] //
	enum { BODY_LENGTH = 4 };
	enum { HEADER_LENGTH = BODY_LENGTH };
	enum { MAX_BODY_LENGTH = 2048 };

	// 헤더배열 + 바디배열 리턴 (수정 불가)
	const char* GetData() const;
	// 헤더배열 + 바디배열 리턴 (수정 가능)
	char* GetData();

	// 헤더와 바디 합친 길이 리턴
	size_t GetLength() const;

	// 바디 배열 리턴 (수정 불가)
	const char* GetBody() const;
	// 바디 배열 리턴 (수정 가능)
	char* GetBody();
	// 바디 길이 리턴
	size_t GetBodyLength() const;

	// 패킷 -> 정보 : 패킷에서 정보를 얻어냄 (디코딩)
	bool DecodeHeader();
	// 정보 -> 패킷 : 정보를 패킷으로 만듬 (인코딩)
	void EncodeHeader();

	// 패킷 읽기 포지션 초기화
	void ReadyToRead();
	// 자료형 맞춰서 읽기
	template<typename T>
	T Read()
	{
		if ( !_CanRead( sizeof( T ) ) )
		{
			return T();
		}

		T v;
		memcpy( &v, m_data + HEADER_LENGTH + m_readPosition, sizeof( T ) );
		m_readPosition += sizeof( T );
		return v;
	}
	// 1 바이트만 읽기
	UInt8 ReadByte();
	// PREFIX_BYTE_FOR_STRING 를 읽은 후 그 길이만큼 실제 스트링 얻기
	std::string ReadString( uint16_t stringLen = 0 );

	/**
	 * @brief 패킷에 기본 자료형 변수를 추가시킴
	 * param T value : 패킷에 추가할 값
	 * @return bool : 추가에 성공 했는지 여부
	 */
	template<typename T>
	bool Append( T value )
	{
		if ( !_CanAdd( sizeof( T ) ) )
		{
			// 추가할 자료형의 길이가 남아있지 않다면 실패
			return false;
		}

		// 끝 자리에 T value를 추가 한후 Length를 T 길이 만큼 추가
		memcpy( m_data + HEADER_LENGTH + m_bodyLength, &value, sizeof( T ) );
		m_bodyLength += sizeof( T );
		return true;
	}

	// 1Byte 를 패킷에 추가
	bool AppendByte( UInt8 value );
	// string 을 패킷에 추가
	bool AppendString( const std::string& value );

private:
	// string 을 추가할때는 몇글자인지 앞에 적어두고 써야함
	// [n][n 글자 스트링]
	const int PREFIX_BYTE_FOR_STRING = sizeof( UInt16 );

	// 보낼 내용을 char 배열로 저장
	char m_data[ HEADER_LENGTH + MAX_BODY_LENGTH ];
	// 인코딩, 디코딩시 사용할 body의 길이
	std::size_t m_bodyLength;

	// 버퍼의 남은 자리에 패킷을 추가 할 수 있는지 확인
	bool _CanAdd( size_t size ) const;
	// 남은 길이만큼의 버퍼가 있는지 확인
	bool _CanRead( size_t size ) const;

	// 받아온 패킷에서 파싱하기 위해 필요한 포지션
	int m_readPosition;
};

#endif