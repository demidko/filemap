#include <tgbot/tgbot.h>
#include <spdlog/spdlog.h>

int main() {
  std::unordered_map<int32_t, std::unordered_map<std::string, std::string>> map;
  TgBot::Bot bot(getenv("TOKEN"));
  bot.getEvents().onAnyMessage([&](const TgBot::Message::Ptr &m) {
    if (m->text.empty() || !m->from->id) {
      return;
    }
    if (m->document) {
      map[m->from->id][m->text] = m->document->fileId;
      spdlog::info("{} saved {} as {}", m->from->id, m->document->fileId, m->text);
      return;
    }
    if (map.find(m->from->id) != map.cend()) {
      auto value = map[m->from->id][m->text];
      bot.getApi().sendMessage(m->chat->id, value, false, m->messageId);
      spdlog::info("{} requested {} by {}", m->from->id, value, m->text);
    }
  });
  for (TgBot::TgLongPoll p(bot);; p.start());
}