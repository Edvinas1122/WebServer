#include <Service.hpp>

/*
	Client not assigned to the service list
*/
// bool	Service::Ready(Client &client)
// {
// 	if (findClient(&client) == processList.end())
// 		return (true);
// 	return (false);
// }

// Service::process_it	Service::findClient(Client *client)
// {
// 	process_it it = processList.begin();

// 	while (it != processList.end())
// 	{
// 		if ((*it)->idTest(client))
// 			return (it);
// 		it++;
// 	}
// 	return (it);
// }

// void	Service::SetProcess(ServiceProcess *service)
// {
// 	processList.push_back(service);
// };


// void	Service::RemoveClient(Client *client)
// {
// 	ServiceProcess *process = *findClient(client);

// 	processList.erase(findClient(client));
// 	delete (process);
// }

// Service::~Service()
// {
// 	ServiceProcess *process;

// 	process_it it = processList.begin();

// 	while (it != processList.end())
// 	{
// 		process = *it;
// 		delete process;
// 		it++; 
// 	}
// 	processList.clear();
// }

// void	Service::ServiceDefault(Client &client, Service *service)
// {
// 	process_it	process = FindProcess(client);

// 	if (process == processList.end()) {
// 		service->SetResponse(client);
// 	}
// 	if (!service->Service::Ready(client))
// 		service->Handle(client);
// };