#include <graphviz/gvc.h>

#include <iostream>
#include <string>
#include <sstream>

#include <vector>
#include <tinyxml.h>

// Supported file formats
#include <DotFormat.h>
#include <AsFormat.h>
#include <HexFormat.h>
#include <RmxmlFormat.h>

using namespace dlvhex;
using namespace dlvhex::dd::tools::graphconverter;
using namespace std;

vector<IFormat*> loadFormats(){
	vector<IFormat*> formats;
	formats.push_back(new DotFormat());
	formats.push_back(new HexFormat());
	formats.push_back(new AsFormat());
	formats.push_back(new RmxmlFormat());
	return formats;
}

void unloadFormats(vector<IFormat*> formats){
	for (vector<IFormat*>::iterator it = formats.begin(); it != formats.end(); it++){
		delete *it;
	}
}

// Will read a diagram in the given format and return a pointer (or NULL if the format is not recognized or the diagram is illegal)
dlvhex::dd::util::DecisionDiagram* readDiagram(vector<IFormat*> formats, std::string inputformat){
	dlvhex::dd::util::DecisionDiagram* dd = NULL;

	// search and apply the appropriate format interpretation
	for (vector<IFormat*>::iterator fit = formats.begin(); fit != formats.end(); fit++){
		if (inputformat == (*fit)->getName() || inputformat == (*fit)->getNameAbbr()){
			return (*fit)->read();
		}
	}

	throw DecisionDiagram::InvalidDecisionDiagram(std::string("Input format \"") + inputformat + std::string("\" was not recognized"));
}

// Writes a diagram in the given format to standard out. Returns a boolean value to notify the caller about the success.
void writeDiagram(vector<IFormat*> formats, dlvhex::dd::util::DecisionDiagram* dd, std::string outputformat){

	// search and apply the appropriate format interpretation
	for (vector<IFormat*>::iterator fit = formats.begin(); fit != formats.end(); fit++){
		if (outputformat == (*fit)->getName() || outputformat == (*fit)->getNameAbbr()){
			(*fit)->write(dd);
			return;
		}
	}

	throw DecisionDiagram::InvalidDecisionDiagram(std::string("Output format \"") + outputformat + std::string("\" was not recognized"));
}

// prints a help message
void showHelp(){
	std::cout	<<	"graphconverter" << std::endl
			<<	"--------------" << std::endl << std::endl
			<<	"The graphconverter is used to convert decision diagrams between several" << std::endl
			<<	"file formats. It expects either one  or two parameters, namely:" << std::endl
			<<	"   1. --toas   or   --todot" << std::endl
			<<	"or" << std::endl
			<<	"   2. the source and the destination format" << std::endl << std::endl
			<<	" The following formats are implemented:" << std::endl << std::endl
			<<	" answerset or as ... The diagram is represented by atoms over" << std::endl
			<<	"                     the predicates root/1, innernode/1, leafnode/2" << std::endl
			<<	"                     conditionaledge/5 and elseedge/2, there the." << std::endl
			<<	"                     number is the arity." << std::endl
			<<	" hexprogram or   ... Essentially the same as \"answerset\", but this" << std::endl
			<<	"     hex             time in form of a set of facts. Thus, there will" << std::endl
			<<	"                     be points (.) rather than colons (,) between " << std::endl
			<<	"                     atoms, and there will be no brances ({, }) at" << std::endl
			<<	"                     the start and end of the string" << std::endl
			<<	" dot                 The diagram is in dot file format" << std::endl
			<<	"                     (see http://www.graphviz.org)" << std::endl << std::endl
			<<	" rmxml or            decision tree in xml format as used by" << std::endl << std::endl
			<<	"     xml             RapidMiner (http://www.rapidminer.com)" << std::endl << std::endl
			<<	"If the program is called with --todot, the source will be \"hex\"" << std::endl
			<<	"and the destination \"dot\"." << std::endl
			<<	"If the program is called with --toas, the source will be \"dot\"" << std::endl
			<<	"and the destination \"as\"." << std::endl << std::endl
			<<	"Passing the parameter --help will display this help message." << std::endl;
}

// ==================== Main program ====================
// Recognized parameters:
//   --help		... Output description of the program and it's parameters
//   --toas		... Convert a dot file (read from standard input)
//			    into an answer-set representation (written to
//			    standard output)
//			    This is the default mode if no option is proveded!
//   --todot		... Convert an answer set representation (read from
//			    standard input) of a graph into a dot file (written
//			    to standard output)
int main(int argc, char **argv){

	vector<IFormat*> formats = loadFormats();

	// will point to the input decision diagram
	dlvhex::dd::util::DecisionDiagram* dd = NULL;

	std::string inputformat;
	std::string outputformat;

	if (argc == 1){
		// default: --toas
		inputformat = std::string("dot");
		outputformat = std::string("as");
	}else if (argc == 2){
		// interpret conversion rule or as help call
		if (std::string(argv[1]) == std::string("--todot")){
			inputformat = std::string("hex");
			outputformat = std::string("dot");
		}else if (std::string(argv[1]) == std::string("--toas")){
			inputformat = std::string("dot");
			outputformat = std::string("as");
		}else if (std::string(argv[1]) == std::string("--help")){
			showHelp();
			return 0;
		}else{
			std::cerr << "Unknown option: " << argv[1];
			unloadFormats(formats);
			return 1;
		}
	}else if (argc == 3){
		// read source and destination format
		inputformat = std::string(argv[1]);
		outputformat = std::string(argv[2]);
	}

	// actual translation
	try{
		dd = readDiagram(formats, inputformat);

		// reading succeeded: write it
		writeDiagram(formats, dd, outputformat);

		// writing successful
		delete dd;
		unloadFormats(formats);
		return 0;
	}catch(DecisionDiagram::InvalidDecisionDiagram idd){
		// error
		unloadFormats(formats);
		std::cerr << idd.getMessage() << std::endl;
		return 1;
	}
}
