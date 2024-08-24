#pragma once

#include <string>

namespace dfe {
namespace config {

/**
 * Retrieves the path of the executable file.
 *
 * @return The path of the executable file as a wide string.
 */
std::wstring executable_path();

/**
 * Retrieves the path of the Python executable.
 *
 * @return The path of the Python executable as a wide string.
 */
std::wstring python_executable_path();

/**
 * Retrieves the directory of the Python virtual environment.
 *
 * @return The directory of the Python virtual environment as a wide string.
 */
std::wstring python_venv_directory();

/**
 * Retrieves the directory of the entry point.
 *
 * @return The directory of the entry point as a wide string.
 */
std::wstring entrypoint_directory();


/**
 * Retrieves the entrypoint script as a wide string.
 *
 * @return The entrypoint script as a wide string.
 */
std::wstring entrypoint_script();


/**
 * Retrieves the entry point module as a wide string.
 *
 * @return The entry point module as a wide string.
 */
std::string entrypoint_module();

/**
 * Retrieves the dependencies module as a wide string.
 *
 * @return The dependencies module as a wide string.
 */
std::string dependencies_module();

} // namespace config
} // namespace dfe