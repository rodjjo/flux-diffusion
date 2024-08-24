#pragma once

#include "simple-ui/dfe_ui.h"

namespace dfe
{

class MainWindow : public dfe_ui::Window {
    public:
        MainWindow();
        virtual ~MainWindow();
        
    private:
        std::shared_ptr<dfe_ui::Scrollbox> m_scroll_nodes;
};  
    
} // namespace dfe
