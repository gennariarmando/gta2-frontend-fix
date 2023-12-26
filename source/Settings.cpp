#include "plugin.h"
#include "Settings.h"

Settings settings;

void Settings::Read() {
    plugin::config_file config(PLUGIN_PATH("FrontendFixII.ini"));

    EnablePauseMenu = config["EnablePauseMenu"].asBool();
    EnableLoadingScreen = config["EnableLoadingScreen"].asBool();
    ClipMousePosition = config["ClipMousePosition"].asBool();
    DefaultControls = config["DefaultControls"].asInt(DEFAULT_CONTROLS_FRONTEND_FIX);

}
