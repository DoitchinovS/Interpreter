#include <iostream>
#include <cassert>
#include "ProgramInterpreter.h"
#include <fstream>



int main()
{
   
    try {
        ProgramInterpreter interpret;
        interpret.runFromFile("Number_Sum.txt");

    }
    catch (std::invalid_argument& er) {
        std::cerr << er.what() << '\n';
    }
    catch (std::runtime_error& er) {
        std::cerr << er.what() << '\n';
    }
    catch (std::underflow_error& er) {
        std::cerr << er.what() << '\n';
    }
    catch (std::overflow_error& er) {
        std::cerr << er.what() << '\n';
    }
    catch (std::logic_error& er) {
        std::cerr << er.what()<< '\n';
    }

    return 0;
    
}


