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

#ifndef DRIVERMODEL2_VISUALPERCEPTION_H
#define DRIVERMODEL2_VISUALPERCEPTION_H

#include <stdint.h>

namespace salbox {
	struct VisualPerception {
		VisualPerception() {
			id = 0;
			probability = 0;
			memory = 0;
		};

		/**
		 * @brief The unique identifier.
		 * This it should correspond the the identifier to tracked objects.
		 */
		uint32_t id;

		/**
		 * @brief The chance that the driver has noticed an object.
		 */
		float probability;

		float memory;
	};
}
#endif //DRIVERMODEL2_VISUALPERCEPTION_H
