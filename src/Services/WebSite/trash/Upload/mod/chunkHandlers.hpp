#ifndef CHUNK_HANDLERS_HPP
# define CHUNK_HANDLERS_HPP

# include <includes.hpp>
# include <Client.hpp>
# include <Buffer.hpp>

void	unchunkEnd(Client *client, std::string const &delimeter);
void	unchunkBegining(Client *client, std::string const &delimeter);

#endif