/*
 * IP.lzz
 *
 *  Created on: 14.01.2017
 *      Author: luca
 */

#include <string>
#include <netinet/in.h>
#include <sstream>
#include <exception>
#include <algorithm>
#include <netdb.h>

namespace sock{
class Address{
public:
	Address(){};
/*	static Address parse(std::string str){

	};*/
	virtual std::string toString() const = 0;
};

class AddressException : public std::runtime_error{
private:
	Address* address;

public:
	AddressException(Address* addr, const std::string msg): std::runtime_error(msg.c_str()), address(addr){
	}
	AddressException(Address* addr, const char* msg): std::runtime_error(msg), address(addr){
	}
};

class IP : Address{
public:
	IP(){};
	/*static IP parse(std::string str){

	};*/
};

class IPv4 : IP{
private:
	IPv4(){};
	uint32_t ip = 0;
public:
	IPv4(const IPv4& obj){
		ip = obj.ip;
	};
	IPv4(const uint32_t val){
		ip = val;
	};
	IPv4(const sockaddr_in val){
		ip = val.sin_addr.s_addr;
	};
	uint32_t getIP(){
		return ip;
	}
	std::string toString() const{
		std::string str;
		uint8_t* ipparts = (uint8_t*)&ip;
		str = std::to_string(ipparts[3]) + "." + std::to_string(ipparts[2]);
		str += "." + std::to_string(ipparts[1]) + "." + std::to_string(ipparts[0]) ;
		return str;
	};
	static IPv4 parse(const char* str){
		return parse(std::string(str));
	}
	static IPv4 parse(const std::string str){
		std::string elemStr;
		uint8_t ipparts[4] = {0};
		int elemCount = 0;
		for(unsigned int i = 0; i < str.length(); i++){
			if(str[i] == '.' || (i+1) == str.length()){
				if((i+1) == str.length()){
					elemStr += str[i];
				}
				if(elemStr.length() == 0){
					throw AddressException(NULL, std::string("Could not parse: ")+str);
				}
				unsigned int elemVal;
				try{
					elemVal = std::stoul(elemStr);
				}catch(...){
					throw AddressException(NULL, std::string("Could not parse: ")+str);
				}
				if(elemVal > 255){
					throw AddressException(NULL, std::string("Could not parse: ")+str);
				}
				ipparts[3-elemCount] = elemVal;
				elemCount++;
			}else{
				if(std::string("0123456789").find(str[i]) == std::string::npos){
					throw AddressException(NULL, std::string("Could not parse: ")+str);
				}
				elemStr += str[i];
			}
			if(elemStr.length() > 3){
				throw AddressException(NULL, std::string("Could not parse: ")+str);
			}
			if(str[i] == '.')
				elemStr.clear();
		}
		if(elemCount != 4){
			throw AddressException(NULL, std::string("Could not parse: ")+str);
		}
		return IPv4(*((uint32_t*)ipparts));
	};
};

/*class IPv6 : IP{
private:
	IPv6(){

	};
public:
	std::string toString(){

	};
	static IPv6 parse(std::string str){

	};
};
*/
class URI : Address{
private:
	URI(){};
	std::string host;
public:
	URI(const std::string strInp){
		std::string str(strInp);
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		if(str.find_first_not_of("abcdefghiklmnopqrstuvwxyz-1234567890.") != std::string::npos){
			throw AddressException(NULL, "Parse URI Error: Invalid character in: "+str);
		}
				host = str;
	};
	std::string toString(){
		return host;
	};
};
/*
class URL : URI{
private:
	URL(){};
	std::string original;
	std::string proto;
	std::string host;
	std::string path;
public:
	static URI parse(std::string str){

	};
	std::string toString(){

	};
};*/

class NetworkEndpoint : public IP{
public:
	enum tlproto{
		tcp, udp
	};
private:
	uint16_t port = 0;
	std::string proto = "";
	enum tlproto tlproto;
public:
	NetworkEndpoint(IP& ip, uint16_t port, enum tlproto tlproto, std::string proto = ""):
	IP(ip), port(port), tlproto(tlproto), proto(proto){

	}
	NetworkEndpoint(IP& ip, std::string proto = ""):
	IP(ip), port(proto2port(proto)), proto(proto){

	}
	static uint16_t proto2port(const char* proto){
		struct servent* endpt = getservbyname(proto, NULL);
		if(!endpt){
			throw AddressException(NULL, "Protocol '"+std::string(proto)+"' was not found!");
		}
		uint16_t port = endpt->s_port;
		endservent();
		return port;
	}
	static uint16_t proto2port(const std::string proto){
		return proto2port(proto.c_str());
	}
};


}

