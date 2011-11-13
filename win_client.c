#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <time.h>

#include <conio.h>
#include <dos.h>
#include <windows.h>
#include <winsock2.h>


#define MY_UDP_PORT  6099
#define MY_HOST_ADDR "10.10.10.12"	//10.10.10.12
#define SERVER_ADDR  "10.10.10.15"  //10.10.10.15

#define WS_VERSION_REQD 0x0101
#define BUFFER_SIZE 1024


WSADATA stWSAData;


int main( int argc, char** argv )
{
	SOCKET      socket_descr;
	SOCKADDR_IN my_addr;
	SOCKADDR_IN serv_addr;

	unsigned char in_buffer[BUFFER_SIZE];
	unsigned char out_buffer[BUFFER_SIZE];

	int i;
	int error; 
	int result = -1; 
	int level;
	int serv_addr_size; // = sizeof( serv_addr );
	char key;

	result = WSAStartup( WS_VERSION_REQD, &stWSAData );
	
	if( result != 0 ) 
		exit(0);

	socket_descr = socket( PF_INET, SOCK_DGRAM, 0 );
   
	if( socket_descr == INVALID_SOCKET ) 
		exit(0);

	memset(	&my_addr, 0, sizeof( my_addr ) );
	my_addr.sin_family      = AF_INET;
	my_addr.sin_addr.s_addr = inet_addr( MY_HOST_ADDR );
	my_addr.sin_port        = htons( MY_UDP_PORT );

	result = bind( socket_descr,(LPSOCKADDR) &my_addr, sizeof( my_addr ) );

	if( result == SOCKET_ERROR )
    {
		error = WSAGetLastError();
		printf( "bind - error=%d\n", error );
		//exit(0);
    }


	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr( SERVER_ADDR );
	serv_addr.sin_port        = htons( MY_UDP_PORT );
	
	
	while(1)
	{
		Sleep(500);

		out_buffer[0] = 'A';
		out_buffer[1] = 'B';
		out_buffer[2] = 'C';
		out_buffer[3] = 'D';

		result = sendto( socket_descr, out_buffer, 4, 0, (LPSOCKADDR)&serv_addr, sizeof( serv_addr ) );

		if( result == SOCKET_ERROR )
		{
			error = WSAGetLastError();
			//exit(0);
		}
		else
		{
			printf( "Wysylka\n" );
		}


		result = recvfrom( socket_descr, in_buffer, 4, 0, (LPSOCKADDR)&serv_addr, &serv_addr_size );

		if( result == SOCKET_ERROR )
		{
			error = WSAGetLastError();
			printf("recvfrom, error=%d\n", error);
			//exit(0);
		}
		else
		{
			printf( "Odebrano od serwera: %4s\n", in_buffer );
		}


	}

	result = WSACleanup();

	return 0;
}



