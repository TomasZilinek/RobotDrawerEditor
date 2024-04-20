#include "BezierCurve.h"


								/////////////////////////////
								// SimpleBezierCurve Class //
								/////////////////////////////


SimpleBezierCurve::SimpleBezierCurve() {

}


								////////////////////////////////
								// CompositeBezierCurve Class //
								////////////////////////////////


CompositeBezierCurve::CompositeBezierCurve() {

}

CompositeBezierCurve::CompositeBezierCurve(SimpleBezierCurve* firstCurve) {
	curves.push_back(firstCurve);
}

void CompositeBezierCurve::addCurve(SimpleBezierCurve* newCurve, bool endpointIsSharp) {
	curves.push_back(newCurve);
	endPointsSharpness.push_back(endpointIsSharp);
}
