#pragma once

#include <gz/sim/System.hh>
#include <gz/sensors/Sensor.hh>
#include <gz/transport/Node.hh>

namespace dummysensor
{
  // This is the main plugin's class. It must inherit from System and at least
  // one other interface.
  // Here we use `ISystemPostUpdate`, which is used to get results after
  // physics runs. The opposite of that, `ISystemPreUpdate`, would be used by
  // plugins that want to send commands.
  class DummySensorSystem:
    public gz::sim::System,
    public gz::sim::ISystemPreUpdate,
    public gz::sim::ISystemPostUpdate
  {
    // Documentation inherited.
    // During PreUpdate, check for new sensors that were inserted
    // into simulation and create more components as needed.
    public: void PreUpdate(const gz::sim::UpdateInfo &_info,
      gz::sim::EntityComponentManager &_ecm) override;

    // Documentation inherited.
    // During PostUpdate, update the known sensors and publish their data.
    // Also remove sensors that have been deleted.
    public: void PostUpdate(const gz::sim::UpdateInfo &_info,
        const gz::sim::EntityComponentManager &_ecm) override;

    /// \brief Remove custom sensors if their entities have been removed from
    /// simulation.
    /// \param[in] _ecm Immutable reference to ECM.
    private: void RemoveSensorEntities(
        const gz::sim::EntityComponentManager &_ecm);

    /// \brief A map of custom entities to their sensors
    private: std::unordered_map<gz::sim::Entity,
        std::shared_ptr<DummySensor>> entitySensorMap;
  };
}
