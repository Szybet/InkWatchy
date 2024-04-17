#include "config.h"

#if WIFI_TOOL
#include "wifiTool.h"
#include <stdio.h>
#ifdef LOG_LOCAL_LEVEL
#undef LOG_LOCAL_LEVEL
#endif
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
#include "esp_event.h"

extern "C"
{
#include "attack.h"
#include "wifi_controller.h"
#include "webserver.h"
}

bool wifiToolRunning = false;
static const char *TAG = "main";

void initWifiTool()
{
    debugLog("Running initWifiTool");
    wifiToolRunning = true;
    setCpuMhz(maxSpeed);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    ESP_LOGD(TAG, "app_main started");
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifictl_mgmt_ap_start();
    attack_init();
    webserver_run();
    setButton(LongBack);
}

void stopWifiTool()
{
    debugLog("Running stopWifiTool");
    // Doesnt work so we just go to sleep
    
    //ESP_ERROR_CHECK(esp_timer_stop(attack_timeout_handle));
    //ESP_ERROR_CHECK(esp_timer_delete(attack_timeout_handle));

    //ESP_ERROR_CHECK(esp_event_handler_unregister(WEBSERVER_EVENTS, WEBSERVER_EVENT_ATTACK_REQUEST, &attack_request_handler));
    //ESP_ERROR_CHECK(esp_event_handler_unregister(WEBSERVER_EVENTS, WEBSERVER_EVENT_ATTACK_RESET, &attack_reset_handler));

    //ESP_ERROR_CHECK(esp_event_loop_delete_default());

    //ESP_ERROR_CHECK(httpd_unregister_uri_handler(server, uri_root_get.uri, uri_root_get.method));
    //ESP_ERROR_CHECK(httpd_unregister_uri_handler(server, uri_reset_head.uri, uri_reset_head.method));
    //ESP_ERROR_CHECK(httpd_unregister_uri_handler(server, uri_ap_list_get.uri, uri_ap_list_get.method));
    //ESP_ERROR_CHECK(httpd_unregister_uri_handler(server, uri_run_attack_post.uri, uri_run_attack_post.method));
    //ESP_ERROR_CHECK(httpd_unregister_uri_handler(server, uri_status_get.uri, uri_status_get.method));
    //ESP_ERROR_CHECK(httpd_unregister_uri_handler(server, uri_capture_pcap_get.uri, uri_capture_pcap_get.method));
    //ESP_ERROR_CHECK(httpd_unregister_uri_handler(server, uri_capture_hccapx_get.uri, uri_capture_hccapx_get.method));
    //ESP_ERROR_CHECK(httpd_stop(&server));

    wifictl_ap_stop();

    wifiToolRunning = false;
    setCpuMhz(minimalSpeed);
    setButton(LongBack);
    sleepDelayMs = sleepDelayMs + SLEEP_EVERY_MS;
}

String wifiToolStatus()
{
    if (wifiToolRunning == true)
    {
        return "running";
    }
    else
    {
        return "not running";
    }
}

#endif
