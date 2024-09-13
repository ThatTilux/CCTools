# CCTools

CCTools is a C++ library designed to simplify the development of canted-cosine-theta (CCT) magnets by extending the open-source [RAT-Library](https://gitlab.com/Project-Rat). It provides classes for parsing magnet models in JSON format, performing computations, and modifying the models, facilitating efficient magnet design and analysis.

This project is part of the FCC-ee HTS4 research project at CERN.


## Prerequisites

Before using CCTools, ensure that your system meets the following requirements.

#### System Packages

1. **Google Test (gtest) Library**:
   - Google Test is a framework for C++ tests.
   ```sh
   sudo apt-get install libgtest-dev
   ```

2. **spdlog**:
   - spdlog is a fast C++ logging library.
   You can install `spdlog` using by cloning the repository and installing it manually:
   ```sh
   git clone https://github.com/gabime/spdlog.git
   cd spdlog
   mkdir build && cd build
   cmake ..
   make -j
   sudo make install
   ```

#### RAT-Library
Follow the [RAT documentation](https://gitlab.com/Project-Rat/rat-documentation) to install the RAT library. This may take a while.
- When a CUDA compatible GPU is available, make sure to follow the steps for installing CUDA. This is highly recommended as it reduces the runtime of this software by some orders of magnitude.
- When using CUDA with WSL, it is advised to increase the allocated memory for WSL, see [here](https://geronimo-bergk.medium.com/optimizing-wsl2-performance-setting-memory-and-cpu-allocation-on-windows-513eba7b6086).


## Installation
### Using CCTools in your project

To incorporate CCTools into your C++ project, follow these steps.

1. **Include CCTools in `CMakeLists.txt`:**

   Use CMake's `FetchContent` module to fetch and include CCTools.

   ```cmake
   include(FetchContent)

   FetchContent_Declare(
     CCTools
     GIT_REPOSITORY https://github.com/ThatTilux/CCTools.git
     GIT_TAG        main
   )

   FetchContent_MakeAvailable(CCTools)
   ```

2. **Link CCTools to Your Executable:**

   In your `CMakeLists.txt`, link CCTools to your target executable.

   ```cmake
   target_link_libraries(your_executable PRIVATE CCTools)
   ```

3. **Include Directories (if necessary):**

   Ensure that the include directories are set correctly.

   ```cmake
   target_include_directories(your_executable PRIVATE ${CCTools_INCLUDE_DIRS})
   ```


### Building CCTools Standalone

Please note that this step is  **not required** to use this library in your C++ project. If you wish to build and test CCTools independently, follow these steps.

1. **Clone the Repository and Build the Project:**
    ```sh
    git clone <repository_url>
    cd <repository_directory>
    mkdir build
    cd build
    cmake ..
    make -j
    ```

2. **Run Tests:**
    ```sh
    ./bin/cctools_test
    ```

## Author

Ole Kuhlmann  
Email: [tim.ole.kuhlmann@cern.ch](mailto:tim.ole.kuhlmann@cern.ch)  
GitHub: [ThatTilux](https://github.com/ThatTilux)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
