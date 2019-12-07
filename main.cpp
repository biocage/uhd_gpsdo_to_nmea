/* Quick tool to pull out GPGGA NMEA strings
   from the GPSDO on the USRP.
*/

#include <uhd/utils/thread.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>
#include <uhd/types/tune_request.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <chrono>

int UHD_SAFE_MAIN(int argc, char *argv[]) {
    uhd::set_thread_priority_safe();

    std::string device_args("");

    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(device_args);

    while(! (usrp->get_mboard_sensor("gps_locked",0).to_bool()) ) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    usrp->set_clock_source("gpsdo");
    usrp->set_time_source("gpsdo");

    std::string last = "";
    while(true) {
      std::string now = usrp->get_mboard_sensor("gps_gpgga").to_pp_string();

      // Polling since I haven't found a way to only get updates.
      // We check every update to see if it's the same as
      // the last one - if not, print it out.
      if(last.compare(now) != 0) {
        std::cout << now << std::endl;
        last = now;
      }
      
      // 500 microseconds gives me CPU utilization around 10% 
      // GPGGA messages are timestamped so raising this shouldn't
      // increase clock jitter, but I haven't tested that yet.
      
      std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
    return 0;
}