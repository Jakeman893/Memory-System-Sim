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

Cache* cache;

uns64 cycle = 1;

// Test initialization of cache
TEST(CacheTests, InitFunct) {
    cache = cache_new(32 * 1024, 8, 64, 0);
    EXPECT_FALSE(cache == NULL);
}

// Check cache_access when empty
TEST(CacheTests, EmptyCacheAccess) {
    Addr mockAddr = 0x123456789ABCDEF;
    Flag is_write = FALSE;
    uns core_id = 0;
    Flag result = cache_access(cache, mockAddr, is_write, core_id);
    EXPECT_EQ(MISS, result);
}

// Test insert of cache line
TEST(CacheTests, LineInsert) {
    Addr mockAddr = 0x123456789ABCDEF;
    mockAddr = mockAddr/cache->num_ways;
    Flag is_write = FALSE;
    uns core_id = 0;
    cache_install(cache, mockAddr, is_write, core_id);
    EXPECT_EQ(0, cache->sets->line[0].core_id);
    EXPECT_TRUE(cache->sets->line[0].valid);
    EXPECT_EQ(mockAddr, cache->sets->line[0].tag);
}

// Test cache_access with one entry

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    delete cache;
}
