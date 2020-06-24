#pragma once

#include <gtest/gtest.h>

#include "command_publisher.hpp"

class Test2Observer : public CommandObserver {
public:
    void bulk_executed(const BulkResult& res) override {
        messages.push(res);
    }
    
    std::string message() {
        std::string msg = messages.front();
        messages.pop();
        return msg;
    }
    
protected:
    std::queue<std::string> messages;
};

class TestPublisher : public CommandPublisher {
public:
    void send_notify(const std::string& msg) {
        notify(&CommandObserver::bulk_executed, msg);
    }
};

TEST(Publisher, Notification) {

    Test2Observer* observer1 = new Test2Observer();
    Test2Observer* observer2 = new Test2Observer();
    
    std::shared_ptr<TestPublisher> publisher = std::make_shared<TestPublisher>();
    observer1->subscribe(publisher);
    observer2->subscribe(publisher);
    
    publisher->send_notify("hello1");

    ASSERT_EQ(observer1->message(), "hello1");
    ASSERT_EQ(observer2->message(), "hello1");
}

TEST(Publisher, ObserversDeleted) {

    Test2Observer* observer1 = new Test2Observer();
    Test2Observer* observer2 = new Test2Observer();
    
    std::shared_ptr<TestPublisher> publisher = std::make_shared<TestPublisher>();
    observer1->subscribe(publisher);
    observer2->subscribe(publisher);

    ASSERT_EQ(publisher->observers_count(), 2U);
    
    // ТУТ ИДИОМУ ААА ПОХОЖЕ КРАСИВО НЕ СДЕЛАТЬ
    
    delete observer1;
    ASSERT_EQ(publisher->observers_count(), 1U);
    
    publisher->send_notify("!!!");
    ASSERT_EQ(observer2->message(), "!!!");
    
    delete observer2;
    
    ASSERT_EQ(publisher->observers_count(), 0U);
    
    // НЕ ЗНАЮ КАК ПРОВЕРИТЬ ЧТО МЫ НЕ УПАЛИ
    publisher->send_notify("####");
}
