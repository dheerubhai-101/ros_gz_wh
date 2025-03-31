#pragma once

#include <gz/sensors/Sensor.hh>
#include <gz/sensors/SensorTypes.hh>
#include <gz/transport/Node.hh>
#include <gz/common/Console.hh>

#include <string>

namespace dummysensor
{   
    /// @brief Dummy Sensor to publish messages
    class DummySensor:
        public gz::sensors::Sensor
    {
        /// \brief Load the sensor with SDF parameters.
        /// \param[in] _sdf SDF Sensor parameters.
        /// \return True if loading was successful
        public: virtual bool Load(const sdf::Sensor &_sdf) override;

        /// \brief Update the sensor and generate data
        /// \param[in] _now The current time
        /// \return True if the update was successfull
        public: virtual bool Update(
          const std::chrono::steady_clock::duration &_now) override;    
        
        /// \brief Set the current postiion of the robot, so the odometer can
        /// calculate the distance travelled.
        /// \param[in] _pos Current position in world coordinates.
        public: void NewPosition(const gz::math::Vector3d &_pos);

        /// \brief Get the latest world postiion of the robot.
        /// \return The latest position given to the odometer.
        public: const gz::math::Vector3d &Position() const;
        
        // /// @brief Prints Hello World from _pose
        // /// @param _pose Pose of the robot in the world
        // public: void Print(const gz::math::Vector3d &_pose);
        
        private: std::string Message{"Hello World!"}; 

        /// \brief Noise that will be applied to the sensor data
        private: gz::sensors::NoisePtr noise{nullptr};
        
        /// \brief Node for communication
        private: gz::transport::Node node;

        /// \brief Publishes sensor data
        private: gz::transport::Node::Publisher pub;
    };

}