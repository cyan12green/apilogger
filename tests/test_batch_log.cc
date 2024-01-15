#include <string>
#include <yaml-cpp/yaml.h>

#include "../src/cpp/api_logger_client.h"
const std::string kClientCert = "../../certs/client.pem";
const std::string kClientKey = "../../certs/client.key";
const std::string kCACert = "../../certs/ca.pem";

int main(int argc, char** argv) 
{ 
  YAML::Node config = YAML::LoadFile("../../config.yaml");
  ApiLoggerClient client (
      config["client_add"].as<std::string>(),
      config["use_tls"].as<bool>(),
      kClientKey,
      kClientCert,
      kCACert, 
      10);
  for (int i=1; i<9; i++ )
    std::cout << client.SendLog(std::to_string(i) + std::string("batch"), ApiLoggerClient::log_priority::LOW);
}
