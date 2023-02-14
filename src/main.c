#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"

#define button 0

#include "wifi.h"
#define LED_INFRAVERMELHO 23
#include "mqtt.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"


#define TEMPERATURE_SENSOR_GPIO 4

static const char *TAG = "Temperature Sensor";

SemaphoreHandle_t conexaoWifiSemaphore;
SemaphoreHandle_t conexaoMQTTSemaphore;

void conectadoWifi(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
    {
      // Processamento Internet
      mqtt_start();
    }
  }
}

void read_temperature_sensor(void *pvParameter)
{
    while (1)
    {
        int level = gpio_get_level(TEMPERATURE_SENSOR_GPIO);
        ESP_LOGI(TAG, "Temperature Sensor: %d", level);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void trataComunicacaoComServidor(void * params)
{
  char mensagem[50];
  if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY))
  {
    while(true)
    {
      int temperatura = gpio_get_level(TEMPERATURE_SENSOR_GPIO);
      sprintf(mensagem, "{\"temperatura\": %d}",  temperatura);
      mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
      printf("%s \n", mensagem);

       //sprintf(mensagem, "temperatura1: %f", temperatura);
      // mqtt_envia_mensagem("sensores/temperatura", mensagem);
      vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }
}

void ledInfra_start() {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL<<LED_INFRAVERMELHO);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
}

void app_main(void)
{
    // Inicializa o NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();
    printf("kkkkkkkkkkkkkkkkk");
    wifi_start();
    ledInfra_start();


    ESP_LOGI(TAG, "Initializing temperature sensor...");

    gpio_config_t io_conf = {
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    xSemaphoreGive(conexaoMQTTSemaphore);
    xSemaphoreGive(conexaoWifiSemaphore);

    xTaskCreate(read_temperature_sensor, "Read Temperature Sensor", 2048, NULL, 5, NULL);
    xTaskCreate(&conectadoWifi,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
    xTaskCreate(&trataComunicacaoComServidor, "Comunicação com Broker", 4096, NULL, 1, NULL);
}
