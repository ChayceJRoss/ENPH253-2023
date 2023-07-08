//motor pins

#define MOTOR_A_P1 //forward pwm motor A (back left)
#define MOTOR_A_P2 //backward pwm motor A
#define MOTOR_B_P1 ////forward pwm motor B (back right)
#define MOTOR_B_P2 //backward pwm motor B


//reflective sensor pins
#define SENSOR_A PA0 //leftmost sensor pin (a)
#define SENSOR_B PA1 //central sensor pin (b)
#define SENSOR_C PA2 //rightmost sensor pin (c)


//ir sensor pins
#define SENSOR_IR //IR sensor pin

//servo pin
#define SERVO //servo pwm pin number


//common functions

//find the current state int form
int find_state(bool sensorStates[])
{
  int state;

  int a = sensorStates[0];
  int b = sensorStates[1];
  int c = sensorStates[2];

  state = 2*(a-c)+(b*a)-(b*c);
  return state;
};

//calculate the current error

class PD{

  private:
    int prevState = 1;
    int state = 1;


  public:
    double Kd;
    double Kp;
    double tau;

    PD(double Kd, double Kp, double tau){
      Kd = Kd;
      Kp = Kp;
      tau = tau;
    }

    double getP(int state){
      double prop = Kp * state;

      return prop;
    };

    int getState(){
      return this->state;
    };

    void resetState(int newState){
      this->prevState = this->state;
      this->state = newState;
    };

    int getPrev(){
      return this->prevState;
    };

    double d(){
      double der;

      der = Kd*(state - prevState)/tau;
      
      return der;
    };

    double p(){
      double prop;

      prop = Kp*state;

      return prop;
    }
    
};

