#include <boost/asio.hpp>

#include "session.hpp"
#include "publisher.hpp"
#include "database.h"

using boost::asio::ip::tcp;

class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    , db(std::make_shared<my_db::Database>()) {
        
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket), db)->start();
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor_;
    std::shared_ptr<my_db::Database> db;
};
