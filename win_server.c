
 #include <condefs.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <dos.h>
 #include <time.h>
 #include <mem.h>

 #include <winsock2.h>

 #define WS_VERSION_REQD 0x0101

 #define MY_UDP_PORT  6000
 #define MY_HOST_ADDR "127.0.0.1"

 #define BUFFER_SIZE  1024

 WSADATA stWSAData;

 int main(int argc, char **argv)
  {
   SOCKET      socket_descr;
   SOCKADDR_IN my_addr, client_addr;

   unsigned char in_buffer[BUFFER_SIZE],
                 out_buffer[BUFFER_SIZE];

   int i, error, result = -1, client_addr_size;
   char key;

   result = WSAStartup(WS_VERSION_REQD, &stWSAData);
   if (result != 0) { exit(0);}

   socket_descr = socket (PF_INET, SOCK_DGRAM, 0);
   if (socket_descr == INVALID_SOCKET) { exit(0);}

   memset(&my_addr, 0, sizeof(my_addr));

   my_addr.sin_family      = AF_INET;
   my_addr.sin_addr.s_addr = INADDR_ANY;
   my_addr.sin_port        = htons(MY_UDP_PORT);

   result = bind(socket_descr,(LPSOCKADDR)&my_addr, sizeof(my_addr));

   if (result == SOCKET_ERROR)
    {
     error = WSAGetLastError();
     printf("bind, error=%d\n", error);
//     exit(0);
    }

   for(;;)
    {
     _sleep(10);

     result = recvfrom (socket_descr, in_buffer, 4, 0,
                       (LPSOCKADDR)&client_addr, &client_addr_size);

     if (result == SOCKET_ERROR)
      {
       error = WSAGetLastError();
       printf("recvfrom, error=%d\n", error);
  //     exit(0);
      }
     else
      {
       printf("odebrano\n");
       printf("%4s", in_buffer);
      }

//     client_addr.sin_family      = AF_INET;
//     client_addr.sin_addr.s_addr = inet_addr(DESTINATION_MCAST);
//     client_addr.sin_port        = htons(MY_UDP_PORT);

     out_buffer[0] = 'W';
     out_buffer[1] = 'X';
     out_buffer[2] = 'Y';
     out_buffer[3] = 'Z';
 /*
     result = sendto (socket_descr, out_buffer, 4, 0,
                     (LPSOCKADDR)&client_addr, sizeof(client_addr));

     if (result == SOCKET_ERROR)
      {
       error = WSAGetLastError();
       printf("sendto, error=%d\n", error);
    //   exit(0);
      }
     else
      {
       printf("wyslal\n");
      }
      */
   }

   result = WSACleanup();

  return 0;
}



