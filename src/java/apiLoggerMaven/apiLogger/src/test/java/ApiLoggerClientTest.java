import org.junit.Test;

public class ApiLoggerClientTest {


    private static final String serverAddress = "157.245.104.78:5051";
    //in project_root/src/java/lib
    private static final String certs = "./../../../../certs/";
    private static final String CApath = certs + "ca.pem";
    private static final String ClientCertPath = certs + "client.pem";
    private static final String ClientKey = certs + "client.key";

    private static final boolean useTls = false;

    @Test
    public void checkSingleLog(){
        ApiLoggerClient client = new ApiLoggerClient(serverAddress, false, "","", "", 10);

        client.SendLog("json: ", ApiLoggerClient.log_priority.HIGH);

        client.ShutDown();

    }
    @Test
    public void checkBatchLog(){
        ApiLoggerClient client = new ApiLoggerClient(serverAddress, useTls, CApath, ClientCertPath, ClientKey, 10);

        for(int i = 0; i<19; i++)
            client.SendLog("batch log from java"+ String.valueOf(i), ApiLoggerClient.log_priority.LOW);

        client.ShutDown();

    }}
