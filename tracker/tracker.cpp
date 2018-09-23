#include "tracker.h"
int count1 = 100;
int mutlock = 0;
char *logfile;
char *seederfile;
using namespace std;

map<string, set<string>> seedermap;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
void logMessage(char *message)
{

    FILE *fptr;
    fptr = fopen(logfile, "r");
    if (fptr == NULL)
        fptr = fopen(logfile, "w");
    else
        fptr = fopen(logfile, "a");
    if (fptr != NULL)
    {    
    fprintf(fptr, "%s\n", message);
    fclose(fptr);
    }
}
void handleClient(int socketid, char *ip, int port, char *clientip)
{
    int n;
    char buffer[2048];
    char hash[256];
    bzero(hash, 256);
    mutex mut;
    int error1 = 0;
    string hashRec;
    string message;
    bzero(buffer,2048);
    int writeflag=1;
   string receivedMsg;

    n = recv(socketid,buffer, 2048, 0);
    if (n < 0)
            error("ERROR filename from socket");

    cout<<"revied"<<buffer;
        receivedMsg=buffer;


        
    if(receivedMsg.find("remove")!=string::npos && receivedMsg!="")
    {

        receivedMsg=receivedMsg.substr(receivedMsg.find("|")+1,receivedMsg.length());
        string hashReceived=receivedMsg.substr(0,receivedMsg.find("|"));
        string clientInfo=receivedMsg.substr(receivedMsg.find("|")+1,receivedMsg.length());  
        if(seedermap.find(hashReceived)!=seedermap.end())
        {
            set<string> clients;
            clients=seedermap[hashReceived];
            if(clients.find(clientInfo)!=clients.end())
            {
                clients.erase(clientInfo);
            }
        }


    }
    else if(receivedMsg!="")
    {
        string hashReceived=receivedMsg.substr(0,receivedMsg.find("|"));
        string clientInfo=receivedMsg.substr(receivedMsg.find("|")+1,receivedMsg.length());   
        set<string> clients;
        FILE *fp;
        fp = fopen(seederfile, "r");
        if (fp == NULL)
        {
                    fp = fopen(seederfile, "w");
                
                    clients.insert(clientInfo);
                    seedermap.insert(make_pair(hashReceived, clients));
        
        }
        else
        {
            fp = fopen(seederfile, "r");
            char line[128];                             
            while (fgets(line, sizeof line, fp) != NULL) 
            {
    
                string fileLine = line;
                string hashPresent = fileLine.substr(0, fileLine.find("|"));

                vector<string> seeders;
                string test;
                istringstream tokenStream(fileLine);
            
                while (getline(tokenStream, test, '|'))
                {
                    seeders.push_back(test);
                }
            
                clients.insert(seeders[1]+"|"+seeders[2]);
                seedermap.insert(make_pair(seeders[0], clients));
            
                if (seedermap.find(hashReceived) != seedermap.end()) //hash exists
                {
                    cout<<"hash exists";
                    string hashname = seeders[0];
                    auto it = seedermap.find(hashname);
                    clients = it->second;
                    if(clients.find(clientInfo)==clients.end())
                    {
                        
                        clients.insert(clientInfo);
                        seedermap[hashname] = clients;
                    }
                    else
                    {
                    writeflag=0;
                    cout<<"client info exists";
                    }    
                }
                else if(receivedMsg.find("remove")!=string::npos)
    {

        receivedMsg=receivedMsg.substr(receivedMsg.find("|")+1,receivedMsg.length());
        string hashReceived=receivedMsg.substr(0,receivedMsg.find("|"));
        string clientInfo=receivedMsg.substr(receivedMsg.find("|")+1,receivedMsg.length());  
        if(seedermap.find(hashReceived)!=seedermap.end())
        {
            set<string> clients;
            clients=seedermap[hashReceived];
            if(clients.find(clientInfo)!=clients.end())
            {
                clients.erase(clientInfo);
            }
        }


    }
    else
    {
        string hashReceived=receivedMsg.substr(0,receivedMsg.find("|"));
        string clientInfo=receivedMsg.substr(receivedMsg.find("|")+1,receivedMsg.length());   
        set<string> clients;
        FILE *fp;
        fp = fopen(seederfile, "r");
        if (fp == NULL)
        {
                    fp = fopen(seederfile, "w");
                
                    clients.insert(clientInfo);
                    seedermap.insert(make_pair(hashReceived, clients));
        
        }
        else
        {
            fp = fopen(seederfile, "r");
            char line[128];                             
            while (fgets(line, sizeof line, fp) != NULL) 
            {
    
                string fileLine = line;
                string hashPresent = fileLine.substr(0, fileLine.find("|"));

                vector<string> seeders;
                string test;
                istringstream tokenStream(fileLine);
            
                while (getline(tokenStream, test, '|'))
                {
                    seeders.push_back(test);
                }
            
                clients.insert(seeders[1]+"|"+seeders[2]);
                seedermap.insert(make_pair(seeders[0], clients));
            
                if (seedermap.find(hashReceived) != seedermap.end()) //hash exists
                {
                    cout<<"hash exists";
                    string hashname = seeders[0];
                    auto it = seedermap.find(hashname);
                    clients = it->second;
                    if(clients.find(clientInfo)==clients.end())
                    {
                        
                        clients.insert(clientInfo);
                        seedermap[hashname] = clients;
                    }
                    else
                    {
                    writeflag=0;
                    cout<<"client info exists";
                    }    
                }
                else
                {
                    clients.insert(clientInfo);
                    seedermap.insert(make_pair(hashReceived, clients));
                }

                fclose(fp);
                fp = fopen(seederfile, "a");
            }
    
        
        
        


        string finalstring="";

        for(auto itr=seedermap.begin();itr!=seedermap.end();itr++)
            {
              
                finalstring+=itr->first+"|";   
                clients=itr->second;
                string s1;    
                for(string str:clients)
                {
                        s1=str;   
                }
                finalstring+=s1;
                fprintf(fp, "%s\n", finalstring.c_str());


            }
            cout<<"final string is "<<finalstring;

            logMessage("wrriten to seederfile");
            fclose(fp);
    }
    }
                {
                    clients.insert(clientInfo);
                    seedermap.insert(make_pair(hashReceived, clients));
                }

                fclose(fp);
                fp = fopen(seederfile, "a");
            }
    
        
        
        


        string finalstring="";

        for(auto itr=seedermap.begin();itr!=seedermap.end();itr++)
            {
              
                finalstring+=itr->first+"|";   
                clients=itr->second;
                string s1;    
                for(string str:clients)
                {
                        s1=str;   
                }
                finalstring+=s1;
                fprintf(fp, "%s\n", finalstring.c_str());


            }
            cout<<"final string is "<<finalstring;

            logMessage("wrriten to seederfile");
            fclose(fp);
    }
    }

}

