#include <memory>
#include <vector>
#include <iostream>

#include <ignition/math/Color.hh>

#include <gazebo/gazebo.hh>
#include "gazebo/common/Plugin.hh"
#include "gazebo/msgs/msgs.hh"
#include "gazebo/physics/physics.hh"
#include "gazebo/transport/transport.hh"
#include "LedPlugin.hh"

using physics::ModelPtr;
using std::string;
using namespace gazebo;
using physics::JointControllerPtr;
using ignition::math::Pose3d;
using common::Time;

namespace gazebo
{
  class LedSettingPrivate
  {
    
    public: LedSettingPrivate():
      transparency(0.2), defaultEmissiveColor(ignition::math::Color::White),
      visualExists(false)
    {
    }

    // transparency when the light is off.
    public: double transparency;

    // color of the led light.
    public: ignition::math::Color defaultEmissiveColor;

    // Send/publish a command to the visual.
    public: transport::PublisherPtr pubVisual;

    // A message holding a Visual message.
    public: msgs::Visual msg;

    // True if <visual> element exists.
    public: bool visualExists;
  };

  class LedPluginPrivate : public ModelPlugin
  {

    public: transport::NodePtr node;
    public: transport::PublisherPtr pubVisual;
    public: physics::ModelPtr model;
    public: transport::NodePtr node;
    public: transport::SubscriberPtr vel_sub;
    public: transport::SubscriberPtr stat_sub;
    public: transport::PublisherPtr  led_pub;

    LedControlPlugin() {}


    virtual void
    Load(physics::ModelPtr model, sdf::ElementPtr sdf)
    {
        this->model = model;

        if (model->GetJointCount() == 0) {
            std::cerr << "bad model loaded" << std::endl;
        }

        auto model_name = this->model->GetName();
        auto world_name = this->model->GetWorld()->Name();

        std::cerr << "hello from LedPlugin" << std::endl;
        std::cerr << "world: " << world_name << std::endl;
        std::cerr << "model: " << model_name << std::endl;

      
        this->node = transport::NodePtr(new transport::Node());
        this->node->Init(world_name);

        

        string stats_topic = "~/world_stats";
        this->stat_sub = this->node->Subscribe(
            stats_topic, &LedControlPlugin::OnStats, this);
        std::cerr << "Subscribed world_stats: "
                  << this->stat_sub->GetTopic() << std::endl;

        string pose_topic = "~/" + model_name + "/led";
        this->pose_pub = this->node->Advertise<msgs::PoseStamped>(pose_topic, 50);
        std::cerr << "Advertised pose" << std::endl;

        std::cerr << "Led plugin loaded" << std::endl;
    }
  };
}


GZ_REGISTER_MODEL_PLUGIN(LedPlugin)


LedSetting::LedSetting(
  const sdf::ElementPtr &_sdf,
  const physics::ModelPtr &_model,
  const common::Time &_currentTime)
  : FlashLightSetting(_sdf, _model, _currentTime),
  dataPtr(new LedSettingPrivate)
{
  // check if the visual element exists.
  this->dataPtr->visualExists = false;
  msgs::Link msg;
  this->Link()->FillMsg(msg);
  for (auto visualMsg : msg.visual())
  {
    if (visualMsg.name()
      == this->Link()->GetScopedName() + "::" + this->Name())
    {
      if (visualMsg.has_transparency())
      {
        this->dataPtr->transparency = visualMsg.transparency();
      }

      if (visualMsg.has_material()
        && visualMsg.material().has_emissive())
      {
        this->dataPtr->defaultEmissiveColor
          = msgs::Convert(visualMsg.material().emissive());
      }

      this->dataPtr->visualExists = true;
      break;
    }
  }
}


LedSetting::~LedSetting()
{
}


void LedSetting::InitPubVisual(const transport::PublisherPtr &_pubVisual)
{
  // The PublisherPtr
  this->dataPtr->pubVisual = _pubVisual;

  if (this->dataPtr->visualExists)
  {
    // Make a message
    this->dataPtr->msg.set_name(
      this->Link()->GetScopedName() + "::" + this->Name());
    this->dataPtr->msg.set_parent_name(this->Link()->GetScopedName());
    uint32_t id;
    this->Link()->VisualId(this->Name(), id);
    this->dataPtr->msg.set_id(id);
  }
}

//Flashes the Led light

void LedSetting::Flash()
{
  // Call the function of the parent class.
  FlashLightSetting::Flash();

  // Make the appearance brighter.
  this->dataPtr->msg.set_transparency(0.0);
  ignition::math::Color color = this->CurrentColor();
  if (color != ignition::math::Color::Black)
  {
   
    msgs::Set(this->dataPtr->msg.mutable_material()->mutable_diffuse(), color);
    msgs::Set(this->dataPtr->msg.mutable_material()->mutable_emissive(), color);
    msgs::Set(this->dataPtr->msg.mutable_material()->mutable_specular(), color);
    msgs::Set(this->dataPtr->msg.mutable_material()->mutable_ambient(), color);
  }
  else
  {
    // Otherwise, just apply the default color.
    msgs::Set(this->dataPtr->msg.mutable_material()->mutable_emissive(),
      this->dataPtr->defaultEmissiveColor);
  }

  // Send the message.
  if (this->dataPtr->visualExists)
  {
    
    if (this->Link()->GetWorld()->SimTime() > 2.0)
      this->dataPtr->pubVisual->Publish(this->dataPtr->msg);
  }
}

//Dims the LED light

void LedSetting::Dim()
{
  // Call the function of the parent class.
  FlashLightSetting::Dim();

  // Make the appearance darker.
  this->dataPtr->msg.set_transparency(this->dataPtr->transparency);
  msgs::Set(this->dataPtr->msg.mutable_material()->mutable_emissive(),
    ignition::math::Color(0, 0, 0));
  // Send the message.
  if (this->dataPtr->visualExists)
  {
   
    
    if (this->Link()->GetWorld()->SimTime() > 2.0)
      this->dataPtr->pubVisual->Publish(this->dataPtr->msg);
  }
}


LedPlugin::LedPlugin() : FlashLightPlugin(), dataPtr(new LedPluginPrivate)
{

  this->dataPtr->node = transport::NodePtr(new transport::Node());
  this->dataPtr->node->Init();


  this->dataPtr->pubVisual
    = this->dataPtr->node->Advertise<gazebo::msgs::Visual>("~/visual");
  
}


LedPlugin::~LedPlugin()
{
}


std::shared_ptr<FlashLightSetting> LedPlugin::CreateSetting(
  const sdf::ElementPtr &_sdf,
  const physics::ModelPtr &_model,
  const common::Time &_currentTime)
{
  return std::make_shared<LedSetting>(_sdf, _model, _currentTime);
}


void LedPlugin::InitSettingBySpecificData(
    std::shared_ptr<FlashLightSetting> &_setting)
{
  // Call the function of the parent class.
  FlashLightPlugin::InitSettingBySpecificData(_setting);

  std::dynamic_pointer_cast<LedSetting>(_setting)->InitPubVisual(
    this->dataPtr->pubVisual);
}
