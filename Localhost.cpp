#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "httplib.h"

std::string last_message = "No messages yet";
std::mutex msg_mutex;
std::vector<std::string> message_history;
int totalMessages = 0;
std::string username = "admin";
std::string escape_json(const std::string& input) {
    std::string out;
    out.reserve(input.size());
    for (char c : input) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c; break;
        }
    }
    return out;
}

void SendToWebsite(const std::string& data) {
    httplib::Client cli("127.0.0.1", 8080);
    auto res = cli.Post("/send", username + ": " + data, "text/plain");

    if (!res) {
        std::cout << "Failed to send data: no response (connection failed)\n";
        return;
    }

    std::cout << "POST status: " << res->status << "\n";
    if (res->status == 200) {
        std::cout << "Data sent successfully!\n";
    } else {
        std::cout << "Server replied with non-200 status\n";
    }
}

int main() {
    std::string msg;
    httplib::Server svr;

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        std::string current;
        int count;
        {
            std::lock_guard<std::mutex> lock(msg_mutex);
            current = last_message;
            count = totalMessages;
        }

        std::string html =
            "<html><body style='font-family:arial'>"
            "<h1>Latest Message</h1>"
            "<h2>Total messages (<span id='count'>" + std::to_string(count) + "</span>)</h2>"
            "<p id='msg'>" + current + "</p>"
            "<p id='history'>Message History:</p>"
            "<script>"
            "setInterval(async () => {"
            " try {"
            "  const r = await fetch('/state?ts=' + Date.now(), { cache: 'no-store' });"
            "  const s = await r.json();"
            "  document.getElementById('msg').textContent = s.message;"
            "  document.getElementById('history').textContent = 'Message History: ' + s.history;"
            "  document.getElementById('count').textContent = s.count;"
            " } catch (e) {"
            "  console.error('state refresh failed', e);"
            " }"
            "}, 1000);"
            "</script>"
            "</body></html>";

        res.set_content(html, "text/html");
        res.set_header("Cache-Control", "no-store");
        res.status = 200;
    });

    svr.Get("/state", [](const httplib::Request&, httplib::Response& res) {
        std::string current;
        std::string history;
        int count;
        {
            std::lock_guard<std::mutex> lock(msg_mutex);
            current = last_message;
            count = totalMessages;
            for (const auto& msg_item : message_history) {
                if (!history.empty()) {
                    history += " | ";
                }
                history += msg_item;
            }
        }

        std::string json =
            "{\"message\":\"" + escape_json(current) +
            "\",\"history\":\"" + escape_json(history) +
            "\",\"count\":" + std::to_string(count) + "}";

        res.set_content(json, "application/json");
        res.set_header("Cache-Control", "no-store");
        res.status = 200;
    });

    svr.Post("/send", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "Received data: " << req.body << "\n";
        {
            std::lock_guard<std::mutex> lock(msg_mutex);
            last_message = req.body;
            message_history.push_back(req.body);
            totalMessages++;
        }

        res.set_content("Data received!", "text/plain");
        res.status = 200;
    });

    std::thread server_thread([&svr]() {
        svr.listen("127.0.0.1", 8080);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
std::cout << "Please enter your username:";
std::cin >> username;
    while (true) {
        std::cout << "Enter message (or exit): ";
        if (!std::getline(std::cin, msg)) {
            break;
        }
        if (msg == "exit") {
            break;
        }

        SendToWebsite(msg);
    }

    svr.stop();
    server_thread.join();
    return 0;
}
