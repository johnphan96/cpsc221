
// File:        main.cpp
// Author:      cheeren
// Date:        2018-02-25
// Description: Partial test of PA3 functionality
//              Reads Point data from external files
//              Produces PNG images of the point sets


#include "qtcount.h"
#include "qtvar.h"
#include "cs221util/RGBAPixel.h"
#include "cs221util/PNG.h"

using namespace cs221util;
using namespace std;

int main()
{


	// read in image
	PNG origIm1;
	origIm1.readFromFile("images/orig/PA3.png");


	// use it to build a quadtree
	qtcount t1(origIm1);
	qtcount tCopy1(t1);
	

	// prune the quadtree
	tCopy1.prune(10000); 


    
	// render the quadtree
	PNG ppic1 = tCopy1.render();



	ppic1.writeToFile("images/out/output-prunedPA3.png");


	// use it to build a quadtree
	qtvar v1(origIm1);
	qtvar vCopy1(v1);


	// prune the quadtree
	vCopy1.prune(10000); 

	
	// render the quadtree
	PNG vpic1 = vCopy1.render();



	vpic1.writeToFile("images/out/output-prunedPA3-var.png");


    // comparisons

    qtcount countcomp(origIm1);
    qtvar varcomp(origIm1);

    countcomp.prune(countcomp.idealPrune(10000));
    varcomp.prune(varcomp.idealPrune(10000));

/*
// I wrote two member functions to help me understand the different 
// compression strategies. I'm leaving this code here, so you 
// can explore similarly, if you like. The functions have been
// removed from the given code, but you can add them if you want!!
    cout << "count: " << countcomp.count() << endl;
    cout << "count dim: " << countcomp.dimsum() << endl;
    cout << "var: " << varcomp.count() << endl;
    cout << "var dim: " << varcomp.dimsum() << endl;
*/

    PNG countpng = countcomp.render();
    PNG varpng = varcomp.render();

    countpng.writeToFile("images/out/output-comp-count-stanley");
    varpng.writeToFile("images/out/output-comp-var-stanley");

  return 0;
}

