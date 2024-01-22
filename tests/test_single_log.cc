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
  
  std::string json = "{"
  "\"clientIP\": \"exampleClientIP\", "
  "\"timestamp\": 1643068800000, "
  "\"httpMethod\": \"GET\", "
  "\"requestUrl\": \"exampleRequestUrl\","
  "\"traceId\": \"exampleTraceId\","
  "\"responseTime\": 1000,"
  "\"clientId\": \"exampleClientId\","
  "\"productId\": \"exampleProductId\","
  "\"deviceInfo\": \"exampleDeviceInfo\","
  "\"responseCode\": 200"
"}";

  client.SendLog(json, ApiLoggerClient::log_priority::HIGH);
}
