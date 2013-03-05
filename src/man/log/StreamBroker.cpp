#include "StreamBroker.h"

#include <stdexcept>
#include <string>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>

//a few useful print/debug helpers
#define TRACE_BLURB string_format(" in function [%s] at line %d of file \"%s\" \n",\
                      __func__, __LINE__, __FILE__)


#include <cstdarg>
#include <cstdio>

static std::string string_format(const std::string &fmt, ...) {
    int size=100;
    std::string str;
    va_list ap;
    while (1) {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }
        if (n > -1)
            size=n+1;
        else
            size*=2;
    }
}

static std::string errnoString() {
  char * e = std::strerror(errno);
  //strerror is not supposed to fail, but it is never defined whether it *will*
  //always return a valid string; this should really never happen but if it ever did
  //we'd want to know
  return e ? e : "Unkown error! You're screwed?";
}

namespace man {
namespace stream {

StreamBroker::StreamBroker(port_t port) :
  port(port), server_socket_fd(UNINITIALIZED_FD),
  connection_socket_fd(UNINITIALIZED_FD), shouldRun(false) {
}

StreamBroker::~StreamBroker() {
}

void StreamBroker::initializeServerSocket() {

  server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket_fd < 0) {
    throw std::runtime_error(errnoString() + TRACE_BLURB);
  }

  sockaddr_in my_address;
  memset(&my_address, 0, sizeof(my_address));
  my_address.sin_family = AF_INET;
  my_address.sin_port = htons(port);
  my_address.sin_addr.s_addr = htonl(INADDR_ANY);
  int result = bind(server_socket_fd, reinterpret_cast<sockaddr *>(&my_address),
          sizeof(my_address));

  if (result < 0) {
    throw std::runtime_error(errnoString() + TRACE_BLURB);
  }

  //only allow 2 connections to queue up; right now the design
  //only expects one client to be streamed to an any point in time,
  //so this shouldn't matter that much
  result = listen(server_socket_fd, 2);
  if (result < 0) {
    throw std::runtime_error(errnoString() + TRACE_BLURB);
  }
}

void StreamBroker::closeSockets() {
  //http://linux.die.net/man/2/close
  if (connection_socket_fd != UNINITIALIZED_FD) {
    int result = close(connection_socket_fd);
    if (result < 0) {
      throw std::runtime_error(errnoString() + TRACE_BLURB);
    }
    connection_socket_fd = UNINITIALIZED_FD;
  }
  if (server_socket_fd != UNINITIALIZED_FD) {
    int result = close(server_socket_fd);
    if (result < 0) {
      throw std::runtime_error(errnoString() + TRACE_BLURB);
    }
    server_socket_fd = UNINITIALIZED_FD;
  }
}

void StreamBroker::waitForConnection() {
  //common practice, might not be necessary
  memset(&client_address, 0, sizeof(client_address));

  connection_socket_fd = accept(server_socket_fd, &client_address,
            &client_address_len);
  if (connection_socket_fd < 0) {
    throw std::runtime_error(errnoString() + TRACE_BLURB);
  }
}

void StreamBroker::start() {

  initializeServerSocket();

  // Don't let it recreate the same thread!
  if (shouldRun) {
    throw std::runtime_error("Thread is already running!");
  }

  shouldRun = true;

  // We're not detaching this thread so we can join later on in order
  // to ensure the sockets are shut down properly
  pthread_attr_t attr;
  pthread_attr_init (&attr);

  const int result = pthread_create(&thread, &attr, runThread, (void*)this);

  if (result < 0) {
    throw std::runtime_error(errnoString() + TRACE_BLURB);
  }

  // Free space from attr
  pthread_attr_destroy(&attr);
}

int StreamBroker::run() {
  try {
    while(shouldRun) {
      if (connection_socket_fd == UNINITIALIZED_FD) {
        waitForConnection();
      } else {
        //write
      }
    }
  } catch (std::exception &e) {
    std::cerr << "Ending stream broker thread after thrown exception:\n"
      << e.what() << std::endl;
    return -1;
  }

  try {
    closeSockets();
  } catch (std::exception &e) {
    std::cerr << "Error closing the threads:\n"
      << e.what() << std::endl;
    return -1;
  }

  return 0;
}


void* StreamBroker::runThread(void* _this)
{
    StreamBroker* this_instance = reinterpret_cast<StreamBroker*>(_this);

    int result = this_instance->run();

    //call new because pthread_exit expects a pointer
    //the caller has the duty to free the pointer
    //this is a private method so this is handled by the method
    //that does the pthread_join
    pthread_exit(static_cast<void*>(new int(result)));
}

int StreamBroker::stop() {
  //can't join a thread twice!
  //note: if we don't have this check here it would probably error out
  //down the road anyway; can't hurt to have this check so we don't rely on that
  //assumption
  if (shouldRun == false) {
    throw std::runtime_error("Already signalled to stop/stopped!");
  }

  //use shutdown to close sending/receiving on a socket
  //it doesn't close the socket; it leaves it in a 'closed' state in order
  //to prevent the system from re-assigning the same FD number to another
  //socket and thus allowing the broker thread to still 'use' the socket
  //we still need to close the socket after we're done
  //http://stackoverflow.com/questions/2486335/wake-up-thread-blocked-on-accept-call/
  shutdown(server_socket_fd, SHUT_RDWR);
  shouldRun = false;

  void* void_ret;
  //blocks until the thread is done; since we shutdown the socket all I/O operations
  //should return
  int result = pthread_join(thread, &void_ret);
  if (result < 0) {
    throw std::runtime_error(errnoString() + TRACE_BLURB);
  }

  //copy the result so we can free the ret memory
  int* ret = static_cast<int*>(void_ret);
  int thread_result = *ret;
  delete ret;
  return thread_result;
}

}
}
