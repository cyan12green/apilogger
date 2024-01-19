#ifndef __API_LOGGER_CLIENT_H
#define __API_LOGGER_CLIENT_H

#include <vector>

#include <grpc/grpc.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/security/tls_credentials_options.h>
#include <grpcpp/security/tls_certificate_provider.h>



#include "../../cmake/build/api_logger.grpc.pb.h"
#include "../../cmake/build/api_logger.pb.h"


using apilogger::ApiLogger;
using apilogger::BatchLog;
using apilogger::SingleLog;

class ApiLoggerClient { 
  public:
    ApiLoggerClient(const std::string server_address, 
                    const bool use_tls, 
                    const std::string client_key, 
                    const std::string client_cert, 
                    const std::string CA_cert, 
                    const uint16_t batch_size);

    enum class log_priority
    { 
      HIGH, 
      LOW
    };

    int SendLog(const std::string logs, 
                const log_priority priority);

  private:
    std::unique_ptr<ApiLogger::Stub> stub_;
    static std::vector<std::string> batch_log_;
    uint16_t batch_size_;
};



#endif //__API_LOGGER_CLIENT_H
