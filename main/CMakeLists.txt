set(BLE_DIR ble)
file(GLOB_RECURSE BLE_C_SOURCES ${BLE_DIR}/*.c)

set(USB_DIR usb)
file(GLOB_RECURSE USB_C_SOURCES ${USB_DIR}/*.c)

set(CHAIN_BUS_DIR chain_bus)
file(GLOB_RECURSE CHAIN_BUS_C_SOURCES ${CHAIN_BUS_DIR}/*.cpp)

set(WEB_DIR web)
file(GLOB_RECURSE WEB_IMG_C_SOURCES ${WEB_DIR}/*.jpg ${WEB_DIR}/*.png)

idf_component_register(SRCS "main.cpp" "btn_progress.c" "settings.c" "dual_button.c" "adc_detect.c" ${BLE_C_SOURCES} ${USB_C_SOURCES} ${CHAIN_BUS_C_SOURCES}
                       INCLUDE_DIRS "include" ${RM_DIR} ${BLE_DIR} ${USB_DIR} ${CHAIN_BUS_DIR}
                       EMBED_FILES web/index.html web/styles.css web/script.js web/favicon.ico ${WEB_IMG_C_SOURCES})

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-const-variable)

idf_component_get_property(tusb_lib espressif__tinyusb COMPONENT_LIB)
cmake_policy(SET CMP0079 NEW)
target_link_libraries(${tusb_lib} PRIVATE ${COMPONENT_LIB})
