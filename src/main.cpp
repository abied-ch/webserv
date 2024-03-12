#include "../inc/Headers.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	// if (argc != 2)
	// {
	// 	std::cerr << "Error. Invalid number of arguments." << std::endl;
	// 	std::cerr << "Usage: " << argv[0] << " [config file <.conf>]" << std::endl;
	// 	return 1;
	// }
	ServerInfo server_info;
	server_info.set_port(8080);
	server_info.set_type(SERVER_NODE);

	Director director;
	director.add_server_info(server_info);
	if(director.init_servers() < 0)
	{
		std::cerr << "Error initializing servers." << std::endl;
		return (1);
	}
	if (director.run_servers() < 0)
	{
		std::cerr << "Error." << std::endl;
		return (1);
	}
	return 0;
}
/*
	try
	{
		if (argc == 2)
			Config config(argv[1]);
		else
			Config config;
	}
	catch (const std::exception &e)
	{
		std::cerr << "error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
		(void)argc;
	(void)argv;
	// if (argc != 2)
	// {
	// 	std::cerr << "Error. Invalid number of arguments." << std::endl;
	// 	std::cerr << "Usage: " << argv[0] << " [config file <.conf>]" << std::endl;
	// 	return 1;
	// }
	ServerInfo server_info;
	server_info.set_port(8080);
	server_info.set_type(SERVER_NODE);

	Director director;
	director.add_server_info(server_info);
	if(director.init_servers() < 0)
	{
		std::cerr << "Error initializing servers." << std::endl;
		return (1);
	}
	if (director.run_servers() < 0)
	{
		std::cerr << "Error." << std::endl;
		return (1);
	}
	return 0;
}
*/