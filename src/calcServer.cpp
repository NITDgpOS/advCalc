#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <future>
#include <map>
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

class calcServer {
  struct IPCdetails {
    int fd;
    socklen_t length;
    sockaddr_in address;
    IPCdetails() {
      length = sizeof(address);
      memset(&address, 0, length);
      fd = 0;
    }
    const std::string getAddress() const {
      std::string a = inet_ntoa(address.sin_addr);
      a += ":" + std::to_string(ntohs(address.sin_port));
      return a;
    }
    void debug(const std::string s) const {
      std::cout << getAddress() << "> " << s << "\n";
    }
    socklen_t *getLength() {
      length = sizeof(address);
      return &length;
    }
    ~IPCdetails() {
      close(fd);
    }
  } server;
  std::map<std::string, std::shared_ptr<IPCdetails>> clients;
  std::map<std::string, std::shared_future<void>> clientHandles;
  int numberOfConnections = 0;
  int maxClients = 10;
  std::mutex class_mutex;
  void runCommands(std::shared_ptr<IPCdetails> client) {
    client->debug("Thread launched");
    std::string expr;
    do {
      expr = "";
      char s[256];
      int len;
      client->debug("Waiting for message");
      while (true) {
        len = 255;
        len = read(client->fd, s, len);
        if (len > 0 && s[len - 2] == '\n') {
          s[len - 2] = '\0';
          expr += s;
          break;
        } else if (len <= 0) {
          client->debug("Connection closed");
          dropClient(client->getAddress());
          return;
        }
        client->debug("Didn't get '\\n'. Reading again.");
      }
      char c[1000];
      sprintf(c, "Received '%s'", expr.c_str());
      client->debug(c);
      const char *value = execute(expr.c_str());
      sprintf(c, "Sending '%s'", value);
      client->debug(c);
      send(client->fd, value, strlen(value), 0);
      delete value;
    } while (expr != "exit" && expr != "quit");
    dropClient(client->getAddress());
  }
public:

  calcServer(sa_family_t server_byte_order = AF_INET,
             in_addr_t ip_address = INADDR_ANY,
             int port = 3001) {
    server.address.sin_family = server_byte_order;
    server.address.sin_addr.s_addr = ip_address;
    server.address.sin_port = htons(port);
  }

  void init(sa_family_t server_byte_order = AF_INET,
            in_addr_t ip_address = INADDR_ANY,
            int port = 3001) {
    server.address.sin_family = server_byte_order;
    server.address.sin_addr.s_addr = ip_address;
    server.address.sin_port = htons(port);
  }

  ~calcServer() { stopServer(); }

  void startServer() {
    server.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server.fd < 0) {
      printf("Error opening socket\n");
      exit(1);
    }
    if (bind(server.fd, (sockaddr *)&server.address, *server.getLength()) < 0) {
      printf("Unable to bind to that address\n");
      exit(1);
    }
    server.debug("Server started");
    listen(server.fd, maxClients);
    while (true) {
      if (numberOfConnections < maxClients) {
        numberOfConnections++;
        char c[100];
        sprintf(c, "Ready for connection %d", numberOfConnections);
        server.debug(c);
        acceptClient();
      } else {
        server.debug("Sleeping for 2s");
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);
      }
    }
  }

  void stopServer() { close(server.fd); }

  void dropAllClients() {
    clientHandles.clear();
  }
  void acceptClient() {
    auto client = std::make_shared<IPCdetails>();
    server.debug("Accepting new clients");
    client->fd = accept(server.fd, (sockaddr *)&client->address, client->getLength());
    if (client->fd < 0)
      server.debug("Can't connect to client");
    else {
      client->debug("Welcome");
      auto address = client->getAddress();
      client->debug("Registering thread");
      auto handle = std::async(std::launch::async, &calcServer::runCommands, this, client).share();
      client->debug("Thread registered");
      auto old_handle = clientHandles.find(address);
      if (old_handle != clientHandles.end()) {
        char c[50];
        sprintf(c, "Waiting for %s to finish\n", address.c_str());
        server.debug(c);
        old_handle->second.wait();
      }
      class_mutex.lock();
      clientHandles.erase(address);
      clients.erase(address);
      clients[address] = client;
      clientHandles[address] = handle;
      class_mutex.unlock();
    }
  }
  void dropClient(const std::string address) {
    class_mutex.lock();
    if (clientHandles.find(address) != clientHandles.end()) {
      clients.erase(address);
      numberOfConnections--;
    } else {
      printf("Client '%s' didn't connect to this server\n", address.c_str());
    }
    class_mutex.unlock();
  }
} server;
void __error(const char *msg) {
  perror(msg);
  exit(1);
}

//std::vector<std::thread::id> v;

// Register all client threads in this vector
//std::vector<pthread_t> clientThreads;

void stopServer(int sig) {
  printf("Caught a signal\n");
  server.stopServer();
  exit(1);
}

struct clientDetails {
  int fd;
  sockaddr_in client;
};

void runConnection_(void *c) {
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
    // auto exprRunner = std::async(std::launch::async, runConnection, c);
    runConnection_(c);
  } while (c->fd >= 0);
}

int main(int argc, char *argv[])
{
  // int sockfd, portno;
  // struct sockaddr_in serv_addr;

  makeOperatorHashes();

  // if (argc < 2) {
  //   fprintf(stderr,"ERROR, no port provided\n");
  //   exit(1);
  // }

  // // socket(int domain, int type, int protocol)
  // sockfd =  socket(AF_INET, SOCK_STREAM, 0);
  // if (sockfd < 0)
  //   __error("ERROR opening socket");

  // // clear address structure
  // memset(&serv_addr, 0, sizeof(serv_addr));

  // portno = atoi(argv[1]);

  // // server byte order
  // serv_addr.sin_family = AF_INET;

  // // automatically be filled with current host's IP address
  // serv_addr.sin_addr.s_addr = INADDR_ANY;

  // // portno must be converted into network byte order
  // serv_addr.sin_port = htons(portno);

  // // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
  // // Bind  the socket to the current IP address on port, portno
  // if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  //   __error("ERROR on binding");

  signal(SIGINT, stopServer);
  signal(SIGKILL, stopServer);
  signal(SIGABRT, stopServer);

  // getConnections(sockfd);
  server.startServer();

  // close(sockfd);
  return 0;
}
