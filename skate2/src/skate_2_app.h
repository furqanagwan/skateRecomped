#pragma once

#include <memory>

#include "recomp/app/game_recomp_app.h"

class Skate2App final : public recomp::GameRecompApp {
 public:
  static std::unique_ptr<rex::ui::WindowedApp> Create(rex::ui::WindowedAppContext& context) {
    return std::unique_ptr<Skate2App>(new Skate2App(context));
  }

 private:
  explicit Skate2App(rex::ui::WindowedAppContext& context)
      : GameRecompApp(context, Descriptor(), PPCImageConfig) {}

  static recomp::GameDescriptor Descriptor() {
    recomp::GameDescriptor descriptor;
    descriptor.app_name = "skate_2";
    descriptor.display_name = "Skate 2";
    descriptor.title_update = recomp::TitleUpdateDescriptor{
        .label = "Title Update 2",
        .title_id = 0x4541087F,
        .media_id = 0x6ADB5821,
        .version = 0x00000002,
        .code_patches = {{
            .path = "default.xexp",
            .size = 1411072,
            .content_hash = "805506add8662f859b3fbac932240a97",
        }},
    };
#ifdef RECOMP_DEVELOPMENT_GAME_ROOT
    descriptor.development_game_root = RECOMP_DEVELOPMENT_GAME_ROOT;
#endif
    return descriptor;
  }
};
