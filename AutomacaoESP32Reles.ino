//** Inclusão das Bibliotecas **
#include <WiFi.h>
#include <PubSubClient.h>

#define DEBUG

//** Definição dos pinos da ESP32 para acionamento dos Relés
#define RELE1 27   //Relé 1
#define RELE2 26   //Relé 2
#define RELE3 25   //Relé 3
#define RELE4 33   //Relé 4
#define RELE5 32   //Relé 5
#define RELE6 5    //Relé 6
#define RELE7 19   //Relé 7
#define RELE8 18   //Relé 8

//** Informações da rede Wi-Fi **
const char* ssid = "Redmi-rsz"; //SSID da Rede
const char* password =  "zapata1031"; //Senha da Rede

//** Informações do Broker MQTT **
const char* mqttServer = "tailor.cloudmqtt.com"; //Server
const int mqttPort = 17991; //Porta
const char* mqttUser = "wfhwhfxr"; //Usuário
const char* mqttPassword = "ULr4Jzp8TMod"; //Senha
const char* mqttTopicSub = "esp_eng"; //Tópico

 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
  pinMode(RELE1, OUTPUT);
  pinMode(RELE2, OUTPUT);
  pinMode(RELE3, OUTPUT);
  pinMode(RELE4, OUTPUT);
  pinMode(RELE5, OUTPUT);
  pinMode(RELE6, OUTPUT);
  pinMode(RELE7, OUTPUT);
  pinMode(RELE8, OUTPUT);
  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE2, HIGH);
  digitalWrite(RELE3, HIGH);
  digitalWrite(RELE4, HIGH);
  digitalWrite(RELE5, HIGH);
  digitalWrite(RELE6, HIGH);
  digitalWrite(RELE7, HIGH);
  digitalWrite(RELE8, HIGH);
  
//** Conexão do Wi-Fi **
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUG
    Serial.println("Conectando ao Wi-Fi...");
    #endif
  }
  #ifdef DEBUG
  Serial.println("Conectado com sucesso na rede:");
  Serial.print(ssid);
  Serial.print("\nIP obtido: "); 
  Serial.println(WiFi.localIP() ); // IP da Rede 
  Serial.println(WiFi.macAddress() ); // Endereço MAC da Rede
  
  #endif

//** Conexão no Broker MQTT **
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.println("Conectando ao Broker MQTT...");
    #endif
 
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      #ifdef DEBUG
      Serial.println("Conectado");  
      #endif
 
    } else {
      #ifdef DEBUG 
      Serial.print("falha estado  ");
      Serial.print(client.state());
      #endif
      delay(2000);
 
    }
  }

//** Subscrição no Tópico **
  client.subscribe(mqttTopicSub);
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {

  //armazena msg recebida em uma sring
  payload[length] = '\0';
  String strMSG = String((char*)payload);

  #ifdef DEBUG
  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  Serial.print(strMSG);
  Serial.println();
  Serial.println("-----------------------");
  #endif

//** Acionamento de Saídas ** 
//! Observação: O relé funciona em nível baixo (LOW; 0)

//Relé 1
  if (strMSG == "rele1_on"){         //se msg "rele1_on"
     digitalWrite(RELE1, LOW);  //coloca saída em LOW para ligar
  }else if (strMSG == "rele1_off"){   //se msg "rele1_off"
     digitalWrite(RELE1, HIGH);   //coloca saída em HIGH para desligar
  }

//Relé 2  
   if (strMSG == "rele2_on"){         
     digitalWrite(RELE2, LOW);  
  }else if (strMSG == "rele2_off"){   
     digitalWrite(RELE2, HIGH);  
  }
   
//Relé 3   
   if (strMSG == "rele3_on"){         
     digitalWrite(RELE3, LOW);  
  }else if (strMSG == "rele3_off"){   
     digitalWrite(RELE3, HIGH);  
  }
   
//Relé 4   
   if (strMSG == "rele4_on"){    
     digitalWrite(RELE4, LOW);  
  }else if (strMSG == "rele4_off"){ 
     digitalWrite(RELE4, HIGH);  
  }
   
//Relé 5   
   if (strMSG == "rele5_on"){  
     digitalWrite(RELE5, LOW);  
  }else if (strMSG == "rele5_off"){  
     digitalWrite(RELE5, HIGH); 
  }
   
//Relé 6   
   if (strMSG == "rele6_on"){  
     digitalWrite(RELE6, LOW); 
  }else if (strMSG == "rele6_off"){ 
     digitalWrite(RELE6, HIGH);
  }
   
//Relé 7   
   if (strMSG == "rele7_on"){   
     digitalWrite(RELE7, LOW); 
  }else if (strMSG == "rele7_off"){ 
     digitalWrite(RELE7, HIGH);
  }
   
//Relé 8   
   if (strMSG == "rele8_on"){
     digitalWrite(RELE8, LOW); 
  }else if (strMSG == "rele8_off"){ 
     digitalWrite(RELE8, HIGH);
  }
}

//** Função para reconectar ao servidor MQTT **
void reconect() {

  while (!client.connected()) {
    #ifdef DEBUG
    Serial.print("Tentando conectar ao servidor MQTT...");
    #endif
     
    bool conectado = strlen(mqttUser) > 0 ?
                     client.connect("ESP32Client", mqttUser, mqttPassword) :
                     client.connect("ESP32Client");

    if(conectado) {
      #ifdef DEBUG
      Serial.println("Conectado com Sucesso");
      #endif
      //Subscreve no tópico
      client.subscribe(mqttTopicSub, 1); //nivel de qualidade: QoS 1
    } else {
      #ifdef DEBUG
      Serial.println("Falha durante a conexão.Code: ");
      Serial.println( String(client.state()).c_str());
      Serial.println("Tentando novamente em 10 s");
      #endif
      //Aguarda 10 segundos 
      delay(10000);
    }
  }
}
 
void loop() {
  if (!client.connected()) {
    reconect();
  }
  client.loop();
}
