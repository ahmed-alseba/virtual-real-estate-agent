/*
** Buyer2.cpp -- Buyer2
*/

#include <cstdio> //printf
#include <cstdlib> //atoi, rand, srand, malloc, free, realloc
#include <ctime> //time
#include <iostream> //cout, cin
#include <string> //string type
#include <fstream>
#include <sstream>
#include <vector>
#include <stdint.h> //uint8_t, int32_t, etc.
#include <netdb.h> //AI_PASSIVE, getaddrinfo
#include <unistd.h> //close, exit, fork
#include <sys/types.h> //size_t
#include <sys/socket.h> //socklen_t, sa_family_t, sockaddr, SOCK_DGRAM, SOCK_STREAM, AF_INET, etc.
#include <netinet/in.h> //sockaddr_in, sa_family_t, IPPROTO_IP, IPPROTO_TCP, IPPROTO_TCP
#include <arpa/inet.h> //htonl, htons, etc.

using namespace std;

#define PORT "22244"  // the port users will be connecting to
#define PORTAGENT1 "4444"  // the port users will be connecting to
#define PORTAGENT2 "4544"  // the port users will be connecting to
#define BACKLOG 7	 // how many pending connections queue will hold
#define PORT4 "4744" // the port client will be connecting to 

int main(void)
{
	int sellerPrices[4] = { 0,0,0,0 };
	int sellerFootages[4] = { 0,0,0,0 };

	int sockfd;  //listen on sock_fd: parent socket desc., new connection on new_fd: child socket descriptor
	struct addrinfo hints, *servinfo;
	struct sockaddr their_addr; // connector's address information
	socklen_t sin_size;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if (getaddrinfo(NULL, PORT, &hints, &servinfo) != 0) {
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

	char buyerName[128];
	gethostname(buyerName, sizeof buyerName);
	struct hostent *he;
	struct in_addr **addr_list;

	if ((he = gethostbyname(buyerName)) == NULL) { // get the host info
		cout << "Error getting localhost name...\n";
		return 2;
	}

	addr_list = (struct in_addr **)he->h_addr_list;

	cout << "The Buyer2 has UDP port " << PORT << " and IP address " << inet_ntoa(*addr_list[0]) << " for Phase 1 part 2\n";

	sin_size = sizeof their_addr;
	int packetsize = 100;
	char pack[packetsize];
	stringstream temp1;
	stringstream temp2;
	int count = 0;
	int numbytes = 0;
	while (1)
	{
		if ((numbytes = recvfrom(sockfd, pack, packetsize - 1, 0, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
			continue;
		}
		pack[numbytes] = '\0';
		if (pack[5] == '1')
		{
			temp1 << pack;
			cout << "Received house information from Agent1\n";
		}

		else if (pack[5] == '2')
		{
			temp2 << pack;
			cout << "Received house information from Agent2\n";
		}

		count++;
		if (count == 2)
			break;
	}

	close(sockfd);

	cout << "End of Phase 1 part 2 for Buyer2\n";

	//Part 3---------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------

	string ts;

	getline(temp1, ts);
	getline(temp1, ts);
	sellerPrices[0] = atoi(ts.c_str());
	getline(temp1, ts);
	sellerFootages[0] = atoi(ts.c_str());
	getline(temp1, ts);
	sellerPrices[1] = atoi(ts.c_str());
	getline(temp1, ts);
	sellerFootages[1] = atoi(ts.c_str());

	getline(temp2, ts);
	getline(temp2, ts);
	sellerPrices[2] = atoi(ts.c_str());
	getline(temp2, ts);
	sellerFootages[2] = atoi(ts.c_str());
	getline(temp2, ts);
	sellerPrices[3] = atoi(ts.c_str());
	getline(temp2, ts);
	sellerFootages[3] = atoi(ts.c_str());

	ifstream buyerFile;
	buyerFile.open("buyer2.txt");
	int mySquareFootage = 0;
	int myBudget = 0;

	string temp;

	getline(buyerFile, temp);
	stringstream manip1(temp);
	getline(manip1, temp, ':');
	getline(manip1, temp, ':');
	temp.erase(0, 1);
	mySquareFootage = atoi(temp.c_str());

	getline(buyerFile, temp);
	stringstream manip2(temp);
	getline(manip2, temp, ':');
	getline(manip2, temp, ':');
	temp.erase(0, 1);
	myBudget = atoi(temp.c_str());

	buyerFile.close();

	vector<int> tempVec;

	for (int i = 0; i <= 3; i++)
	{
		if ((sellerPrices[i] <= myBudget) && (sellerFootages[i] >= mySquareFootage))
			tempVec.push_back(sellerPrices[i]);
		else
			tempVec.push_back(0);
	}

	sort(tempVec.begin(), tempVec.end());
	vector<int> houseNums; houseNums.push_back(0); houseNums.push_back(0); houseNums.push_back(0); houseNums.push_back(0);
	for (int i = 0; i <= 3; i++)
	{
		for (int j = 0; j <= 3; j++)
		{
			if ((sellerPrices[j] == tempVec[i]) && (sellerFootages[j] >= mySquareFootage))
				houseNums[i] = j + 1;
		}
	}

	//////////////////////////////////////////////////////////////////////

	int sockfdP3;
	struct addrinfo hintsP3, *servinfoP3;

	memset(&hintsP3, 0, sizeof hintsP3);
	hintsP3.ai_family = AF_UNSPEC;
	hintsP3.ai_socktype = SOCK_STREAM;

	if (getaddrinfo("nunki.usc.edu\0", PORTAGENT1, &hintsP3, &servinfoP3) != 0) {
		cout << "Failed to get remote host address infromation...\n";
		return 1;
	}

	if ((sockfdP3 = socket(servinfoP3->ai_family, servinfoP3->ai_socktype, servinfoP3->ai_protocol)) == -1) {
		cout << "Failed to get scoket descriptor...\n";
		return 1;
	}

	if (connect(sockfdP3, servinfoP3->ai_addr, servinfoP3->ai_addrlen) == -1) {
		cout << "Failed to connect...\n";
		close(sockfdP3);
		return 1;
	}

	char buyerIP[INET_ADDRSTRLEN];
	struct sockaddr_in buyerAddress;
	memset(&buyerAddress, 0, sizeof buyerAddress);
	socklen_t addresslength = sizeof buyerAddress;
	getsockname(sockfdP3, (struct sockaddr*)&buyerAddress, &addresslength);
	inet_ntop(AF_INET, &buyerAddress.sin_addr, buyerIP, sizeof(buyerIP));
	int buyerPort = buyerAddress.sin_port;

	cout << "The Buyer2 has TCP port " << buyerPort << " and IP address " << buyerIP << " for Phase 1 part 3\n";

	freeaddrinfo(servinfoP3); // all done with this structure

	stringstream bta;
	bta << "Budget: " << myBudget << "\n";

	int nums = 0;
	for (int j = 0; j <= 3; j++)
	{
		if (houseNums[j] != 0)
		{
			nums++;
			if (houseNums[j] == 1)
				bta << "Option " << nums << ": " << "HouseA\n";
			else if (houseNums[j] == 2)
				bta << "Option " << nums << ": " << "HouseB\n";
			else if (houseNums[j] == 3)
				bta << "Option " << nums << ": " << "HouseC\n";
			else if (houseNums[j] == 4)
				bta << "Option " << nums << ": " << "HouseD\n";
		}

	}
	for (int i = 0; i < 1000000; i++);
	if (nums != 0)
	{
		if (send(sockfdP3, bta.str().c_str(), strlen(bta.str().c_str()), 0) == -1)
		{
			cout << "Failed to send message...\n";
			return 1;
		}
		cout << "Buyer2 has sent " << bta.str().c_str() << " to Agent1\n";
	}
	else
	{
		if (send(sockfdP3, " : NAK\0", strlen(" : NAK\0"), 0) == -1)
		{
			cout << "Failed to send message...\n";
			return 1;
		}
		cout << "Buyer2 has sent NAK to Agent1\n";
	}

	close(sockfdP3);

	/////////////////////////////////////////////////////////////////////

	if (getaddrinfo("nunki.usc.edu\0", PORTAGENT2, &hintsP3, &servinfoP3) != 0) {
		cout << "Failed to get remote host address infromation...\n";
		return 1;
	}

	if ((sockfdP3 = socket(servinfoP3->ai_family, servinfoP3->ai_socktype, servinfoP3->ai_protocol)) == -1) {
		cout << "Failed to get scoket descriptor...\n";
		return 1;
	}

	if (connect(sockfdP3, servinfoP3->ai_addr, servinfoP3->ai_addrlen) == -1) {
		cout << "Failed to connect...\n";
		close(sockfdP3);
		return 1;
	}

	memset(&buyerAddress, 0, sizeof buyerAddress);
	getsockname(sockfdP3, (struct sockaddr*)&buyerAddress, &addresslength);
	inet_ntop(AF_INET, &buyerAddress.sin_addr, buyerIP, sizeof(buyerIP));
	buyerPort = buyerAddress.sin_port;

	cout << "The Buyer2 has TCP port " << buyerPort << " and IP address " << buyerIP << " for Phase 1 part 3\n";

	freeaddrinfo(servinfoP3); // all done with this structure
	for (int i = 0; i < 1000000; i++);
	if (nums != 0)
	{
		if (send(sockfdP3, bta.str().c_str(), strlen(bta.str().c_str()), 0) == -1)
		{
			cout << "Failed to send message...\n";
			return 1;
		}
		cout << "Buyer2 has sent " << bta.str().c_str() << " to Agent2\n";
	}
	else
	{
		if (send(sockfdP3, "NAK\0", strlen("NAK\0"), 0) == -1)
		{
			cout << "Failed to send message...\n";
			return 1;
		}
		cout << "Buyer2 has sent NAK to Agent2\n";
	}

	close(sockfdP3);

	cout << "End of Phase 1 part 3 for Buyer2\n";

	/////////////////////////////////////
	/////Part 4

	int new_fd;  //listen on sock_fd: parent socket desc., new connection on new_fd: child socket descriptor


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

	if ((he = gethostbyname(agentName)) == NULL) { // get the host info
		cout << "Error getting localhost name...\n";
		return 2;
	}

	addr_list = (struct in_addr **)he->h_addr_list;

	cout << "Buyer2 has TCP port " << PORT4 << " and IP address " << inet_ntoa(*addr_list[0]) << " for Phase 1 part 4\n";
	if (nums == 0)
	{
		cout << "NAK\n";
		cout << "End of Phase 1 part 4 for Buyer2\n";
		exit(0);
	}

	while (1)
	{  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, &their_addr, &sin_size);

		if (new_fd == -1) {
			continue;
		}

			close(sockfd); // child doesn't need the listener
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
				cout << "Will buy house from Seller" << buff[6] << "\n";
			cout << "End of Phase 1 part 4 for Buyer2\n";
			close(new_fd);

			exit(0);


		break;
	}

	return 0;
}
