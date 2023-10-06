# Ultrasonic sensor interface for STM32

## Requirements
* C++17
* cmake minimum version 3.7
* STM32CubeMx generated code

## How to use
* Clone this repo to your STM32 project folder. For example:
```bash
git clone https://github.com/aufam/stm32_ultrasonic.git your_project_path/Middlewares/Third_Party/stm32_ultrasonic
```
* Add these line to your project CMakeLists.txt:
```cmake
add_subdirectory(Middlewares/Third_Party/stm32_ultrasonic)
target_link_libraries(${PROJECT_NAME}.elf ultrasonic)
```
* (Optional) Add the submodule:
```bash
git submodule add https://github.com/aufam/stm32_ultrasonic.git your_project_path/Middlewares/Third_Party/stm32_ultrasonic
```
