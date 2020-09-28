# virtual-real-estate-agent
A socket program that simulates the selling process of houses through virtual agents using TCP and UDP sockets in a networked client/server architecture. The program supports several agents, buyers and sellers. The agents facilitate the sale according to the requirement of the buyer and the minimum selling price of the seller. All modules run concurrently and each can be run on a seperate computer or on the same computer for testing purposes.

### Testing
Tested this using sellerX.txt (X: A - D) and buyerX.txt (X: 1 - 5)files created manually on Windows and then transferred to Unix server--only mentioning this because of the different end of line styles but the program works well on Solaris.

### Code Files + A Brief Description
	* SellerX.cpp (X: A - D): Reads the seller's house info text, sends it to the assigned agent and waits for agent to send it the buyers' offers.
	* AgentX.cpp (X: 1 - 2): Receives the seller's house info from the assigned seller and sends it to the prospective buyers who then send back their offers to the agent who then sends the best offers to the sellers and the sellers' acceptances to the buyers
	* BuyerX.cpp (X: 1 - 5): Receives seller's house information from the agents, reads the buyer's own housing and budget limitations and decides what houses it would like to buy and sends the offers to the agents

### Running Sequence
I prefer that you run the program in this sequence: Buyer1, Buyer2, Buyer3, Buyer4, Buyer5, Agent1, Agent2, SellerA, SellerB, SellerC, Seller D; but, I have added a 25-second delay prior to part 2 to give you plenty of time to start up the program in the assignment specified sequence of Agent->Seller->Buyer. This is a limitation of UDP which could've been overcome by manually sending an ACK, but then why use UDP if we wanted and ACK in the first place? And, hence, the added timer.
	
### Reused Code
Only reused code from Beej's Guide to Network Programming.

### Failure Scenarios
The program isn't going to produce accurate results if you run the Buyers or the Sellers out of order (B-A or 2-1 instead of A-B or 1-2).
