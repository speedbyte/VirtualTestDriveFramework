/*
 * Drivermodel2
 *
 * Copyright (c) 2005-20017 NORDSYS GmbH Braunschweig.
 * All rights reserved.
 *
 * Authors:
 * Andre Geisler, NORDSYS GmbH, a.geisler@nordsys.de
 * 
 * Correspondence should be directed to NORDSYS:
 * 
 * NORDSYS GmbH 
 * Andre Geisler
 * Altewiekring 20A
 * D-38102 Braunschweig
 * GERMANY
 * www.nordsys.de
 *
 * email: info@nordsys.de
 *
 * 
 * Neither NORDSYS nor the authors admit liability
 * nor provide any warranty for any of this software.
 * This material is provided "AS-IS"
 *
 *
 * Until the distribution is granted by NORDSYS 
 * this sourcecode is under non disclosure and must
 * only be used within projects with controlling interest of NORDSYS.
 */

#include <adtf/salbox/Image.h>
#include <adtf/salbox/Gaze.h>
#include <adtf/salbox/TrackedObject.h>
#include <adtf/salbox/VisualPerception.h>
#include <iostream>

using namespace std;
using namespace salbox;

int main() {
	RGBImage rgb(2, 2);
	cout << "rgb size=" << rgb.size() << endl;
	*rgb.pixel(0, 0)[0] = 255;
	*rgb.pixel(1, 1)[0] = 255;
	for (int i = 0; i < rgb.size(); ++i) {
		cout << "pixel=" << i << ": r=" << ((int) rgb.buffer[i][0]) << " g=" << ((int) rgb.buffer[i][1]) << " b="
		     << ((int) rgb.buffer[i][2]) << endl;
	}

	SalientMap salmap(3, 3);
	*salmap.pixel(0, 0) = 1.f;
	*salmap.pixel(1, 1) = 1.f;
	*salmap.pixel(2, 2) = 1.f;
	for (int i = 0; i < salmap.size(); ++i) {
		cout << "pixel=" << i << ": f=" << salmap.buffer[i] << endl;
	}

	Gaze g;
	GazeMovementClassifier gmc;
	TrackedObject to;
	VisualPerception vp;

	return 0;
}