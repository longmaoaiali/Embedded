
// ESP32的SmartConfig 自动配网示例
/* =============
头文件包含
=============*/
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_wpa2.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_smartconfig.h"


// wifi连接事件
static EventGroupHandle_t s_wifi_event_group;

#define LOG_TAG "ESP32_SmartConfig Example"

static const int CONNECTED_BIT = BIT0;
static const int ESPTOUCH_DONE_BIT = BIT1;


// SmartConfig任务
void SmartConfig_Task(void *parm);


static void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
{
	// STA开始工作
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		xTaskCreate(SmartConfig_Task, "SmartConfig_Task", 4096, NULL, 3, NULL);
	} 
	// STA 断开事件
	else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		esp_wifi_connect();
		xEventGroupClearBits(s_wifi_event_group, CONNECTED_BIT);
	} 
	// STA 获取到IP
	else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		xEventGroupSetBits(s_wifi_event_group, CONNECTED_BIT);
	} 
	// SmartConfig 扫描完成
	else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
		ESP_LOGI(LOG_TAG, "Scan done");
	}
	// SmartConfig 找到信道
	else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
		ESP_LOGI(LOG_TAG, "Found channel");
	}
	// SmartConfig 获取到WIFI名和密码
	else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
		ESP_LOGI(LOG_TAG, "Got SSID and password");
		smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
		wifi_config_t wifi_config;
		uint8_t ssid[33] = { 0 };
		uint8_t password[65] = { 0 };
		bzero(&wifi_config, sizeof(wifi_config_t));
		memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
		memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
		wifi_config.sta.bssid_set = evt->bssid_set;
		if (wifi_config.sta.bssid_set == true) {
			memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
		}


		
		ESP_ERROR_CHECK( esp_wifi_disconnect() );
		
		ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
		
		ESP_ERROR_CHECK( esp_wifi_connect() );


		memcpy(ssid, evt->ssid, sizeof(evt->ssid));
		memcpy(password, evt->password, sizeof(evt->password));
		ESP_LOGI(LOG_TAG, "SSID:%s", ssid);
		ESP_LOGI(LOG_TAG, "PASSWORD:%s", password);
		// 断开默认的
		ESP_ERROR_CHECK( esp_wifi_disconnect() );
		// 设置获取的ap和密码到寄存器
		ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
		// 连接获取的ssid和密码
		ESP_ERROR_CHECK( esp_wifi_connect() );
	}
	// SmartConfig 发送应答完成
	else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
		xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);
	}
}




// SmartConfig 任务
void SmartConfig_Task(void *parm)
{
	EventBits_t uxBits;
	//使用ESP-TOUCH配置工具
	ESP_ERROR_CHECK(esp_smartconfig_set_type(SC_TYPE_ESPTOUCH));
	//开始SmartConfig
	smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );
	while (1){
		//死等事件组：CONNECTED_BIT | ESPTOUCH_DONE_BIT
		uxBits = xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
		//SmartConfig结束
		if (uxBits & ESPTOUCH_DONE_BIT){
			ESP_LOGI(LOG_TAG, "smartconfig over");
			esp_smartconfig_stop();
			//此处smartconfig结束，并且连上设置的AP，以下可以展开自定义工作了
			vTaskDelete(NULL);
		}
		if (uxBits & CONNECTED_BIT){//连上ap
			ESP_LOGI(LOG_TAG, "WiFi Connected to ap");
		}
	}
}


void app_main(void)
{
    ESP_ERROR_CHECK( nvs_flash_init() );

	ESP_LOGI(LOG_TAG, "APP Start\n");
	printf("\n\n-------------------------------- Get Systrm Info Start------------------------------------------\n");
	//获取IDF版本
	printf("     SDK version:%s\n", esp_get_idf_version());
	//获取芯片可用内存
	printf("     esp_get_free_heap_size : %d  \n", esp_get_free_heap_size());
	//获取从未使用过的最小内存
	printf("     esp_get_minimum_free_heap_size : %d  \n", esp_get_minimum_free_heap_size());
	//获取mac地址（station模式）
	uint8_t mac[6];
	esp_read_mac(mac, ESP_MAC_WIFI_STA);
	printf("esp_read_mac(): %02x:%02x:%02x:%02x:%02x:%02x \n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	printf("\n\n-------------------------------- Get Systrm Info End------------------------------------------\n");

	ESP_ERROR_CHECK(esp_netif_init());
	s_wifi_event_group = xEventGroupCreate();
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
	assert(sta_netif);
	// wifi设置:默认设置，等待SmartConfig配置
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	// 注册wifi事件
	ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL) );
	ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL) );
	ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL) );
	// 设置sta模式
	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
	// 启动wifi
	ESP_ERROR_CHECK( esp_wifi_start() );

}



