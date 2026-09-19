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
    descriptor.title_update = recomp::TitleUpdateDescriptor{
        .label = "Title Update 2",
        .title_id = 0x45410813,
        .media_id = 0x1F6E4912,
        .version = 0x00000004,
        .code_patches = {{
            .path = "default.xexp", .size = 772096, .content_hash = "1302c285a37e608b79a7605791870805",
        }},
    };
    // skate. shipped no add-ons: EA Black Box said at the time that they did
    // not want to charge again for shoes and shirts. The empty list is the
    // right answer here, not a gap.
#ifdef RECOMP_DEVELOPMENT_GAME_ROOT
    descriptor.development_game_root = RECOMP_DEVELOPMENT_GAME_ROOT;
#endif
    return descriptor;
  }
};
