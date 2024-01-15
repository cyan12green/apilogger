import com.google.protobuf.ByteString;
import com.google.protobuf.Empty;
import com.proto.apilogger.ApiLoggerGrpc;
import com.proto.apilogger.BatchLog;
import com.proto.apilogger.SingleLog;
import io.grpc.*;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

public class ApiLoggerClient {
    private final ManagedChannel channel;

    private final ApiLoggerGrpc.ApiLoggerBlockingStub blockingStub;

    private static final Logger logger = Logger.getLogger(ApiLoggerClient.class.getName());

    private static ArrayList<byte[]> log_batch_;
    private static int batch_size_;
    public ApiLoggerClient(String serverAddress,
                           Boolean useTLS,
                           String caPath,
                           String ClientCertPath,
                           String KeyPath,
                           int batch_size){
        if (useTLS) {
            File caFile, clientCertFile, keyFile;
            caFile = new File(caPath);
            clientCertFile = new File(ClientCertPath);
            keyFile = new File(KeyPath);

            ChannelCredentials creds = null;
            try {
                creds = TlsChannelCredentials.newBuilder()
                        .trustManager(caFile)
                        .keyManager(clientCertFile, keyFile)
                        .build();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            channel = Grpc.newChannelBuilder(serverAddress, creds)
                    .build();
        } else {
            channel = ManagedChannelBuilder.forTarget(serverAddress).usePlaintext().build();
        }
        blockingStub = ApiLoggerGrpc.newBlockingStub(channel);
        batch_size_ = batch_size;
        log_batch_ = new ArrayList<byte[]>();
    }
    public void ShutDown() {
        channel.shutdownNow();
    }

    public int SendLog(String logs, log_priority priority){
        if (priority == log_priority.HIGH){
            SingleLog request = SingleLog.newBuilder().setLogData(ByteString.copyFrom(logs.getBytes())).build();
            Empty reply;

            try{
                reply = blockingStub.sendSingleLog(request);
                return 0;
            }catch(StatusRuntimeException e){
                logger.log(Level.WARNING, "RPC failed: {0}", e.getStatus());
                return -1;
            }
        }
        log_batch_.add(logs.getBytes());

        if(log_batch_.size() < batch_size_)
            return 0;

        BatchLog.Builder builder = BatchLog.newBuilder();
        for (byte[] b: log_batch_)
            builder.addLogBatch(ByteString.copyFrom(b));
        BatchLog request = builder.build();

        Empty reply;
        try{
            reply = blockingStub.sendBatchLog(request);
            log_batch_.clear();
            return 0;
        }catch(StatusRuntimeException e){
            logger.log(Level.WARNING, "RPC failed: {0}", e.getStatus());
            return -1;
        }

    }

    public enum log_priority{
        HIGH,
        LOW
    }
}
