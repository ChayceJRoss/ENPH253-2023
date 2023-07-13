#include "includes.h"

int find_state(PD pd, int * sensorStates)
{
  int state = 0;
  int prevState = pd.getPrev();
  int a = sensorStates[0];
  int b = sensorStates[1];
  int c = sensorStates[2];
  if (b == 1)
  {
    state = 0;
  }
  else if (a == 1)
  {
    state = 1;
  }
  else if (c == 1)
  {
    state = -1;
  }
  else if (a == 0 && b == 0 && c == 0)
  {
    state = 2 * ((prevState > 0) ? 1 : - 1);
  }
  return state;
}

//calculate the current error

