#include <exception>
#include <fstream>
#include <string>
#include <iostream>
#include <memory>

#include "configuration_network.h"
#include "configuration_console.h"
#include "exception_network.h"
#include "network_connection.h"
#include "memory_common.h"
#include "console_color_manipulator.h"
#include "console_input_disabler.h"




int main() {
  
  try {
    auto connection = std::make_unique<network::Connection>(configuration::serverAdress, configuration::serverPortString);
    std::string consoleInput;

   

    auto colorManipulator = std::make_unique<console::ColorManipulator>(configuration::consoleColor); // change color
    //std::cin >> consoleInput; // username
    consoleInput = "test";
    connection->send(consoleInput);
    std::cout << connection->receive() << std::endl;
    {
      auto inputDisabler = std::make_unique<console::InputDisabler>(); // disable input
      //std::cin >> consoleInput; // passwd
      consoleInput = "test";
      std::cout << std::endl; // make space of a row
      connection->send(consoleInput);
      std::cout << connection->receive() << std::endl;
    }
    //std::cin >> consoleInput; // klijent unese izbor
    consoleInput = "1";
    connection->send(consoleInput);
    std::cout << connection->receive() << std::endl;

    consoleInput = R"(|testiculus|w|19|laplap/jave.exe|next|filename.jar|josMaloLazi.jpeg)"; // klijent unese izbor
    connection->send(consoleInput);
    
    consoleInput = R"(laplap/jave.exe - nema tu kruva)"; // klijent unese izbor
    connection->send(consoleInput);

    consoleInput = R"(æææææ)"; // klijent unese izbor
    connection->send(consoleInput);
    
    consoleInput = R"(èèèè)"; // klijent unese izbor
    connection->send(consoleInput);

    consoleInput = R"(žž)"; // klijent unese izbor
    connection->send(consoleInput);

    consoleInput = R"(1)"; // klijent unese izbor
    connection->send(consoleInput);
    
    std::cin >> consoleInput;


  } catch(const std::exception& e) {
     std::cout << e.what();
  }


 
  return 0;
}


/*#include "network_connection.h"

#include "exception_network.h"

#include <stdexcept>
#include <exception>

#include <string>




int main() {
  int a;
  /*
  std::string s("1234567890");
  try {
    network::Connection conn("localhost", "1200");
    for(auto i = 0; i < 10; ++i) {
      conn.send(s);
      std::cout << conn.receive() << std::endl;
    }
  } catch(const std::exception& e) {
     std::cout << e.what();
  }
  
  network::Connection conn("localhost", "1200");
  conn.send("kiki");
  std::cout << conn.receive() << std::endl;
  conn.send("1");
  std::cout << conn.receive() << std::endl;

  std::string s("1234567890");
  for(auto i = 0; i < 10; ++i) {
      conn.send(s);
      std::cout << conn.receive() << std::endl;
    }

  std::cin >> a;
  return 0;
}*/