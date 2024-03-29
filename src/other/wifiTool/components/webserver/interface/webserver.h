/**
 * @file webserver.h
 * @author risinek (risinek@gmail.com)
 * @date 2021-04-05
 * @copyright Copyright (c) 2021
 * 
 * @brief Provides interface to control and communicate with Webserver component
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "esp_event.h"
#include "esp_http_server.h"

extern httpd_config_t config;
extern httpd_handle_t server;
extern httpd_uri_t uri_root_get;
extern httpd_uri_t uri_reset_head;
extern httpd_uri_t uri_ap_list_get;
extern httpd_uri_t uri_run_attack_post;
extern httpd_uri_t uri_status_get;
extern httpd_uri_t uri_capture_pcap_get;
extern httpd_uri_t uri_capture_hccapx_get;

ESP_EVENT_DECLARE_BASE(WEBSERVER_EVENTS);
enum {
    WEBSERVER_EVENT_ATTACK_REQUEST,
    WEBSERVER_EVENT_ATTACK_RESET
};

/**
 * @brief Struct to deserialize attack request parameters 
 * 
 */
typedef struct {
    uint8_t ap_record_id;   //< ID of chosen AP. It can be used to access ap_records array from wifi_controller - ap_scanner
    uint8_t type;           //< Chosen type of attack
    uint8_t method;         //< Chosen method of attack
    uint8_t timeout;        //< Attack timeout in seconds
} attack_request_t;

/**
 * @brief Initializes and starts webserver 
 */
void webserver_run();

#endif
