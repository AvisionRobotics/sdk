//
//  main.cpp
//
//  Created by Serhii Semenov on 4/19/19.
//  Copyright © 2019 Avision Inc. All rights reserved.
//

#include <iostream>

#include "AvisionAPI.h"

int main(int argc, const char * argv[]) {
    avision_init();
    HttpResponse response;
//    response = avision_login_create("example@mail.com", "Bob", "Slavensky", "password", "+15012345");
//    std::cout << "*********************** avision_login_create ***********************" << std::endl
//              << response.httpHeader << std::endl << response.httpBody << std::endl << std::endl;
    
    response = avision_login("example@mail.com", "password");
    std::cout << "*********************** avision_login ***********************" << std::endl
              << response.httpHeader << std::endl << response.httpBody << std::endl << std::endl;
    
//    response = avision_logout();
//    std::cout << "*********************** avision_logout ***********************" << std::endl
//              << response.httpHeader << std::endl << response.httpBody << std::endl << std::endl;
    
//    response = avision_flightPlan_create();
//    std::cout << "*********************** avision_flightPlan_create ***********************" << std::endl
//              << response.httpHeader << std::endl << response.httpBody << std::endl << std::endl;
    
//    response = avision_flightPlan_delete(flightplan_id);
//    std::cout << "*********************** avision_flightPlan_delete ***********************" << std::endl
//              << response.httpHeader << std::endl << response.httpBody << std::endl << std::endl;
    
//    response = avision_flightPlan_get();
//    std::cout << "*********************** avision_flightPlan_get ***********************" << std::endl
//              << response.httpHeader << std::endl << response.httpBody << std::endl << std::endl;
    
    response = avision_mqtt_login("example@mail.com", "password");
    std::cout << "*********************** avision_mqtt_login ***********************" << std::endl
              << response.httpHeader << std::endl << response.httpBody << std::endl << std::endl;
    
    // curl_global_cleanup
    avision_cleanup();
    return 0;
}
