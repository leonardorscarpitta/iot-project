# Projeto IoT com ESP32 e MQTT
Este projeto utiliza um ESP32 para monitorar luminosidade, umidade e temperatura de um ambiente controlado de uma adega, e envia esses dados via MQTT para um broker. Além disso, as informações são visualizadas em um dashboard web desenvolvido com Dash e Plotly. O sensor de temperatura e umidade utilizado é o DHT22, e a comunicação é realizada via WiFi.

### Funcionalidades
- Monitoramento de luminosidade, umidade e temperatura.
- Envio dos dados via MQTT para um broker configurado.
- Controle remoto do LED onboard via comandos MQTT.
- Exibição dos dados em um dashboard web.
## Requisitos
### Hardware
- ESP32
- Sensor DHT22 (Ou DHT11)
- Resistor de pull-up (10kΩ) para o LDR
- LDR
- Cabos jumpers
- Conexão WiFi
> [!TIP]
> Caso não tenha os dispositivos físicos citados acima, o Wokwi possibilita a simulação, [clique aqui](https://wokwi.com/projects/407480429631038465) para acessar a URL do projeto.
### Software
Arduino IDE com suporte ao ESP32
- Bibliotecas necessárias:
  - WiFi.h
  - PubSubClient.h
  - DHT.h
  - Broker MQTT (pode ser um broker local ou na nuvem)
  - Python com Dash e Plotly para o dashboard.
Estrutura do Projeto
Arquivo Principal: `sketch.ino` <br>
#### Este arquivo contém o código que:

- **Inicializa** o sensor DHT22 e conecta o ESP32 à rede WiFi e ao broker MQTT.
- **Publica** os valores de luminosidade, umidade e temperatura em tópicos MQTT específicos.
- **"Escuta"** um tópico de controle MQTT para acionar ou desligar o LED onboard.
- **Reenvia** o estado atual do LED para o broker após cada comando.
### Configurações do Projeto
- **SSID e Senha do WiFi**: Defina sua rede WiFi editando as variáveis default_SSID e default_PASSWORD.
- **Broker MQTT**: Insira o endereço IP do seu broker MQTT na variável default_BROKER_MQTT e a porta na variável default_BROKER_PORT.
- **Tópicos MQTT**: Existem quatro tópicos configurados para publicar os dados:
 - **Luminosidade**: TOPICO_PUBLISH_2
 - **Umidade**: TOPICO_PUBLISH_3
 - **Temperatura**: TOPICO_PUBLISH_4
 - **Status do LED**: TOPICO_PUBLISH_1
 - E um tópico para "escutar" comandos de on | off (Referente ao Led do ESP32): TOPICO_SUBSCRIBE.
### Funções Principais
`handleUmidityTemperature`: Lê os valores do sensor DHT22 e os publica nos tópicos MQTT configurados. <br>
`handleLuminosity`: Lê o valor do sensor de luminosidade e o publica via MQTT. <br>
`mqtt_callback`: Função chamada quando uma mensagem é recebida no tópico de controle, responsável por ligar ou desligar o LED onboard. <br>
`VerificaConexoesWiFIEMQTT`: Monitora as conexões WiFi e MQTT, reconectando automaticamente se necessário.
## Dashboard para Visualização de Dados
O dashboard foi desenvolvido em Python com o framework Dash. Ele exibe os dados históricos de luminosidade, umidade e temperatura em gráficos, atualizando a cada 10 segundos.

Configuração do Dashboard
1. Dependências: Instale as dependências necessárias:

```bash
pip install dash plotly requests pytz
```
2. Configuração do IP e Porta:
- Altere o valor da variável IP_ADDRESS para o endereço IP onde o broker MQTT está sendo executado.
- Certifique-se de que o serviço STH (Short-Term History) do FIWARE está acessível na porta definida por PORT_STH.
3. Executar o Dashboard: Rode o dashboard com o seguinte comando:

```bash
python sth-dashboard.py
```
O dashboard estará disponível no endereço `http://localhost:8050` (ou no IP:Porta configurados em caso de alteração no código).

## Como Rodar o Projeto
Carregar o código no ESP32:

1. Abra o arquivo .ino na Arduino IDE.
Instale as bibliotecas necessárias.
Configure os parâmetros de WiFi e broker MQTT.
Faça o upload do código no ESP32.

3. Configurar e executar o dashboard:
Configure o arquivo app.py conforme instruído na seção anterior.
Execute o script Python para visualizar os dados.

## Desenvolvedores do projeto:

| **Nome** | **RM**                 | **LinkedIn** |
|--------------------------------|------------------------|----------|
| Fabricio Muniz                 | RM 555017              |  |
| Gustavo Melanda da Nova        | RM 556081              | <a target="_blank" href="https://www.linkedin.com/in/gustavo-melanda-073181266/"><img src="https://media.licdn.com/dms/image/v2/D4D03AQF4Xv6pD7M5vQ/profile-displayphoto-shrink_800_800/profile-displayphoto-shrink_800_800/0/1723431081627?e=1733961600&v=beta&t=Ux96lHAYwBZnCWfABuznIdH8VSWMCHM4CBJtmEpyQQ0" width="80"></a> |
| Murilo Justi                   | RM 554512              | <a target="_blank" href="https://www.linkedin.com/in/murilo-justi-rodrigues-b336b22b7/"><img src="https://media.licdn.com/dms/image/v2/D4D03AQGnXBOl96aCtQ/profile-displayphoto-shrink_800_800/profile-displayphoto-shrink_800_800/0/1709252884484?e=1733961600&v=beta&t=_W2l37rEiTdk8HSG-GUrS4R_V6KddfAGj13CbkA_k0g" width="80"></a> |
| Renan Dias Utida               | RM 558540              | <a target="_blank" href="https://www.linkedin.com/in/renan-dias-utida-1b1228225/"><img src="https://media.licdn.com/dms/image/v2/D4D03AQHZyF9WkCRtDg/profile-displayphoto-shrink_800_800/profile-displayphoto-shrink_800_800/0/1727923002401?e=1733961600&v=beta&t=foOm4Ar-LZJK6z8mu_ypyoXfkqYesw3MAc4acpeAqpU" width="80"></a> |
| Leonardo Rocha Scarpitta       | RM 555460              | <a target="_blank" href="https://www.linkedin.com/in/leonardorscarpitta/"><img src="https://media.licdn.com/dms/image/v2/D4D03AQG6zoS4UPTtQw/profile-displayphoto-shrink_800_800/profile-displayphoto-shrink_800_800/0/1718304566524?e=1733961600&v=beta&t=xK7Amp3IFfd7eoDSI7-OXn7roBt2p8ga5UgOeFtgWs4" width="80"></a> |
