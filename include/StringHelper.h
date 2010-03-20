#ifndef _STRINGHELPER_H_
#define _STRINGHELPER_H_

#include <string>
#include <map>
#include <vector>

/**
 * \brief
 * This class is designed to extract certain parts of strings as they are used to represent inner and leaf nodes of a decision diagram.
 * The supported file formats (dot, answer-sets, xml, etc.) provide different possibilities how the information of a node can be encoded. Some of them are stronger and others are weaker.
 * Therefore it is sometimes necessary to "compress" the information into a single string value. For instance, the dot format does only allow a node name and a label, but we need to store
 * maps of key/value pairs. This can only be done by combining them into a single value such that it can be decomposed again later.
 * The most general format, where all the information is put into one string value, is as follows:
 *        nodename [class {c1:n1,c2:n2,...,cm:nm}]
 * nodename is an arbitrary string (without squared brackets or braces)
 * class is the classification in this node (a leaf node), without curly braces
 * c_i/n_i represent the class value distribution. For instance, a legal label is: "class2 {class1:3,class2:5}", meaning that in this node we have 3 samples of class1 and 5 of class2,
 * thus the classification is class2.
 * Node that parts of this general syntax can be skipped as needed. For instance, inner nodes will not need to store any label. Further, some decision diagrams will not store a distribution map.
 * This class will always try to figure out the intention and return the string parts respectvely
 */
namespace dlvhex{
	namespace dd{
		namespace util{
			class StringHelper{
			public:
				/**
				 * \brief
				 * Is thrown when a string part should be extracted that is actually not contained
				 */
				class NotContainedException{};
				static std::string unquote(std::string rs);
				static std::string extractLabel(std::string str);
				static std::string extractClassification(std::string str);
				static std::string extractClass(std::string classification);
				static std::map<std::string, int> extractDistribution(std::string classification);
				static std::string encodeDistributionMap(std::map<std::string, int> map);
				static std::string extractKey(std::string str);
				static std::string extractValue(std::string str);
				static std::string toString(int v);
				static std::string toString(double v);
				static int atoi(std::string str);
				static double atof(std::string str);
			};
		}
	}
}

#endif


/*! \fn static std::string dlvhex::dd::util::StringHelper::unquote(std::string str);
 * Removes leading and tailing whitespaces
 * \param str Input
 *  \return std::string Unquoted string
 */

/*! \fn static std::string dlvhex::dd::util::StringHelper::extractLabel(std::string str);
 * Expects a string of the form: "nodename [nodelabel]", where nodename and nodelabel can be arbitrary strings.
 * \param str Input
 *  \return std::string Will return the nodename-part of the string
 */

/*! \fn static std::string dlvhex::dd::util::StringHelper::extractClassification(std::string str);
 * Expects a string of the form: "nodename [nodelabel]", where nodename and nodelabel can be arbitrary strings.
 * \param str Input
 *  \return std::string Will return the nodelabel-part of the string
 */

/*! \fn static std::string dlvhex::dd::util::StringHelper::extractClass(std::string classification)
 * Expects a class label of the form: "class {c1:v1,c2:v2,...,cn:vn}",  where "class" is some classification cathegory and c_i/v_i represent a distribution over the possibilities.
 * \param str Input
 *  \return std::string Will return the class-part of the string
 */

/*! \fn static std::map<std::string, int> dlvhex::dd::util::StringHelper::extractDistribution(std::string classification)
 * Expects a class label of the form: "class {c1:v1,c2:v2,...,cn:vn}",  where "class" is some classification cathegory and c_i/v_i represent a distribution over the possibilities.
 * \param str Input
 *  \return std::map<std::string, int> Will return the v_i/n_i-pairs of the string (in form of a map)
 */

/*! \fn static std::string dlvhex::dd::util::StringHelper::encodeDistributionMap(std::map<std::string, int> map)
 * Encodes the given distribution map as string
 * \param map Input
 *  \return std::string A string representation of map
 */

/*! \fn static std::string dlvhex::dd::util::StringHelper::extractKey(std::string str)
 * Expects a key-value pair of kind "key:value"
 * \param str Input
 *  \return std::string The key part of the string
 */

/*! \fn static std::string dlvhex::dd::util::StringHelper::extractValue(std::string str)
 * Expects a value-value pair of kind "key:value"
 * \param str Input
 *  \return std::string The value part of the string
 */

/*! \fn static std::string dlvhex::dd::util::StringHelper::toString(int v)
 * Converts an integer value into a string
 * \param v Input
 *  \return std::string The input as string
 */

/*! \fn static std::string dlvhex::dd::util::StringHelper::toString(double v)
 * Converts an double value into a string
 * \param v Input
 *  \return std::string The input as string
 */

/*! \fn static int dlvhex::dd::util::StringHelper::atoi(std::string str)
 * Converts the input into an integer. Will throw a NotContainedException in case that str does not contain an integer.
 * \param str Input
 *  \return int The integer value of str
 */

/*! \fn static double dlvhex::dd::util::StringHelper::atof(std::string str)
 * Converts the input into a double value. Will throw a NotContainedException in case that str does not contain a valid double value.
 * \param str Input
 *  \return double The double value of str
 */
