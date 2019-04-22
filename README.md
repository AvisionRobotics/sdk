# Avision SDK: Cross-Platform C++ Library
This is a cross-platform C++ Library for request avisionrobotics.com REST API

# Integration
## Bash
```bash
g++ -lcurl AvisionAPI.cpp test.cpp -o testAPI
```

# Usage
Use avision methods between `avision_init()` and `avision_cleanup()`. First you need `avision_login(email, password)` or `avision_login(apikey)`:
```c
avision_init();
HttpResponse response;
response = avision_login("example@mail.com", "password");
if (response.statusCode != 200) {
    std::cout << "Error in avision_login():" << response.httpBody << std::endl;
} else {
    // call avision methods
    // ...
}

avision_cleanup();
```
For register use `avision_login_create()`.
To switch account use `avision_logout()`.

# Dependencies

- [libcurl](https://curl.haxx.se/libcurl/c/)
- [Paho MQTT C](https://github.com/eclipse/paho.mqtt.c)
