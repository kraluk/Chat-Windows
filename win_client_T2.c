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

SOCKET      socket_descr;
SOCKADDR_IN serv_addr;

CRITICAL_SECTION sekcja;


DWORD WINAPI odbiorca( void* );

void gotoxy( int, int );
void clearline( int );


typedef struct mmsg
{
	char buffer[BUFFER_SIZE];
	int len;
} MMSG;


int main( int argc, char** argv )
{
	SOCKADDR_IN my_addr;
	HANDLE hWatek;

	unsigned char in_buffer[BUFFER_SIZE];
	unsigned char out_buffer[BUFFER_SIZE];

	int i;
	int error; 
	int result = -1; 
	int level;
	char key;

	result = WSAStartup( WS_VERSION_REQD, &stWSAData );
	
	if( result != 0 ) 
		return 1;

	socket_descr = socket( PF_INET, SOCK_DGRAM, 0 );
   
	if( socket_descr == INVALID_SOCKET ) 
		return 1;

	memset(	&my_addr, 0, sizeof( my_addr ) );
	my_addr.sin_family      = AF_INET;
	my_addr.sin_addr.s_addr = inet_addr( MY_HOST_ADDR );
	my_addr.sin_port        = htons( MY_UDP_PORT );

	result = bind( socket_descr,(LPSOCKADDR) &my_addr, sizeof( my_addr ) );

	if( result == SOCKET_ERROR )
    {
		error = WSAGetLastError();
		printf( "bind - error=%d\n", error );
		getch();
		//return 1;
    }


	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr( SERVER_ADDR );
	serv_addr.sin_port        = htons( MY_UDP_PORT );
	
	InitializeCriticalSection( &sekcja );
	hWatek = CreateThread( NULL, 0, odbiorca, 0, 0, 0 );
	
	gotoxy( 0, 11 );
	printf( "---------------------------\n" );
	gotoxy( 0, 13 );
	
	int k1 = 0;
	int k2 = 0;
	volatile int counter2 = 0;
	
	char c = ' ';
	char a;
	
	MMSG nad;
	
	
//-----NADAWANIE-START---------------------------------

	while(1)
	{
		Sleep( 100 );
		nad.len = 0;

		while( !TryEnterCriticalSection( &sekcja ) )
		{
			continue;
			Sleep( 1 );
		}
		
		gotoxy( 0, 13+counter2 );
		
		printf( "Ja: " );
		
		while( (a=getch()) != 13 )
		{
			nad.buffer[nad.len++] = a;
			printf( "%c", a );
        }
		
		//LeaveCriticalSection( &sekcja );
		
		k1 = 0;

		result = sendto( socket_descr, (void*) & nad, sizeof( nad ), 0, (LPSOCKADDR)&serv_addr, sizeof( serv_addr ) );

		/*if( result == SOCKET_ERROR )
		{
			error = WSAGetLastError();
			//exit(0);
		}
		else
		{
			printf( "Wysylka\n" );
		}*/
		
		if( counter2==10 )
		{
			for( k1=13; k1<=23; k1++ )
			{
				clearline( k1 );
			}
			
			counter2 = 0;
		}
		else
			InterlockedIncrement( &counter2 );
			
		LeaveCriticalSection( &sekcja );
	}
	
//-----NADAWANIE-STOP----------------------------------

	result = WSACleanup();

	return 0;
}

DWORD WINAPI odbiorca( void* v )
{	
	unsigned char in_buffer[BUFFER_SIZE] = {0};
	
	int serv_addr_size = sizeof( serv_addr );
	
	int result;
	int error;
	
	int k1;
	int k2;
	volatile int counter1 = 0;
	
	char c = ' ';
	
	MMSG odb;


//-----ODBIERANIE-START---------------------------------
	
	while( 1 )
	{
		Sleep( 100 );
		
		while( (result = recvfrom( socket_descr, (void*) & odb, sizeof( odb ), 0, (SOCKADDR *) & serv_addr, &serv_addr_size )) > 0 )
		{
			if( result == SOCKET_ERROR )
			{
				error = WSAGetLastError();
				//printf( "recvfrom, error=%d\n", error );
			}
			else
			{	
				while( !TryEnterCriticalSection( &sekcja ) )
				{
					continue;
					Sleep( 1 );
				}
			
				gotoxy( 0, 0+counter1 );
				//printf( "Ktos: %c%c%c%c\n", in_buffer[0], in_buffer[1], in_buffer[2], in_buffer[3] );
			
				printf( "Ktos: " );
				for( k1 = 0; k1<odb.len; k1++ )
					printf( "%c", odb.buffer[k1]);
			
				//LeaveCriticalSection( &sekcja );
			}
	
			if( counter1==10 )
			{
				for( k1=0; k1<=10; k1++ )
					clearline( k1 );
					
				counter1 = 0;
			}
			else
				InterlockedIncrement( &counter1 );
				
			LeaveCriticalSection( &sekcja );
		}
	}
	
//-----NADAWANIE-STOP----------------------------------
	
	return 1;
}

//-----Implementacja funkcji gotoxy(x, y)
void gotoxy( int x, int y )
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), coord );
}

//-----Implementacja funkcji czyszczącej linię
void clearline( int y )
{
	int k1;
	char c = ' ';
	
	gotoxy( 0, y );
	
	for( k1=0; k1<71; k1++ )
		putchar( c );
}

