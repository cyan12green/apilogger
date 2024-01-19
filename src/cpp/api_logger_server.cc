#include <cppkafka/configuration.h>
#include <cppkafka/message_builder.h>
#include <cppkafka/producer.h>
#include <thread>

#include <grpcpp/completion_queue.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/support/status.h>
#include <grpc/grpc.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/server.h>
#include <grpcpp/server_context.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/security/tls_certificate_provider.h>
#include <grpcpp/security/tls_credentials_options.h>

#include <yaml-cpp/node/parse.h>
#include "yaml-cpp/yaml.h"

#include <cppkafka/cppkafka.h>

#include "../../cmake/build/api_logger.grpc.pb.h"
#include "../../cmake/build/api_logger.pb.h"



using grpc::Server;
using grpc::ServerBuilder;
using grpc::experimental::FileWatcherCertificateProvider;


using apilogger::ApiLogger;
using apilogger::SingleLog;
using apilogger::BatchLog;
using apilogger::Empty;


struct CallData { 
  ApiLogger::AsyncService* service;
  grpc::ServerCompletionQueue* cq;
  cppkafka::Producer *producer;
  cppkafka::MessageBuilder *kafka_msg_builder;
};


//base class to cast the void* tags we get from the completion queue
//and call Proceed() on them
class Call {
  public:
    virtual void Proceed(bool ok) = 0;
};


class AsyncServer final { 
  public:
    //RPCs
    //send single log
    class SendSingleLogCall final : public Call { 
      public:
        explicit SendSingleLogCall(CallData *data)
          : data_(data), responder_(&ctx_), status_(REQUEST) { 
            data->service->RequestsendSingleLog(&ctx_, &request_, &responder_, data_->cq,
                data_->cq, this);
          } 
        void Proceed(bool ok) 
        { 
          std::string input;
          switch(status_)
          { 
            case REQUEST: 
              new SendSingleLogCall(data_);
              if (!ok) {
                std::cerr << "SendSingleLogCall in REQUEST was not ok. Finishing." << std::endl;
                responder_.FinishWithError(grpc::Status::CANCELLED, this);
                status_ = FINISH;
                break;
              } 
              else 
              { 
                responder_.Finish(reply_, grpc::Status::OK, this); 
                status_ = PROCESS;
                break;
              }

            case PROCESS:
              input = "HIGH " + request_.log_data(); 
              data_->kafka_msg_builder->payload( input );
              data_->producer->produce(*(data_->kafka_msg_builder));
              data_->producer->flush();

              status_ = FINISH;
              break;

            case FINISH:
              if (!ok)
                std::cerr << "SendSingleLog RPC finished unexpectedly" << std::endl;
              delete this;
              break;
          }
        }
      private:
        CallData *data_;
        grpc::ServerContext ctx_;
        grpc::ServerAsyncResponseWriter<Empty> responder_;
        SingleLog request_;
        Empty reply_;
        enum CallStatus { REQUEST, PROCESS, FINISH };
        CallStatus status_;
    };
  
    class SendBatchLogCall final : public Call { 
      public:
        explicit SendBatchLogCall(CallData *data)
          : data_(data), responder_(&ctx_), status_(REQUEST) { 
            data->service->RequestsendBatchLog(&ctx_, &request_, &responder_, data_->cq,
                data_->cq, this);
          } 
        void Proceed(bool ok) 
        { 
          switch(status_)
          { 
            case REQUEST: 
              new SendBatchLogCall(data_);
              if (!ok) {
                std::cerr << "SendBatchLogCall in REQUEST was not ok. Finishing." << std::endl;
                responder_.FinishWithError(grpc::Status::CANCELLED, this);
                status_ = FINISH;
                break;
              } 
              else 
              { 
                responder_.Finish(reply_, grpc::Status::OK, this); 
                status_ = PROCESS;
                break;
              }

            case PROCESS:
              for (std::string logs: request_.log_batch()){ 
                std::string input = "LOW " + logs;
                data_->kafka_msg_builder->payload( input );
                data_->producer->produce(*(data_->kafka_msg_builder));
              }
              data_->producer->flush();

              status_ = FINISH;
              break;

            case FINISH:
              if (!ok)
                std::cerr << "SendBatchLog RPC finished unexpectedly" << std::endl;
              delete this;
              break;
          }
        }
      private:
        CallData *data_;
        grpc::ServerContext ctx_;
        grpc::ServerAsyncResponseWriter<Empty> responder_;
        BatchLog request_;
        Empty reply_;
        enum CallStatus { REQUEST, PROCESS, FINISH };
        CallStatus status_;
    };
    
