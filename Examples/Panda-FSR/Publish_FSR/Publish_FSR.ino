#include <WiFi.h>
#include <WebSocketClient.h> 

// Set these to your desired credentials.
const char *ssid = "robot_lan_2G";
const char *password = "";

// Update with ROS master IP and rosbridge port (default 9090)
const char *host_ip = "172.31.1.21";
const int host_port = 9090;
char host[] = "ws://172.31.1.21:9090";

char path[] = "/";

WiFiClient client;
WebSocketClient webSocketClient;

bool testBit = true;


void setup() {
  
  Serial.begin(9600);
  delay(1000);
  Serial.flush();
  Serial.println("Setup..");
  delay(10);
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
   
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
   
  delay(5000);

  if(client.connect(host_ip, host_port)) {
    Serial.println("Connected");
  }else {
      Serial.println("Connection failed.");
      while(1) {
      // Hang on failure
    }
  }

  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    while(1) {
      // Hang on failure
    } 
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Loopin..");
  
  
  if (client.connected()) {
    if (testBit){
      String msg = "{\"op\": \"advertise\", \"topic\": \"/panda_touching\", \"type\": \"std_msgs/Float32\"}";
      Serial.println(msg);
      webSocketClient.sendData(msg);
      testBit=false;
    } else {
      String msg = "{\"op\": \"publish\", \"topic\": \"/panda_touching\", \"msg\": {\"data\":";
      msg+= String(analogRead(A3));      
      msg+= "}}";
      Serial.println(msg);
      webSocketClient.sendData(msg);
    }
    
 
  } else {
    Serial.println("Client disconnected.");
  }

}
