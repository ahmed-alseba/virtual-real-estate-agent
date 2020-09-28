/*
** Agent1.cpp -- Agent1
*/

#include <cstdio> //printf
#include <cstdlib> //atoi, rand, srand, malloc, free, realloc
#include <ctime> //time
#include <iostream> //cout, cin
#include <string> //string type
#include <vector>
#include <stdint.h> //uint8_t, int32_t, etc.
#include <netdb.h> //AI_PASSIVE, getaddrinfo
#include <unistd.h> //close, exit, fork
#include <sys/types.h> //size_t
#include <sys/socket.h> //socklen_t, sa_family_t, sockaddr, SOCK_DGRAM, SOCK_STREAM, AF_INET, etc.
#include <netinet/in.h> //sockaddr_in, sa_family_t, IPPROTO_IP, IPPROTO_TCP, IPPROTO_TCP
#include <arpa/inet.h> //htonl, htons, etc.
#include <fstream>
#include <sstream> 
#include <signal.h>
#include <sys/wait.h>

using namespace std;

#define PORT "4444"  // the port users will be connecting to
#define BACKLOG 7	 // how many pending connections queue will hold

void sigchld_handler(int s)
{
	// waitpid() might overwrite errno, so we save and restore it:
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void)
{
	struct sigaction sa;

	int sellerAPrice = 0;
	int sellerAFootage = 0;
	int sellerBPrice = 0;
	int sellerBFootage = 0;

	int sockfd, new_fd;  //listen on sock_fd: parent socket desc., new connection on new_fd: child socket descriptor
	struct addrinfo hints, *servinfo;
	struct sockaddr their_addr; // connector's address information
	socklen_t sin_size;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
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

	cout << "The Agent1 has TCP port " << PORT << " and IP address " << inet_ntoa(*addr_list[0]) << " for Phase 1 part 1\n";

	int sellerCount = 0;

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		exit(1);
	}

	while (1)
	{  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, &their_addr, &sin_size);

		if (new_fd == -1) {
			continue;
		}

		sellerCount++;

		if (!fork()) // this is the child process
		{
			close(sockfd); // child doesn't need the listener
			int tempPrice = 0;
			int tempFootage = 0;
			while (1)
			{
				if (recv(new_fd, &tempPrice, sizeof tempPrice, 0) == -1) {
					continue;
				}
				break;
			}

			while (1)
			{
				if (recv(new_fd, &tempFootage, sizeof tempFootage, 0) == -1) {
					continue;
				}
				break;
			}

			ofstream agentOutput;

			//Saving child process results into a file for access by parent
			if (sellerCount == 1)
				agentOutput.open("agentSellerAInfo.txt");
			else if (sellerCount == 2)
				agentOutput.open("agentSellerBInfo.txt");

			agentOutput << tempPrice << "\n" << tempFootage << "\n";

			agentOutput.close();

			if (sellerCount == 1)
				cout << "Received house information from SellerA\n";
			else if (sellerCount == 2)
				cout << "Received house information from SellerB\n";

			close(new_fd);
			exit(0);
		}

		if (sellerCount != 2)
		{
			continue;
		}


		close(new_fd);  // parent doesn't need this
		break;
	}

	for (int i = 0; i < 24000000; i++) { continue; }; //time delay to wait for the child processes to save to file

	ifstream sellerFile;
	sellerFile.open("agentSellerAInfo.txt");
	string temp;
	getline(sellerFile, temp);
	sellerAPrice = atoi(temp.c_str());
	getline(sellerFile, temp);
	sellerAFootage = atoi(temp.c_str());
	sellerFile.close();

	sellerFile.open("agentSellerBInfo.txt");
	getline(sellerFile, temp);
	sellerBPrice = atoi(temp.c_str());
	getline(sellerFile, temp);
	sellerBFootage = atoi(temp.c_str());
	sellerFile.close();


	cout << "End of Phase 1 part 1 for Agent1\n";

	//Part 2---------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------
	for (int i = 0; i < 1200000000; i++) { continue; }

	for (int i = 1; i <= 5; i++)
	{
		int sockfd2;
		struct addrinfo hints2, *servinfo2;

		memset(&hints2, 0, sizeof hints2);
		hints2.ai_family = AF_UNSPEC;
		hints2.ai_socktype = SOCK_DGRAM;

		string port = "00000";

		if (i == 1)
			port = "22144";
		else if (i == 2)
			port = "22244";
		else if (i == 3)
			port = "22344";
		else if (i == 4)
			port = "22444";
		else if (i == 5)
			port = "22544";

		if (getaddrinfo("nunki.usc.edu\0", port.c_str(), &hints2, &servinfo2) != 0) {
			cout << "Failed to get remote host address infromation...\n";
			return 1;
		}

		if ((sockfd2 = socket(servinfo2->ai_family, servinfo2->ai_socktype, servinfo2->ai_protocol)) == -1) {
			cout << "Failed to get scoket descriptor...\n";
			return 1;
		}

		stringstream packet;
		packet << "Agent1\n" << sellerAPrice << "\n" << sellerAFootage << "\n" << sellerBPrice << "\n" << sellerBFootage << "\n";
		char pack[100];
		strcpy(pack, packet.str().c_str());

		if (sendto(sockfd2, pack, strlen(pack) + 1, 0, servinfo2->ai_addr, servinfo2->ai_addrlen) == -1)
		{
			cout << "failed to send...\n";
			exit(1);
		}

		int agentPort;
		struct sockaddr_in agentAddress;
		memset(&agentAddress, 0, sizeof agentAddress);
		socklen_t addresslength = sizeof agentAddress;
		getsockname(sockfd2, (struct sockaddr*)&agentAddress, &addresslength);
		agentPort = agentAddress.sin_port;
		cout << "The Agent1 has UDP port " << agentPort << " and IP address " << inet_ntoa(*addr_list[0]) << " for Phase 1 part 2\n";

		cout << "Agent1 has sent \"" << pack << "\" to Buyer" << i << "\n";
		freeaddrinfo(servinfo2);
		close(sockfd2);
	}
	cout << "End of Phase 1 part 2 for Agent1\n";

	//Part 3---------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------

	cout << "The Agent1 has TCP port " << PORT << " and IP address " << inet_ntoa(*addr_list[0]) << " for Phase 1 part 3\n";

	int buyerCount = 0;

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		exit(1);
	}
	while (1)
	{  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, &their_addr, &sin_size);

		if (new_fd == -1) {
			continue;
		}

		buyerCount++;

		if (!fork()) // this is the child process
		{
			close(sockfd); // child doesn't need the listener
			char buff[100];
			int numbytes = 0;
			while (1)
			{
				if ((numbytes = recv(new_fd, buff, 99, 0)) == -1) {
					continue;
				}
				break;
			}
			buff[numbytes] = '\0';

			ofstream agentOutputP3;

			//Saving child process results into a file for access by parent
			if (buyerCount == 1)
				agentOutputP3.open("agent1Buyer1Info.txt");
			else if (buyerCount == 2)
				agentOutputP3.open("agent1Buyer2Info.txt");
			else if (buyerCount == 3)
				agentOutputP3.open("agent1Buyer3Info.txt");
			else if (buyerCount == 4)
				agentOutputP3.open("agent1Buyer4Info.txt");
			else if (buyerCount == 5)
				agentOutputP3.open("agent1Buyer5Info.txt");

			agentOutputP3 << buff << "\n";

			agentOutputP3.close();

			if (buyerCount == 1)
				cout << "Agent1 receives the offer from Buyer1\n";
			else if (buyerCount == 2)
				cout << "Agent1 receives the offer from Buyer2\n";
			else if (buyerCount == 3)
				cout << "Agent1 receives the offer from Buyer3\n";
			else if (buyerCount == 4)
				cout << "Agent1 receives the offer from Buyer4\n";
			else if (buyerCount == 5)
				cout << "Agent1 receives the offer from Buyer5\n";

			close(new_fd);
			exit(0);
		}

		if (buyerCount != 5)
		{
			continue;
		}


		close(new_fd);  // parent doesn't need this
		break;
	}

	for (int i = 0; i < 11000000; i++); //time delay to wait for the child processes to save to file

	cout << "End of Phase 1 part 3 for Agent1\n";

	//Part 4---------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------

	vector<int> budgets(5, 0);
	vector<int> tV(4, 0);
	vector< vector<int> > options(5, tV);

	for (int i = 1; i <= 5; i++)
	{
		ifstream buyerFile;
		string tempStr;
		string fileName;

		if (i == 1)fileName = "agent1Buyer1Info.txt"; else if (i == 2)fileName = "agent1Buyer2Info.txt";  else if (i == 3)fileName = "agent1Buyer3Info.txt";
		else if (i == 4)fileName = "agent1Buyer4Info.txt";  else if (i == 5)fileName = "agent1Buyer5Info.txt";

		buyerFile.open(fileName.c_str());

		getline(buyerFile, tempStr);
		stringstream manip1(tempStr);
		getline(manip1, tempStr, ':');
		getline(manip1, tempStr, ':');
		tempStr.erase(0, 1);
		if (tempStr == "NAK")
		{
			buyerFile.close();
			continue;
		}
		else
		{
			budgets[i - 1] = atoi(tempStr.c_str());
			int optioncount = 0;
			while (buyerFile.good())
			{
				getline(buyerFile, tempStr);
				stringstream manip2(tempStr);

				if (manip2.str().find("Option") != 0)
					break;

				getline(manip2, tempStr, ':');
				getline(manip2, tempStr, ':');
				tempStr.erase(0, 1);

				if(tempStr == "HouseA")options[i-1][optioncount] = 1; else if (tempStr == "HouseB")options[i - 1][optioncount] = 2;
				else if (tempStr == "HouseC")options[i - 1][optioncount] = 3; else if (tempStr == "HouseD")options[i - 1][optioncount] = 4;

				optioncount++;

			}
			buyerFile.close();
		}
	}

	int houseABuyer = 0;
	int houseBBuyer = 0;
	int houseCBuyer = 0;
	int houseDBuyer = 0;

	for (int i = 0; i <= 4; i++)
	{
		for (int j = 0; j <= 3; j++)
		{
			if (options[i][j] == 1 && houseABuyer != i+1 && houseBBuyer != i+1 && houseCBuyer != i+1 && houseDBuyer != i+1)
			{
				if (houseABuyer == 0)
					houseABuyer = i + 1;
				else if (budgets[i] > budgets[houseABuyer - 1])
					houseABuyer = i + 1;
			}
			else if (options[i][j] == 2 && houseABuyer != i + 1 && houseBBuyer != i + 1 && houseCBuyer != i + 1 && houseDBuyer != i + 1)
			{
				if (houseBBuyer == 0)
					houseBBuyer = i + 1;
				else if (budgets[i] > budgets[houseBBuyer - 1])
					houseBBuyer = i + 1;
			}
			else if (options[i][j] == 3 && houseABuyer != i + 1 && houseBBuyer != i + 1 && houseCBuyer != i + 1 && houseDBuyer != i + 1)
			{
				if (houseCBuyer == 0)
					houseCBuyer = i + 1;
				else if (budgets[i] > budgets[houseCBuyer - 1])
					houseCBuyer = i + 1;
			}
			else if (options[i][j] == 4 && houseABuyer != i + 1 && houseBBuyer != i + 1 && houseCBuyer != i + 1 && houseDBuyer != i + 1)
			{
				if (houseDBuyer == 0)
					houseDBuyer = i + 1;
				else if (budgets[i] > budgets[houseDBuyer - 1])
					houseDBuyer = i + 1;
			}
			
		}
	
	}

	///////////////////////////////////////////////////////////////

	for (int i = 1; i <= 2; i++)
	{
		int sockfd2;
		struct addrinfo hints2, *servinfo2;

		memset(&hints2, 0, sizeof hints2);
		hints2.ai_family = AF_UNSPEC;
		hints2.ai_socktype = SOCK_STREAM;

		string port = "0000";

		if (i == 1)
			port = "4044";
		else if (i == 2)
			port = "4144";
		else if (i == 3)
			port = "4244";
		else if (i == 4)
			port = "4344";


		if (getaddrinfo("nunki.usc.edu\0", port.c_str(), &hints2, &servinfo2) != 0) {
			cout << "Failed to get remote host address infromation...\n";
			return 1;
		}

		if ((sockfd2 = socket(servinfo2->ai_family, servinfo2->ai_socktype, servinfo2->ai_protocol)) == -1) {
			cout << "Failed to get scoket descriptor...\n";
			return 1;
		}

		if (connect(sockfd2, servinfo2->ai_addr, servinfo2->ai_addrlen) == -1) {
			cout << "Failed to connect...\n";
			close(sockfd2);
			return 1;
		}

		char agentIP[INET_ADDRSTRLEN];
		int agentPort;
		struct sockaddr_in agentAddress;
		memset(&agentAddress, 0, sizeof agentAddress);
		socklen_t addresslength = sizeof agentAddress;
		getsockname(sockfd2, (struct sockaddr*)&agentAddress, &addresslength);
		inet_ntop(AF_INET, &agentAddress.sin_addr, agentIP, sizeof(agentIP));
		agentPort = agentAddress.sin_port;

		cout << "The Agent1 has TCP port " << agentPort << " and IP address " << agentIP << " for Phase 1 part 4\n";

		string a;
		if (i == 1)
		{
			if (houseABuyer == 0)a = "NAK"; else if (houseABuyer == 1)a = "Buyer 1"; else if (houseABuyer == 2)a = "Buyer 2";
			else if (houseABuyer == 3)a = "Buyer 3"; else if (houseABuyer == 4)a = "Buyer 4"; else if (houseABuyer == 5)a = "Buyer 5";
		}
		if (i == 2)
		{
			if (houseBBuyer == 0)a = "NAK"; else if (houseBBuyer == 1)a = "Buyer 1"; else if (houseBBuyer == 2)a = "Buyer 2";
			else if (houseBBuyer == 3)a = "Buyer 3"; else if (houseBBuyer == 4)a = "Buyer 4"; else if (houseBBuyer == 5)a = "Buyer 5";
		}
		if (i == 3)
		{
			if (houseCBuyer == 0)a = "NAK"; else if (houseCBuyer == 1)a = "Buyer 1"; else if (houseCBuyer == 2)a = "Buyer 2";
			else if (houseCBuyer == 3)a = "Buyer 3"; else if (houseCBuyer == 4)a = "Buyer 4"; else if (houseCBuyer == 5)a = "Buyer 5";
		}
		if (i == 4)
		{
			if (houseDBuyer == 0)a = "NAK"; else if (houseDBuyer == 1)a = "Buyer 1"; else if (houseDBuyer == 2)a = "Buyer 2";
			else if (houseDBuyer == 3)a = "Buyer 3"; else if (houseDBuyer == 4)a = "Buyer 4"; else if (houseDBuyer == 5)a = "Buyer 5";
		}

		if (send(sockfd2, a.c_str(), strlen(a.c_str()), 0) == -1)
		{
			cout << "Failed to send message...\n";
			return 1;
		}

		if(i == 1)
			cout << "Agent1 has sent the result to SellerA\n";
		else if(i == 2)
			cout << "Agent1 has sent the result to SellerB\n";
		else if (i == 3)
			cout << "Agent1 has sent the result to SellerC\n";
		else if (i == 4)
			cout << "Agent1 has sent the result to SellerD\n";

		freeaddrinfo(servinfo2);
		close(sockfd2);
	}
	////////////////////////////////////
	/////////////////////////////////////

	for (int i = 1; i <= 5; i++)
	{
		string a;

		if (houseABuyer == i)a = "SellerA"; else if (houseBBuyer == i)a = "SellerB"; else if (houseCBuyer == i)continue;
		else if (houseDBuyer == i)continue; else continue;

		if (options[i - 1][0] == 0)
			continue;
		int sockfd2;
		struct addrinfo hints2, *servinfo2;

		memset(&hints2, 0, sizeof hints2);
		hints2.ai_family = AF_UNSPEC;
		hints2.ai_socktype = SOCK_STREAM;

		string port = "0000";

		if (i == 1)
			port = "4644";
		else if (i == 2)
			port = "4744";
		else if (i == 3)
			port = "4844";
		else if (i == 4)
			port = "4944";
		else if (i == 5)
			port = "5044";


		if (getaddrinfo("nunki.usc.edu\0", port.c_str(), &hints2, &servinfo2) != 0) {
			cout << "Failed to get remote host address infromation...\n";
			return 1;
		}

		if ((sockfd2 = socket(servinfo2->ai_family, servinfo2->ai_socktype, servinfo2->ai_protocol)) == -1) {
			cout << "Failed to get scoket descriptor...\n";
			return 1;
		}

		if (connect(sockfd2, servinfo2->ai_addr, servinfo2->ai_addrlen) == -1) {
			cout << "Failed to connect...\n";
			close(sockfd2);
			return 1;
		}

		char agentIP[INET_ADDRSTRLEN];
		int agentPort;
		struct sockaddr_in agentAddress;
		memset(&agentAddress, 0, sizeof agentAddress);
		socklen_t addresslength = sizeof agentAddress;
		getsockname(sockfd2, (struct sockaddr*)&agentAddress, &addresslength);
		inet_ntop(AF_INET, &agentAddress.sin_addr, agentIP, sizeof(agentIP));
		agentPort = agentAddress.sin_port;

		cout << "Agent1 has TCP port " << agentPort << " and IP address " << agentIP << " for Phase 1 part 4\n";




		if (send(sockfd2, a.c_str(), strlen(a.c_str()), 0) == -1)
		{
			cout << "Failed to send message...\n";
			return 1;
		}

		if (i == 1)
			cout << "Agent1 has sent the result to Buyer1\n";
		else if (i == 2)
			cout << "Agent1 has sent the result to Buyer2\n";
		else if (i == 3)
			cout << "Agent1 has sent the result to Buyer3\n";
		else if (i == 4)
			cout << "Agent1 has sent the result to Buyer4\n";
		else if (i == 5)
			cout << "Agent1 has sent the result to Buyer5\n";

		cout << "End of Phase 1 part 4 for Agent1\n";
		freeaddrinfo(servinfo2);
		close(sockfd2);
	}

	exit(0);
}
