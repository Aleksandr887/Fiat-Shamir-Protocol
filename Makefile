.PHONY: all clean run_serv run_client

BUILD_SERVER_DIR = build/server
BUILD_CLIENT_DIR = build/client
BUILD_LIB_DIR = build/libs
BIN_SERVER_DIR = bin/server
BIN_CLIENT_DIR = bin/client
SRC_SERVER_DIR = src/server
SRC_CLIENT_DIR = src/client
SRC_LIB_DIR = src/libs
CC = g++

all: $(BIN_SERVER_DIR)/server $(BIN_CLIENT_DIR)/client

$(BIN_SERVER_DIR)/server: $(BUILD_SERVER_DIR)/server.o $(BUILD_LIB_DIR)/operations.o $(BUILD_LIB_DIR)/tcp_lib.o
	$(CC) -Wall -I $(SRC_LIB_DIR) -o $(BIN_SERVER_DIR)/server $(BUILD_SERVER_DIR)/server.o $(BUILD_LIB_DIR)/operations.o $(BUILD_LIB_DIR)/tcp_lib.o

$(BUILD_SERVER_DIR)/server.o: $(SRC_SERVER_DIR)/server.cpp
	$(CC) -Wall -I $(SRC_LIB_DIR) -c $(SRC_SERVER_DIR)/server.cpp -o $(BUILD_SERVER_DIR)/server.o

$(BIN_CLIENT_DIR)/client: $(BUILD_CLIENT_DIR)/client.o $(BUILD_LIB_DIR)/operations.o $(BUILD_LIB_DIR)/tcp_lib.o
	$(CC) -Wall -I $(SRC_LIB_DIR) -o $(BIN_CLIENT_DIR)/client $(BUILD_CLIENT_DIR)/client.o $(BUILD_LIB_DIR)/operations.o $(BUILD_LIB_DIR)/tcp_lib.o

$(BUILD_CLIENT_DIR)/client.o: $(SRC_CLIENT_DIR)/client.cpp
	$(CC) -Wall -I $(SRC_LIB_DIR) -c $(SRC_CLIENT_DIR)/client.cpp -o $(BUILD_CLIENT_DIR)/client.o

$(BUILD_LIB_DIR)/operations.o: $(SRC_LIB_DIR)/operations.cpp
	$(CC) -Wall -c $(SRC_LIB_DIR)/operations.cpp -o $(BUILD_LIB_DIR)/operations.o

$(BUILD_LIB_DIR)/tcp_lib.o: $(SRC_LIB_DIR)/tcp_lib.cpp
	$(CC) -Wall -c $(SRC_LIB_DIR)/tcp_lib.cpp -o $(BUILD_LIB_DIR)/tcp_lib.o

clean:
	rm $(BIN_SERVER_DIR)/* $(BIN_CLIENT_DIR)/* $(BUILD_LIB_DIR)/* $(BUILD_SERVER_DIR)/* $(BUILD_CLIENT_DIR)/*

run_serv:
	cd bin && cd server && ./server

run_client:
	cd bin && cd client && ./client