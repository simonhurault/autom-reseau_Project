#
# Compiler configuration
#
#
USER_DEFINES=-DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 -DDO_NOT_USE_SHARED_MEMORY
CC=gcc -O3 $(USER_DEFINES)

#
# Set VREP path for include files and 
# required C files in remoteApi folder:
#     extApi.c, extApiPlatform.c 
#
PATH_VREP=/home/VREP
INCLUDE1 = $(PATH_VREP)/programming/include
INCLUDE2 = $(PATH_VREP)/programming/remoteApi
INCLUDE = -I$(INCLUDE1) -I$(INCLUDE2)

#
# Targets
#
#
EXE = control
OBJS = extApi.o extApiPlatform.o main.o

all: 		$(OBJS)
			g++ $? -o $(EXE) -lpthread -lrt -LextApi.o -LextApiPlatform.o 


extApi.o: 	$(INCLUDE2)/extApi.c
			$(CC) -c $(INCLUDE) $(INCLUDE2)/extApi.c
			
extApiPlatform.o:	$(INCLUDE2)/extApiPlatform.c
			$(CC) -c $(INCLUDE) $(INCLUDE2)/extApiPlatform.c
			
main.o: 	main.cpp
			$(CC) -c $(INCLUDE) main.cpp
			

			
clean:
			rm *.o $(EXE)
