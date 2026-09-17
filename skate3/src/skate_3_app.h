#pragma once

#include <memory>

#include "recomp/app/game_recomp_app.h"

class Skate3App final : public recomp::GameRecompApp {
 public:
  static std::unique_ptr<rex::ui::WindowedApp> Create(rex::ui::WindowedAppContext& context) {
    return std::unique_ptr<Skate3App>(new Skate3App(context));
  }

 private:
  explicit Skate3App(rex::ui::WindowedAppContext& context)
      : GameRecompApp(context, Descriptor(), PPCImageConfig) {}

  static recomp::GameDescriptor Descriptor() {
    recomp::GameDescriptor descriptor;
    descriptor.app_name = "skate_3";
    descriptor.display_name = "Skate 3";
#ifdef RECOMP_DEVELOPMENT_GAME_ROOT
    descriptor.development_game_root = RECOMP_DEVELOPMENT_GAME_ROOT;
#endif
    return descriptor;
  }
};
