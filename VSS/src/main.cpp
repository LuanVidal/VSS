// Code By: Luan Gabriel Vidal
// Studant IFMG 
// Project VSS

#define ROSSERIAL_ARDUINO_TCP

#include <WiFi.h>
#include <Arduino.h> 
#include <ros.h>
#include <geometry_msgs/Twist.h>

const char* ssid = "Wi-Fi_407";
const char* password = "#labor@torio.407#";

// ros::Rate rate(10);
float v = 0.0;
float w = 0.0;

void callback(const geometry_msgs::Twist &msg){

  v = msg.linear.x;
  w = msg.angular.z;
  
}

// Defina o endereço IP do servidor do soquete (rosserial)
IPAddress server(192,168,0,119);

// Defina a porta do servidor do soquete (rosserial)
const uint16_t serverPort = 11411;

ros::NodeHandle nh;

// habilitando a mensagem a ser enviada
geometry_msgs::Twist msg;
// star da msg
ros::Publisher pub("esp32/cmd_vel", &msg);
ros::Subscriber<geometry_msgs::Twist> sub("robot0/cmd_vel/", &callback);


void setup() {

  // Usando o monitor serial do ESP

  Serial.begin(9600);
  Serial.println();
  Serial.print("Conectando em ");
  Serial.println(ssid);

  // conecta o ESP ao wifi

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Definindo a conexão com o servidor do soquete (rosserial)

  nh.getHardware() -> setConnection(server, serverPort);
  nh.initNode();

  // Outra maneira de conseguir um IP
  Serial.print("IP = ");
  Serial.println(nh.getHardware() -> getLocalIP());

    // star da msg
  nh.advertise(pub);
  nh.subscribe(sub);
  // ros::Subscriber sub = nh.subscribe("/robot0/cmd_vel/", 10, &callback);
}

void loop() {

  if (nh.connected()) {

    Serial.println("Conectado");

    // envio da mensagem
    msg.linear.x = 10.0;
    msg.angular.z = 1.0;
    pub.publish( &msg );
    Serial.print("ok");
    Serial.println(v);
    Serial.println(w);

  } else {
    Serial.println("Não conectado");
    pub.publish( &msg );
    Serial.print("ok");
    Serial.println(v);
    Serial.println(w);
  }

  // rate.sleep();

  nh.spinOnce();

  // Loop para perfomace
  
  delay(10);
}
