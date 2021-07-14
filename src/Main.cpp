#include <tgbot/tgbot.h>
#include <spdlog/spdlog.h>

int main() {
  std::unordered_map<int32_t, std::unordered_map<std::string, std::string>> map;
  TgBot::Bot bot(getenv("TOKEN"));
  bot.getEvents().onAnyMessage([&](const TgBot::Message::Ptr &m) {
    spdlog::warn("{}: {}", m->text);
    if (m->text.empty() || !m->from->id) {
      return;
    }
    if (m->document) {
      map[m->from->id][m->text] = m->document->fileId;
      spdlog::info("{} saved {} as {}", m->from->id, m->document->fileId, m->text);
      return;
    }
    if (auto db = map[m->from->id]; db.find(m->text) != db.cend()) {
      auto val = db[m->text];
      bot.getApi().sendMessage(m->chat->id, val, false, m->messageId);
      spdlog::info("{} requested {} by {}", m->from->id, val, m->text);
    }
  });
  for (TgBot::TgLongPoll p(bot);; p.start());
}