#include <tgbot/tgbot.h>
#include <fmt/core.h>
#include <fmt/color.h>

int main() {
  std::unordered_map<int32_t, std::string> map;
  TgBot::Bot bot(getenv("TOKEN"));
  bot.getEvents().onAnyMessage([&](auto &&m) {
    if (!m->from->id) {
      return;
    }
    if (map.find(m->from->id) != map.cend()) {
      bot.getApi().sendMessage(m->chat->id, map[m->from->id], false, m->messageId);
      fmt::print(fg(fmt::color::dark_olive_green), "user {} requested file {}\n", m->from->id, map[m->from->id]);
    }
    if (m->document) {
      map[m->from->id] = m->document->fileId;
      fmt::print(fg(fmt::color::dark_blue), "user {} saved file {}", m->from->id, m->document->fileId);
    }
  });
  for (TgBot::TgLongPoll p(bot);; p.start());
}