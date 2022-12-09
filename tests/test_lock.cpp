// Test binary for the challenge

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <atomic>
#include <thread>
#include <Challenge/Lock.hpp>

using namespace Challenge;

static constexpr size_t kDefaultIteration = 100000; // number of testing iteration
static constexpr size_t kDefaultSize = 30;          // size of sync data in thread func
static constexpr size_t kDefaultNumThreads = 2;        // number of sync threads

// A list of supported algorithms
static constexpr const char* _KAlgorithms[] = {
    "prob",
    "peterson",
    "test_set",
    "test_set_sleep",
};

// I usually use boost::program_options but don't want to add additional dependencies// at this time. Those codes were copied from https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c
char* getCmdOption( char ** begin, char ** end, const std::string & option )
{
    char ** itr = std::find( begin, end, option );
    if( itr != end && ++itr != end ) {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists( char** begin, char** end, const std::string& option )
{
    return std::find( begin, end, option ) != end;
}

// A thread function for detecting any anomalies
template<typename T>
void syncThreadFunc( size_t maxIteration, uint16_t offset,
                     std::vector<uint16_t>& testData,
                     std::atomic<uint32_t>& failCount )
{
    try {
        T syncObj;
        for( size_t iteration = 0; iteration < maxIteration; ++iteration ) {
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
    catch( const std::exception& e ) {
        failCount.fetch_add( 1, std::memory_order_relaxed );
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void usage()
{
    // Print out the usage
    std::cout << "Usage: test_lock -a {algorithm} -i" \
        " {iteration} -s {sync data size} -t {number of threds}" <<
        std::endl << std::endl;

    std::cout << "Supported algorithms:" << std::endl;
    for( size_t i = 0; i < sizeof( _KAlgorithms ) / sizeof( const char* ); ++i ) {
        std::cout << _KAlgorithms[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Default iteration: " << kDefaultIteration << std::endl;
    std::cout << "Default size: " << kDefaultSize << std::endl;
    std::cout << "Default number of threads: " << kDefaultNumThreads << std::endl;
}

bool isValidAlgorithm( const std::string& algorithm )
{
    for( size_t i = 0; i < sizeof( _KAlgorithms ) / sizeof( const char* ); ++i ) {
        if( algorithm == _KAlgorithms[i] ) {
            return true;
        }
    }

    return false;
}

int main(int argc, char * argv[])
{
    if( argc == 1 || cmdOptionExists( argv, argv + argc, "-h" ) ) {
        usage();
        return -1;
    }

    std::string algorithm;
    if( auto str = getCmdOption(argv, argv + argc, "-a"); !str ) {
        // -a option is mandatory
        usage();
        return -1;
    }
    else {
        algorithm = str;
        if( !isValidAlgorithm( algorithm ) ) {
            std::cerr << "ERROR! " << "Wrong algorithm name: " << algorithm <<
                std::endl;
            return -1;
        }
    }

    size_t iteration = kDefaultIteration;
    size_t size = kDefaultSize;
    size_t numThreads = kDefaultNumThreads;

    if( auto str = getCmdOption(argv, argv + argc, "-i"); str ) {
        iteration = std::atoi( str );
    }
    if( auto str = getCmdOption(argv, argv + argc, "-s"); str ) {
        size = std::atoi( str );
    }
    if( auto str = getCmdOption(argv, argv + argc, "-t"); str ) {
        numThreads = std::atoi( str );
    }

    // Print out options
    std::cout << "algorithm: " << algorithm << ", iteration: " << iteration <<
        ", size: " << size << ", threads: " << numThreads << std::endl;

    if( iteration == 0 || size == 0 || numThreads == 0) {
        std::cerr << "ERROR! " << "Some option values are zero." << std::endl;
        return -1;
    }

    // Run a test
    std::vector<uint16_t> testData( size, 0 );
    std::atomic<uint32_t> failCount{};

    std::vector<std::thread> threads;
    for( size_t i = 0; i < numThreads; ++i )
    {
        if( algorithm == "prob" ) {
            threads.emplace_back( syncThreadFunc<ProbSync>, iteration, i * size,
                    std::ref( testData ), std::ref( failCount ) );
        }
        else if( algorithm == "peterson" ) {
            threads.emplace_back( syncThreadFunc<PetersonSync>, iteration, i * size,
                    std::ref( testData ), std::ref( failCount ) );
        }
        else {
            // It's unlikely to happen as the variable was already validated
            // at the above code.
            std::cerr << "ERROR! " << "Unknown algorithm: " << algorithm <<
                std::endl;
            return -1;
        }
    }

    for( auto& thread: threads ) {
        thread.join();
    }

    // Print out the result
    if( failCount.load() == 0 ) {
        std::cout << "Tests passed successfully." << std::endl;
    }
    else {
        std::cout << "Tests failed " << failCount.load() << " times." << std::endl;
    }

    return 0;
}

