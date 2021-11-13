# the compiler: gcc for C program, define as g++ for C++
CC = g++ -std=c++17

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build targets executable:
TARGET_1 = oddeven_seq
TARGET_2 = oddeven_par
TARGET_3 = oe_ff

all: $(TARGET_1) $(TARGET_2) $(TARGET_3)

$(TARGET_1): $(TARGET_1).cpp
	$(CC) -o $(TARGET_1) $(TARGET_1).cpp

$(TARGET_2): $(TARGET_2).cpp
	$(CC) $(CFLAGS) -o $(TARGET_2) $(TARGET_2).cpp -pthread

$(TARGET_3): $(TARGET_3).cpp
	$(CC) -g -I /home/usman/Downloads/study/spm/fastflow -O3 -finline-functions -DNDEBUG $(TARGET_3).cpp -o $(TARGET_3) -pthread

clean:
	$(RM) $(TARGET_1) $(TARGET_2) $(TARGET_3)