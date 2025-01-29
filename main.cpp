#include "include/water_simulation.h"

#if __has_include(<fmt/core.h>)
#include <fmt/color.h>
#include <fmt/core.h>
#define FMT_AVAILABLE 1
#else
#define FMT_AVAILABLE 0
#include <iostream>
#endif

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <thread>

int main()
{
#ifdef FMT_AVAILABLE
    fmt::print(fmt::emphasis::bold | fg(fmt::color::blue), "Enter the target water height (in meters from 0 to 900): ");
#else
    std::cout << "using cout" << std::endl;
    std::cout << "Enter the target water height (in meters from 0 to 900): ";
#endif

    double targetWaterHeight = 150.0;
    std::cin >> targetWaterHeight;

    if (std::cin.fail())
    {
        std::srand(static_cast<unsigned int>(std::time(0)));
        targetWaterHeight = std::rand() % (890 - 10 + 1) + 10;

#ifdef FMT_AVAILABLE
        fmt::print(fmt::emphasis::bold | fg(fmt::color::red), "Invalid input, targetWaterHeight will be: {}\n",
                   targetWaterHeight);
#else
        std::cerr << "Invalid input, targetWaterHeight will be: " << targetWaterHeight << std::endl;
#endif
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    double kp = 1.0;
    double ki = 0.1;
    double kd = 0.01;

    my::WaterSimulation* sim = my::WaterSimulation::getInstance(targetWaterHeight, kp, ki, kd);

    double finalRate = sim->runSimulation();

#ifdef FMT_AVAILABLE
    fmt::print(fmt::emphasis::bold | fg(fmt::color::green), "Final output rate: {:.2f} m³/s\n", finalRate);
#else
    std::cout << "Final output rate: " << finalRate << " m³/s" << std::endl;
#endif

    return 0;
}
