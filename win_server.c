#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mem.h>

#include <conio.h>
#include <dos.h>
#include <windows.h>
#include <winsock2.h>
 

#define MY_UDP_PORT  6099
#define MY_HOST_ADDR "10.10.10.15"

#define BUFFER_SIZE  	1024
#define WS_VERSION_REQD 0x0101


WSADATA stWSAData;


int main( int argc, char** argv )
{
	SOCKET      socket_descr;
	SOCKADDR_IN my_addr; 
	SOCKADDR_IN client_addr;

	unsigned char in_buffer[BUFFER_SIZE];
	unsigned char out_buffer[BUFFER_SIZE];

	int i;
	int error;
	int result = -1; 
	int client_addr_size;
	char key;

	result = WSAStartup( WS_VERSION_REQD, &stWSAData );
	if (result != 0) { exit(0);}

	socket_descr = socket( PF_INET, SOCK_DGRAM, 0 );
	if( socket_descr == INVALID_SOCKET ) { exit(0);}

	memset( &my_addr, 0, sizeof( my_addr ) );

	my_addr.sin_family      = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port        = htons( MY_UDP_PORT );

	result = bind( socket_descr,(LPSOCKADDR)&my_addr, sizeof( my_addr ) );

	if( result == SOCKET_ERROR )
	{
		error = WSAGetLastError();
		printf("bind, error=%d\n", error);
//     	exit(0);
    }

	for(;;)
    {
		Sleep( 550 );

		result = recvfrom( socket_descr, in_buffer, 4, 0,
                       (LPSOCKADDR)&client_addr, &client_addr_size );

		if( result == SOCKET_ERROR )
		{
			error = WSAGetLastError();
			printf( "recvfrom, error=%d\n", error );
		}
		else
		{
			printf( "Odebrano: %c%c%c%c\n", in_buffer[0], in_buffer[1], in_buffer[2], in_buffer[3] );
			//printf( "%4s\n", in_buffer );
		}

		out_buffer[0] = 'W';
		out_buffer[1] = 'X';
		out_buffer[2] = 'Y';
		out_buffer[3] = 'Z';
 
		result = sendto( socket_descr, out_buffer, 4, 0,
                     (LPSOCKADDR)&client_addr, sizeof( client_addr ) );

		if( result == SOCKET_ERROR )
		{
			error = WSAGetLastError();
			printf( "sendto, error=%d\n", error );
		}
		else
		{
			printf("Wyslano.\n");
		}
      
	}

	result = WSACleanup();

	return 0;
}



