//
// Created by alireza on 21/07/22.
//

#ifndef SCOT_SRC_SOLVER_SETTINGS_H_
#define SCOT_SRC_SOLVER_SETTINGS_H_
#include "Environment.h"
namespace Scot {

using MapStringSetting = std::map<std::string, std::string>;
using MapDoubleSetting = std::map<std::string, double>;
class Settings {
 public:
  void setStrSetting(std::string setting, std::string value) {
    map_string_setting_[setting] = value;
  }

  void setDblSetting(std::string setting, double value) {
    map_double_setting_[setting] = value;
  }

  std::string getStrSetting(std::string key) {
    return map_string_setting_[key];
  }

  double getDblSetting(std::string key) {
    return map_double_setting_[key];
  }

 private:
  MapStringSetting map_string_setting_;
  MapDoubleSetting map_double_setting_;
};

}
#endif //SCOT_SRC_SOLVER_SETTINGS_H_
