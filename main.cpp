#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

void resolveDNSAndConnect(const std::string& hostname) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4

    if (getaddrinfo(hostname.c_str(), nullptr, &hints, &res) != 0) {
        cerr << "Failed to resolve DNS for " << hostname << endl;
        return;
    }

    for (struct addrinfo* p = res; p != nullptr; p = p->ai_next) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(p->ai_family, &((struct sockaddr_in*)p->ai_addr)->sin_addr, ip, sizeof(ip));
        cout << "Resolved IP address: " << ip << endl;

        // ソケット作成
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Socket creation failed." << std::endl;
            continue;
        }

        // 接続先情報設定
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(80); // HTTPポート
        inet_pton(AF_INET, ip, &server_addr.sin_addr);

        // 接続
        cout << "Attempting to connect to " << ip << "..." << endl;
        if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
            cout << "Successfully connected to " << ip << endl;
            close(sockfd);
            break; // 最初に成功した接続で終了
        } else {
            cerr << "Connection to " << ip << " failed." << endl;
            close(sockfd);
        }
    }

    freeaddrinfo(res);
}

int main() {
    string hostname;
    cout << "Enter a domain name: ";
    cin >> hostname;
    resolveDNSAndConnect(hostname);
    return 0;
}
