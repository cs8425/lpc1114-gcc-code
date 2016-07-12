#ifndef FUZZYSETUP_H
#define FUZZYSETUP_H

#include "fuzzyInt.h"
//#include "fuzzyC.h"

//PD-type Sugeno Fuzzy Module

FuzzyInput Ain1;
FuzzyInput Ain2;
FuzzyRule Arule;
Fuzzy Af;

void FuzzySetup(void) {
	inputInit(&Ain1,7,-1024.000,1024.000);
	inputInit(&Ain2,7,-1024.000,1024.000);
	fuzzyInit(&Af,&Ain1,&Ain2,&Arule);

	setTrimf(&Ain1,0,-1024.000,-877.7000,-877.7000,-585.1400);
	setTrimf(&Ain1,1,-877.7000,-585.1400,-585.1400,-292.5700);
	setTrimf(&Ain1,2,-585.1400,-292.5700,-292.5700, 0.000000);
	setTrimf(&Ain1,3,-292.5700, 0.000000, 0.000000, 292.5700);
	setTrimf(&Ain1,4, 0.000000, 292.5700, 292.5700, 585.1400);
	setTrimf(&Ain1,5, 292.5700, 585.1400, 585.1400, 877.7000);
	setTrimf(&Ain1,6, 585.1400, 877.7000, 877.7000, 1024.000);

	setTrimf(&Ain2,0,-1024.000,-877.7000,-877.7000,-585.1400);
	setTrimf(&Ain2,1,-877.7000,-585.1400,-585.1400,-292.5700);
	setTrimf(&Ain2,2,-585.1400,-292.5700,-292.5700, 0.000000);
	setTrimf(&Ain2,3,-292.5700, 0.000000, 0.000000, 292.5700);
	setTrimf(&Ain2,4, 0.000000, 292.5700, 292.5700, 585.1400);
	setTrimf(&Ain2,5, 292.5700, 585.1400, 585.1400, 877.7000);
	setTrimf(&Ain2,6, 585.1400, 877.7000, 877.7000, 1024.000);

	setRule(&Arule,0,6, 0.000000);
	setRule(&Arule,0,5,-409.6000);
	setRule(&Arule,0,4,-819.2000);
	setRule(&Arule,0,3,-1024.000);
	setRule(&Arule,0,2,-1024.000);
	setRule(&Arule,0,1,-1024.000);
	setRule(&Arule,0,0,-1024.000);

	setRule(&Arule,1,6, 409.6000);
	setRule(&Arule,1,5, 0.000000);
	setRule(&Arule,1,4,-409.6000);
	setRule(&Arule,1,3,-819.2000);
	setRule(&Arule,1,2,-819.2000);
	setRule(&Arule,1,1,-819.2000);
	setRule(&Arule,1,0,-1024.000);

	setRule(&Arule,2,6, 819.2000);
	setRule(&Arule,2,5, 409.6000);
	setRule(&Arule,2,4, 0.000000);
	setRule(&Arule,2,3,-409.6000);
	setRule(&Arule,2,2,-819.2000);
	setRule(&Arule,2,1,-819.2000);
	setRule(&Arule,2,0,-1024.000);

	setRule(&Arule,3,6, 1024.000);
	setRule(&Arule,3,5, 819.2000);
	setRule(&Arule,3,4, 409.6000);
	setRule(&Arule,3,3, 0.000000);
	setRule(&Arule,3,2,-409.6000);
	setRule(&Arule,3,1,-819.2000);
	setRule(&Arule,3,0,-1024.000);

	setRule(&Arule,4,6, 1024.000);
	setRule(&Arule,4,5, 819.2000);
	setRule(&Arule,4,4, 819.2000);
	setRule(&Arule,4,3, 409.6000);
	setRule(&Arule,4,2, 0.000000);
	setRule(&Arule,4,1,-409.6000);
	setRule(&Arule,4,0,-819.2000);

	setRule(&Arule,5,6, 1024.000);
	setRule(&Arule,5,5, 819.2000);
	setRule(&Arule,5,4, 819.2000);
	setRule(&Arule,5,3, 819.2000);
	setRule(&Arule,5,2, 409.6000);
	setRule(&Arule,5,1, 0.000000);
	setRule(&Arule,5,0,-409.6000);

	setRule(&Arule,6,6, 1024.000);
	setRule(&Arule,6,5, 1024.000);
	setRule(&Arule,6,4, 1024.000);
	setRule(&Arule,6,3, 1024.000);
	setRule(&Arule,6,2, 819.2000);
	setRule(&Arule,6,1, 409.6000);
	setRule(&Arule,6,0, 0.000000);

//	setFuzzifiType(&Af,0);
//	setAndType(&Af,1);
//	setImpType(&Af,1);
}


#endif

