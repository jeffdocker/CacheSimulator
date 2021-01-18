# CacheSimulator
C project that simulates a level 1 memory cache
Input taken in form of:
./cache <size of cache> <associativity> <cache policy used> <block size> <trace file to read from>
  The associativity is either:
    direct - which simulates directly mapped cache
    assoc - simulates fully associative cache
    assoc:n - n way associative cache with n being a power of 2
  The valid cache policy is:
    lru
