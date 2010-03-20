#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <vector>

#include "features/f1.h"
#include "features/f2.h"
#include "features/f3.h"
#include "features/f4.h"
#include "features/f5.h"
#include "features/f6.h"
#include "features/f7.h"
#include "features/f8.h"
#include "features/f9.h"
#include "features/f10.h"
#include "features/f11.h"
#include "features/f12.h"
#include "features/f13.h"
#include "features/f14.h"
#include "features/f15.h"
#include "features/f16.h"
#include "features/f17.h"
#include "features/f18.h"
#include "features/f19.h"
#include "features/f20.h"
#include "features/f21.h"
#include "features/f22.h"

using namespace std;
using namespace dlvhex::dd::tools::dna;


int main(int argc, char* argv[])
{
	if (argc < 2){
		cout << "Filename expected!" << endl;
		return 1;
	}else{
		ifstream ifs(argv[1]);
		string sequence;
		vector<DNAFeature*> features;

		// instanciate features
		f1 _f1; f2 _f2; f3 _f3; f4 _f4; f5 _f5; f6 _f6; f7 _f7; f8 _f8; f9 _f9; f10 _f10; f11 _f11; f12 _f12; f13 _f13; f14 _f14; f15 _f15; f16 _f16; f17 _f17; f18 _f18; f19 _f19; f20 _f20; // f21 _f21; f22 _f22;
		features.push_back(&_f1); features.push_back(&_f2); features.push_back(&_f3); features.push_back(&_f4); features.push_back(&_f5); features.push_back(&_f6); features.push_back(&_f7); features.push_back(&_f8); features.push_back(&_f9); features.push_back(&_f10); features.push_back(&_f11); features.push_back(&_f12); features.push_back(&_f13); features.push_back(&_f14); features.push_back(&_f15); features.push_back(&_f16); features.push_back(&_f17); features.push_back(&_f18); features.push_back(&_f19); features.push_back(&_f20); // features.push_back(&_f21); 

		// use comma as decimal point
		/*
		struct lconv locale_structure;
		struct lconv *locale_ptr=&locale_structure;

		locale_ptr=localeconv();
		*(locale_ptr->decimal_point) = ',';
		*/

		// compute features
		while(std::getline(ifs, sequence)){
			cout << sequence;
			for (vector<DNAFeature*>::iterator feature = features.begin(); feature != features.end(); feature++){
				printf(" %f", (*feature)->extract(sequence));
			}
			cout << endl;
		}

		return 0;
	}
}
