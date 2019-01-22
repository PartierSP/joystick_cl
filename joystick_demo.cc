// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Copyright Drew Noakes 2013-2016

#include <iostream>
#include "joystick.h"
#include <unistd.h>

int main(int argc, char** argv)
{
  // Create an instance of Joystick
  Joystick joystick("/dev/input/js0");

  // Ensure that it was found and that we can use it
  if (!joystick.isFound())
  {
    printf("open failed.\n");
    exit(1);
  }

  while (true)
  {
    // Restrict rate
    usleep(1000);

    // Attempt to sample an event from the joystick
    JoystickEvent event;
    if (joystick.sample(&event))
    {
        std::cout << "[isInitialState?" << event.isInitialState() << "] ";
        std:: cout << " t=" << event.time;
        if (event.isButton())
        {
            std::cout << " Button#" << (int)event.number ;
            std::cout << " value=" << ((event.value == 0) ? "up" : "down") ;
            std::cout << std::endl;
        }
        else if (event.isAxis())
        {
            std::cout << "Axis#" << (int)event.number;
            std::cout << " value=" << (int)event.value;
            std::cout << std::endl;
        }
    }
  }
}
