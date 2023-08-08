#include "quill/Quill.h"

int main() {
  std::shared_ptr<quill::Handler> handler =
      quill::stdout_handler(); /** for stdout **/
  // std::shared_ptr<quill::Handler> handler =
  // quill::file_handler("quickstart.log", "w");  /** for writing to file **/
  handler->set_pattern(
      "%(ascii_time) [%(thread)] %(fileline:<28) LOG_%(level_name) %(message)");

  // set configuration
  quill::Config cfg;
  cfg.default_handlers.push_back(handler);

  // to simulate message dropping easily we can to set a high sleep duration to
  // the logging thread
  cfg.backend_thread_sleep_duration = std::chrono::seconds{1};

  // and a small queue size
  cfg.default_queue_capacity = 4'096;

  // Apply configuration and start the backend worker thread
  quill::configure(cfg);
  quill::start();

  std::thread thread([]() {
    for (size_t i = 0; i < 1000; ++i) {
      LOG_INFO(quill::get_logger(), "Hello {} #{}", "world", i);
    }
  });

  thread.join();
  quill::flush();
  return 0;
}
