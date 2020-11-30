#ifndef GAZEBO_PLUGINS_LEDPLUGIN_HH_
#define GAZEBO_PLUGINS_LEDPLUGIN_HH_

#include <memory>

#include "gazebo/common/Plugin.hh"
#include "gazebo/physics/physics.hh"

#include "FlashLightPlugin.hh"

namespace gazebo
{
  // forward declaration
  class LedSettingPrivate;

  // Internal data class to hold individual LED light settings.
  class GZ_PLUGIN_VISIBLE LedSetting: public FlashLightSetting
  {
    // Constructor.
    public: LedSetting(
      const sdf::ElementPtr &_sdf,
      const physics::ModelPtr &_model,
      const common::Time &_currentTime);

    // Destructor.
    public: virtual ~LedSetting();

    
    public: virtual void InitPubVisual(
      const transport::PublisherPtr &_pubVisual) final;

    // Documentation inherited.
    protected: virtual void Flash();

    // Documentation inherited.
    protected: virtual void Dim();

    /// \brief Pointer to private data
    private: std::unique_ptr<LedSettingPrivate> dataPtr;
  };

  // forward declaration
  class LedPluginPrivate;

  
  class GZ_PLUGIN_VISIBLE LedPlugin : public FlashLightPlugin
  {

    public: LedPlugin();


    public: virtual ~LedPlugin();


    protected: virtual std::shared_ptr<FlashLightSetting> CreateSetting(
      const sdf::ElementPtr &_sdf,
      const physics::ModelPtr &_model,
      const common::Time &_currentTime);


    protected: virtual void InitSettingBySpecificData(
        std::shared_ptr<FlashLightSetting> &_setting);

    /// \brief Pointer to private data
    private: std::unique_ptr<LedPluginPrivate> dataPtr;
  };
}
#endif
