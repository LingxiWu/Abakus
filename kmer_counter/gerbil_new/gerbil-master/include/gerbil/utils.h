#ifndef TYPES_UTILS_
#define TYPES_UTILS_

#include <sys/time.h>

struct SimpleTimer {
  timeval tv1, tv2;
  long long time_elapsed;

  SimpleTimer() { reset(); }
  void reset() { time_elapsed = 0; }
  void start() { gettimeofday(&tv1, nullptr); }
  double endTimer() { gettimeofday(&tv2, nullptr); } // might be unecessary
  void stop() {
    gettimeofday(&tv2, nullptr);
    time_elapsed += static_cast<long long>(tv2.tv_sec - tv1.tv_sec) * 1000000 +
                    tv2.tv_usec - tv1.tv_usec;
  }
  double elapsed() { return time_elapsed / 1000000.0; }

};

#endif /* TYPES_UTILS_ */