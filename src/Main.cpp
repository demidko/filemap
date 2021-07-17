#include "Bot.h"
#include <spdlog/spdlog.h>

int main() {
  using Telegram::Bot;
  using simdjson::ondemand::object;

  Bot::polling(getenv("TOKEN"), [](Bot &bot, simdjson::ondemand::object message) {
    auto envelope = message["text"].get_string();
    if (envelope.error()) {
      return;
    }
    auto text = envelope.value();
    if (text.empty()) {
      return;
    }
    spdlog::info("text message:\n{}", text);
  });
}