#include <ul.h>
#include <js.h>
#include <iostream>

//gcc -I./ -I../util/ testjoy.cxx -o test_joy -L.  -lplibjs -lstdc++ -L../util -lplibul

int main()
{
  jsInit();
  jsJoystick* joy=new jsJoystick(0);
  std::cerr<<"name="<<joy->getName()<<std::endl;
  std::cerr<<"axes number="<<joy->getNumAxes()<<std::endl;
  
  int buttons[2];
  float axes[7];
  
  while (1)
    {
      joy->rawRead(buttons,axes);
      std::cerr<<"axes="<<axes[0]<<" "<<axes[1]<<" "<<axes[2]<<" "<<axes[3]<<" "<<axes[4]<<" "<<axes[5]<<" "<<axes[6]<<" buttons ="<<buttons[0]<<std::endl;
    }
  return 1;

}
