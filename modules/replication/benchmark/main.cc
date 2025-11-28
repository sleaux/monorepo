/**
 * Run:
         bazel run //benchmark:main -c opt
 */
#include <benchmark/benchmark.h>

#include <string>

void SomeFunction(size_t size) {
    std::string s1(size, '-');
    std::string s2(size, '-');
    benchmark::DoNotOptimize(s1.compare(s2));
}

static void BM_SomeFunction(benchmark::State& state) {
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        SomeFunction(state.range(0));
    }
}

BENCHMARK(BM_SomeFunction)->RangeMultiplier(2)->Range(1 << 10, 1 << 20);

// Run the benchmark
BENCHMARK_MAIN();
