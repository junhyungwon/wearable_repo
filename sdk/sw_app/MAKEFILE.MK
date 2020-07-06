app:
	make -fMAKEFILE.MK libs TARGET=depend
	make -fMAKEFILE.MK libs
	make -fMAKEFILE.MK apps TARGET=exe

hw_test:
	make -fMAKEFILE.MK libs_test TARGET=depend
	make -fMAKEFILE.MK libs_test
	make -fMAKEFILE.MK apps_test TARGET=exe

app_fitt:
	make -fMAKEFILE.MK -C$(APP_DIR)/device MODULE=app_fitt $(TARGET)
	make -fMAKEFILE.MK -C$(APP_DIR)/graphic MODULE=app_fitt $(TARGET)
	make -fMAKEFILE.MK -C$(APP_DIR)/rtsptx MODULE=app_fitt $(TARGET)
	make -fMAKEFILE.MK -C$(APP_DIR)/app MODULE=app_fitt $(TARGET)
	make -fMAKEFILE.MK -C$(APP_DIR)/control_sock MODULE=app_fitt $(TARGET)

app_fitt_hw_test:
	make -fMAKEFILE.MK -C$(APP_DIR)/graphic MODULE=hw_test $(TARGET)
	make -fMAKEFILE.MK -C$(APP_DIR)/device MODULE=hw_test $(TARGET)
	make -fMAKEFILE.MK -C$(APP_DIR)/hw_test MODULE=hw_test $(TARGET)


libs: app_fitt

libs_test: app_fitt_hw_test

apps:
	make -fMAKEFILE.MK -C$(APP_DIR)/app MODULE=app_fitt $(TARGET)

apps_test:
	make -fMAKEFILE.MK -C$(APP_DIR)/hw_test MODULE=hw_test $(TARGET)

clean:
	make -fMAKEFILE.MK libs TARGET=clean

clean_test:
	make -fMAKEFILE.MK libs_test TARGET=clean

all: clean apps

.PHONY: hw_test app_fitt app_fitt_hw_test libs libs_test apps apps_test clean clean_test app all
