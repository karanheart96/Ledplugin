#ifndef GAZEBO_PLUGINS_FLASHLIGHTPLUGIN_HH_
#define GAZEBO_PLUGINS_FLASHLIGHTPLUGIN_HH_

#include <memory>
#include <string>

#include <ignition/math/Color.hh>

#include "gazebo/common/Plugin.hh"
#include "gazebo/physics/physics.hh"

namespace gazebo
{
  // forward declaration
  class FlashLightSettingPrivate;

  
  class GZ_PLUGIN_VISIBLE FlashLightSetting
  {
    
    public: FlashLightSetting(
      const sdf::ElementPtr &_sdf,
      const physics::ModelPtr &_model,
      const common::Time &_currentTime);

    /// \brief Destructor.
    public: virtual ~FlashLightSetting();

    
    public: virtual void InitPubLight(
      const transport::PublisherPtr &_pubLight) final;

                          
    public:
      virtual void UpdateLightInEnv(const common::Time &_currentTime) final;

    
    public: virtual const std::string Name() const final;


    public: virtual const physics::LinkPtr Link() const final;


    public: virtual void SwitchOn() final;


    public: virtual void SwitchOff() final;

 
    public: virtual void SetDuration(
      const double _duration, const int _index) final;

   
    public: virtual void SetDuration(const double _duration) final;

    
    public: virtual void SetInterval(
      const double _interval, const int _index) final;


    public: virtual void SetInterval(const double _interval) final;

   
    public: virtual void SetColor(
      const ignition::math::Color &_color, const int _index) final;

   
    public: virtual void SetColor(const ignition::math::Color &_color) final;

   
    public: virtual unsigned int BlockCount() final;

    
    public: virtual bool RemoveBlock(const int _index) final;

    
    public: virtual void InsertBlock(
      const double _duration, const double _interval,
      const ignition::math::Color &_color, const int _index) final;

   
    protected: virtual void Flash();


    protected: virtual void Dim();

   
    protected: virtual ignition::math::Color CurrentColor() final;

    /// \brief Pointer to private data
    private: std::unique_ptr<FlashLightSettingPrivate> dataPtr;
  };


  // forward declaration
  class FlashLightPluginPrivate;

  
  class GZ_PLUGIN_VISIBLE FlashLightPlugin : public ModelPlugin
  {
    // brief Constructor.
    public: FlashLightPlugin();

    // brief Destructor.
    public: virtual ~FlashLightPlugin();

    // Documentation inherited.
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf) override;

    // brief Called by the world update start event
    protected: virtual void OnUpdate();

    
    protected: virtual bool TurnOn(const std::string &_lightName) final;

   
    protected: virtual bool TurnOn(
      const std::string &_lightName, const std::string &_linkName) final;

    
    protected: virtual bool TurnOnAll() final;

    
    protected: virtual bool TurnOff(const std::string &_lightName) final;

   
    protected: virtual bool TurnOff(
      const std::string &_lightName, const std::string &_linkName) final;

    
    protected: virtual bool TurnOffAll() final;

   
    protected: virtual bool ChangeDuration(
      const std::string &_lightName, const std::string &_linkName,
      const double _duration, const int _index) final;

   
    protected: virtual bool ChangeDuration(
      const std::string &_lightName, const std::string &_linkName,
      const double _duration) final;

    
    protected: virtual bool ChangeInterval(
      const std::string &_lightName, const std::string &_linkName,
      const double _interval, const int _index) final;

    
    protected: virtual bool ChangeInterval(
      const std::string &_lightName, const std::string &_linkName,
      const double _interval) final;

    
    protected: virtual bool ChangeColor(
      const std::string &_lightName, const std::string &_linkName,
      const ignition::math::Color &_color, const int _index) final;

   
    protected: virtual bool ChangeColor(
      const std::string &_lightName, const std::string &_linkName,
      const ignition::math::Color &_color) final;

    
    protected: virtual std::shared_ptr<FlashLightSetting> CreateSetting(
      const sdf::ElementPtr &_sdf,
      const physics::ModelPtr &_model,
      const common::Time &_currentTime);

   
    protected:
      virtual void InitSettingBySpecificData(
        std::shared_ptr<FlashLightSetting> &_setting);

    
    private: std::unique_ptr<FlashLightPluginPrivate> dataPtr;
  };
}
#endif
