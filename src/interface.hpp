/*
 * interface.cpp
 *
 *  Created on: 15.01.2017
 *      Author: luca
 */

#ifndef INTERFACE_CPP_
#define INTERFACE_CPP_

#include <string>
#include <net/if.h>
#include <exception>
#include <cerrno>
#include <cstring>
#include <deque>
#include <ifaddrs.h>

namespace sock{

class InterfaceException : public std::runtime_error{
public:
	InterfaceException(const std::string msg): std::runtime_error(msg.c_str()){
	}
	InterfaceException(const char* msg): std::runtime_error(msg){
	}
};

class Interface{
private:
	std::string name;
public:
	Interface(const char* name){
		Interface(std::string(name));
	}
	Interface(std::string name): name(name){
		if(!if_nametoindex(name.c_str())){
			throw InterfaceException("Interface Error ("+name+"): "+strerror(errno));
		}
	}
	unsigned int index(){
		return if_nametoindex(name.c_str());
	}
	static std::deque<Interface> getInterfaces(){
		std::deque<Interface> iflist;
		struct ifaddrs *addrs,*tmp;
		getifaddrs(&addrs);
		tmp = addrs;
		while (tmp){
		    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET)
		    	iflist.push_back(Interface(tmp->ifa_name));
		    tmp = tmp->ifa_next;
		}
		freeifaddrs(addrs);
		return iflist;
	}
};

}



#endif /* INTERFACE_CPP_ */
