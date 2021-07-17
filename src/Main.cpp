#include "Telegram.h"
#include <spdlog/spdlog.h>

int main() {
  using Bot::Bot;
  using simdjson::ondemand::object;

  Bot::polling(getenv("TOKEN"), [](Bot const &bot, object message) {
    try {
      auto envelope = message["text"].get_string();
      if (envelope.error()) {
        return;
      }
      auto text = envelope.value();
      if (text.empty()) {
        return;
      }
      spdlog::info("text message:\n{}", text);
    } catch (const simdjson::simdjson_error &e) {
      spdlog::error(e.what());
    }
  });
}