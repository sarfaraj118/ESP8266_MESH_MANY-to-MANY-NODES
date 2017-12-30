#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMesh.h>

extern "C" {
#include "user_interface.h"
}
// AP Wi-Fi credentials level wise!
const char* ssid = "DCU";
const char* password = "india";
const char* ssid1 = "MESH";
const char* password1 = "india";
//const char* ssid2 = "MESH1";
//const char* password2 = "india";
const char* logServer = "192.168.4.1";
const char* logServer1 = "192.167.8.6";
//const char* logServer2 = "192.169.7.61";
// Local ESP web-server address level wise!
String serverHost = "http://192.168.4.1/feed";
String serverHost_ack ="http://192.168.4.1/response";
String serverHost1 = "http://192.167.8.6/feed";
String serverHost_ack1 ="http://192.167.8.6/response";
//String serverHost2 = "http://192.169.7.6/feed";
//String serverHost_ack2 ="http://192.169.7.6/response";
String data;

int sleepInterval = 5;

int failConnectRetryInterval = 2;
int counter = 0,rou_x=0,Status=1,reset =0;
char *X;
char *m;
String A,B,C,D,E,F,G ;
// Static network configuration
//IPAddress ip(192, 168, 4, 4);
//IPAddress gateway(192, 168, 4, 1);
//IPAddress subnet(255, 255, 255, 0);
IPAddress L1_IP(192, 167, 8, 6);//level 1
IPAddress L2_IP(192, 169, 7, 6);//level 2
IPAddress L3_IP(192, 162, 4, 6);//level 2
WiFiClient client;
ESP8266WebServer server(80);

