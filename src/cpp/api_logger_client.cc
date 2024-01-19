#include "api_logger_client.h"


using grpc::ClientContext;
using grpc::Status;
using grpc::experimental::FileWatcherCertificateProvider;


using apilogger::ApiLogger;
using apilogger::BatchLog;
using apilogger::SingleLog;
using apilogger::Empty;


#define DEBUG false

std::vector<std::string> ApiLoggerClient::batch_log_;


ApiLoggerClient::ApiLoggerClient(std::string server_address, 
    bool use_tls,
    const std::string client_key,
    const std::string client_cert,
    const std::string CA_cert, 
    const uint16_t batch_size)
{ 

  std::shared_ptr<grpc::ChannelCredentials> creds;

  if (use_tls)
  { 

    auto tls_certificate_provider = std::make_shared<FileWatcherCertificateProvider>(
        client_key, client_cert, CA_cert, 1);
    grpc::experimental::TlsChannelCredentialsOptions options;
    options.set_certificate_provider(tls_certificate_provider);
    options.watch_root_certs();
    options.set_root_cert_name("root_cert_name");
    options.watch_identity_key_cert_pairs();
    options.set_identity_cert_name("identity_cert_name");

    creds = grpc::experimental::TlsCredentials(options);

  } 
  else
  { 
    creds = grpc::InsecureChannelCredentials();
  }
  
  stub_ = ApiLogger::NewStub(grpc::CreateChannel(server_address, creds));
  batch_log_ = { };
  batch_size_ = batch_size;
}

int ApiLoggerClient::SendLog(std::string logs, ApiLoggerClient::log_priority priority){ 
   
  if (priority == ApiLoggerClient::log_priority::HIGH)
  { 
    //send immediately 
    Empty response;
    SingleLog l;
    l.set_log_data(logs);

    ClientContext ctx;  
    Status status = stub_->sendSingleLog(&ctx, l, &response); 
    
    if (status.ok())
      return 0;
    else
      return -1;
  }
  
  batch_log_.push_back(logs);
  
  if (batch_log_.size() < batch_size_)
    return 0;
  

  Empty response;
  BatchLog b;
  for (auto log: batch_log_)
    b.add_log_batch(log);
  
  ClientContext ctx;
  Status status = stub_->sendBatchLog(&ctx, b, &response);
  
  batch_log_.clear();
  
  if(status.ok())
    return 0;
  else
    return -1;
}

