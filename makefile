all:
	g++ -o Agent1 Agent1.cpp -lsocket -lnsl -lresolv
	g++ -o Agent2 Agent2.cpp -lsocket -lnsl -lresolv
	g++ -o SellerA SellerA.cpp -lsocket -lnsl -lresolv
	g++ -o SellerB SellerB.cpp -lsocket -lnsl -lresolv
	g++ -o SellerC SellerC.cpp -lsocket -lnsl -lresolv
	g++ -o SellerD SellerD.cpp -lsocket -lnsl -lresolv
	g++ -o Buyer1 Buyer1.cpp -lsocket -lnsl -lresolv
	g++ -o Buyer2 Buyer2.cpp -lsocket -lnsl -lresolv
	g++ -o Buyer3 Buyer3.cpp -lsocket -lnsl -lresolv
	g++ -o Buyer4 Buyer4.cpp -lsocket -lnsl -lresolv
	g++ -o Buyer5 Buyer5.cpp -lsocket -lnsl -lresolv
