#pragma once

#include <memory>

#include "recomp/app/game_recomp_app.h"

class SkateApp final : public recomp::GameRecompApp {
 public:
  static std::unique_ptr<rex::ui::WindowedApp> Create(rex::ui::WindowedAppContext& context) {
    return std::unique_ptr<SkateApp>(new SkateApp(context));
  }

 private:
  explicit SkateApp(rex::ui::WindowedAppContext& context)
      : GameRecompApp(context, Descriptor(), PPCImageConfig) {}

  static recomp::GameDescriptor Descriptor() {
    recomp::GameDescriptor descriptor;
    descriptor.app_name = "skate";
    descriptor.display_name = "Skate";
#ifdef RECOMP_DEVELOPMENT_GAME_ROOT
    descriptor.development_game_root = RECOMP_DEVELOPMENT_GAME_ROOT;
#endif
    return descriptor;
  }
};
