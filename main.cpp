#include "include/water_simulation.h"

#ifdef FMT_VERSION
#include <fmt/color.h>
#include <fmt/core.h>
#else
#include <iostream>
#endif
#include <chrono>
#include <thread>

int main()
{
#ifdef FMT_VERSION
    fmt::print(fmt::emphasis::bold | fg(fmt::color::blue), "Enter the target water height (in meters from 0 to 900): ");
#else
    std::cout << "Enter the target water height (in meters from 0 to 900): ";
#endif

    double targetWaterHeight = 150.0;
    std::cin >> targetWaterHeight;

    if (std::cin.fail())
    {
        targetWaterHeight = 150.0;
#ifdef FMT_VERSION
        fmt::print(fmt::emphasis::bold | fg(fmt::color::red), "Invalid input, targetWaterHeight will default to: {}\n",
                   targetWaterHeight);
#else
        std::cerr << "Invalid input, targetWaterHeight will default to: " << targetWaterHeight << std::endl;
#endif
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    double kp = 1.0;
    double ki = 0.1;
    double kd = 0.01;

    my::WaterSimulation* sim = my::WaterSimulation::getInstance(targetWaterHeight, kp, ki, kd);

    double finalRate = sim->runSimulation();

#ifdef FMT_VERSION
    fmt::print(fmt::emphasis::bold | fg(fmt::color::green), "Final output rate: {:.2f} m³/s\n", finalRate);
#else
    std::cout << "Final output rate: " << finalRate << " m³/s" << std::endl;
#endif

    return 0;
}
