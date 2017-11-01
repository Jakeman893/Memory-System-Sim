// Copyright 2006, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>

#include "gtest/gtest.h"
#include "../../src/types.h"
#include "../../src/cache.h"
#include "../../src/memsys.h"

#define DCACHE_HIT_LATENCY   1
#define ICACHE_HIT_LATENCY   1
#define L2CACHE_HIT_LATENCY  10
#define DRAM_LATENCY_FIXED  100

Memsys* sys;

uns64 cycle = 1;

MODE        SIM_MODE        = SIM_MODE_B;
uns64       CACHE_LINESIZE  = 64;
uns64       REPL_POLICY     = 0; // 0:LRU 1:RAND

uns64       DCACHE_SIZE     = 32*1024; 
uns64       DCACHE_ASSOC    = 8; 

uns64       ICACHE_SIZE     = 32*1024; 
uns64       ICACHE_ASSOC    = 8; 

uns64       L2CACHE_SIZE    = 1024*1024; 
uns64       L2CACHE_ASSOC   = 16;
uns64       L2CACHE_REPL    = 0;

uns64       SWP_CORE0_WAYS  = 0;

uns64       NUM_CORES       = 1;

Addr mockAddrs[] = {0x6b8b4567, 0x327b23c6, 0x643c9869, 0x66334873,
                    0x74b0dc51, 0x19495cff, 0x2ae8944a, 0x12345678,
                    0x5f00d4ce, 0x1e5c2a58, 0x7aebd230, 0x53cd4764,
                    0x06f21576, 0x7bb31cd7, 0x5b132938};

// Test initialization of cache
TEST(MemsysTests, InitFunct) {
    sys = memsys_new();
    EXPECT_FALSE(sys == NULL);
}

// Test access of empty instruction cache
TEST(MemsysTests, InstructionCacheEmpty) {
    Addr mockAddr = mockAddrs[0];
    Access_Type type = ACCESS_TYPE_IFETCH;
    uns core_id = 0;
    Cache* cache = sys->icache;
    uns64 delay = memsys_access_modeBC(sys, mockAddr, type, core_id);
    EXPECT_EQ(0, cache->sets[0].line[0].core_id);
    EXPECT_TRUE(cache->sets[0].line[0].valid);
    EXPECT_EQ(mockAddr, cache->sets[0].line[0].tag);
    EXPECT_EQ(0, cache->stat_dirty_evicts);
    EXPECT_EQ(ICACHE_HIT_LATENCY + L2CACHE_HIT_LATENCY + DRAM_LATENCY_FIXED, delay);
}

// Test access of empty data cache with no write
TEST(MemsysTests, DataCacheEmptyNoWrite) {
    Addr mockAddr = mockAddrs[0];
    Access_Type type = ACCESS_TYPE_LOAD;
    uns core_id = 0;
    Cache* cache = sys->dcache;
    uns64 delay = memsys_access_modeBC(sys, mockAddr, type, core_id);
    EXPECT_EQ(0, cache->sets[0].line[0].core_id);
    EXPECT_TRUE(cache->sets[0].line[0].valid);
    EXPECT_EQ(mockAddr, cache->sets[0].line[0].tag);
    EXPECT_FALSE(cache->sets[0].line[0].dirty);
    EXPECT_EQ(0, cache->stat_dirty_evicts);
    EXPECT_EQ(DCACHE_HIT_LATENCY + L2CACHE_HIT_LATENCY + DRAM_LATENCY_FIXED, delay);
}

// Test access of single line data cache with hit and write
TEST(MemsysTests, DataCacheHitWrite) {
    Addr mockAddr = mockAddrs[0];
    Access_Type type = ACCESS_TYPE_STORE;
    uns core_id = 0;
    Cache* cache = sys->dcache;
    uns64 delay = memsys_access_modeBC(sys, mockAddr, type, core_id);
    EXPECT_EQ(0, cache->sets[0].line[0].core_id);
    EXPECT_TRUE(cache->sets[0].line[0].valid);
    EXPECT_EQ(mockAddr, cache->sets[0].line[0].tag);
    EXPECT_TRUE(cache->sets[0].line[0].dirty);
    EXPECT_EQ(0, cache->stat_dirty_evicts);
    EXPECT_EQ(DCACHE_HIT_LATENCY, delay);
}

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    delete sys;
}
