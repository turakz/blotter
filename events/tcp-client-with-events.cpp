// blotter includes
#include "events.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/system/error_code.hpp>

#include <openssl/ssl.h>

#include <iostream>
#include <iostream>

class tcp_client : public blotter::events::event_manager_base {
public:
    // client construction -> never construct without parameters
    // ---------------------------------------------------------
    tcp_client(const std::string& prompt,
            boost::asio::io_context& ioc,
            boost::asio::ssl::context& ssl_ctx, 
            const std::string& url, 
            const std::string& port, 
            const std::string& message)
        :   events_{},
            resolver_{boost::asio::make_strand(ioc)}, 
            websocket_{boost::asio::make_strand(ioc), ssl_ctx}, 
            readBuffer_{}, 
            prompt_{prompt},
            url_{url}, 
            port_{port},
            message_{message},
            nBytesRead_{0} 
    {
        add_event("on resolve", new on_resolve{this});
        add_event("on connect", new on_connect{this});
        add_event("on tcp handshake", new on_tcp_handshake{this});
        add_event("on tls handshake", new on_tls_handshake{this});
        add_event("on send", new on_send{this});
        add_event("on read", new on_read{this});
        add_event("on response", new on_response{this});
        add_event("on disconnect", new on_disconnect{this});
    }
    ~tcp_client() override
    {
        for (std::unordered_map<std::string, blotter::events::event_base*>::iterator itr = std::begin(events_);
                itr != std::end(events_); ++itr)
        {
            delete itr->second;
            itr->second = nullptr;
        }
    }
    // interface implementation
    // ------------------------
    void add_event(const std::string& which, blotter::events::event_base* event) override
    {
        auto e = events_.find(which);
        if (e == std::end(events_))
        {
            events_.insert(std::make_pair(which, std::move(event)));
        }
    }
    void remove_event(const std::string& which) override
    {
        auto e = events_.find(which);
        if (e != std::end(events_))
        {
            // all events were allocated by new
            // delete and then remove the entry from the hmp
            if (e->second)
            {
                delete e->second;
                e->second = nullptr;
            }
            events_.erase(which);
        }
    }
    bool raise_event(const std::string& which, boost::system::error_code& ec) override
    {
        auto e = events_.find(which);
        if (e != std::end(events_))
        {
            events_[which]->handler(ec);
            return true;
        }
        return false;
    }
    // tcp client api
    // --------------
    void LogError(boost::system::error_code& ec, const std::string& what)
    {
        std::cerr << prompt_ << "::what -> " << what << ' '
                  << (ec ? ec.message() : "OK")
                  << std::endl;
    }
    void connect() 
    {
        std::cout << prompt_ << "resolving " << url_ << ":" << port_ << "..." << std::endl;
        std::cout << prompt_ << "attempting to connect..." << std::endl;
        resolver_.async_resolve(url_, port_,
            [this](auto ec, auto endpoint)
            {
                if (ec)
                {
                    LogError(ec, "tcp_client::connect()");
                    return;
                }
                endpoint_ = std::move(endpoint);
                raise_event("on resolve", ec);
            }
        );
    }
    void listen(boost::system::error_code& ec)
    {
        if (ec == boost::asio::error::operation_aborted)
        {
            // socket connection closed, begin disconnect and end listening loop
            return;
        }
        std::cout << prompt_ << "listening for response from server..." << std::endl;
        websocket_.async_read(readBuffer_,
            [this](auto ec, auto nBytes)
            {
                nBytesRead_ = std::move(nBytes);
                raise_event("on read", ec);
                // recursively listen until disconnection/error
                listen(ec);
            }
        );
    }
    void send_request(const std::string& request, boost::system::error_code& ec)
    {
        if (ec)
        {
            LogError(ec, "tcp_client::send_request(error_code)");
            return;
        }
        std::cout << prompt_ << "sending request..." << std::endl;
        websocket_.async_write(boost::asio::buffer(std::move(request)),
            [this](auto ec, auto) //nBytesTransferred
            {
                raise_event("on send", ec); 
            }
        );
    }
    void send_request(boost::system::error_code& ec)
    {
        if (ec)
        {
            LogError(ec, "tcp_client::send_request(error_code)");
            return;
        }
        std::cout << prompt_ << "sending request..." << std::endl;
        websocket_.async_write(boost::asio::buffer(std::move(ec.message())),
            [this](auto ec, auto) //nBytesTransferred
            {
                raise_event("on send", ec); 
            }
        );
    }
    void read_response(boost::system::error_code& ec)
    {
        // parse respone from server
        std::string response {boost::beast::buffers_to_string(readBuffer_.data())};
        readBuffer_.consume(nBytesRead_);
        std::cout << prompt_ << "server response -> " << response << std::endl;
        raise_event("on response", ec);

    }
    void disconnect()
    {
        std::cout << prompt_ << "disconnecting..." << std::endl;
        websocket_.async_close(boost::beast::websocket::close_code::none,
            [this](auto ec)
            {
                raise_event("on disconnect", ec);
            }
        );
    }
private:
    // event handling
    // --------------
    // resolve event
    class on_resolve : public blotter::events::event_base {
    public:
        on_resolve(tcp_client* client)
            : client_context_(client) {}
        void handler(boost::system::error_code& ec) override
        {
            if (ec)
            {
                client_context_->LogError(ec, "tcp_client::on_resolve");
                // initiate error response
                client_context_->raise_event("on connect", ec);
                return;
            }
            std::cout << client_context_->prompt_ << "resolved " << client_context_->url_ << ":" << client_context_->port_ << std::endl;
            std::cout << client_context_->prompt_ << "connecting..." << std::endl;
            // timeout is only there for connecting to the tcp socket
            // once connected, reset to something boost suggests
			// tcp is lowest layer: websocket > tls > tcp
            boost::beast::get_lowest_layer(client_context_->websocket_).expires_after(std::chrono::seconds(5));

			// connect to tcp socket
            boost::beast::get_lowest_layer(client_context_->websocket_).async_connect(*client_context_->endpoint_,
				[this](auto ec) 
				{
					client_context_->raise_event("on connect", ec);
				}
			); 

        }
    private:
        tcp_client* client_context_;
    };
    // connect event
    class on_connect : public blotter::events::event_base {
    public:
        on_connect(tcp_client* client)
            : client_context_(client) {}
        void handler(boost::system::error_code& ec) override
        {
            if (ec)
            {
                // send error response back to server
                client_context_->LogError(ec, "tcp_client::on_connect");
                client_context_->send_request(ec);
            }
            std::cout << client_context_->prompt_ << "connected to " << client_context_->url_ << ":" << client_context_->port_ << std::endl;
            // set timeouts
            boost::beast::get_lowest_layer(client_context_->websocket_).expires_never();
            client_context_->websocket_.set_option(boost::beast::websocket::stream_base::timeout::suggested(
                boost::beast::role_type::client
            ));
            // websocket connection established, attempt tls handshake
            std::cout << client_context_->prompt_ << "tls handshaking over websocket..." << std::endl;

            client_context_->websocket_.next_layer().async_handshake(boost::asio::ssl::stream_base::client,
                [this](auto ec)
                {
                   client_context_->raise_event("on tls handshake", ec); 
                }
            );
        }
    private:
        tcp_client* client_context_;
    };
    // handshake events
    class on_tls_handshake : public blotter::events::event_base {
    public:
        on_tls_handshake(tcp_client* client)
            : client_context_(client) {}
        void handler(boost::system::error_code& ec)
        {
            if (ec)
            {
                client_context_->LogError(ec, "tcp_client::on_tls_handshake");
                // initiate error request
                client_context_->raise_event("on connect", ec);
            }
            std::cout << client_context_->prompt_ << "handshaking over websocket..." << std::endl;
            client_context_->websocket_.async_handshake(client_context_->url_, "/",
                [this](auto ec)
                {
                    client_context_->raise_event("on tcp handshake", ec);
                }
            );
        }
    private:
        tcp_client* client_context_;
    };
    class on_tcp_handshake : public blotter::events::event_base {
    public:
        on_tcp_handshake(tcp_client* client)
            : client_context_(client) {}
        void handler(boost::system::error_code& ec)
        {
            if (ec)
            {
                client_context_->LogError(ec, "tcp_client::on_tcp_handshake");
                // send error resposne
                client_context_->send_request(ec);
                return;
            } 
            // send requests as text stream
            client_context_->websocket_.text(true);
            // begin async listening loop
            client_context_->listen(ec);
            // dispatch other callbacks
            client_context_->send_request(client_context_->message_, ec);
        }
    private:
        tcp_client* client_context_;
    };
    // on send event
    class on_send : public blotter::events::event_base {
    public:
        on_send(tcp_client* client)
            : client_context_(client) {}
        void handler(boost::system::error_code& ec)
        {
            if (ec)
            {
                client_context_->LogError(ec, "tcp_client::on_send");
                return; 
            }
        }
    private:
        tcp_client* client_context_;
    };
    // read response event
    class on_read : public blotter::events::event_base {
    public:
        on_read(tcp_client* client)
            : client_context_(client) {}
        void handler(boost::system::error_code& ec)
        {
            // ignore failed reads
            if (ec) return;
            client_context_->read_response(ec);
            client_context_->nBytesRead_ = 0;
        }
    private:
        tcp_client* client_context_;
    };
    class on_response : public blotter::events::event_base {
    public:
        on_response(tcp_client* client)
            : client_context_(client) {}
        void handler(boost::system::error_code& ec)
        {
            if (ec)
            {
                client_context_->LogError(ec, "tcp_client::on_response");
                return;
            }
            client_context_->disconnect();
        }
    private:
        tcp_client* client_context_;
    };
    class on_disconnect : public blotter::events::event_base {
    public:
        on_disconnect(tcp_client* client)
            : client_context_(client) {}
        void handler(boost::system::error_code& ec)
        {
            if (ec)
            {
                client_context_->LogError(ec, "tcp_client::on_disconnect");
                return;
            }
            std::cout << client_context_->prompt_ << "disconnected...goodbye!" << std::endl;
        }
    private:
        tcp_client* client_context_;
    };
    std::unordered_map<std::string, blotter::events::event_base*> events_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::resolver::iterator endpoint_;
    boost::beast::websocket::stream<
        boost::beast::ssl_stream<boost::beast::tcp_stream>
    > websocket_;
    boost::beast::flat_buffer readBuffer_;
    std::string prompt_;
    std::string url_;
    std::string port_;
    std::string message_;
    std::size_t nBytesRead_;
};

int main(void)
{
    // init some output stuffs
    auto prompt = std::string{"< ~ client ~ >: "};
    auto url = std::string{"echo.websocket.org"};
    auto port = std::string{"443"};
    auto message = std::string{""};
    // let user decide what to send
    std::cout << prompt << "please type a message to send to " << url << ":" << port << std::endl;
    std::cout << prompt;
    std::getline(std::cin, message);
    // every boost io service needs an execution context
    boost::asio::io_context ioc {};
    boost::asio::ssl::context ssl_ctx {boost::asio::ssl::context::tlsv12_client};
    // load ssl cert
    ssl_ctx.load_verify_file("cacert.pem");
    // init our client (and allocate event handlers)
    tcp_client client(prompt, ioc, ssl_ctx, url, port, message);
    // begin async callback sequence
    client.connect();
    // do work
    ioc.run();

    return 0;
}
