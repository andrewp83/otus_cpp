#include <boost/asio.hpp>

#include "database.h"

using boost::asio::ip::tcp;

class Session
  : public std::enable_shared_from_this<Session> {
      
public:
    Session(tcp::socket socket, std::shared_ptr<my_db::Database> db)
      : socket_(std::move(socket)), db(db) {
          
         // executor = std::make_shared<bulk::Executor>(bulk_size);
    }

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length), [this, self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                //std::cout << "receive " << length << "=" << std::string{data_, length} << bulk_size << std::endl;
                //do_write(length);
                std::string str = std::string(data_, length);
                //executor->parse_buffer(str);
                
                my_db::QueryResult result = db->query(str);
                std::string answer;
                if (result.is_ok()) {
                    answer = serialize_query_result(result);
                    answer += "OK";
                } else {
                    answer += "ERR ";
                    answer += result.get_error_message();
                }
                answer += "\n";
                
                do_write(answer);
                //do_read();
            }
        });
    }

    void do_write(const std::string& answer) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(answer.c_str(), answer.size()), [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                do_read();
            }
        });
    }
      
    std::string serialize_query_result(const my_db::QueryResult& result) const {
        std::string serialized;
        for (const std::string& row : result.get_rows()) {
            serialized += row;
            serialized += "\n";
        }
        return serialized;
    }

    tcp::socket socket_;
    std::shared_ptr<my_db::Database> db;
    enum { max_length = 1024 };
    char data_[max_length];
};
