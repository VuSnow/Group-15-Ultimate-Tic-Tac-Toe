- Instruction to run server <br>
    +) open cmd and run this command: ifconfig\n
    +) find the IP of your network\n
    +) change the IP of the server to your IP. (this line of code: address.sin_addr.s_addr = inet_addr("192.168.1.6");)\n
    +) change the IP of the client to your server IP (this line of code in mainwindow.cpp in client : client.connectToDevice("192.168.1.6", 3000);)\n
    +) rebuild server by this command: g++ server-remake.cpp -o server-remake\n
    +) run client and log in accounts in accounts.txt\n
