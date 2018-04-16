#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define ATT_CID 4 																		// ATT_CID = 4, For l2cap socket to use BLE

/**
  This method first opens a socket, and then binds it to the first 
  available physical bluetooth adapter on the chip. After, it listens 
  for a connection, and creates a connection when a client connects. 
  It can then read and write data in the connection with the client. 
  **/
  int ble_client; 
  
void ble_server()
{
    struct sockaddr_l2 loc_addr = { 0 }; 												// Local bluetooth address
    struct sockaddr_l2 rem_addr = { 0 };												// Remote bluetooth address
    char buf[1024] = { 0 };																// Buffer for reading data
    int connection_socket; 																
    int bytes_read;
    int device_id;
    int dd;
    socklen_t opt = sizeof(rem_addr);
    
    device_id = hci_get_route(NULL);
    dd = hci_open_dev(device_id);

    connection_socket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);            // Allocate socket
    
    

    loc_addr.l2_family = AF_BLUETOOTH; 									
    loc_addr.l2_bdaddr = *BDADDR_ANY;													// Bind socket to the first available bluetooth adapter
    loc_addr.l2_cid = htobs(ATT_CID);                                   				// ATT_CID = 4, For l2cap to use BLE
	loc_addr.l2_bdaddr_type = BDADDR_LE_PUBLIC;											// Tell the adapter to use low energy		

    bind(connection_socket, (struct sockaddr *)&loc_addr, sizeof(loc_addr));            // Bind socket

    listen(connection_socket, 1);														// Put socket into listening mode
	
	while(1) {
		
		
		int testings = hci_le_set_advertise_enable(dd, 1, 10000);
		printf("%d\n" , testings);
		ble_client = accept(connection_socket, (struct sockaddr *)&rem_addr, &opt);			// Accept one connection

		ba2str( &rem_addr.l2_bdaddr, buf );												// Print bluetooth address of the ble_client 
		fprintf(stderr, "accepted connection from %s\n", buf);

		memset(buf, 0, sizeof(buf));
		while(1){
		bytes_read = read(ble_client, buf, sizeof(buf));									// Read data from the ble_client
		if( bytes_read > 0 ) {
			printf("received [%s]\n", buf);
			write(ble_client, "toggle\n", 7);														// Message over ble to other pi
		}
	}
		
		close(ble_client);																	// Close connection
	}
    close(connection_socket);
}
