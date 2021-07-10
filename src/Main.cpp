#include <tgbot/tgbot.h>

int main() {
  std::unordered_map<int32_t, std::string> storage;
  TgBot::Bot bot(getenv("TOKEN"));
  bot.getEvents().onAnyMessage([&](auto &&m) {
    if (!m->from->id) {
      return;
    }
    if (storage.contains(m->from->id)) {
      bot.getApi().sendMessage(m->chat->id, storage[m->from->id], false, m->messageId);
    }
    if (m->document) {
      storage[m->from->id] = m->document->fileId;
    }
  });
  for (TgBot::TgLongPoll p(bot);; p.start());
}