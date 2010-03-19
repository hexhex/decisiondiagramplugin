#include <StringHelper.h>

#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include <sstream>


std::string StringHelper::unquote(std::string rs){
	if (rs.length() >= 2 && rs[0] == '\"' && rs[rs.length() - 1] == '\"') return rs.substr(1, rs.length() - 2);
	else return rs;
}

std::string StringHelper::extractLabel(std::string str){
	return (str.find_first_of("[") != std::string::npos) ? boost::trim_copy(str.substr(0, str.find_first_of("["))) : str;
}

std::string StringHelper::extractClassification(std::string str){
	if (str.find_first_of("[") > 0 && str.find_last_of("]") > 0 && str.find_first_of("[") < str.find_last_of("]")){
		return str.substr(str.find_first_of("[") + 1, str.find_last_of("]") - str.find_first_of("[") - 1);
	}else{
		// no classification
		throw NotContainedException();
	}
}

std::string StringHelper::extractClass(std::string classification){
	return (classification.find_first_of("{") != std::string::npos) ? boost::trim_copy(classification.substr(0, classification.find_first_of("{"))) : classification;
}

std::map<std::string, int> StringHelper::extractDistribution(std::string classification){

	if (classification.find_first_of("{") > 0 && classification.find_last_of("}") > 0 && classification.find_first_of("{") < classification.find_last_of("}")){
		std::map<std::string, int> map;
		std::string mappart = classification.substr(classification.find_first_of("{") + 1, classification.find_last_of("}") - classification.find_first_of("{") - 1);

		// for all kv-pairs
		std::vector<std::string> kvpairs; //split(mappart, ',');
		boost::split(kvpairs, mappart, boost::is_any_of(","));
		for (std::vector<std::string>::iterator kv = kvpairs.begin(); kv != kvpairs.end(); kv++){
			// extract key and value
			std::string key = extractKey(*kv);
			int value = StringHelper::atoi(extractValue(*kv));
			// add it
			map.insert(std::pair<std::string, int>(key, value));
		}

		return map;
	}else{
		// no classification
		throw NotContainedException();
	}
}

std::string StringHelper::encodeDistributionMap(std::map<std::string, int> map){
	std::stringstream ss;
	ss << "{";
	for (std::map<std::string, int>::iterator it = map.begin(); it != map.end(); it++){
		if (it != map.begin()) ss << ",";
		ss << it->first << ":" << it->second;
	}
	ss << "}";
	return ss.str();
}

std::string StringHelper::extractKey(std::string str){
	if (str.find_first_of(":"))
		return str.substr(0, str.find_first_of(":"));
	else
		throw NotContainedException();
}

std::string StringHelper::extractValue(std::string str){
	if (str.find_first_of(":"))
		return str.substr(str.find_first_of(":") + 1);
	else
		throw NotContainedException();
}

std::string StringHelper::toString(int v){
	std::stringstream ss;
	ss << v;
	return ss.str();
}

std::string StringHelper::toString(double v){
	std::stringstream ss;
	ss << v;
	return ss.str();
}

int StringHelper::atoi(std::string str){
	int v = ::atoi(str.c_str());
	if (v == 0){
		// plausibility check: error or actually 0?
		if (str.length() == 0) throw NotContainedException();
		for (int i = 0; i < str.length(); i++)
			if (str[i] != '0') throw NotContainedException();
	}
	return v;
}

double StringHelper::atof(std::string str){
	double v = ::atof(str.c_str());
	if (v == 0.0){
		// plausibility check: error or actually 0?
		if (str.length() == 0) throw NotContainedException();
		bool commaFound = true;
		for (int i = 0; i < str.length(); i++){
			if (commaFound && str[i] != '0' || !commaFound && str[i] != '0' && str[i] != '.') throw NotContainedException();
			if (str[i] == '.') commaFound = true;
		}
	}
	return v;
}
