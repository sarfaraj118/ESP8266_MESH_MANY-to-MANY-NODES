
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
extern "C" {
#include "user_interface.h"
}
// IoT platform Credentials
String apiKey = "DCU";

const char* logServer = "104.208.142.491";
String data;
// Internet router credentials
const char* ssid = "DCU";
const char* password = "india";
int Status=1;
ESP8266WebServer server(80);
String serv_ack ;
String tmp ;
int  ACK_SERV, kk,jj = 0;
const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;
/**************************XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX************************************************/
void setup() {
Serial.begin(115200);
Serial.println("");
Serial.println("Remember to to set Both NL & CR in the serial monitor.");
Serial.println("Ready");
Serial.println(""); 
//WiFi.mode(WIFI_AP_STA);
WiFi.mode(WIFI_AP);
struct softap_config config;
wifi_softap_get_config(&config); // Get config first.
config.max_connection = 8;

setupAccessPoint();
GSM_MODULE_INIT();TCP_INIT();
}
void client_status() {

unsigned char number_client;
struct station_info *stat_info;

struct ip_addr *IPaddress;
IPAddress address;
int i=1;

number_client= wifi_softap_get_station_num();
yield();
stat_info = wifi_softap_get_station_info();
Serial.print("Total connected_client are = ");
Serial.println(number_client);
while (stat_info != NULL) 
{
IPaddress = &stat_info->ip;
address = IPaddress->addr;

Serial.print("client= ");

Serial.print(i);
Serial.print(" ip address is = ");
Serial.print((address));
Serial.print(" with mac address is = ");

Serial.print(stat_info->bssid[0],HEX);
Serial.print(stat_info->bssid[1],HEX);
Serial.print(stat_info->bssid[2],HEX);
Serial.print(stat_info->bssid[3],HEX);
Serial.print(stat_info->bssid[4],HEX);
Serial.println(stat_info->bssid[5],HEX);
stat_info = STAILQ_NEXT(stat_info, next);
i++;
}delay(300);
}



// Handling the / root web page from my server 
void handle_index() {
server.send(200, "text/plain", "WELCOME IN DCU WORLD !!");
}

// Handling the /feed page from my server
void  handle_feed(){
Status= 0;
String A = server.arg("packet-A");
String B = server.arg("packet-B");
String C = server.arg("packet-C");
String D = server.arg("packet-D");


tmp=A;
server.send(200, "text/plain", "HELLO");
Serial.println(A);
Serial.println(B);
Serial.println(C);
Serial.println(D);

setupStMode(A,B,C,D);

}

void setupAccessPoint(){
Serial.println("** SETUP ACCESS POINT **"); 
client_status() ; 
Serial.println("- disconnect from any other modes");
//WiFi.disconnect();
Serial.println("- start ap with SID: "+ String(ssid));
WiFi.softAP(ssid, password);
IPAddress myIP = WiFi.softAPIP();
Serial.print("- AP IP address is :");
Serial.print(myIP);
setupServer();
}

void setupServer(){

Serial.println("/r/n** setupServer() **/r/n");
Serial.println("** SETUP SERVER **");
Serial.println("- starting server :");
server.on("/", handle_index);
server.on("/feed", handle_feed);
server.begin();

};

//void setupStMode(String t, String v){
/*************************************************************************************/
void setupStMode(String A,String B,String C,String D){
Serial.println("*************** SETUP STATION MODE**************************** **");
//Serial.begin(115200);Serial.swap();
TCP_ACTIVATION(); TCP_CONN_ESTABLISED();TCP_DATA_TX();//TCP_DATA_RX();

Serial.println();Serial.println("- succesfully connected");Serial.println("- starting client");
WiFiClient client;

//Read what the browser has sent into a String class and print the request to the monitor
//String request = client.readString();

//Serial.println(request);
//Serial.println("- SIDE-1 : "+String(t));
//Serial.println("- SIDE-2 : "+String(v));
Serial.println("- connecting to Database server: " + String(logServer));
client.stop();
Serial.println("- stopping the client");
Status= 1;
client_status();
Serial.println("**************************XXXXXXXXXXX!!!E---N---D!!!XXXXXXXXXXXXXXX************************************************");
Serial.println("");

}



/****************************QUECTEL-EC-25_Module*****************************************/

void GSM_MODULE_INIT()   /*GSM_MODULE initilization*/
{ 
   int i,  num =11; 
  char *GSM_MODULE_INIT[11] ={"ATV1","ATE0","AT+CMEE=2","AT+IPR=115200;&W","AT+QURCCFG=\"URCPORT\",\"uart1\"","AT+CPIN?","AT+CIMI","AT+QCCID","AT+CSQ","AT+CREG?"};

 for (i = 0; i <num; i++) 
{
Serial.println(GSM_MODULE_INIT[i]);
//uartreceive();
recvWithEndMarker();showNewData();
}}
void TCP_INIT()
{ 
int i,  num =3; 
char *TCP_INIT[3] = {"AT+CPIN?","AT+CREG?","AT+CSQ"} ;
for (i = 0; i <num; i++) 
  { Serial.println(TCP_INIT[i]); 
recvWithEndMarker();showNewData();//uartreceive();
}
  }
