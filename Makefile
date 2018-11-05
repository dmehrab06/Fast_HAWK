SHELL := /bin/bash
CPP_FLAG = -lpthread
CPP_FOLDER=cpp/src
CPP_FILE=${CPP_FOLDER}/*.cpp
OBJECT_FILE=${CPP_FOLDER}/*.o


all: log_reg_case.out log_reg_control.out


log_reg_case.out: log_reg_case.o ${OBJECT_FILE}
	g++ $^  ${CPP_FLAG} -o $@

log_reg_control.out: log_reg_control.o ${OBJECT_FILE}
	g++ $^ ${CPP_FLAG} -o $@

log_reg_case.o: log_reg_case.cpp
	g++ $^ -c -o $@

log_reg_control.o: log_reg_control.cpp
	g++ $^  -c -o $@

${OBJECT_FILE}: ${CPP_FILE}
	for f in `ls ${CPP_FILE}`;do echo $$f;g++ $$f -c -o $$f.o; done

clean:
	rm *.o
	rm ${CPP_FOLDER}/*.o
