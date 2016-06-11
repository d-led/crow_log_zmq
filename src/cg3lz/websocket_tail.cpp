#include "websocket_tail.h"

#include <spdlog/sinks/sink.h>

#include <zmq_addon.hpp>


class websocket_sink : public spdlog::sinks::sink //base_sink < std::mutex >
{
    void log(const spdlog::details::log_msg& msg) override
    {
        // Your code here.
        // details::log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
        // msg.formatted contains the formatted log.
        // msg.raw contains pre formatted log
        //std::cout << msg.formatted.str();
    }
};


// based on https://github.com/zaphoyd/websocketpp/blob/master/examples/broadcast_server/broadcast_server.cpp
#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <set>

/*#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>*/
#include <websocketpp/common/thread.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

/* on_open insert connection_hdl into channel
* on_close remove connection_hdl from channel
* on_message queue send to all channels
*/

enum action_type {
    SUBSCRIBE,
    UNSUBSCRIBE,
    MESSAGE
};

struct action {
    action(action_type t, connection_hdl h) : type(t), hdl(h) {}
    action(action_type t, connection_hdl h, server::message_ptr m)
        : type(t), hdl(h), msg(m) {}

    action_type type;
    websocketpp::connection_hdl hdl;
    server::message_ptr msg;
};

class broadcast_server {
public:
    broadcast_server() {
        // Initialize Asio Transport
        m_server.init_asio();

        // Register handler callbacks
        m_server.set_open_handler(bind(&broadcast_server::on_open, this, ::_1));
        m_server.set_close_handler(bind(&broadcast_server::on_close, this, ::_1));
        m_server.set_message_handler(bind(&broadcast_server::on_message, this, ::_1, ::_2));
    }

    void run(uint16_t port) {
        m_server.clear_access_channels(websocketpp::log::alevel::all);
        // listen on specified port
        m_server.listen(port);

        // Start the server accept loop
        m_server.start_accept();

        // Start the ASIO io_service run loop
        try {
            m_server.run();
        }
        catch (const std::exception & e) {
            std::cout << e.what() << std::endl;
        }
    }

    void on_open(connection_hdl hdl) {
        {
            lock_guard<mutex> guard(m_action_lock);
            //std::cout << "on_open" << std::endl;
            m_actions.push(action(SUBSCRIBE, hdl));
        }
        m_action_cond.notify_one();
    }

    void on_close(connection_hdl hdl) {
        {
            lock_guard<mutex> guard(m_action_lock);
            //std::cout << "on_close" << std::endl;
            m_actions.push(action(UNSUBSCRIBE, hdl));
        }
        m_action_cond.notify_one();
    }

    void on_message(connection_hdl hdl, server::message_ptr msg) {
        // queue message up for sending by processing thread
        {
            lock_guard<mutex> guard(m_action_lock);
            //std::cout << "on_message" << std::endl;
            m_actions.push(action(MESSAGE, hdl, msg));
        }
        m_action_cond.notify_one();
    }

    void push(std::string const& msg) {
        unique_lock<mutex> lock(m_action_lock);
        con_list::iterator it;
        for (it = m_connections.begin(); it != m_connections.end(); ++it) {
            m_server.send(*it, msg.data(), msg.length(), websocketpp::frame::opcode::TEXT);
        }
    }

    void process_messages() {
        while (1) {
            unique_lock<mutex> lock(m_action_lock);

            while (m_actions.empty()) {
                m_action_cond.wait(lock);
            }

            action a = m_actions.front();
            m_actions.pop();

            lock.unlock();

            if (a.type == SUBSCRIBE) {
                lock_guard<mutex> guard(m_connection_lock);
                m_connections.insert(a.hdl);
            }
            else if (a.type == UNSUBSCRIBE) {
                lock_guard<mutex> guard(m_connection_lock);
                m_connections.erase(a.hdl);
            }
            else if (a.type == MESSAGE) {
                lock_guard<mutex> guard(m_connection_lock);

                con_list::iterator it;
                for (it = m_connections.begin(); it != m_connections.end(); ++it) {
                    m_server.send(*it, a.msg);
                }
            }
            else {
                // undefined.
            }
        }
    }
private:
    typedef std::set<connection_hdl, std::owner_less<connection_hdl> > con_list;

    server m_server;
    con_list m_connections;
    std::queue<action> m_actions;

    mutex m_action_lock;
    mutex m_connection_lock;
    condition_variable m_action_cond;
};

struct websocket_ticker::impl {
    broadcast_server server;
};

websocket_ticker::websocket_ticker(zmq::context_t& c, int port):
    ctx(c),
    pimpl(std::make_unique<impl>())
{
}

websocket_ticker::~websocket_ticker()
{
}

void websocket_ticker::run()
{
    run_thread = std::thread([this] {
        broadcast_server s;
        ws_loop = std::thread(bind(&broadcast_server::process_messages, &s));
        uint16_t port = 9002;

        pair_loop = std::thread([this, &s] {
            zmq::socket_t pair(ctx, ZMQ_PAIR);
            pair.bind("inproc://tick");
            while (true) {
                zmq::multipart_t m(pair);
                s.push(m.popstr());
            }
        });

        s.run(port);
    });
}

void websocket_ticker::stop()
{
}