void TCP_ACTIVATION()
{
int i,  num =5; 
char *TCP_ACTIVATION[5] = {"AT+CPIN?","AT+CREG?","AT+QICSGP=1,1,\"UNINET\",\"\",\"\",0","AT+QIACT=1","AT+QIACT?"} ;
for (i = 0; i < num; i++) 
   {
 Serial.println(TCP_ACTIVATION[i]); delay(500);
recvWithEndMarker();showNewData();//uartreceive();
}}

 void TCP_CONN_ESTABLISED()
{
int i,  num =1; 
char *TCP_CONN_ESTABLISED[1] = {"AT+QIOPEN=1,0,\"TCP\",\"104.208.142.491\",567,0,1"} ;//103.208.152.191//117.218.35.237
for (i = 0; i < num; i++) 
{
Serial.println(TCP_CONN_ESTABLISED[i]); delay(200);
recvWithEndMarker();showNewData();//uartreceive();

delay(500);}}   

void TCP_DATA_TX()
{
int i,  num =6; 
char *TCP_DATA_TX[6] = {"AT+QISEND=0","","\x1A","AT+QISEND=0,0","AT+QISEND=0,0","AT+QIRD=0,0"} ;
for (i = 0; i < num; i++) 
{
if(i==1)
{ Serial.println(tmp);
i++;}
Serial.println(TCP_DATA_TX[i]);delay(300);
if (i==2)
{ACK_SERV =1;}
recvWithEndMarker();showNewData();//uartreceive();
//delay(500); 
}}

void TCP_DATA_RX()
{
int i,  num =2; 
char line[20];
char *TCP_DATA_RX[2] = {"AT+QIRD=0,0","AT+QIRD=0,0"} ;

for (i = 0; i < num; i++) 
{    
  Serial.println(TCP_DATA_RX[i]); 
  delay(500);
recvWithEndMarker();showNewData();//uartreceive();
//i++;

delay(500);
}
}



/********************ACKNLOGEMENT-MECHANISM***************************************/
void recvWithEndMarker() {

 static byte ndx = 0;
 char endMarker = '\n';
 char recv;
 char Q_S_ACK[9]="SEND OK",Q_SER_ACK[9]="HELLO";
 int i;
 // if (Serial.available() > 0) {
           while (Serial.available() > 0 ) {
 recv = Serial.read();

 if (recv != endMarker) {
 receivedChars[ndx] = recv;
// Serial.println(ndx);
// Serial.println(recv);
 ndx++;
 if(ACK_SERV=1)
 {
  if(((Q_SER_ACK[0]) ==(receivedChars[0]))&&((Q_SER_ACK[1]) ==(receivedChars[1]))&&((Q_SER_ACK[2]) ==(receivedChars[2]))&&
 ((Q_SER_ACK[3]) ==(receivedChars[3]))&&((Q_SER_ACK[4]) ==(receivedChars[4])))
 {Serial.println("!!!!!!!!!!!!!!!!!!!!!!HELLOOOO RECEIVED FROM SERVER !!!!!!!!!!!!!!!!!!!!!!!!!!");
  server.send(200, "text/plain", "DONE!");}
 
 }
 
 if (((Q_S_ACK[0]) ==(receivedChars[0]))&&((Q_S_ACK[1]) ==(receivedChars[1]))&&((Q_S_ACK[2]) ==(receivedChars[2]))&&
 ((Q_S_ACK[3]) ==(receivedChars[3]))&&((Q_S_ACK[4]) ==(receivedChars[4]))&&((Q_S_ACK[5]) ==(receivedChars[5]))&&((Q_S_ACK[6]) ==(receivedChars[6])))
{
  Serial.println("!!!!!!!!!!!!!!!!!!!!!!DATA SENT TO SERVER !!!!!!!!!!!!!!!!!!!!!!!!!!");
 // server.send(200, "text/plain", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx !");
  delay(500);
  }
 if (ndx >= numChars) {
 ndx = numChars - 1;
 }
 }
 else {
 receivedChars[ndx] = '\0'; // terminate the string
 
 ndx = 0;
 newData = true;
 }
 }
}

void showNewData() {
 while (newData == true) {
 //Serial.print("This just in ... ");
 Serial.println(receivedChars);
  newData = false;

 }
}
/**********************************************************************************************/
//void sendHttpRequest() {
//HTTPClient http;
////  http.begin(serverHost);
//http.addHeader("Content-Type", "application/x-www-form-urlencoded");
//http.POST(data);
//http.writeToStream(&Serial);
//http.end();
//
//}

void loop() {
  if (Status == 1)
  {    server.handleClient();
 
  }
  else
  Serial.println("AT");
  
}
