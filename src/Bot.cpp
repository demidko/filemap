#include "Bot.h"
#include <spdlog/spdlog.h>

using std::string_literals::operator ""s;

Telegram::Bot::Bot(std::string_view t, Handler h) :
  url{"https://api.telegram.org/bot"s + t.data() + "/"},
  handler{move(h)} {}

void Telegram::Bot::handleUpdates() {
  simdjson::padded_string json =
    cpr::Get(
      updates,
      cpr::Parameters{timeout, types, {"offset", std::to_string(offset)}}
    ).text;
  try {
    auto document = parser.iterate(json);
    for (auto update: document["result"]) {
      offset = update["update_id"].get_int64() + 1;
      if (auto m = update["message"]; !m.error()) {
        Bot &b = *this;

        pool.push_task(handler, b, m);
        pool.enqueue(handler, *this, m.get_object().value());
        handler(*this, m);
      } else if (auto p = update["channel_post"]; !p.error()) {
        handler(*this, p);
      }
    }
  } catch (const std::runtime_error &e) {
    spdlog::error("{}\n{}", e.what(), json.u8data());
  }
}

[[noreturn]] void Telegram::Bot::polling(std::string_view t, Handler h) {
  for (Bot bot{t, move(h)};; bot.handleUpdates());
}
