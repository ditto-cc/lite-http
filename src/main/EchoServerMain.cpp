
#include "core/EventLoop.h"
#include "tcp/TcpServer.h"
#include "log/Log.h"

using namespace lite_http;

void OnMessage(const TcpConnectionPtr &conn_sp) {
  Buffer &buf_in = conn_sp->GetBufIn();
  std::string msg = buf_in.GetReadableAsStr();
  LOG_INFO("receive msg from connection(%s): %s.",
           conn_sp->ConnName(),
           msg.substr(0, msg.find_first_of('\r')).c_str());
  conn_sp->Send(msg.c_str(), msg.size());
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: Port\n");
    exit(0);
  }

  const char *server_name = "EchoServer";
  uint16_t port = atoi(argv[1]);

  EventLoop loop(std::string(server_name) + "_loop");
  INetAddress listen_addr(port);
  TcpServer server(&loop, listen_addr, server_name);
  server.SetConnCb(
      [](const TcpConnectionPtr &conn_sp) {
        LOG_INFO("connection callback(%s) %s", conn_sp->ConnName(), conn_sp->Connected() ? "UP" : "DOWN");
      });
  server.SetMessageCb(OnMessage);
  server.SetWriteComCb(
      [](const TcpConnectionPtr &conn_sp) {
        LOG_INFO("write callback(%s)", conn_sp->ConnName());
      });
  server.Start();
  loop.RunLoop();

  return 0;
}
