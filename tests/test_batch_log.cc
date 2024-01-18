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
      2);

  std::string sampleLogs = "17/Jan/24 {12:10:18.263} [TR:1769445] [I] : [192.168.1.2] \"POST getEncryptionKey\" 200 {12:10:18.253} [ : ] {12:10:18.263}\n" 
    "17/Jan/24 {12:12:41.339} [TR:1769446] [I] : [192.168.1.2] \"POST getAuthenticationToken\" 200 {12:12:41.336} [ : ] {12:12:41.339}\n" 
    "17/Jan/24 {12:13:04.373} [TR:1769447] [I] : [192.168.1.2] \"GET getLicenseKey\" 500 {12:13:04.369} [ : ] {12:13:04.373}\n" 
    "17/Jan/24 {12:13:42.593} [TR:1769448] [I] : [192.168.1.2] \"GET getLicenseKey\" 500 {12:13:42.589} [ : ] {12:13:42.593}\n" 
    "17/Jan/24 {12:14:06.959} [TR:1769449] [I] : [192.168.1.2] \"POST getAuthenticationToken\" 200 {12:14:06.954} [ : ] {12:14:06.958}\n" 
    "17/Jan/24 {12:14:29.498} [TR:1769450] [I] : [192.168.1.2] \"GET getLicenseKey\" 500 {12:14:29.493} [ : ] {12:14:29.498}\n" 
    "17/Jan/24 {12:14:39.506} [TR:1769451] [I] : [192.168.1.2] \"POST getEncryptionKey\" 200 {12:14:39.501} [ : ] {12:14:39.506}\n" 
    "17/Jan/24 {12:14:59.520} [TR:1769452] [I] : [192.168.1.2] \"GET getLicenseKey\" 500 {12:14:59.517} [ : ] {12:14:59.520}\n" 
    "17/Jan/24 {12:15:19.558} [TR:1769453] [I] : [192.168.1.2] \"POST getEncryptionKey\" 200 {12:15:19.552} [ : ] {12:15:19.558}\n";

  size_t pos = 0;
  std::string token;
  while ((pos = sampleLogs.find('\n')) != std::string::npos) {
    token = sampleLogs.substr(0, pos);
    //std::cout << token << std::endl;
    client.SendLog(token, ApiLoggerClient::log_priority::LOW);
    sampleLogs.erase(0, pos + 1);
  }

}
