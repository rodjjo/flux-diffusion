#define PY_SSIZE_T_CLEAN

#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <chrono>

#include <python3.12/Python.h>
#include <pybind11/embed.h> 

#include "simple-ui/win.h"
#include "include/dfe/py_wrapper.h"
#include "include/dfe/config.h"

// #include "src/config/config.h"

// #include "src/python/raw_image.h"
// #include "src/windows/progress_window.h"

namespace py11 = pybind11;

namespace dfe {
namespace py {

namespace 
{
    std::shared_ptr<PythonMachine> machine;
    std::shared_ptr<dfe_ui::Window> window;
    py11::module_ *main_module = NULL;
    py11::module_ *deps_module = NULL;


    void show_progress_window() {
        // replace me
    }    

    void hide_progress_window() {
        // replace me
    }    


    PYBIND11_EMBEDDED_MODULE(dexpert, m) {
        m.def("progress", [](size_t p, size_t m, py11::dict image) {
            /*image_ptr_t img;
            if (image.contains("data")) {
                img = dexpert::py::rawImageFromPyDict(image);
            }
            dexpert::set_progress(p, m, img); */
        });

        m.def("progress_title", [](const char *text) {
            /*printf("%s\n", text);
            dexpert::set_progress_title(text); */
        });
        
        m.def("progress_canceled", []() {
            // if (dexpert::should_cancel_progress())
            //    return true;
            return false;
        });
    }

} // unnamed namespace 


PythonMachine::PythonMachine() {
}

void PythonMachine::stop_machine() {
    terminated_ = true;
}

py11::module_ &getModule() {
    return *main_module;
}

py11::module_ *depsModule() {
    return deps_module;
}

void create_python_venv() {
    std::wstring venv_dir = config::python_venv_directory();
    std::string command = "python3 -m venv " + std::string(venv_dir.begin(), venv_dir.end());
    if (!std::filesystem::exists(venv_dir)) {
        std::system(command.c_str());
    } else {
        puts("Virtual environment directory already exists.");
    }
}

void PythonMachine::run_machine() {
    puts("Creating python environment");

    create_python_venv();

    puts("Initializing Python");

    py11::scoped_interpreter guard{};
    py11::module_ sys = py11::module_::import("sys");
    
    sys.attr("executable") = config::python_executable_path();
    sys.attr("_base_executable") = config::python_executable_path();
    py11::sequence sp = sys.attr("path").cast<py11::sequence>();
    sp.attr("append")(config::entrypoint_directory().c_str());

    puts("Installing dependencies using dependencies.py");
    
    // execute callbacks that check/install dependencies
    {
        py11::module_ deps = py11::module_::import(config::dependencies_module().c_str());
        deps.attr("install_dependencies")();
    }

    puts("Running entrypoint.py");

    deps_module = NULL;

    py11::module_ dexp = py11::module_::import("dexpert");
    py11::module_ main = py11::module_::import(config::entrypoint_module().c_str());
    main.attr("__file__") = config::entrypoint_script();

    main_module = &main;

    ready_ = true;

    while (!terminated_) {
      execute_callback_internal();
    }
}

void PythonMachine::setDepsOk() {
    deps_ok_ = true;
}

PythonMachine::~PythonMachine() {
    terminated_ = true;
}

void PythonMachine::execute_callback_internal() {
    async_callback_t cb;
    {
        std::unique_lock<std::mutex> lk(callback_mutex_);
        cb = callback_;
    }

    if (cb) {
        try {
            cb();
        } catch(pybind11::cast_error err) {
            printf("Errored: %s\n", err.what());
            fflush(stdout);
            exit(10);
        }
        std::unique_lock<std::mutex> lk(callback_mutex_);
        callback_ = async_callback_t();
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void PythonMachine::execute_callback(async_callback_t callback) {
   replace_callback(callback);
   show_progress_window();
   wait_callback();
   hide_progress_window();
}

void PythonMachine::replace_callback(async_callback_t callback) {
    while (!callback_mutex_.try_lock()) {
        if(window) {
            window->pool_events();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    callback_ = callback;
    callback_mutex_.unlock();
}

void PythonMachine::wait_callback() {
    bool should_continue = true;
    bool locked = false;
    while (should_continue) {
        callback_mutex_.lock();
        if (!callback_) {
            should_continue = false;
        }
        callback_mutex_.unlock();
        if (should_continue) {
            if(window) {
                window->pool_events();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    }
}

bool PythonMachine::ready() {
    return ready_;
}

std::shared_ptr<PythonMachine> get_py() {
    if (machine) {
        return machine;
    }
    machine.reset(new PythonMachine());
    return machine;
}

void py_main() {
    get_py()->run_machine();
}

void py_end() {
    get_py()->stop_machine();
}

bool py_ready() {
    return get_py()->ready();
}

void set_window(std::shared_ptr<dfe_ui::Window> value) {
    window = value;
}

void wait_ready() {
    while(!py_ready()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

}  // namespace py
}  // namespace dexpert