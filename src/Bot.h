#pragma once

#include <functional>
#include <string_view>
#include <cpr/cpr.h>
#include <simdjson.h>
#include "ThreadPool.hpp"

namespace Telegram {

  class Bot;

  using Handler = std::function<void(Bot &, simdjson::ondemand::object)>;

  class Bot {
    cpr::Url const url;
    Handler handler;
    simdjson::ondemand::parser parser;
    cpr::Url const updates{url + "getUpdates"};
    cpr::Parameter const timeout{"timeout", "10"};
    cpr::Parameter const types{"allowed_updates", R"(["message","channel_post"])"};
    int64_t offset{};
    thread_pool pool{4};

    void handleUpdates();

    Bot(std::string_view, Handler);

  public:

    [[noreturn]] static void polling(std::string_view, Handler);
  };
}