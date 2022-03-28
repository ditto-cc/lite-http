#ifndef _TCP_CONNECTION_H
#define _TCP_CONNECTION_H

#include <string>

#include "core/EventLoop.h"
#include "core/Buffer.h"
#include "socket/Socket.h"
#include "util/nocopyable.h"

namespace lite_http {

class TCPConnection;
typedef std::function<void(const std::shared_ptr<TCPConnection>&)> ConnCallback;

class TCPConnection : private nocopyable, public std::enable_shared_from_this<TCPConnection> {
public:
    TCPConnection(
        EventLoop* loop,
        int connfd,
        std::string name)
    : m_loop(loop),
        m_state(kConnecting),
        m_name(std::move(name)),
        m_socket(connfd),
        m_ch(new Channel(loop, connfd)) {
        AsyncLogger::LogInfo("TCPConnection ctor %d", m_ch->get_fd());
        m_ch->set_read_callback(std::bind(&TCPConnection::handle_read, this));
        m_ch->set_write_callback(std::bind(&TCPConnection::handle_write, this));
    }
    ~TCPConnection() {
        AsyncLogger::LogInfo("TCPConnection dtor %d", m_ch->get_fd());
    }

    EventLoop* get_loop() { return m_loop; }

    Buffer& get_buf_in() { return m_buf_in; }
    Buffer& get_buf_out() { return m_buf_out; }
    const char* get_conn_name() const { return m_name.c_str(); }
    bool connected() const { return m_state == kConnected; }
    bool disconnected() const { return m_state == kDisconnected; }

    void shutdown();
    void force_close();
    void start_read();
    void stop_read();

    void set_conn_cb(ConnCallback cb) { conn_cb = std::move(cb); }
    void set_conn_close_cb(const ConnCallback& cb) { conn_close_cb = cb; }
    void set_write_com_cb(const ConnCallback& cb) { write_com_cb = cb; }
    void set_message_cb(const ConnCallback& cb) { message_cb = cb; }

    void establish();
    void destory();

    void send(const char* data, size_t size);
private:
    enum State {kConnecting, kConnected, kDisconnecting, kDisconnected};
    ConnCallback conn_cb, conn_close_cb, write_com_cb, message_cb;

    EventLoop* m_loop;
    std::string m_name;
    Buffer m_buf_in, m_buf_out;
    std::unique_ptr<Channel> m_ch;
    Socket m_socket;
    int m_state;

    void handle_read();
    void handle_write();
    void handle_close();
    void handle_error();

    void send_in_loop(const char* data, size_t len);

};
}

#endif