    ~AsyncServer()
    { 
      server_->Shutdown();
      for (const auto& cq: compl_queues_)
        cq->Shutdown();
    }
    
    void Init(std::string path_to_config) 
    {
      YAML::Node config = YAML::LoadFile(path_to_config);

      std::string server_address = config["server_address"].as<std::string>();
      

      //kafka
      std::string kafka_broker_url = config["kafka_broker_url"].as<std::string>();
      std::string kafka_topic_name = config["kafka_topic_name"].as<std::string>();
      cppkafka::Configuration config_kafka = { 
        { "metadata.broker.list", kafka_broker_url }
      };
      
      kafka_msg_builder = new cppkafka::MessageBuilder(kafka_topic_name);  
      producer = new cppkafka::Producer(config_kafka);


      ServerBuilder builder;
      std::shared_ptr<grpc::ServerCredentials> creds;

      if ( config["use_tls"].as<bool>() ) 
      {
        //tls support
        std::cerr << "Using TLS\n";

        auto tls_certificate_provider = 
          std::make_shared<FileWatcherCertificateProvider>(
              config["certificate_path"]["ServerKey"].as<std::string>(), 
              config["certificate_path"]["ServerCert"].as<std::string>(), 
              config["certificate_path"]["CACert"].as<std::string>(),  
              1
              );

        grpc::experimental::TlsServerCredentialsOptions options(tls_certificate_provider);
        options.watch_root_certs();
        options.set_root_cert_name("root_cert_name");
        options.watch_identity_key_cert_pairs();
        options.set_identity_cert_name("identity_cert_name");
        options.set_cert_request_type(
            GRPC_SSL_REQUEST_AND_REQUIRE_CLIENT_CERTIFICATE_AND_VERIFY);
        creds = grpc::experimental::TlsServerCredentials(options);
      } 
      else 
      { 
        //insecure channel
        std::cerr << "Using insecure connection\n";
        creds = grpc::InsecureServerCredentials();
      } 

      builder.AddListeningPort(server_address, creds);
      builder.RegisterService(&service_);

      uint16_t num_of_threads = config["number_of_threads"].as<uint16_t>();
      for(auto i{0}; i<num_of_threads; i++)
        compl_queues_.emplace_back(builder.AddCompletionQueue());

      server_ = builder.BuildAndStart();
      
      std::cerr << "Server listening on " << server_address << std::endl;
      std::cerr << "Server using " << num_of_threads << " threads" << std::endl;

      std::vector<std::thread> threads;
      for(auto i{0}; i < num_of_threads; i++)
        threads.emplace_back(std::thread(&AsyncServer::HandleRpcs, this, compl_queues_[i].get()));
      for(auto i{0}; i<threads.size(); i++)
        threads[i].join();
    }
    
    void HandleRpcs(grpc::ServerCompletionQueue* cq) 
    {     
      CallData data{&service_, cq, producer, kafka_msg_builder};
      new SendSingleLogCall(&data);
      new SendBatchLogCall(&data);
      void *tag;
      bool ok;
      for(;;)
      { 
        if (!cq->Next(&tag, &ok)) {
          std::cerr << "Shutting down..." << std::endl;
          break;
        }
        static_cast<Call*>(tag)->Proceed(ok);
      }
    } 
  private:
    std::unique_ptr<grpc::Server> server_;
    std::vector<std::unique_ptr<grpc::ServerCompletionQueue>> compl_queues_;
    ApiLogger::AsyncService service_;
    cppkafka::MessageBuilder *kafka_msg_builder;
    cppkafka::Producer *producer;
};


int main()
{ 
  AsyncServer server;
  server.Init("../../config.yaml");
  return 0;
}
