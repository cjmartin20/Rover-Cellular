#include <stdint.h>
#include <string.h>
#include <math.h>

#define MAX_TURN_ANGLE 105.0 //degrees
#define REFRESH_RATE 500 //miliseconds
 
class RoverPilot {
  public:
    void begin(int baud); 

    bool readIncomingCommands();
    bool sendStatus();
    bool sendHealth();

    void turn(void);
    void accelerate(void);
    void brake(void);
    void shutdown(void);
};
 
#endif