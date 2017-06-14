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

#ifndef DRIVERMODEL2_TRACKEDOBJECT_H
#define DRIVERMODEL2_TRACKEDOBJECT_H

#include <stdint.h>
#include <stddef.h>

namespace salbox {
	struct TrackedObject {
		TrackedObject() {
			timestamp = 0;
			id = 0;
			position[0] = 0;
			position[1] = 0;
			position[2] = 0;
			orientation[0] = 0;
			orientation[1] = 0;
			orientation[2] = 0;
			width = 0;
			length = 0;
			height = 0;
			classification = 0;
		}

		/**
 		* @brief The timestamp in milli seconds.
 		*/
		int64_t timestamp;

		/**
		 * @brief Unique object identifier.
		 */
		uint32_t id;

		/**
		 * @brief The position of the tracked object relative to vehicle.
		 * The reference point is always the lower left corner of the tracked object.
		 * @todo Define coordinate system
		 */
		double position[3];

		/**
		 * @brief Orientation of the tracked objects as normalized 3D position vector.
		 */
		double orientation[3];

		/**
		 * @brief The width of this tracked object in meter.
		 */
		double width;

		/**
		 * @brief The length of this tracked object in meter.
		 */
		double length;

		/**
		 * @brief The height of this tracked object in meter.
		 */
		double height;

		/**
		 * @brief The classification of this tracked object (e.g. passenger car, bus, truck, bicycle, ..).
		 */
		uint32_t classification;
	};

	struct TrackedObjectSaliency {
		TrackedObjectSaliency() {
			id = 0;
			max = 0;
			mean = 0;
			integral = 0;
			numPixels = 0;
			maxRel = 0;
			meanRel = 0;
		};

		/**
 		* @brief Unique object identifier.
 		*/
		uint32_t id;

		/**
		* maximal saliency value in object boundingbox
		*/
		double max;

		/**
		 * mean saliency value in object boundingbox
		 */
		double mean;

		/**
		 * integral of the saliency values in object boundingbox
		 */
		double integral;

		/**
		 * number of saliency pixels
		 */
		size_t numPixels;

		/**
		 * maximal saliency value in object boundingbox normalized
		 * by the maximal saliency value of the scene
		 */
		double maxRel;

		/**
		 * mean saliency value in object boundingbox normalized
		 * by the mean saliency value of the scene
		 */
		double meanRel;
	};
}

#endif //DRIVERMODEL2_TRACKEDOBJECT_H
