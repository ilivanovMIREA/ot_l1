#include <iostream>
#include <spdlog/spdlog.h>
#include "version.h"

int main(){
  auto logger = spdlog::stdout_logger_mt("console");
  logger->info("version {} was 1 started", version());

  return 0;
}
