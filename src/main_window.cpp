#include "dfe/main_window.h"

namespace dfe
{

MainWindow::MainWindow() : dfe_ui::Window(1280, 720, "Image generator") {
    m_scroll_nodes.reset(new dfe_ui::Scrollbox(window(), 0, 0, 1000, 720));
}

MainWindow::~MainWindow() {

}

} // namespace dfe
