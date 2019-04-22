//
//  AvisionAPI.h
//
//  Created by Serhii Semenov on 4/19/19.
//  Copyright © 2019 Avision Inc. All rights reserved.
//

#ifndef AvisionAPI_h
#define AvisionAPI_h

#define AvisionAPI_Version "1.0"

#include <string>

typedef struct {
    std::string httpBody;
    std::string httpHeader;
    long statusCode;
} HttpResponse;

// init native socket
void avision_init(void);

// curl_global_cleanup
void avision_cleanup(void);

//************************** Users *************************/

// Create a user with AVISION UTM platform via UI or API
// return id - User id, use it in the further queries
HttpResponse avision_login(const char* email, const char* password);

// Login to the system with login/password and get a token
// return token
HttpResponse avision_login(const char* apikey);

// Or login to the system with apikey and get a token
// return token
HttpResponse avision_login_create(const char* email, const char* firstName, const char* lastName, const char* password, const char* phone);

HttpResponse avision_logout(void);

//************************** Drones *************************/

// Create a drone
// param name - Name of the drone
// param manufacturer - Manufacturer name (DJI, 3DRobotics, Parrot etc.)
// param model - Iris, Mavic, Phantom
// param serialNumber - Drone serial number (e.g. 08RDE8F00102YM )
// param autopilotManufacturer - Manufacturer name (DJI, 3DRobotics, Parrot etc.)
// param autopilotModel - Autopilot model name (NAZA N3 etc.)
// param gcsProgram - Ground station software name (DJI Ground Station etc.)
// param commSystem - Drone communications system
// return drone_Id - Drone id obtained from the Avision platform
HttpResponse avision_drone_create(const char* name, const char* manufacturer, const char* model, const char* serialNumber, const char* autopilotManufacturer, const char* autopilotModel, const char* gcsProgram, const char* commSystem);

// Delete a drone
// param drone_Id - Drone id obtained from the Avision platform
// If successful, http status should be 200
HttpResponse avision_drone_delete(const char* drone_Id);

//************************** Flight plans *************************/

// Create a flight plan
HttpResponse avision_flightPlan_create(void);

// Update a flight plan
// flightplan_json is a json representation params
// for more details see documentation
HttpResponse avision_flightPlan_update(const char* flightplan_id, const char* flightplan_json);

// Delete a flight plan
HttpResponse avision_flightPlan_delete(const char* flightplan_id);

// Get flight plans
HttpResponse avision_flightPlan_get(void);

//************************** Flights *************************/

// Explicitly start a flight plan when the drone is ready to fly
// This call will manually initiate flight activity and all USS processes to notify UTM on the start of operation.
// Note that activity start will also be initiated automatically after the first telemetry call for a specified flight plan, however there may be a delay or decline of approval by UTM.
HttpResponse avision_flight_start(const char* flightplan_id, const char* drone_Id);

// Stop flight plan when the drone finished the flight
// This call will manually stop a flight plan and initiate all USS processes
// to close an operation. In case of telemetry not coming during 5 minutes,
// flight plan and operation both will be closed.
// If successful, http status should be 200 and json with fields:
// id, drone_Id, name, status
HttpResponse avision_flight_stop(const char* flightplan_id);

//************************** Telemetry *************************/

// Send telemetry to the system
// param flightplan_id - Flight plan id obtained from the Avision platform
// telemetry_json is a json representation params
// param timestamp - UNIX time (that is, seconds since midnight GMT on 1 Jan 1970)
// param latitude - Current drone latitude (e.g. 36.1595 for Nevada)
// param longitude - Current drone longitude (e.g. -114.9424 for Nevada)
// param altitude - Current altitude in meters above Ground Level
// param altitudeMsl - Current altitude in meters above mean sea level (ie. GPS)
// param pressure - Current pressure in hPa
// param yaw - Current course in degrees
// param pitch - Current pitch in degrees
// param roll - Current roll in degrees
// param speed - Current drone speed in m/s
// param charge - Current drone charge in percents
// If successful, http status should be 200
HttpResponse avision_telemetry_send(const char* flightplan_id, const char* telemetry_json);

// Explicitly start a off-nominal report
// param flightplan_id - Flight plan id obtained from the Avision platform
// param roleResponsibility - Reporter role and responsibility
// param reportingNarrative - Reporter narrative about the off-nominal event
// param reasonForReport - The reason for this report
// If successful, http status should be 200
HttpResponse avision_telemetry_offnominal(const char* flightplan_id, const char* roleResponsibility, const char* reportingNarrative, const char* reasonForReport);

//************************** MQTT (Mostly done, details in progress) *************************/

// Connect to the MQTT server
// param login - Avision platform user login
// param password - Token obtained from the Avision platform
HttpResponse avision_mqtt_login(const char* login, const char* password);

// Telemetry
// param flightplan_id - Flight plan id obtained from the Avision platform
// telemetry_json is a json representation params
// param timestamp - UNIX time (that is, seconds since midnight GMT on 1 Jan 1970)
// param latitude - Current drone latitude (e.g. 36.1595 for Nevada)
// param longitude - Current drone longitude (e.g. -114.9424 for Nevada)
// param altitude - Current altitude in meters above Ground Level
// param altitudeMsl - Current altitude in meters above mean sea level (ie. GPS)
// param pressure - Current pressure in hPa
HttpResponse avision_mqtt_telemetry(const char* flightplan_id, const char* telemetry_json);

// Alerts
// param login - Avision platform user login
// param timestamp - UNIX time (that is, seconds since midnight GMT on 1 Jan 1970)
// param id - Drone id obtained from the Avision platform in case of COMMAND or area id in case of AREA
// param type - Message type (e.g. COMMAND, AREA, POSITION etc.)
// param command - Command to the drone (RTB, LAND, LOITER, INFO)
// param altitude - Command altitude in meters above Ground Level
// param startDate - Start date change
// param endDate - End date change
// param geometry - GeoJSON Polygon area of the specified command (Could be a point, closed polygon or a linestring geojson.)
// param text - Message description
HttpResponse avision_mqtt_message(const char* login, const char* message_json);

#endif /* AvisionAPI_h */
