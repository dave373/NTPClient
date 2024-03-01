#pragma once

#include "Arduino.h"

#include <Udp.h>

#define SEVENZYYEARS 2208988800UL
#define NTP_PACKET_SIZE 48
#define NTP_DEFAULT_LOCAL_PORT 1337
#define LEAP_YEAR(Y)     ( (Y>0) && !(Y%4) && ( (Y%100) || !(Y%400) ) )


struct tm_struct {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
};

class NTPClient {
  private:
    
    UDP*          _udp;
    bool          _udpSetup       = false;

    const char*   _poolServerName = "pool.ntp.org"; // Default time server
    int           _port           = NTP_DEFAULT_LOCAL_PORT;
    int           _timeOffset     = 0;

    unsigned long _updateInterval = 60000;  // In ms

    unsigned long _currentEpoc    = 0;      // In s
    unsigned long _lastUpdate     = 0;      // In ms

    byte          _packetBuffer[NTP_PACKET_SIZE];

    void          sendNTPPacket();
    bool          isValid(byte * ntpPacket);

  public:
    NTPClient(UDP& udp);
    NTPClient(UDP& udp, int timeOffset);
    NTPClient(UDP& udp, const char* poolServerName);
    NTPClient(UDP& udp, const char* poolServerName, int timeOffset);
    NTPClient(UDP& udp, const char* poolServerName, int timeOffset, unsigned long updateInterval);

    /**
     * Starts the underlying UDP client with the default local port
     */
    void begin();

    /**
     * Starts the underlying UDP client with the specified local port
     */
    void begin(int port);

    /**
     * This should be called in the main loop of your application. By default an update from the NTP Server is only
     * made every 60 seconds. This can be configured in the NTPClient constructor.
     *
     * @return true on success, false on failure
     */
    bool update();

    /**
     * This will force the update from the NTP Server.
     *
     * @return true on success, false on failure
     */
    bool forceUpdate();

    /** 
     * Has the time been set since starting? This done not guarantee a correct time, but it probably is.
    */
    bool isTimeSet();

    /**
     * Get a time struct of all values
     * @param tm_struct output time, 
     * @param unsignedlong secs epoch time to process
     * 
     * @return time in millis to process
    */
    int getTime(tm_struct *tm, int secs);

    /**
    * @return Year  YYYY
    */
    int getYear();
    
    /***
    * @return the month 1-12 = Jan-dec
    */
    int getMonth();

    /***
    * @return date of month 1-31
    */
    int getDayOfMonth();

    /**
    * @return day of week 0->6 = Sunday->Saturday
    */
    int getDayOfWeek();

    /**
    * @return Hour 0-23  
    */
    int getHours();

    /** 
    * @return Minute 0-59
    */
    int getMinutes();

    
    /** 
     * @return Seconds 0-59
    */
    int getSeconds();

    /**
     * Changes the time offset. Useful for changing timezones dynamically
     */
    void setTimeOffset(int timeOffset);

    /**
     * Set the update interval to another frequency. E.g. useful when the
     * timeOffset should not be set in the constructor, 
     * @param updateInterval in milliseconds
     */
    void setUpdateInterval(unsigned long updateInterval);

    /**
     * Return a formatted string like `hh:mm:ss` if use_ampm is false,
    *   or 'hh:mm:ss(am/pm)'
    * @param secs alternative epoch (or 0 for current time) 
    * @param use_ampm Provide 12 hour output when true
    * @return String formatted time
    */
    String getFormattedTime(unsigned long secs = 0, bool use_ampm = false);

    /**
     * @return time in seconds since Jan. 1, 1970 with timezone offset applied
     */
    unsigned long getEpochTime();
  
    /**
    * @param secs alternative epoch time (or 0 for current date) 
    * 
    * @return String formatted to ISO 8601 like `2004-02-12T15:19:21+00:00`
    */
    String getISODate(unsigned long secs = 0);

    /**
     * @return alternative epoch time (or 0 for current date) 
     * @return formatted to Australian/UK like `DD/MM/YYYY` 
     *    (None of this silly MM-DD-YYYY gibberish)
    */
    String getFormattedDate(unsigned long secs = 0);

    /**
     * Stops the underlying UDP client
     */
    void end();

    /**
    * Replace the NTP-fetched time with seconds since Jan. 1, 1970
    * @param secs seconds since epoch
    */
    void setEpochTime(unsigned long secs);
};
