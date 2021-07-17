#pragma once

#include <functional>
#include <string_view>
#include <cpr/cpr.h>
#include <simdjson.h>
#include <ThreadPool/ThreadPool.h>
#include <variant>

namespace Telegram {

  class Bot;

  using ChatId = std::variant<int64_t, std::string>;

  struct Action {
    ChatId chatId;
    int64_t senderId;
    int64_t messageId;
    std::string messageText;

    Action(Action &&) = default;
  };

  using ActionHandler = std::function<void(const Bot &, Action)>;

  struct Interaction {
    ChatId chatId;
    int64_t senderId;
    int64_t recipientId;
    int64_t senderMessageId;
    int64_t recipientMessageId;
    std::string senderMessageText;

    Interaction(Interaction &&) = default;
  };

  using InteractionHandler = std::function<void(const Bot &, Interaction)>;


  class Bot {
    const cpr::Url url;
    ActionHandler actionHandler;
    InteractionHandler interactionHandler;
    simdjson::ondemand::parser parser;
    const cpr::Url updates{url + "getUpdates"};
    const cpr::Parameter timeout{"timeout", "10"};
    const cpr::Parameter allowedUpdates{"allowed_updates", R"(["message","channel_post"])"};
    const std::string expectedTypes[2]{"message", "channel_post"};
    int64_t offset{};
    ThreadPool pool{4};

    bool handleUpdate(simdjson::simdjson_result<simdjson::ondemand::value>);

    void handleUpdates();

  public:

    explicit Bot(std::string_view);

    // todo common api methods: mute unmute pin unpin sendfile sendtext deletemessage

    Bot &onAction(ActionHandler);

    Bot &onInteraction(InteractionHandler);

    [[noreturn]] void run();
  };
}