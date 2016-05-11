#ifndef CMMC_OTA_H
#define CMMC_OTA_H

#include <Arduino.h>
#include <functional>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

#ifdef CMMC_OTA_DEBUG
    #define CMMC_OTA_PRINTER Serial
    #define CMMC_OTA_PRINT(...) { CMMC_OTA_PRINTER.print(__VA_ARGS__); }
    #define CMMC_OTA_PRINTLN(...) { CMMC_OTA_PRINTER.println(__VA_ARGS__); }
#else
    #define CMMC_OTA_PRINT(...) { }
    #define CMMC_OTA_PRINTLN(...) { }
#endif


class CMMC_OTA {
  private:
    bool _initialised = false;
  public:
    OTA_CALLBACK(_user_on_start_callback) = NULL;
    OTA_CALLBACK(_user_on_end_callback) = NULL;
    OTA_CALLBACK_ERROR(_user_on_error_callback) = NULL;
    OTA_CALLBACK_PROGRESS(_user_on_progress_callback) = NULL;

    CMMC_OTA* init() {
      static CMMC_OTA* s_instance = this;
      if (!_initialised)  {
        ArduinoOTA.onStart([]() {
          if (s_instance->_user_on_start_callback != NULL) {
            s_instance->_user_on_start_callback();
          }
        });

        ArduinoOTA.onEnd([]() {
          if (s_instance->_user_on_end_callback != NULL) {
            s_instance->_user_on_end_callback();
          }
        });

        ArduinoOTA.onProgress([&](unsigned int progress, unsigned int total) {
          if (s_instance->_user_on_progress_callback != NULL) {
            s_instance->_user_on_progress_callback(progress, total);
          }
        });

        ArduinoOTA.onError([](ota_error_t error) {
          if (s_instance->_user_on_error_callback != NULL) {
            s_instance->_user_on_error_callback(error);
          }
        });

        ArduinoOTA.begin();
        _initialised = true;
      }
      return s_instance;
    }

    void on_start(OTA_CALLBACK(fn)) {
      _user_on_start_callback = fn;
    }

    void on_end(OTA_CALLBACK(fn)) {
      _user_on_end_callback = fn;
    }

    void on_progress(OTA_CALLBACK_PROGRESS(fn)) {
      _user_on_progress_callback = fn;
    }

    void on_error(OTA_CALLBACK_ERROR(fn)) {
      _user_on_error_callback = fn;
    }

    CMMC_OTA();
    ~CMMC_OTA();

    void loop() {
      if (_initialised) {
        ArduinoOTA.handle();
      }
    }
};

// CMMC_OTA* CMMC_OTA::s_instance = NULL;

#endif /* CMMC_OTA */
