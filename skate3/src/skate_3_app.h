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
    descriptor.title_update = recomp::TitleUpdateDescriptor{
        .label = "Title Update 3",
        .title_id = 0x454108E6,
        .media_id = 0x5C087C2C,
        .version = 0x00000003,
        .code_patches = {{
            .path = "default.xexp", .size = 1701888, .content_hash = "dc4cd0306f52326baae80c8c851bf6aa",
        }, {
            .path = "data/webkit/EAWebkit.xexp", .size = 4096, .content_hash = "8dfe7618d1a69047903b3893a249020c",
        }},
    };
    descriptor.dlc = {
        {.label = "Danny Way's Hawaiian Dream"},
        {.label = "Maloof Money Cup 2010 NYC Pack"},
        {.label = "San Van Party Pack"},
        {.label = "Black Box Distribution Skate Park"},
        {.label = "skate.Create Upgrade Pack"},
        {.label = "Time Is Money Pack"},
    };
#ifdef RECOMP_DEVELOPMENT_GAME_ROOT
    descriptor.development_game_root = RECOMP_DEVELOPMENT_GAME_ROOT;
#endif
    return descriptor;
  }
};
