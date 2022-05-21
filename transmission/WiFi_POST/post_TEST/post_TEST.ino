/**
 * POST
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "Hope or Hopeless";      // wifi名
const char* password = "QwQ32767";  // wifi密码

const char* host = "test.gmsure.plus";  // 连接的主机域名
const int httpsPort = 80;  // https端口
String url = "";  // 请求的页面地址(后面代码中进行拼接)


void connectWifi();
void postDataToServer(String data, size_t len);
float getDataToServer();
void setup() {
  Serial.begin(115200);
  connectWifi();
  while (!Serial)
   {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  while(Serial.read()>=0){}   //清空串口缓存
  Serial.println("Goodnight moon!"); // set the data rate for the SoftwareSerial port
    Serial1.begin(115200);
    Serial1.println("AT");
}

void loop() {
  String data = "";
  size_t len = 0;
  if(Serial.available()>0){
      delay(100);
      data = Serial.readStringUntil('-');
      Serial.print("Serial.readString:");
      Serial.println(data);
    }
  if(data != ""){
    len = data.length();
    postDataToServer(data,len);
    data="";
    delay(899);
  }
  
//  getDataToServer();
  
}

/**
 * 连接wifi
 */
void connectWifi(){
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);    // 设置wifi模式
  WiFi.begin(ssid, password);   // 连接wifi
  while (WiFi.status() != WL_CONNECTED) { //判断连接状态
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/**
 * Post方法
 */
void postDataToServer(String data, size_t len){
  // Use WiFiClientSecure class to create TLS connection
  WiFiClient client;          // HTTP
//  WiFiClientSecure client;    // HTTPS
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {   // 判断连接情况
    Serial.println("connection failed");
    return;
  }

  Serial.print("requesting URL: ");
  // 组拼url地址
  url = "/data/test/hello";//?id=3&val=OKk
  Serial.println(url);

  // 发送POST请求
  
  
  // 组拼HTTPS请求的Header
  String jsonStr = String("") + "id="+ len +"&val=" + String(data) ;
  String getStr = String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               //"User-Agent: test\r\n" +
               "Accept: */*\r\n" +
               //"Accept-Encoding: gzip, deflate, br\r\n" +
               //"Connection: keep-alive\r\n" +
               "Content-Type: application/x-www-form-urlencoded\r\n" + 
               //; boundary=<calculated when request is sent>
               "Content-Length: " + jsonStr.length() + "\r\n\r\n" +
               jsonStr;
  client.print(getStr);   // 发送Headers头
//  client.print(jsonStr);  // 发送json数据
  Serial.println("send was:");
  Serial.println(getStr + jsonStr);
  String line = client.readStringUntil('\n');
  Serial.println("reply was:");
  Serial.println(line);   // 打印接受到的数据

}

float getDataToServer(){
  // Use WiFiClientSecure class to create TLS connection
  WiFiClient client;          // HTTP
//  WiFiClientSecure client;    // HTTPS
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {   // 判断连接情况
    Serial.println("connection failed");
    return -1;
  }

  Serial.print("requesting URL: ");
  // 组拼url地址
  url = "/data/test/query";
  Serial.println(url);

  // 发送GET请求
  
  // 组拼HTTPS请求的Header
  String getStr = String("GET ") + url + "\r\n" ;
//               "Host: " + host + "\r\n";
  client.print(getStr);

  Serial.println("request sent");

  // 读取连接情况
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);   // 打印接受到的数据
  Serial.println("==========");
  Serial.println("closing connection");
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  // 读取数据
  String line = client.readStringUntil('\n');
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);   // 打印接受到的数据
  Serial.println("==========");
  Serial.println("closing connection");

  // TODO:解析json字符串,获取需要的数据
  return 0;
}
