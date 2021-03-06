#include <iostream>
#include <string>
#include <chrono>
#include <boost/program_options.hpp>
#include <boost/process.hpp>

namespace bp = boost::process;
namespace po = boost::program_options;

int cmake (int sec = 0, std::string str = "Default")
{
	bp::ipstream out;
	std::string command = "cmake -H. -B_builds -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=" + str;
	bp::child c(command, bp::std_out > out);
	if (sec) {
    		if (!c.wait_for(std::chrono::seconds(sec)));
     			c.terminate();
  	}
	else
    		c.wait();

	if (c.exit_code() == 0) {

		bp::child c2("cmake --build _builds", bp::std_out > out);
		if (sec) {
    			if (!c2.wait_for(std::chrono::seconds(sec)));
     				c2.terminate();
  		}
		else
    			c2.wait();
		return c2.exit_code();
	}

 	return c.exit_code();
}

int build (std::string str, int sec = 0)
{
	bp::ipstream out;
	std::string command = "cmake --build _builds --target " + str;

	bp::child c(command, bp::std_out > out);
	if (sec) {
    		if (!c.wait_for(std::chrono::seconds(sec)));
     			c.terminate();
  	}
	else
    		c.wait();

 	return c.exit_code();
}

int main(int ac, char *av[])
{
	po::options_description desc("Allowed options");
	desc.add_options()
   	 	("help", "выводим вспомогательное сообщение")
		("config", po::value<std::string>(), "указываем конфигурацию сборки (по умолчанию Debug)")
    		("install", "добавляем этап установки (в директорию _install)")
		("pack", "удобавляем этап упаковки (в архив формата tar.gz)")
		("timeout",  po::value<int>(), "указываем время ожидания (в секундах)");

	po::variables_map vm;
	po::store(po::parse_command_line(ac, av, desc), vm);
	po::notify(vm); 
	int result = 0;	
	
	if (vm.count("help")) {
 		std::cout << desc << std::endl;
   		return 1;
	}   

	else if (vm.count("config"))
	{
		result = cmake(0, vm["config"].as<std::string>());		
	}

	else if (vm.count("install"))
	{
		if (!cmake() == 0)
			result = build("install");
	}

	else if (vm.count("pack"))
	{
		if (!cmake() == 0)
			result = build("package");
	}

	else if (vm.count("timeout"))
	{
		result = cmake(vm["timeout"].as<int>());
	}	
	
	else
	{
		result = cmake();
	}

	return result;
}
 

