#include "Telegram.h"
#include <spdlog/spdlog.h>

using std::string_literals::operator ""s;

Telegram::Bot::Bot(std::string_view t) : url{"https://api.telegram.org/bot"s + t.data() + "/"} {}

void Telegram::Bot::handleUpdates() {
  simdjson::padded_string json =
    cpr::Get(
      updates,
      cpr::Parameters{timeout, allowedUpdates, {"offset", std::to_string(offset)}}
    ).text;
  try {
    auto document = parser.iterate(json);
    for (auto update: document["result"]) {
      offset = update["update_id"].get_int64() + 1;
      for (const auto &type: expectedTypes) {
        if (handleUpdate(update[type])) {
          break;
        }
      }
    }
  } catch (const simdjson::simdjson_error &e) {
    spdlog::error("{}\n{}", e.what(), json.u8data());
  }
}

bool Telegram::Bot::handleUpdate(simdjson::simdjson_result<simdjson::ondemand::value> update) {
  if (auto container = update.get_object(); !container.error()) {
    pool.push_task([this, envelope = container.value()] {
      handler(*this, envelope);
    });
    //handler(*this, container.value());
    return true;
  }
  return false;
}

Telegram::Bot &Telegram::Bot::onAction(Bot::ActionHandler h) {
  actionHandler = move(h);
  return *this;
}

Telegram::Bot &Telegram::Bot::onInteraction(Bot::InteractionHandler h) {
  interactionHandler = move(h);
  return *this;
}

[[noreturn]] void Telegram::Bot::run() {
  for (;; handleUpdates());
}
