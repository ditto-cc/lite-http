#ifndef _TCP_CONNECTION_H
#define _TCP_CONNECTION_H

#include "core/EventLoop.h"
#include "core/Buffer.h"


namespace lite_http {
    typedef std::function<void()> Callback;
    class TCPConnection {
    private:
        int m_fd{ -1 };
        std::shared_ptr<EventLoop> m_eventloop;

        Callback connectionCompleteCallback;
        Callback connectionCloseCallback;
        Callback writeCompleteCallback;
        Callback messageCallback;

        Buffer m_buf_in, m_buf_out;

    public:
        TCPConnection(int conn_fd, Callback& conn_com_cb, Callback& conn_close_cb,
                      Callback& write_com_cb, Callback& message_cb) : m_fd(conn_fd),
                      connectionCloseCallback(conn_close_cb), writeCompleteCallback(write_com_cb),
                      messageCallback(message_cb) {
            Channel ch(conn_fd, EVENT_READ);
            ch.set_read_callback([&]() {
                if (m_buf_in.read_from_fd(ch.get_fd()) > 0) {
                    if (messageCallback)
                        messageCallback();
                } else
                    connectionCloseCallback();
            });
            ch.set_write_callback([&]() {
                int ret = m_buf_out.write_to_fd(ch.get_fd());
                if (ret > 0) {

                    // TODO disable write
                    if (m_buf_out.get_readable_size() == 0)


                    if (writeCompleteCallback)
                        writeCompleteCallback();
                } else {
                    // TODO Log
                }
            });
            if (connectionCompleteCallback)
                connectionCompleteCallback();
            m_eventloop->add_channel_event(conn_fd, ch);
        }
        ~TCPConnection() {}
    };
}

#endif
