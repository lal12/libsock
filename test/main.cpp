/*
 * main.cpp
 *
 *  Created on: 14.01.2017
 *      Author: luca
 */
#include <string>
#include <iostream>
#include <deque>
#include <utility>
using namespace std;

#include "address.hpp"
#include "interface.hpp"
using namespace sock;

int testIPv4(){
	unsigned int failed = 0;
	deque<pair<const char*, uint32_t>> arr1 = {
		{"0.0.0.0", 0},
		{"255.255.255.255", 0xFFFFFFFF},
		{"10.11.192.208", 0x0A0BC0D0},
		{"1.2.3.4", 0x01020304},
	};
	for(unsigned int i = 0; i < arr1.size(); i++){
		IPv4 ip(arr1[i].second);
		string ipStr = ip.toString();
		if(ipStr != arr1[i].first){
			failed++;
			cout << "Failed: Input: " << arr1[i].second << " Output: " << ipStr;
			cout << " != Expected: " << arr1[i].first << endl;
		}
		try{
			ip = IPv4::parse(arr1[i].first);
			if(ip.getIP() != arr1[i].second){
				failed++;
				cout << "Failed: Input: " << arr1[i].first << " Output: " << ip.getIP();
				cout << " != Expected: " << arr1[i].second << endl;
			}
		}catch(AddressException& e){
			failed++;
			cout << "Failed: Parsing '" << arr1[i].first << "' throwed exception: " << e.what() << endl;
		}
	}
	deque<const char*> arr2 = {"255.255.255", "255.255.255.0111", "0.255.344.255", "0001.255.23.255", "1a.25d.23.255"};
	for(unsigned int i=0; i<arr2.size(); i++){
		try{
			IPv4::parse(arr2[i]);
			failed++;
			cout << "Failed: Parsing " << arr2[i] << " didn't throw exception" << endl;
		}catch(AddressException& e){}
	}
	return !failed;
}

int testNetworkEndpoint(){
	NetworkEndpoint::proto2port("");
	return 1;
}

int testInterface(){

	return 1;
}


int main(int argc, char** argv){
	int success = 0;
	deque<pair<string, int(*)()>> tests = {
		{"IPv4", testIPv4},
		{"NetworkEndpoint", testNetworkEndpoint},
		{"Interface", testInterface}
	};
	for(unsigned int i=0; i < tests.size(); i++){
		cout << "Testing " << tests[i].first << "..." << endl;
		if(!tests[i].second()){
			cout << "Test " << tests[i].first << " failed!" << endl;
		}else{
			success++;
			cout << "Test " << tests[i].first << " succeeded!" << endl;
		}
	}
	cout << success << "/" << tests.size() << " Test succeeded!" << endl;
	return 0;
}
