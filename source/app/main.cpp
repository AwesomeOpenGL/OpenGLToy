#include "app/Application.h"

#include <exception>
#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        Alice::App::Application application;
        return application.Run(argc, argv);
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;
        return 1;
    }
}
