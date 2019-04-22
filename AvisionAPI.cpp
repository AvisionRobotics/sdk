//
//  AvisionAPI.cpp
//
//  Created by Serhii Semenov on 4/19/19.
//  Copyright © 2019 Avision Inc. All rights reserved.
//

#include "AvisionAPI.h"

#include <iostream>
#include <stdlib.h>
#include <curl/curl.h>

static const char* ServerApiPath = "https://avisionrobotics.com/api/";
static const char* ServerApiUserAgent = "AvisionAPI/" AvisionAPI_Version;

static const char* HttpMethodGet = "GET";
static const char* HttpMethodPut = "PUT";
static const char* HttpMethodPost = "POST";
static const char* HttpMethodDelete = "DELETE";

// this is your token
static char authorization[PATH_MAX];
static bool isLogin = false;

void avision_init() {
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
}

void avision_cleanup() {
    curl_global_cleanup();
}

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, realsize);
    return realsize;
}

HttpResponse avision_getJson(const char* httpMethod, const char* apiMethod, const char* data);

#pragma mark - Users

HttpResponse avision_login(const char* email, const char* password) {
    char data[PATH_MAX];
    sprintf(data, "{\"email\":\"%s\",\"password\":\"%s\"}", email, password);
    
    HttpResponse response = avision_getJson(HttpMethodPost, "auth/v1.0/login", data);
    if (response.statusCode == 200) {
        // parse http header and find Authorization
        const char* auth = strstr(response.httpHeader.c_str(), "Authorization");
        strncpy(authorization, auth, strchr(auth, '\n') - auth - 1);
        isLogin = true;
    }
    
    return response;
}

HttpResponse avision_login(const char* apikey) {
    char method[PATH_MAX];
    strcpy(method, "auth/v1.0/login/");
    strcat(method, apikey);
    
    HttpResponse response = avision_getJson(HttpMethodGet, method, 0);
    if (response.statusCode == 200) {
        // parse http header and find Authorization
        const char* auth = strstr(response.httpHeader.c_str(), "Authorization");
        strncpy(authorization, auth, strchr(auth, '\n') - auth - 1);
        isLogin = true;
    }
    return response;
}

HttpResponse avision_login_create(const char* email,
                                  const char* firstName,
                                  const char* lastName,
                                  const char* password,
                                  const char* phone) {
    char data[PATH_MAX];
    sprintf(data, "{\"email\":\"%s\",\"firstName\":\"%s\",\"lastName\":\"%s\",\"password\":\"%s\",\"phone\":\"%s\"}", email, firstName, lastName, password, phone);
    
    return avision_getJson(HttpMethodPost, "users/v1.0/create", data);
}

HttpResponse avision_logout(void) {
    HttpResponse response = avision_getJson(HttpMethodPost, "auth/v1.0/logout", 0);
    isLogin = false;
    authorization[0] = 0; // fast set empty C-string
    return response;
}

#pragma mark - Drones

HttpResponse avision_drone_create(const char* name,
                                  const char* manufacturer,
                                  const char* model,
                                  const char* serialNumber,
                                  const char* autopilotManufacturer,
                                  const char* autopilotModel,
                                  const char* gcsProgram,
                                  const char* commSystem) {
    char data[PATH_MAX];
    sprintf(data, "{\"name\":\"%s\",\"manufacturer\":\"%s\",\"model\":\"%s\",\"serialNumber\":\"%s\",\"autopilotManufacturer\":\"%s\",\"autopilotModel\":\"%s\",\"gcsProgram\":\"%s\",\"commSystem\":\"%s\"}", name, manufacturer, model, serialNumber, autopilotManufacturer, autopilotModel, gcsProgram, commSystem);
    
    return avision_getJson(HttpMethodPost, "drones/v1.0", data);
}

HttpResponse avision_drone_delete(const char* drone_Id) {
    char method[PATH_MAX];
    strcpy(method, "drones/v1.0/");
    strcat(method, drone_Id);
    return avision_getJson(HttpMethodDelete, method, 0);
}

#pragma mark - Flight plans

HttpResponse avision_flightPlan_create(void) {
    return avision_getJson(HttpMethodPost, "flightproject/v1.0", "{}");
}

HttpResponse avision_flightPlan_update(const char* flightplan_id, const char* flightplan_json) {
    char method[PATH_MAX];
    strcpy(method, "flightproject/v1.0/");
    strcat(method, flightplan_id);
    return avision_getJson(HttpMethodPut, method, flightplan_json);
}

HttpResponse avision_flightPlan_delete(const char* flightplan_id) {
    char method[PATH_MAX];
    strcpy(method, "flightproject/v1.0/");
    strcat(method, flightplan_id);
    return avision_getJson(HttpMethodDelete, method, 0);
}

HttpResponse avision_flightPlan_get(void) {
    return avision_getJson(HttpMethodGet, "flightproject/v1.0", 0);
}

#pragma mark - Flights

HttpResponse avision_flight_start(const char* flightplan_id, const char* drone_Id) {
    char method[PATH_MAX];
    sprintf(method, "flightproject/v1.0/%s/%s/start", flightplan_id, drone_Id);
    return avision_getJson(HttpMethodPost, method, "{}");
}

HttpResponse avision_flight_stop(const char* flightplan_id) {
    char method[PATH_MAX];
    sprintf(method, "flightproject/v1.0/%s/stop", flightplan_id);
    return avision_getJson(HttpMethodPost, method, "{}");
}

#pragma mark - Telemetry

HttpResponse avision_telemetry_send(const char* flightplan_id, const char* telemetry_json) {
    char method[PATH_MAX];
    sprintf(method, "flightproject/v1.0/%s/telemetry", flightplan_id);
    return avision_getJson(HttpMethodPut, method, telemetry_json);
}

HttpResponse avision_telemetry_offnominal(const char* flightplan_id, const char* roleResponsibility, const char* reportingNarrative, const char* reasonForReport) {
    char method[PATH_MAX];
    strcpy(method, "report/v1.0/offnominal/");
    strcat(method, flightplan_id);
    
    char data[PATH_MAX];
    sprintf(data, "{\"roleResponsibility\":\"%s\",\"reportingNarrative\":\"%s\",\"reasonForReport\":\"%s\"}", roleResponsibility, reportingNarrative, reasonForReport);
    return avision_getJson(HttpMethodPost, method, data);
}

#pragma mark - MQTT

HttpResponse avision_mqtt_login(const char* login, const char* password) {
    HttpResponse response;
    return response;
}

#pragma mark - cURL

HttpResponse avision_getJson(const char* httpMethod, const char* apiMethod, const char* data) {
    CURL *curl;
    CURLcode res;
    HttpResponse response;
    
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *hs=NULL;
        char url[PATH_MAX];
        strcpy(url, ServerApiPath);
        strcat(url, apiMethod);
        /* no progress meter please */
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, ServerApiUserAgent);
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip, deflate");
        
        if (isLogin) {
            hs = curl_slist_append(hs, authorization);
        }
        
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, httpMethod);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        /* we want to use our own write function */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        /* pointer to pass to our write function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response.httpBody);
        
        /* headers to pass to our write function */
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&response.httpHeader);
        
        if (data) {
            hs = curl_slist_append(hs, "Content-Type: application/json");
            
            /* Now specify the POST data */
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));
        }
        
        if (hs) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);
        }
        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.statusCode);
        
        /* always cleanup */
        if (hs) {
            curl_slist_free_all(hs);
        }
        curl_easy_cleanup(curl);
        
    }
    
    return response;
}
