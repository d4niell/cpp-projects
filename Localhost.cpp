#include <iostream>
#include "httplib.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

class Client{
    public:
    std::string last_message = "No messages yet";
    std::mutex msg_mutex;
    std::vector<std::string> message_history;
    int totalMessages = 0;

};

void SendToWebsite(const std::string& data) {
    Client client;
    httplib::Client cli("127.0.0.1", 8080); // host + port avoids URL parsing issues
    auto res = cli.Post("/send", data, "text/plain");

    if (!res) {
        std::cout << "Failed to send data: no response (connection failed)\n";
        return;
    }

    std::cout << "POST status: " << res->status << "\n";
    if (res->status == 200) {
        client.totalMessages++;
        std::cout << "Data sent successfully!\n";
    } else {
        std::cout << "Server replied with non-200 status\n";
    }
}

int main() {
    Client client;
    std::string msg;
    httplib::Server svr;
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
            Client client;
            std::string current;
        {
            std::lock_guard<std::mutex> lock(client.msg_mutex);
            current = client.last_message;
        }


                        std::string html =
                            "<html><body style='font-family:arial'>"
                            "<h1>Latest Message</h1>"
                            "<h2>Total messages (<span id='count'>" + std::to_string(client.totalMessages) + "</span>)</h2>"
                            "<p id='msg'>" + current + "</p>"
                            "<p id='history'>Message History:</p>"
                            "<script>"
                            "setInterval(async () => {"
                            " const r = await fetch('/message');"
                            " const h = await fetch('/history');"
                            " const hi = await h.text();"
                            " const t = await r.text();"
                            " document.getElementById('msg').textContent = t;"
                            " document.getElementById('history').textContent = hi;"
                            " const c = await fetch('/count');"
                            " const n = await c.text();"
                            " document.getElementById('count').textContent = n;"
                            "}, 1000);"
                            "</script>"
                            "</body></html>";
        res.set_content(html, "text/html");
        res.status = 200;
    });
    svr.Get("/count", [](const httplib::Request&, httplib::Response& res) {
        Client client;
        res.set_content(std::to_string(client.totalMessages), "text/plain");
        res.status = 200;
    });
    svr.Get("/history", [](const httplib::Request&, httplib::Response& res) {
        Client client;
        std::string history;
        {
            std::lock_guard<std::mutex> lock(client.msg_mutex);
            for (const auto& msg : client.message_history) {
                history += msg + " | ";
            }
        }
        res.set_content(history, "text/html");
        res.status = 200;
    });
    svr.Get("/message", [](const httplib::Request&, httplib::Response& res) {
        Client client;
        std::string current;
        {
            std::lock_guard<std::mutex> lock(client.msg_mutex);
            current = client.last_message;
        }
        res.set_content(current, "text/plain");
        res.status = 200;
    });
    svr.Post("/send", [](const httplib::Request& req, httplib::Response& res) {
        Client client;
        std::cout << "Received data: " << req.body << "\n";
        {
        std::lock_guard<std::mutex> lock(client.msg_mutex);
        client.last_message = req.body;
        client.message_history.push_back(req.body);
        }

        res.set_content("Data received!", "text/plain");
        res.status = 200;
});

    std::thread server_thread([&svr]() {
        svr.listen("127.0.0.1", 8080); // use same address family as client
});

    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // let server start

    while (true) {
        std::cout << "Enter message (or exit): ";
        std::getline(std::cin, msg);
        if (msg == "exit") break;

        SendToWebsite(msg);
    }

    svr.stop();
    server_thread.join();
    return 0;
}