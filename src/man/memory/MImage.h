/**
 * MImage.hpp
 *
 * @class MImage: this is the memory object representation of an Image
 *
 */

#pragma once

#include <protos/Sensors.pb.h>
#include "RoboImage.h"
#include "MObject.h"
#include "Sensors.h"
#include "ClassHelper.h"

namespace man {
namespace memory {

class MImage: public MObject<proto::PImage> {

ADD_SHARED_PTR(MImage);

public:
    static const MObject_ID id = MIMAGE_ID;

public:
    MImage(boost::shared_ptr<Sensors> sensors,
           data_ptr data = data_ptr(new proto::PImage));
    virtual ~MImage();
    /**
     * Updates all the fields of the underlying proto::PSensors with values
     * from the Sensors object
     */
    void updateData();

    boost::shared_ptr<const proto::PImage> getThresholded() const {
        return thresholded_data;
    }

private:
    boost::shared_ptr<Sensors> sensors;
    boost::shared_ptr<proto::PImage> thresholded_data;

};
}
}
