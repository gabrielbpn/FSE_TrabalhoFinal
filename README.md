# FSE_TrabalhoFinal

## Link da apresentação:

- https://youtu.be/LHoTkhmdo7w

## Integrantes

- Gabriel Bonifácio, matrícula 18/0145088.

## Objetivo do trabalho

O objetivo deste trabalho é criar sensores e atuadores distribuídos baseados nos microcontroladores ESP32 interconectados via Wifi através do protocolo MQTT, podendo ser aplicada em diversos contextos de automação a exemplo das áreas de Automação Residencial, Predial ou Industrial.
Os microcontroladores ESP32 irão controlar a aquisição de dados de sensores, botões e chaves e acionar saídas como leds, dentre outras. Haverão dois modos de operação dos dispositivos: modo energia que representa o dispositivo ligado à tomada e modo bateria que representa o dispositivo que deverá operar em modo de baixo consumo.
Toda a comunicação será feita via rede Wifi com o protocolo MQTT e será provido um servidor central para cadastro e controle dos dispositivos através da plataforma Thingsboard.

## Componentes utilizados:

- Sensor de temperatura digital - KY028.
- Sensor de Infravermelho.

## Bibliotecas

Linguagem C

- Bibliotecas:

<stdio.h>
<string.h>
<driver/adc.h>
"nvs_flash.h"
"esp_wifi.h"
"esp_event.h"
"esp_http_client.h"
"esp_log.h"
"freertos/semphr.h"
<driver/ledc.h>
"freertos/FreeRTOS.h"
"freertos/task.h"
"freertos/event_groups.h"
"driver/gpio.h"
"sdkconfig.h"
"esp_system.h"
"lwip/err.h"
"lwip/sys.h"
"mqtt.h"
"dht11.h"

## Como rodar

- Clone o repositório.
- Faça a instalação e correta configuração do PlatformIO.
- Adicionar os sensores na protoboard com suas devidas conexões.
- Faça o build e o upload para poder aplicar o serial monitor.
