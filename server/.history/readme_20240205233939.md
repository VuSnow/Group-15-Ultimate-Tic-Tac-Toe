- Instruction to run server
    +) open cmd and run this command: ifconfig
    +) find the IP of your network
    +) change the IP of the server to your IP. (this line of code: address.sin_addr.s_addr = inet_addr("192.168.1.6");)
    +) change the IP of the client to your server IP (this line of code in mainwindow.cpp in client : client.connectToDevice("192.168.1.6", 3000);)
    +) rebuild server by this command: g++ server-remake.cpp -o server-remake
    +) run client and log in accounts in accounts.txt