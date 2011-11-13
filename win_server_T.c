#ifndef WIN32_LEAN_AND_MEAN /* Void BUG!! - Will not compile with winsock.h AND winsock2.h */
#define WIN32_LEAN_AND_MEAN
#endif

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

SOCKET      socket_descr;
SOCKADDR_IN client_addr;

int counter = 0;


DWORD WINAPI odbiorca( void* );

void odbieranie( void );
void nadawanie( void );

void gotoxy( int, int );

int main( int argc, char** argv )
{
	SOCKADDR_IN my_addr; 
	
	int i;
	int error;
	int result = -1; 
	char key;

	result = WSAStartup( WS_VERSION_REQD, &stWSAData );
	if (result != 0)
		return 1;

	socket_descr = socket( PF_INET, SOCK_DGRAM, 0 );
	if( socket_descr == INVALID_SOCKET )
		return 1;

	memset( &my_addr, 0, sizeof( my_addr ) );

	my_addr.sin_family      = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port        = htons( MY_UDP_PORT );

	result = bind( socket_descr,(LPSOCKADDR)&my_addr, sizeof( my_addr ) );

	if( result == SOCKET_ERROR )
	{
		error = WSAGetLastError();
		printf("bind, error=%d\n", error);
		getch();
    }
	
	gotoxy( 0, 11 );
	printf( "---------------------------\n" );
	gotoxy( 0, 0 );
	
	for( ; ; )
	{
		Sleep( 550 );
		odbieranie();
		nadawanie();
		
		if( counter==10 )
		{
			system("pause");
			system("cls");
			gotoxy( 0, 11 );
			printf( "---------------------------\n" );
			gotoxy( 0, 0 );
			counter = 0;
		}
		else
			counter++;
	}
	
	result = WSACleanup();
	
	CloseSocket( socket_descr );
	return 0;
}

DWORD WINAPI odbiorca( void* v )
{
	odbieranie();
}

void odbieranie( void )
{
	unsigned char in_buffer[BUFFER_SIZE] = {0};
	
	int client_addr_size = sizeof( client_addr );
	
	int result;
	int error;
	
	result = recvfrom( socket_descr, in_buffer, 4, 0, (SOCKADDR *) & client_addr, &client_addr_size );
	

	if( result == SOCKET_ERROR )
	{
		error = WSAGetLastError();
		printf( "recvfrom, error=%d\n", error );
	}
	else
	{	
		gotoxy( 0, 0+counter );
		printf( "Odebrano: %c%c%c%c\n", in_buffer[0], in_buffer[1], in_buffer[2], in_buffer[3] );
	}
}

void nadawanie( void )
{
	unsigned char out_buffer[BUFFER_SIZE];
	
	int result;
	int error;
	
	out_buffer[0] = 'W';
	out_buffer[1] = 'X';
	out_buffer[2] = 'Y';
	out_buffer[3] = 'Z';
 
	result = sendto( socket_descr, out_buffer, 4, 0, (LPSOCKADDR)&client_addr, sizeof( client_addr ) );

	if( result == SOCKET_ERROR )
	{
		error = WSAGetLastError();
		printf( "sendto, error=%d\n", error );
	}
	else
	{
		gotoxy( 0, 13+counter );
		printf("Wyslano: %c%c%c%c.\n", out_buffer[0], out_buffer[1], out_buffer[2], out_buffer[3] );
	}
}

void gotoxy( int x, int y )
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), coord );
}