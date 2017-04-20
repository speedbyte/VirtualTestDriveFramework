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

#ifndef DRIVERMODEL2_GAZE_H
#define DRIVERMODEL2_GAZE_H

#include <stdint.h>
#include <stddef.h>

namespace salbox {
	/**
	 * @brief ...
	 * @todo Should we define a coordinate system?
	 * @todo Should we add a quality value?
	 * @todo Should we add right and left eye?
	 */
	struct Gaze {
		Gaze() {
			timestamp = 0;
			gazeOrigin[0] = 0;
			gazeOrigin[1] = 0;
			gazeOrigin[2] = 0;
			gazeDirection[0] = 0;
			gazeDirection[1] = 0;
			gazeDirection[2] = 0;
		};

		/**
		 * @brief The timestamp in milli seconds.
		 */
		int64_t timestamp;

		/**
		 * @brief The origin of gaze as 3D vector in meter.
		 */
		double gazeOrigin[3];

		/**
		 * @brief The direction of gaze as 3D vector.
		 * @todo Should this vector be normalized (length of 1)?
		 */
		double gazeDirection[3];
	};

	/**
	 * @brief ...
	 */
	struct GazeMovementClassifier {
		GazeMovementClassifier() : gazeClass(GC_UNKNOWN), angle(0), angleSpeed(0) {};

		/**
 		 * @brief The gaze class.
 		 */
		enum GazeClassifiers {
			GC_UNKNOWN,
			GC_SACCADE,
			GC_FIXATION
		} gazeClass;

		/**
 		 * @brief The angle in degrees between this gaze and the one before.
 		 */
		double angle;

		/**
		 * @brief The angle speed in degrees/sec between this gaze the one before.
		 */
		double angleSpeed;
	};


	struct GazeTracking {
		/*
		 * fixation object id
		 */
		uint32_t id;

		/*
		 * number of recorded fixations
		 */
		size_t numFixations;

		/*
		 * timestamp of the last fixation
		 */
		int64_t lastFixation;

		/*
		 * timestamp of the first fixation
		 */
		int64_t firstFixation;

		/*
		 * maximal value of the object density
		 */
		double maxDense;
	};

}


#endif //DRIVERMODEL2_GAZE_H
