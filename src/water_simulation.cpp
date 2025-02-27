#include "../include/water_simulation.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

namespace my {
    WaterSimulation* WaterSimulation::instance = nullptr;

    WaterSimulation::WaterSimulation(double TargetWaterHeight, double inputKp, double inputKi, double inputKd)
        : Kp_(inputKp),
          Ki_(inputKi),
          Kd_(inputKd),
          m_currentWaterHeight(Constants::InitialWaterHeight),
          m_simulationTime(0.0),
          m_waterInputRate(0.7),
          m_waterOutputRate(0.0),
          m_targetWaterHeight(TargetWaterHeight),
          pid(Kp_, Ki_, Kd_, 0.0, computeMaxInputRate())
    {
        if (m_targetWaterHeight < 0 || m_targetWaterHeight > Constants::MaxPoolHeight)
        {
            m_targetWaterHeight = 750.0;
#ifdef FMT_AVAILABLE
            fmt::print(fmt::emphasis::bold | fg(fmt::color::red),
                       "Invalid input. Using default target height of {} meters.\n", m_targetWaterHeight);
#else
            std::cerr << "Invalid input. Using default target height of " << m_targetWaterHeight << " meters."
                      << std::endl;
#endif
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    WaterSimulation* WaterSimulation::getInstance(double TargetWaterHeight, double inputKp, double inputKi,
                                                  double inputKd)
    {
        if (instance == nullptr)
        {
            instance = new WaterSimulation(TargetWaterHeight, inputKp, inputKi, inputKd);
        }
        return instance;
    }

    double WaterSimulation::runSimulation()
    {
        std::cout << "m_targetWaterHeight: " << m_targetWaterHeight << std::endl;

        while (Constants::MaxSimulationTime > m_simulationTime)
        {
            m_waterOutputRate = computeWaterOutputRate();
            m_waterInputRate  = pid.compute(m_targetWaterHeight, m_currentWaterHeight, Constants::TimeStep);

            if (m_waterInputRate < 0)
            {
                m_waterInputRate = 0;
            }

            m_currentWaterHeight += (m_waterInputRate - m_waterOutputRate) * Constants::TimeStep;

            if (m_currentWaterHeight > Constants::MaxPoolHeight)
            {
                m_currentWaterHeight = Constants::MaxPoolHeight;
            }

            if (m_currentWaterHeight < 0)
            {
                m_currentWaterHeight = 0;
            }

#ifdef FMT_AVAILABLE
            fmt::print("Current time: {} seconds, Water height: {} meters, Water input rate: {} m^3/s\n",
                       m_simulationTime, m_currentWaterHeight, m_waterInputRate);
#else
            std::cout << "Current time: " << m_simulationTime << " seconds, Water height: " << m_currentWaterHeight
                      << " meters, Water input rate: " << m_waterInputRate << " m^3/s" << std::endl;
#endif

            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            m_simulationTime += Constants::TimeStep;
        }

#ifdef FMT_AVAILABLE
        fmt::print(fmt::emphasis::bold | fg(fmt::color::green),
                   "Max simulation time reached. Final water height: {} meters with water input rate: {} m^3/s\n",
                   m_currentWaterHeight, m_waterInputRate);
#else
        std::cout << "Max simulation time reached. Final water height: " << m_currentWaterHeight
                  << " meters with water input rate: " << m_waterInputRate << " m^3/s" << std::endl;
#endif

        return m_waterInputRate;
    }

} // namespace my