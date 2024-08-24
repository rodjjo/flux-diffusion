#include <thread>
#include <stdio.h>

#include "simple-ui/dfe_ui.h"
#include "dfe/main_window.h"
#include "dfe/py_wrapper.h"

namespace dfe
{

auto ui_entrypoint = [] () {
    dfe::py::wait_ready();
    auto wnd = std::make_shared<dfe::MainWindow>();
    dfe::py::set_window(wnd);
    wnd->run();
    dfe::py::py_end();
};

} // namespace dfe


int main() {
    // init python
    std::thread ui_thread(dfe::ui_entrypoint);
    dfe::py::py_main();
    ui_thread.join();
    return 0;
}