**Objective**
setup for the lolygo T-SIMCAM ESP32-S3

**Prerequisites**
-VScode
-Platform.io (extension)

Install Platform.io
    - select create project
    - type box and choose s3-box
    - choose arduino framework
    - select project location

To implement ##paltformio.ini##
-type this 
    [env:esp32s3box]
    platform = espressif32
    board = esp32s3box
    framework = arduino
    board_build.arduino.memory_type = qio_opi
    board_build.partitions = default_8MB.csv
    build_flags =
        -DBOARD_HAS_PSRAM
        -DARDUINO_USB_MODE=1
        -DARDUINO_USB_CDC_ON_BOOT=1
        -DCORE_DEBUG_LEVEL=5 


