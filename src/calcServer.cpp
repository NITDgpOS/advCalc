#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <pthread.h>
#include <signal.h>

#include "calcParser.hpp"

inline const char *execute(constStr input) {
  char *retVal = new char[30];
  try { // Parsing the input
    calcParse<float64_t> parser(input);
    parser.startParsing();
    sprintf(retVal, "{ \"ans\": %lf }", parser.Ans());
  } catch (ERROR *e) { // Catch any errors
    if (e->isSet()) {
      sprintf(retVal, "{ \"error\": \"%s\" }", e->toString());
    }
  }
  return retVal;
}

void __error(const char *msg)
{
  perror(msg);
  exit(1);
}

//std::vector<std::thread::id> v;

// Register all client threads in this vector
//std::vector<pthread_t> clientThreads;

void exitAllClients(int sig) {
  printf("Caught a signal\n");
  exit(1);
}

struct clientDetails {
  int fd;
  sockaddr_in client;
};

void runConnection(void *c) {
  clientDetails *client = (clientDetails *)c;
  printf("server: got connection from %s port %d\n",
         inet_ntoa(client->client.sin_addr), ntohs(client->client.sin_port));

  char expr[256];
  memset(expr, 0, 256);

  int n = read(client->fd, expr, 255);
  if (n < 0)
    __error("ERROR reading from socket");
  printf(">> %s\n", expr);
  const char *value = execute(expr);
  send(client->fd, value, strlen(value), 0);
  delete value;

  close(client->fd);
  delete client;
  // pthread_exit(0);
}

void getConnections(int sockfd) {
  clientDetails *c;
  socklen_t clilen = sizeof(clientDetails::client);

  // pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
  // pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);
  listen(sockfd, 5);

  do {
    c = new clientDetails;
    memset(&c->client, 0, clilen);
    printf("Ready to accept\n");
    c->fd = accept(sockfd, (sockaddr *)&c->client, &clilen);
    if (c->fd < 0)
      __error("ERROR on accept");
    //pthread_t t;
    //pthread_create(&t, NULL, runConnection, c);
    // clientThreads.push_back(t);
    //t.join();
    runConnection(c);
  } while (c->fd >= 0);
}

int main(int argc, char *argv[])
{
  int sockfd, portno;
  struct sockaddr_in serv_addr;

  makeOperatorHashes();

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  // socket(int domain, int type, int protocol)
  sockfd =  socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    __error("ERROR opening socket");

  // clear address structure
  memset(&serv_addr, 0, sizeof(serv_addr));

  portno = atoi(argv[1]);

  // server byte order
  serv_addr.sin_family = AF_INET;

  // automatically be filled with current host's IP address
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  //  must be converted into network byte order
  serv_addr.sin_port = htons(portno);

  // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
  // Bind  the socket to the current IP address on port, portno
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    __error("ERROR on binding");

  signal(SIGINT, exitAllClients);
  signal(SIGKILL, exitAllClients);
  signal(SIGABRT, exitAllClients);

  getConnections(sockfd);

  close(sockfd);
  return 0;
}
