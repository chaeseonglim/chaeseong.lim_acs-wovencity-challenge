#include <thread>
#include <array>
#include <gtest/gtest.h>
#include <Challenge/Lock.hpp>

using namespace Challenge;

// A basic test to check the basic functionality of Lock.
// It is not intended to detect any timinig sensitive issues.
TEST( Lock, SANITY_Lock )
{
    constexpr size_t kMaxIteration = 10000;
    constexpr size_t kTestDataLength = 10;

    std::array<uint16_t, kTestDataLength> testData;
    std::atomic<uint32_t> failCount{};

    auto syncThreadFunc =
        [&failCount, &testData] ( size_t maxIteration, uint16_t offset ) {
        try {
            PetersonSync syncObj;
            for( size_t iteration = 0; iteration < kMaxIteration; ++iteration ) {
                Lock lock( syncObj );
                for( size_t i = 0; i < testData.size(); ++i ) {
                    testData[i] = i + offset;
                }
                for( size_t i = 0; i < testData.size(); ++i ) {
                    if( testData[i] != i + offset) {
                        failCount.fetch_add( 1, std::memory_order_relaxed );
                    }
                }
            }
        }
        catch( ... ) { FAIL(); } };

    std::thread thread1( syncThreadFunc, kMaxIteration, 0 );
    std::thread thread2( syncThreadFunc, kMaxIteration, testData.size() );

    thread1.join();
    thread2.join();

    EXPECT_EQ( 0u, failCount.load() );
}


