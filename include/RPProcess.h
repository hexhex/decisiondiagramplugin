#ifndef __RPProcess_h_
#define __RPProcess_h_

#include <dlvhex/Process.h>
#include <dlvhex/DLVProcess.h>
#include <dlvhex/globals.h>

#include <stdio.h>

DLVHEX_NAMESPACE_USE

class RPProcess : public DLVProcess{
private:

public:
	RPProcess() : DLVProcess(){
	}

	virtual std::string
	path() const
	{
		return std::string("rpcompiler");
	}

	virtual std::vector<std::string>
	commandline() const
	{
		std::vector<std::string> tmp;
		tmp.push_back(path());
	
		return tmp;
	}

	virtual void
	spawn()
	{
		setupStreams();
		proc.open(commandline());
	}
};

#endif
