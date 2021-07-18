#pragma once

#include <functional>
#include <string_view>
#include <cpr/cpr.h>
#include <simdjson.h>
#include <ThreadPool/ThreadPool.h>
#include <variant>

namespace Telegram {

  class Bot;

  struct File {
    int64_t chatId;
    std::string key;
    std::string fileId;
  };

  using FileHandler = std::function<void(const Bot &, const File &)>;

  struct Request {
    int64_t chatId;
    int64_t messageId;
    std::string key;
  };

  using RequestHandler = std::function<void(const Bot &, const Request &)>;

  class Bot {
    const cpr::Url url;
    FileHandler fileHandler;
    RequestHandler requestHandler;
    simdjson::ondemand::parser parser;
    const cpr::Url getUpdates = url + "getUpdates";
    const cpr::Url sendMessage = url + "sendMessage";
    const cpr::Parameter timeout{"timeout", "10"};
    const cpr::Parameter allowedUpdates{"allowed_updates", R"(["message","channel_post"])"};
    const std::string messageTypes[2]{"message", "channel_post"};
    int64_t offset{};
    ThreadPool pool{4};
    cpr::VerifySsl ssl{false};

    bool handleMessage(simdjson::simdjson_result<simdjson::ondemand::value> container);

    void handleUpdates();

  public:

    explicit Bot(std::string_view);

    Bot &onFile(FileHandler);

    Bot &onRequest(RequestHandler);

    [[noreturn]] void run();

    void reply(int64_t chat, int64_t originalMessage, std::string_view reply) const;
  };
}