void setup() {
  ESP.eraseConfig();
  WiFi.persistent(false);
  Serial.begin(115200);
  
  Serial.println();
  Serial.println("******** BEGIN-1 ***********");
  Serial.print("MAC-ADDRESS:");Serial.println(WiFi.macAddress());
   delay(500);
  Serial.println("- set ESP STA mode");
  WiFi.mode(WIFI_AP_STA);
  Serial.println("- connecting to wifi");
//  WiFi.config(ip, gateway, subnet); 
   
     
/*********/
connection_init();
//WiFi.softAPdisconnect(false);
  /******************/

if (WiFi.SSID()== "DCU"){rou_x=0;WiFi.softAPConfig(L1_IP, L1_IP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("MESH", "india");Serial.println("-OPEN-MESH-NETWORK");}
else if (WiFi.SSID()== "MESH"){rou_x=1;WiFi.softAPConfig(L2_IP, L2_IP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("MESH1", "india");Serial.println("- OPEN-MESH1-NETWORK");}
else if(WiFi.SSID()== "MESH1"){rou_x=2;WiFi.softAPConfig(L3_IP, L3_IP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("MESH2", "india",1,true);Serial.println("- OPEN-MESH2-NETWORK");}

  Serial.println("- wifi connected");
  Serial.println("- read DHT sensor");
  readDHTSensor();
  Serial.println("- build DATA stream string");
  buildDataStream();
  Serial.println("- send GET request");
  sendHttpRequest();
  Serial.println();
  Serial.println("***X*******X*******X******X***");
 //  hibernate(sleepInterval);
}
void connection_init()
{Stop: Serial.println("LET ME RETRYING...OUT FROM NETWORK SEARCHING MODE___!!!!");
WiFi.softAPdisconnect(true);
  /*****switch network*******/
Serial.println(" ");
Serial.println("Level-1-Trying to Connect DCU");
while (WiFi.status() != WL_CONNECTED) {
WiFi.begin(ssid, password);

if(counter > 20){
    Serial.println(" ");
    Serial.println("Level-2-Trying to Connect MESH"); while (WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid1, password1);
            if(counter > 40){
                                                  
             counter=0; goto Stop;              
          }
          else if(reset >60){reset=0;ESP.reset();} 
   
    delay(500);
    Serial.print(".");
    counter++;reset++;
}}
delay(500);
Serial.print(".");
counter++;reset++;
}}
/****RECEIVING*******/
void handle_index() {
server.send(200, "text/plain", "PING SUCCESSFULL..!!");
};
void espreset() {
server.send(200, "text/plain", "RESET SUCCESSFULL..!!");
ESP.reset();
};
void  handle_feed(){ Serial.println("******ENTER HANDLE MODE*********");
Status= 0;
A = server.arg("packet-A");
B = server.arg("packet-B");
C = server.arg("packet-C");
D = server.arg("packet-D");
E = server.arg("packet-E");
F = server.arg("packet-F");
G = server.arg("packet-G");
server.send(200, "text/plain", "THANK YOU!");
delay(100);
Serial.println(A);
Serial.println(B);
Serial.println(C);
Serial.println(D);
Serial.println(E);
Serial.println(F);
Serial.println(G);


//server.send(200, "text/plain", "THANK YOU!");


delay(500);
//client_status() ;
};

void setupAccessPoint(){
Serial.println("** SETUP ACCESS POINT **");  
Serial.println("- disconnect from any other modes");
WiFi.disconnect();
IPAddress myIP = WiFi.softAPIP();
Serial.print("- AP IP address is :");
Serial.println(myIP);
setupServer();
}

void setupServer(){
Serial.println("- starting server :");
server.on("/", handle_index);
server.on("/feed", handle_feed);
server.on("/espreset", espreset);
server.begin();
};

/******************/



void sendHttpRequest() { Serial.println("ENTER IN- sendHttpRequest");
  if((WiFi.status() != WL_CONNECTED)||(WiFi.status() == WL_NO_SHIELD)||(WiFi.status() == WL_NO_SSID_AVAIL)||(WiFi.status() == WL_DISCONNECTED)||
  (WiFi.status() == WL_CONNECTION_LOST)||(WiFi.status() == WL_CONNECT_FAILED) )
  {Serial.println("WI-FI NOT AVALIABLE !!!");connection_init() ;}
 
HTTPClient http;
if(rou_x==0){ Serial.println("ENTER DCU HOSTING");http.begin(serverHost);}
else if(rou_x==1){ Serial.println("ENTER MESH HOSTING");http.begin(serverHost1);}


Serial.print("Connected with:");Serial.println(WiFi.SSID());
client_status();
Serial.println("--------sending data----------");
http.POST(data);

//if (rou_x==0){Serial.println("ENTER DCU ACK wait");http.begin(serverHost);}
//else if (rou_x==1){Serial.println("ENTER MESH1 ACK wait");http.begin(serverHost1);}
//else if (rou_x==2){Serial.println("ENTER MESH2 ACK wait");http.begin(serverHost2);}
String payload = http.getString(); 
Serial.println(payload);
Serial.println(data);
http.end();
delay(100);
//http.writeToStream(&Serial);
Status= 1;
client.flush();
data.remove(0,data.length());A.remove(0,A.length());B.remove(0,B.length());   //||data1.length()||data2.length()||A||B||C);
Serial.print("After clear data lenght:");
Serial.println(data.length());
Serial.println(A.length());
Serial.println(B.length());
}         

void readDHTSensor() {Serial.println("ENTER IN-readDHTSensor()");
  delay(200);
//  X =WiFi.macAddress(); //"*DCU5001,4B,QQQQQQQQQQQQQ,000299.01,0006.6300,908728.46,558383.63,01,196.1,000.0,344749.00,0000,9E34$$";
  //PPPPPPPPP//MMMMMMMMMMM//ZZZZZZZZZZZ//KKKKKKKKKK//SSSSSSSSSSSSS//FFFFFFFFFFFF//QQQQQQQQQQQQQ

    Serial.println("- MY DATA-: "+String(X));//Serial.println(A);
}

void buildDataStream() { Serial.println("ENTER IN-buildDataStream()");
      data = "packet-A=MY DATA:";
      data += String(WiFi.macAddress());
      data += "&packet-B=";
      data += String(A);
      data += "&packet-C=";
      data += String(B);
      data += "&packet-D=";
      data += String(C);
      data += "&packet-E=";
      data += String(D);
      data += "&packet-F=";
      data += String(E);
      data += "&packet-G=";
      data += String(F);
  Serial.println("- data stream: "+data);
}


void hibernate(int pInterval) {
  WiFi.disconnect();
  ESP.deepSleep(10 * 600000 * pInterval, WAKE_RFCAL);
  delay(100);
}
/***********CONNECTING DEVICES STATUS***************/
void client_status() {

unsigned char number_client;
struct station_info *stat_info;
struct ip_addr *IPaddress;
IPAddress address;
int i=0;

number_client= wifi_softap_get_station_num();
stat_info = wifi_softap_get_station_info();

Serial.print(" Total connected_client are = ");
Serial.println(number_client);

while (stat_info != NULL) {

IPaddress = &stat_info->ip;
address = IPaddress->addr;

Serial.print("client= ");
Serial.print(i);
Serial.print(" ip adress is = ");
Serial.print((address));
Serial.print(" with mac adress is = ");

Serial.print(stat_info->bssid[0],HEX);
Serial.print(stat_info->bssid[1],HEX);
Serial.print(stat_info->bssid[2],HEX);
Serial.print(stat_info->bssid[3],HEX);
Serial.print(stat_info->bssid[4],HEX);
Serial.print(stat_info->bssid[5],HEX);

stat_info = STAILQ_NEXT(stat_info, next);
i++;
Serial.println();

}delay(500);
}
/************************************/

void loop() 
{Serial.println("ENTER IN- MAIN loop");
buildDataStream();
sendHttpRequest();
delay(10000);
// delay(1000);delay(1000);delay(1000);delay(1000);delay(1000);
if (Status == 1){
server.handleClient();
setupServer();}
delay(3000);
  
  
  }
