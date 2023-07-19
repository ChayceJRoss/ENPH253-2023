

//common functions

//find the current state int form

//calculate the current error

class PD{

  private:
    int prevState = 0;
    int state = 0;


  public:
    double Kd;
    double Kp;

    PD(double Kd, double Kp){
      Kd = Kd;
      Kp = Kp;
    }

    void newKp(double Kp){
      this->Kp = Kp;
    }

    void newKd(double Kd){
      this->Kd = Kd;
    }

    double getP(int state){
      double prop = Kp * state;

      return prop;
    }

    int getState(){
      return this->state;
    }

    void resetState(int newState){
      this->prevState = this->state;
      this->state = newState;
    }

    int getPrev(){
      return this->prevState;
    }

    double d(int tau){
      double der;

      der = Kd*(state - prevState)/tau;
      
      return der;
    }

    double p(){
      double prop;

      prop = Kp*state;

      return prop;
    }

    double output(int new_state, int tau){
        resetState(new_state);
      double output;

      output = p() + d(tau);

      return output;
    }

    int getKp(){
      return this->Kp;
    }

    int getKd(){
      return this->Kd;
    }
    
};



int find_state(PD pd, int * sensorStates);