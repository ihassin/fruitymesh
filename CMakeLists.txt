cmake_minimum_required(VERSION 3.3)

project(fruitymesh)

# Toolchain path
set(tools /usr/local/gcc-arm-none-eabi-4_9-2015q2)

set(CMAKE_CXX_COMPILER  ${tools}/bin/arm-none-eabi-g++)
set(CMAKE_C_COMPILER    ${tools}/bin/arm-none-eabi-gcc)
set(SIZE_COMMAND        ${tools}/bin/arm-none-eabi-size)
set(OBJCOPY_COMMAND     ${tools}/bin/arm-none-eabi-objcopy)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m0plus)

#enable_language(CXX C)

set(MAIN_TARGET        ${PROJECT_NAME}.elf)

# NRF related
set(NRF_ROOT                $ENV{HOME}/nrf)
set(NRF_SDK                 ${NRF_ROOT}/sdk)
set(NRF_SDK_ROOT            ${NRF_SDK}/nrf51_sdk_latest)
set(SDK_COMPONENTS          ${NRF_SDK_ROOT}/components)
set(NRF_TOOLCHAIN           ${SDK_SDK_COMPONENTS}/toolchain)
set(DEVICE_INCLUDES         ${SDK_SDK_COMPONENTS}/device)
set(GCC                     ${NRF_TOOLCHAIN}/gcc)
set(EHAL_PATH               ${NRF_SDK}/ehal_latest)
set(CMSIS                   ${NRF_SDK}/ehal_latest)

include_directories(
    ./inc
    ./inc_c
    ./config
    ${SDK_COMPONENTS}/ble/ble_radio_notification
    ${SDK_COMPONENTS}/ble/ble_services/ble_dfu
    ${SDK_COMPONENTS}/ble/common
    ${SDK_COMPONENTS}/device
    ${SDK_COMPONENTS}/libraries/timer
    ${SDK_COMPONENTS}/libraries/util
    ${SDK_COMPONENTS}/softdevice/common/softdevice_handler
    ${SDK_COMPONENTS}/softdevice/s130/headers
    ${SDK_COMPONENTS}/toolchain
    ${SDK_COMPONENTS}/toolchain/arm
    ${SDK_COMPONENTS}/toolchain/gcc
    ${SDK_COMPONENTS}/drivers_nrf/pstorage
    ${SDK_COMPONENTS}/drivers_nrf/hal
)

set(C_FILES
    ${EHAL_PATH}/ARM/Nordic/nRF51/src/Vectors_nRF51.c
    ${SDK_COMPONENTS}/libraries/timer/app_timer.c
    ${SDK_COMPONENTS}/ble/ble_radio_notification/ble_radio_notification.c
    ./src/nrf/simple_uart.c
    ${SDK_COMPONENTS}/drivers_nrf/hal/nrf_delay.c
    ${SDK_COMPONENTS}/drivers_nrf/pstorage/pstorage.c
    ${SDK_COMPONENTS}/softdevice/common/softdevice_handler/softdevice_handler.c
)

set(FRUITY_FILES
    ./src/base/AdvertisingController.cpp
    ./src/base/GAPController.cpp
    ./src/base/GATTController.cpp
    ./src/base/ScanController.cpp
    ./src/Main.cpp
    ./src/mesh/Connection.cpp
    ./src/mesh/ConnectionManager.cpp
    ./src/mesh/Node.cpp
    ./src/modules/AdvertisingModule.cpp
    ./src/modules/DFUModule.cpp
    ./src/modules/EnrollmentModule.cpp
    ./src/modules/Module.cpp
    ./src/modules/ScanningModule.cpp
    ./src/modules/StatusReporterModule.cpp
    ./src/modules/DebugModule.cpp
    ./src/modules/IoModule.cpp
    ./src/test/TestBattery.cpp
    ./src/test/Testing.cpp
    ./src/utility/LedWrapper.cpp
    ./src/utility/Logger.cpp
    ./src/utility/PacketQueue.cpp
    ./src/utility/SimpleBuffer.cpp
    ./src/utility/SimplePushStack.cpp
    ./src/utility/SimpleQueue.cpp
    ./src/utility/Storage.cpp
    ./src/utility/Terminal.cpp
    ./src/utility/Utility.cpp
)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -flto -fno-move-loop-invariants -Wextra -g3 -DBLE_STACK_SUPPORT_REQD -DDEBUG -g -O1 -DBOARD_PCA10031 -DNRF51 -D__need___va_list -w -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -DENABLE_LOGGING")
set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS} -std=gnu99 -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -flto -fno-move-loop-invariants -Wextra -g3 -DBLE_STACK_SUPPORT_REQD -DDEBUG -g -O1 -DBOARD_PCA10031 -DNRF51 -D__need___va_list -w -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -DENABLE_LOGGING")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS
        "-L${EHAL_PATH}/ARM/Nordic/nRF51/CMSIS/debug -lCMSIS -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -flto -fno-move-loop-invariants -Wextra -g3 -T${CMAKE_SOURCE_DIR}/linker/cmake_gcc_nrf51_s130_32kb.ld -Xlinker --gc-sections -Wl,-Map,\"${CMAKE_SOURCE_DIR}/build/FruityMesh.map\" --specs=nano.specs")

add_executable(${MAIN_TARGET} ${FRUITY_FILES} ${C_FILES})

add_custom_command(
        TARGET ${MAIN_TARGET}
        COMMAND ${SIZE_COMMAND} ${MAIN_TARGET}
        COMMAND ${OBJCOPY_COMMAND} -O ihex ${MAIN_TARGET} ${PROJECT_NAME}.hex # convert .elf to .hex (redundancy: only one of either .hex or .bin is needed)
        COMMAND ${OBJCOPY_COMMAND} -O binary ${MAIN_TARGET} ${PROJECT_NAME}.bin # convert .elf to .hex
)
