#include "Telegram.h"
#include <spdlog/spdlog.h>
#include <mutex>
#include <unordered_map>

using Database = std::unordered_map<int64_t, std::unordered_map<std::string, std::string>>;

Telegram::FileHandler saveFileTo(Database &db, std::mutex &m) {
  return [&](const auto &bot, const auto &file) {
    spdlog::info("chat id {} saved file id {} with key {}", file.chatId, file.fileId, file.key);
    std::lock_guard guard{m};
    db[file.chatId][file.key] = file.fileId;
  };
}

Telegram::RequestHandler readFileFrom(Database &db) {
  return [&](const auto &bot, const auto &req) {
    spdlog::info("chat id {} requested file with key {}", req.chatId, req.key);
    if (auto file = db[req.chatId][req.key]; !file.empty()) {
      bot.reply(req.chatId, req.messageId, file);
      spdlog::info("chat id {} found file id {} by key {}", req.chatId, file, req.key);
    }
  };
}

int main() {
  Database db;
  std::mutex mutex;
  Telegram::Bot(getenv("TOKEN"))
    .onFile(saveFileTo(db, mutex))
    .onRequest(readFileFrom(db))
    .run();
}