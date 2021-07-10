#include <tgbot/tgbot.h>

int main() {
  std::unordered_map<int32_t, std::string> map;
  TgBot::Bot bot(getenv("TOKEN"));
  bot.getEvents().onAnyMessage([&](auto &&m) {
    if (!m->from->id) {
      return;
    }
    if (map.find(m->from->id) != map.cend()) {
      bot.getApi().sendMessage(m->chat->id, map[m->from->id], false, m->messageId);
    }
    if (m->document) {
      map[m->from->id] = m->document->fileId;
    }
  });
  for (TgBot::TgLongPoll p(bot);; p.start());
}