#include "atest.h"
#include "metrics.h"

TEST(speedup_basic, "S = t1/tN")
{
    ASSERT_NEAR(4.0, speedup(100.0, 25.0), 1e-12);
    ASSERT_NEAR(1.0, speedup(7.5, 7.5), 1e-12);
}

TEST(efficiency_basic, "E = S/N")
{
    ASSERT_NEAR(0.5, efficiency(100.0, 25.0, 8), 1e-12);
    ASSERT_NEAR(1.0, efficiency(80.0, 10.0, 8), 1e-12);
}

TEST(amdahl_basic, "5 percent serial on 16 processors")
{
    /* 1 / (0.05 + 0.95/16) = 1 / 0.109375 */
    ASSERT_NEAR(9.142857142857, amdahl(0.05, 16), 1e-9);
    ASSERT_NEAR(1.0, amdahl(0.05, 1), 1e-12);
}

TEST(amdahl_limit_basic, "the ceiling is 1/s")
{
    ASSERT_NEAR(20.0, amdahl_limit(0.05), 1e-12);
    ASSERT_NEAR(4.0, amdahl_limit(0.25), 1e-12);
}

TEST(gustafson_basic, "S = N - s(N-1)")
{
    ASSERT_NEAR(972.85, gustafson(0.05, 1024), 1e-9);
    ASSERT_NEAR(1.0, gustafson(0.05, 1), 1e-12);
}

TEST(karp_flatt_basic, "recovering the serial fraction from a measurement")
{
    /* t1 = 240, t16 = 22  ->  S = 10.909..., e = 0.0312... */
    double S = speedup(240.0, 22.0);
    ASSERT_NEAR(0.0311111111, karp_flatt(S, 16), 1e-7);
}

TEST(intensity_basic, "I = W/Q")
{
    ASSERT_NEAR(0.125, arithmetic_intensity(2.0, 16.0), 1e-12);
    ASSERT_NEAR(0.25, arithmetic_intensity(1.0, 4.0), 1e-12);
}

TEST(ridge_basic, "ridge = peak / bandwidth")
{
    ASSERT_NEAR(15.0, ridge_point(3000.0, 200.0), 1e-12);
    ASSERT_NEAR(8.0, ridge_point(2000.0, 250.0), 1e-12);
}

TEST(attainable_basic, "attainable = min(peak, bw * I)")
{
    ASSERT_NEAR(25.0, attainable_gflops(3000.0, 200.0, 0.125), 1e-12);
    ASSERT_NEAR(3000.0, attainable_gflops(3000.0, 200.0, 40.0), 1e-12);
}

TEST(bound_basic, "left of the ridge is memory bound")
{
    ASSERT_EQ_INT(1, is_memory_bound(3000.0, 200.0, 0.125));
    ASSERT_EQ_INT(0, is_memory_bound(3000.0, 200.0, 40.0));
}

TEST(fraction_basic, "fraction of ATTAINABLE, not of peak")
{
    /* attainable 25, achieved 22 -> 0.88, which is nothing like 22/3000 */
    ASSERT_NEAR(0.88, roofline_fraction(3000.0, 200.0, 0.125, 22.0), 1e-9);
}

int main(void)
{
    REGISTER(speedup_basic); REGISTER(efficiency_basic); REGISTER(amdahl_basic);
    REGISTER(amdahl_limit_basic); REGISTER(gustafson_basic); REGISTER(karp_flatt_basic);
    REGISTER(intensity_basic); REGISTER(ridge_basic); REGISTER(attainable_basic);
    REGISTER(bound_basic); REGISTER(fraction_basic);
    return atest_run_all();
}
