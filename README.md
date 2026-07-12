# haproxy-spoe-cpp

**Status: early development.** `haproxy-spoe-cpp` is a C++20/23 library for
building SPOA (Stream Processing Offload Agent) servers that speak SPOP with
HAProxy. It targets pure standard C++ with zero external dependencies — no
Boost, no third-party libraries. The wire-format codecs and reactor are being
built incrementally.
