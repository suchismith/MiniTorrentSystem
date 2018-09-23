#include "client.h"
#define IP_PROTOCOL 0
#define NET_BUF_SIZE 255
using namespace std;
char *logfile;
void logMessage(char *message)
{

    FILE *fptr;
    fptr = fopen(logfile, "r");
    if (fptr == NULL)
        fptr = fopen(logfile, "w");
    else
        fptr = fopen(logfile, "a");

    fprintf(fptr, "%s", message);
    fclose(fptr);
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void shareFile(string filename, string torrent, char *ipaddress, int port, int socketid, string clientip)
{

    unsigned char digest[SHA_DIGEST_LENGTH];
   
    FILE *fp;
    int n;
    char message[2048];
    fp = fopen(filename.c_str(), "r");
    fseek(fp, 0L, SEEK_END);
    long sz = ftell(fp);
    sz = sz / 1024;
    char hashString[SHA_DIGEST_LENGTH * 2 + 1];
    int numberOfChunks = sz / (512 * 1024);
    rewind(fp);
    int chunksize = 512 * 1024;

    string finalHash;
    char hashGen[255];
    char buffer[chunksize];
    while (fread(buffer, chunksize, 1, fp) == 1)
    {

        const unsigned char *buffer1 = reinterpret_cast<const unsigned char *>(buffer);
        SHA1(buffer1, SHA_DIGEST_LENGTH, digest);
        for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
            sprintf(&hashString[i * 2], "%02x", (unsigned int)digest[i]);

        finalHash += hashString;
        strcat(hashGen, hashString);
    }

    fp = fopen(torrent.c_str(), "w");
    fprintf(fp, "%s:%d", ipaddress, port);
    const unsigned char *hashToSend = reinterpret_cast<const unsigned char *>(hashGen);
    SHA1(hashToSend, SHA_DIGEST_LENGTH, digest);

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf(&hashString[i * 2], "%02x", (unsigned int)digest[i]);
    fp = fopen(torrent.c_str(), "w");
    fprintf(fp, "%s:%d\n", ipaddress, port);
    fprintf(fp, "filesize %ld\n", sz);
    fprintf(fp, "%s\n", finalHash.c_str());
    fclose(fp);
    logMessage("Torrent File Created");

    finalHash = hashString;
    finalHash.resize(20);
    string finalMessage = "";

    strcat(message, finalHash.c_str());
    strcat(message, "|");
    strcat(message, filename.c_str());
    strcat(message, "|");
    strcat(message, clientip.c_str());
    message[100]='\n';
    n = send(socketid, message, 2048, 0);
    if (n < 0)
        printf("error in sending");
    bzero(message,sizeof(message));
}

void removeFile(string torrentfile,char *ipaddress,int port, int socketid, string clientip)
{

    string finalHash;
    char hashGen[255];
    FILE *fptr=fopen(torrentfile.c_str(),"r");
    char hash[1024];
    while(!feof(fptr))
        fgets(hash, 1024, fptr);
    int n;
    char message[2048];
    char hashString[SHA_DIGEST_LENGTH * 2 + 1];
    unsigned char digest[SHA_DIGEST_LENGTH];
    const unsigned char *hashToSend = reinterpret_cast<const unsigned char *>(hash);
    SHA1(hashToSend, SHA_DIGEST_LENGTH, digest);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf(&hashString[i * 2], "%02x", (unsigned int)digest[i]);
        

    finalHash = hashString;
    finalHash.resize(20);
    string finalMessage = "";
    cout<<finalHash;
    strcat(message,"remove|");
    strcat(message, finalHash.c_str());
    strcat(message, "|");
    strcat(message, clientip.c_str());
  
    n = send(socketid, message, 2048, 0);
    if (n < 0)
        printf("error in sending");
    


}


int main(int argc, char *argv[])
{

    if (argc != 5)
    {

        error("Invalid number of arguments: Usage :");
    }

    string client_ip = argv[1];
    string tracker_ip = argv[2];
    string tracker_ip2 = argv[3];
    logfile = argv[4];
    int sockfd;
    struct sockaddr_in server_addr;
    int addresslength = sizeof(server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(stoi(tracker_ip.substr(tracker_ip.find(":") + 1, tracker_ip.length())));
    server_addr.sin_addr.s_addr = inet_addr((tracker_ip.substr(0, tracker_ip.find(":")).c_str()));
    string trackerIP = inet_ntoa(server_addr.sin_addr);
    char net_buf[10];

    string command;

    sockfd = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL);
    if (sockfd < 0)
        error("ERROR opening socket");

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        cout << "ERROR connecting";
    char buffer[100];
    snprintf(buffer, 100, "Connecting to tracker server %s : %d", tracker_ip, server_addr.sin_port);
    logMessage(buffer);

    while (1)
    {

        cout << "Enter Command";
        string command;
        getline(std::cin, command);
        if (command.find("share") != string::npos)
        {

            std::istringstream iss(command);
            std::vector<std::string> results(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
            string filename = results[1];
            string torrentfile = results[2];
            FILE *fptr = fopen(filename.c_str(), "r");
            if (fptr == NULL)
                error("Invalid file name.");
          
            thread t1(shareFile, filename, torrentfile, inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port), sockfd, client_ip);
            t1.detach();
        }
        else if (command.find("remove") != string::npos)
        {

           std::istringstream iss(command);
           std::vector<std::string> results(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());     
            string torrentfile = results[1];

            thread t1(removeFile,torrentfile,inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port), sockfd, client_ip);
            t1.detach();

        }

        if (command == "exit")
            break;
    }

    return 0;
}
