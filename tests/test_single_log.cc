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
      50);
  
  client.SendLog("17/Jan/24 {12:15:19.558} [TR:1769453] [I] : [192.168.1.2] \"POST getEncryptionKey\" 200 {12:15:19.552} [ : ] {12:15:19.558}", ApiLoggerClient::log_priority::HIGH);
}
