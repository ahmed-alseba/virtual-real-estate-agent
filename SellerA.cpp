/*
** SellerA.cpp -- Seller A
*/

#include <cstdio> //printf
#include <cstdlib> //atoi, rand, srand, malloc, free, realloc
#include <ctime> //time
#include <iostream> //cout, cin
#include <string> //string type
#include <stdint.h> //uint8_t, int32_t, etc.
#include <netdb.h> //AI_PASSIVE, getaddrinfo
#include <unistd.h> //close, exit, fork
#include <sys/types.h> //size_t
#include <sys/socket.h> //socklen_t, sa_family_t, sockaddr, SOCK_DGRAM, SOCK_STREAM, AF_INET, etc.
#include <netinet/in.h> //sockaddr_in, sa_family_t, IPPROTO_IP, IPPROTO_TCP, IPPROTO_TCP
#include <arpa/inet.h> //htonl, htons, etc.

#include <fstream>
#include <sstream> 
//#include <vector>

using namespace std;

#define PORT "4444" // the port client will be connecting to  
#define PORT4 "4044" // the port client will be connecting to  
#define BACKLOG 7

int main()
{
	ifstream sellerFile;
	sellerFile.open("sellerA.txt");
	int listingPrice = 0;
	int squareFootage = 0;

	string temp;

	getline(sellerFile, temp);
	stringstream manip1(temp);
	getline(manip1, temp, ':');
	getline(manip1, temp, ':');
	temp.erase(0, 1);
	listingPrice = atoi(temp.c_str());

	getline(sellerFile, temp);
	stringstream manip2(temp);
	getline(manip2, temp, ':');
	getline(manip2, temp, ':');
	temp.erase(0, 1);
	squareFootage = atoi(temp.c_str());

	sellerFile.close();

	int sockfd;  
	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo("nunki.usc.edu\0", PORT, &hints, &servinfo) != 0) {
		cout << "Failed to get remote host address infromation...\n";
		return 1;
	}

	if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
		cout << "Failed to get scoket descriptor...\n";
		return 1;
	}

	if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		cout << "Failed to connect...\n";
		close(sockfd);
		return 1;
	}

	char sellerIP[INET_ADDRSTRLEN];
	int sellerPort;
	struct sockaddr_in sellerAddress;
	memset(&sellerAddress, 0, sizeof sellerAddress);
	socklen_t addresslength = sizeof sellerAddress;
	getsockname(sockfd, (struct sockaddr*)&sellerAddress, &addresslength);
	inet_ntop(AF_INET, &sellerAddress.sin_addr, sellerIP, sizeof(sellerIP));
	sellerPort = sellerAddress.sin_port;

	cout << "SellerA has TCP port " << sellerPort << " and IP address " << sellerIP << " for Phase 1 part 1\n";
	cout << "SellerA is now connected to the Agent1\n";

	freeaddrinfo(servinfo); // all done with this structure

	if (send(sockfd, &listingPrice, sizeof listingPrice, 0) == -1)
	{
		cout << "Failed to send message...\n";
		return 1;
	}

	if (send(sockfd, &squareFootage, sizeof squareFootage, 0) == -1)
	{
		cout << "Failed to send message...\n";
		return 1;
	}

	cout << "SellerA has sent list price of " << listingPrice << " and square footage of " << squareFootage << " to the agent\n";

	close(sockfd);
	cout << "End of Phase 1 part 1 for SellerA\n";

	/////////////////////////////////////
	/////Part 4

	int new_fd;  //listen on sock_fd: parent socket desc., new connection on new_fd: child socket descriptor
	struct sockaddr their_addr; // connector's address information
	socklen_t sin_size;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if (getaddrinfo(NULL, PORT4, &hints, &servinfo) != 0) {
		cout << "Failed to get local host address infromation...\n";
		return 1;
	}

	if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
		cout << "Failed to get scoket descriptor...\n";
		return 1;
	}

	if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		cout << "Failed to bind...\n";
		return 1;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		cout << "Failed to listen...\n";
		return 1;
	}

	char agentName[128];
	gethostname(agentName, sizeof agentName);
	struct hostent *he;
	struct in_addr **addr_list;

	if ((he = gethostbyname(agentName)) == NULL) { // get the host info
		cout << "Error getting localhost name...\n";
		return 2;
	}

	addr_list = (struct in_addr **)he->h_addr_list;

	cout << "SellerA has TCP port " << PORT4 << " and IP address " << inet_ntoa(*addr_list[0]) << " for Phase 1 part 4\n";

	while (1)
	{  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, &their_addr, &sin_size);

		if (new_fd == -1) {
			continue;
		}


			char buff[100];
			
			while (1)
			{
				if (recv(new_fd, buff, 99, 0) == -1) {
					continue;
				}
				break;
			}

			if (buff[0] == 'N')
				cout << "NAK\n";
			else
				cout << "Buyer" << buff[6] << " buy my house\n";
			cout << "End of Phase 1 part 4 for SellerA\n";
			close(new_fd);

			exit(0);

		break;
	}

	return 0;
}
