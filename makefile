PROJECT         := rosserial_mbed
DEVICES         := NUCLEO_F303RE
GCC4MBED_DIR    := $(GCC4MBED_DIR)
USER_LIBS       := !$(ROS_LIB_DIR) $(ROS_LIB_DIR)/BufferedSerial
NO_FLOAT_SCANF  := 1
NO_FLOAT_PRINTF := 1

include $(GCC4MBED_DIR)/build/gcc4mbed.mk