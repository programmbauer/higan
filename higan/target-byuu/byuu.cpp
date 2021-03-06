#include "byuu.hpp"

namespace ruby {
  Video video;
  Audio audio;
  Input input;
}

auto locate(const string& name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  location = {Path::userData(), "byuu/", name};
  if(inode::exists(location)) return location;

  directory::create({Path::userSettings(), "byuu/"});
  return {Path::userSettings(), "byuu/", name};
}

#include <nall/main.hpp>
auto nall::main(Arguments arguments) -> void {
  Application::setName("byuu");
  Application::setScreenSaver(false);

  for(auto argument : arguments) {
    if(argument == "--fullscreen") {
      program.startFullScreen = true;
    } else if(file::exists(argument)) {
      program.startGameLoad = argument;
    }
  }

  Emulator::construct();
  inputManager.create();
  settings.load();
  Instances::presentation.construct();
  Instances::settingsWindow.construct();

  if(!Path::user().endsWith("/byuu/")) {
    MessageDialog()
    .setTitle("byuu")
    .setText({
      "Note: this is pre-alpha software!\n"
      "Development literally just started.\n\n"
      "Please do not distribute binaries of byuu.\n"
      "Please do not report any bugs.\n"
      "Please don't even run this at all!\n"
      "If you do so anyway, the consequences are on you."
    })
    .information();
  }

  program.create();
  Application::onMain({&Program::main, &program});
  Application::run();

  settings.save();

  Instances::presentation.destruct();
  Instances::settingsWindow.destruct();
}
