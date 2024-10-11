// Autor Rev3: Leonardo Rocha Scarpitta
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

DHT dht(4, DHT22);

// Configurações - variáveis editáveis
const char* default_SSID = "Wokwi-GUEST";                        // Nome da rede Wi-Fi
const char* default_PASSWORD = "";                               // Senha da rede Wi-Fi
const char *default_BROKER_MQTT = "";                            // IP do Broker MQTT
const int default_BROKER_PORT = 1883;                            // Porta do Broker MQTT
const char *default_TOPICO_SUBSCRIBE = "/TEF/iot003/cmd";        // Tópico MQTT de escuta
const char *default_TOPICO_PUBLISH_1 = "/TEF/iot003/attrs";      // Tópico MQTT de envio de informações para Broker
const char *default_TOPICO_PUBLISH_2 = "/TEF/iot003/attrs/l";    // Tópico MQTT de envio de informações para Broker
const char *default_TOPICO_PUBLISH_3 = "/TEF/iot003/attrs/h";    // Tópico MQTT para umidade
const char *default_TOPICO_PUBLISH_4 = "/TEF/iot003/attrs/t";    // Tópico MQTT para temperatura
const char *default_ID_MQTT = "fiware_003";                      // ID MQTT
const int default_D4 = 2;                                        // Pino do LED onboard
// Declaração da variável para o prefixo do tópico
const char *topicPrefix = "iot003";

// Variáveis para configurações editáveis
char *SSID = const_cast<char *>(default_SSID);
char *PASSWORD = const_cast<char *>(default_PASSWORD);
char *BROKER_MQTT = const_cast<char *>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char *TOPICO_SUBSCRIBE = const_cast<char *>(default_TOPICO_SUBSCRIBE);
char *TOPICO_PUBLISH_1 = const_cast<char *>(default_TOPICO_PUBLISH_1);
char *TOPICO_PUBLISH_2 = const_cast<char *>(default_TOPICO_PUBLISH_2);
char *TOPICO_PUBLISH_3 = const_cast<char *>(default_TOPICO_PUBLISH_3);
char *TOPICO_PUBLISH_4 = const_cast<char *>(default_TOPICO_PUBLISH_4);
char *ID_MQTT = const_cast<char *>(default_ID_MQTT);
int D4 = default_D4;

WiFiClient espClient;
PubSubClient MQTT(espClient);
char estadoSaida = '0';

void initSerial()
{
    Serial.begin(115200);
}

void initWiFi()
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}

void initMQTT()
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}

void setup()
{
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    dht.begin();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
}

void loop()
{
    Serial.println("//////////////");
    VerificaConexoesWiFIEMQTT();
    sendStatusOutputMQTT();
    handleLuminosity();
    handleUmidityTemperature();
    MQTT.loop();
    delay(1000);
}

void reconectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());

    // Garantir que o LED inicie desligado
    digitalWrite(D4, LOW);
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    String msg;
    for (int i = 0; i < length; i++)
    {
        char c = (char)payload[i];
        msg += c;
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    // Forma o padrão de tópico para comparação
    String onTopic = String(topicPrefix) + "@on|";
    String offTopic = String(topicPrefix) + "@off|";

    // Compara com o tópico recebido
    if (msg.equals(onTopic))
    {
        digitalWrite(D4, HIGH);
        estadoSaida = '1';
    }

    if (msg.equals(offTopic))
    {
        digitalWrite(D4, LOW);
        estadoSaida = '0';
    }
}

void VerificaConexoesWiFIEMQTT()
{
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}

void sendStatusOutputMQTT()
{
    if (estadoSaida == '1')
    {
        MQTT.publish(TOPICO_PUBLISH_1, "s|on");
        Serial.println("- Led Ligado");
    }

    if (estadoSaida == '0')
    {
        MQTT.publish(TOPICO_PUBLISH_1, "s|off");
        Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
}

void InitOutput()
{
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);
    boolean toggle = false;

    for (int i = 0; i <= 10; i++)
    {
        toggle = !toggle;
        digitalWrite(D4, toggle);
        delay(200);
    }
}

void reconnectMQTT()
{
    while (!MQTT.connected())
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT))
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        }
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000);
        }
    }
}

void handleLuminosity()
{
    const int potPin = 34;
    int sensorValue = analogRead(potPin);
    int luminosity = map(sensorValue, 0, 4095, 0, 100);
    String mensagem = String(luminosity);
    Serial.print("- Valor da luminosidade: ");
    Serial.println(mensagem.c_str());
    MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str());
}

void handleUmidityTemperature()
{
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Verifica se as leituras falharam e sai mais cedo se necessário.
    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println(F("Falha ao ler as informações do DHT!"));
        return;
    }

    Serial.print(F("- Umidade: "));
    Serial.print(humidity);
    Serial.print(F("%"));
    Serial.println();
    Serial.print(F("- Temperatura: "));
    Serial.print(temperature);
    Serial.println(F("°C "));

    // Converte os valores de umidade e temperatura para string simples
    String mensagemUmidade = String(humidity);
    String mensagemTemperatura = String(temperature);

    // Publica os dados de umidade e temperatura via MQTT como texto simples
    MQTT.publish(TOPICO_PUBLISH_3, mensagemUmidade.c_str());
    MQTT.publish(TOPICO_PUBLISH_4, mensagemTemperatura.c_str());
    Serial.println("Informações do DHT publicadas com sucesso.");
}
