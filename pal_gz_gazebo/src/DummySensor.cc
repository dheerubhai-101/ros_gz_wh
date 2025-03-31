#include "DummySensor.hh"

#include <gz/msgs/stringmsg.pb.h>

#include <gz/common/Console.hh>
#include <gz/msgs/Utility.hh>
#include <gz/sensors/Noise.hh>
#include <gz/sensors/Util.hh>

//////////////////////////////////////////////////
bool dummysensor::DummySensor::Load(const sdf::Sensor &_sdf)
{
  auto type = gz::sensors::customType(_sdf);
  if ("rfid" != type)
  {
    gzerr << "Trying to load [rfid] sensor, but got type ["
           << type << "] instead." << std::endl;
    return false;
  }

  // Load common sensor params
  gz::sensors::Sensor::Load(_sdf);

  // Advertise topic where data will be published
  this->pub = this->node.Advertise<gz::msgs::StringMsg>(this->Topic());

  if (!_sdf.Element()->HasElement("gz:rfid"))
  {
    gzdbg << "No custom configuration for [" << this->Topic() << "]"
           << std::endl;
    return true;
  }

  // Load custom sensor params
  auto customElem = _sdf.Element()->FindElement("gz:rfid");

  if (!customElem->HasElement("noise"))
  {
    gzdbg << "No noise for [" << this->Topic() << "]" << std::endl;
    return true;
  }

  sdf::Noise noiseSdf;
  noiseSdf.Load(customElem->FindElement("noise"));
  this->noise = gz::sensors::NoiseFactory::NewNoiseModel(noiseSdf);
  if (nullptr == this->noise)
  {
    gzerr << "Failed to load noise." << std::endl;
    return false;
  }

  return true;
}

//////////////////////////////////////////////////
bool dummysensor::DummySensor::Update(const std::chrono::steady_clock::duration &_now)
{
  gzdbg << "Update function called for sensor [" << this->Name() << "]" << std::endl;

  if (!this->pub.Valid())
  {
    gzerr << "Publisher is not valid for sensor [" << this->Name() << "]" << std::endl;
    return false;
  }

  gz::msgs::StringMsg msg;
  *msg.mutable_header()->mutable_stamp() = gz::msgs::Convert(_now);
  auto frame = msg.mutable_header()->add_data();
  frame->set_key("frame_id");
  frame->add_value(this->Name());

  // Debugging message content
  gzdbg << "Publishing message: [" << this->Message << "] on topic [" 
        << this->Topic() << "]" << std::endl;

  msg.set_data(this->Message);

  this->AddSequence(msg.mutable_header());
  this->pub.Publish(msg);

  return true;
}