void trackerConnect(string tracker_ip1)
{

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[512 * 1024];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr((tracker_ip1.substr(0, tracker_ip1.find(":")).c_str()));
    serv_addr.sin_port = htons(stoi(tracker_ip1.substr(tracker_ip1.find(":") + 1, tracker_ip1.length())));

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
   while (1)
    {

        newsockfd = accept(sockfd,
                           (struct sockaddr *)&cli_addr,
                           &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
            /*
          n = recv(newsockfd,buffer, 2048, 0);
         if (n < 0)
            error("ERROR filename from socket");
        cout<<"revied"<<buffer;
        bzero(buffer,sizeof(buffer));
        cout<<"after clearing"<<buffer;
        */
        printf("Connection accepted from %s:%d : %d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), newsockfd);
        thread t1(handleClient, newsockfd, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), inet_ntoa(cli_addr.sin_addr));
        t1.detach();
      
        
      
    }
   
}
int main(int argc, char *argv[])
{

    if (argc != 5)
    {

        error("Invalid number of arguments: Usage :");
    }

    string tracker_ip1 = argv[1];
    string tracker_ip2 = argv[2];
    logfile = argv[3];
    seederfile = argv[4];
    logMessage("log writing started");

    char buffer[256 * 1024];
    int opt = 1;

    trackerConnect(tracker_ip1);
   
    return 0;
}
