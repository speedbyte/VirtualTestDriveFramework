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

#ifndef DRIVERMODEL2_IMAGE_H
#define DRIVERMODEL2_IMAGE_H

#include <stdint.h>
#include <stddef.h>

namespace salbox {

	// TODO: do we need a timestamp?

	template<class T>
	struct Image {
		Image(int width = 0, int height = 0) : width(width), height(height) {
			if (width > 0 && height > 0)
				buffer = new T[width * height];
			else
				buffer = NULL;
		}

		~Image() {
			delete[] buffer;
		}

		int32_t size() const {
			return width * height;
		}

		T *pixel(int x, int y) const {
			if (x * y >= 0 && x * y <= size())
				return buffer + (y * width + x);
			return NULL;
		}

		const int width, height;

		T *buffer;
	};

	typedef Image<uint8_t> GrayscaleImage;
	typedef Image<uint8_t[3]> RGBImage;
	typedef Image<float> SalientMap; //TODO: rename SaliencyMap
}

#endif //DRIVERMODEL2_IMAGE_H
