
#pragma once

#include <boost/utility.hpp>
#include <netinet/in.h>
#include <stdint.h>
#include "RoboGrams.h"

namespace man {
namespace stream {

class StreamBroker : boost::noncopyable {

public:
  typedef uint16_t port_t;
  static const port_t DEFAULT_PORT = 4242;
  static const int UNINITIALIZED_FD = -1;

public:
  StreamBroker(port_t port = DEFAULT_PORT);

  ~StreamBroker();

  /*
   * Tries to start the broker thread and initializes the server.
   * Throws an error if it fails.
   */
  void start();
  /*
   * Main server loop. Exceptions thrown in this thread that cause the
   * thread to end will be output to cerr.
   *
   * @return the same value as stop; see comments below
   */
  int run();
  /*
   * Closes the server socket, notifies the run thread to stop looping and
   * joins the thread.
   * It's essential to call this or else the server socket and any outstanding
   * client sockets might not close properly.
   * Throws an error if it fails to do so.
   *
   * @return the exit value of the thread; usually 0 if thread stopped with no exceptions
   * or -1 if it didn't; note that this doesn't indicate if the thread stopped because
   * of an abnormal exception or not since we do expect some system calls to return
   * errors after we close the socket (e.g. accept() should return from blocking with an
   * error after we close the socket and that will be raised as an exception; but this is
   * indeed the behavior we want from it; the return will be -1 but the results are the
   * ones desired)
   */
  int stop();

private:
  // this method gets subscribed as a callback for the thread
  // it's the first method to get run when we start the thread
  // and it drives the threaded broker code
  static void* runThread(void* _this);

  //initialize the server; binds the socket to an address
  void initializeServerSocket();

  //block on accepting incoming connections; to break the block
  //shutdown the server socket; that will raise an exception; don't panic
  //that's normal
  void waitForConnection();

  // close the sockets if they still have FD numbers; we assume the FD numbers
  // are valid
  void closeSockets();
private:
  port_t port;
  //socket fd on which we listen for client connections
  int server_socket_fd;
  //socket fd for connected client
  int connection_socket_fd;

  sockaddr client_address;
  socklen_t client_address_len;

  pthread_t thread;

  // Keeps track of whether this thread's main method should be going
  // volatile so that it never gets optimize cached
  volatile bool shouldRun;
};
}
}
