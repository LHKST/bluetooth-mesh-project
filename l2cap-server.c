#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define ATT_CID 4 

int main(int argc, char **argv)
{
    struct sockaddr_l2 loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read, hci_socket;
    int hci_device_id = 0;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

    // bind socket to the first available 
    // bluetooth adapter
    loc_addr.l2_family = AF_BLUETOOTH; 
    loc_addr.l2_bdaddr = *BDADDR_ANY;
    loc_addr.l2_cid = htobs(ATT_CID);                                   //ATT_CID = 4, For l2cap to use BLE
	loc_addr.l2_bdaddr_type = BDADDR_LE_PUBLIC;

    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);
	
	while(1) {
		// accept one connection
		client = accept(s, (struct sockaddr *)&rem_addr, &opt);

		ba2str( &rem_addr.l2_bdaddr, buf );
		fprintf(stderr, "accepted connection from %s\n", buf);

		memset(buf, 0, sizeof(buf));

		// read data from the client
		bytes_read = read(client, buf, sizeof(buf));
		if( bytes_read > 0 ) {
			printf("received [%s]\n", buf);
			write(client, "hi", 2);
		}

		// close connection
		close(client);
	}
    close(s);
}
