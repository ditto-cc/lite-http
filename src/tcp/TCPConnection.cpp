
#include "socket/Common.h"
#include "tcp/TCPConnection.h"
#include "log/Log.h"


namespace lite_http {

void TCPConnection::handle_read() {
    size_t n = m_buf_in.read_from_fd(m_ch->fd());
    if (n > 0) {
        LOG_INFO("handle read");
        message_cb(shared_from_this());
    } else if (n == 0) {
        handle_close();
    } else {
        handle_error();
    }
}

void TCPConnection::handle_write() {
    if (m_ch->writable()) {
        size_t n = DoSend(m_ch->fd(), m_buf_out.read_ptr(), m_buf_out.readable_size());
        if (n > 0) {
            m_buf_out.reterive(n);
            if (m_buf_out.readable_size() == 0) {
                m_ch->disable_write();
                if (write_com_cb)
                    m_loop->queue_in_loop(std::bind(write_com_cb, shared_from_this()));
                if (m_state == kDisconnecting) {
                    if (!m_ch->writable())
                        m_socket.shutdown_write();
                }
            }
        } else {
            // TODO nonblocking
        }
    } else {
        // TODO log
    }
}

void TCPConnection::handle_close() {
    LOG_WARN("%s handle close.", m_name.c_str());
    assert(m_state == kDisconnecting || m_state == kConnected);
    m_state = kDisconnected;
    m_ch->disable_all();
    std::shared_ptr<TCPConnection> conn_sp(shared_from_this());
    conn_cb(conn_sp);
    conn_close_cb(conn_sp);
}

void TCPConnection::handle_error() {
    // TODO
    LOG_WARN("%s handle error.", m_name.c_str());
}

void TCPConnection::send(const char* data, size_t sz) {
    if (m_state == kConnected) {
        send_in_loop(data, sz);
    }
}

void TCPConnection::send_in_loop(const char* data, size_t len) {
    if (m_state == kDisconnected) {
        LOG_WARN("%s disconnected, give up write.", m_name.c_str());
        return;
    }

    size_t nwrite = 0, remain = len;
    bool fault = false;
    // TODO nonblocking send directly
    if (!fault && remain > 0) {
        m_buf_out.append(data + nwrite, remain);
        if (!m_ch->writable())
            m_ch->enable_write();
    }
}

void TCPConnection::establish() {
    assert(m_state == kConnecting);
    m_state = kConnected;
    // channel tie this
    m_ch->tie(shared_from_this());
    m_ch->enable_read();
    conn_cb(shared_from_this());
}

void TCPConnection::destory() {
    if (m_state == kConnected) {
        m_state = kDisconnected;
        m_ch->disable_all();
        conn_cb(shared_from_this());
    }
    m_ch->remove();
}
}
