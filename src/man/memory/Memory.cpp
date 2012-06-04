
#include <iostream>

#include "Common.h"
#include "Memory.h"

namespace man {
namespace memory {

using boost::shared_ptr;
using namespace proto;
using namespace std;

Memory::Memory(shared_ptr<Vision> vision_ptr,
               shared_ptr<Sensors> sensors_ptr,
               shared_ptr<LocSystem> loc_ptr) :
        mVision(new MVision(vision_ptr)),
        mVisionSensors(new MVisionSensors(sensors_ptr)),
        mMotionSensors(new MMotionSensors(sensors_ptr)),
        mImage(new MImage(sensors_ptr)),
        mLocalization(new MLocalization(loc_ptr))
{

#if defined USE_MEMORY || defined OFFLINE
    if(sensors_ptr.get()) {
        sensors_ptr->addSubscriber(mVisionSensors.get(), NEW_VISION_SENSORS);
        sensors_ptr->addSubscriber(mMotionSensors.get(), NEW_MOTION_SENSORS);
        sensors_ptr->addSubscriber(mImage.get(), NEW_IMAGE);
    }
#endif

    mobject_IDMap.insert(MObject_IDPair(string(typeid(MVision).name()), mVision));
    mobject_IDMap.insert(MObject_IDPair(string(typeid(MVisionSensors).name()), mVisionSensors));
    mobject_IDMap.insert(MObject_IDPair(string(typeid(MMotionSensors).name()), mMotionSensors));
    mobject_IDMap.insert(MObject_IDPair(string(typeid(MImage).name()), mImage));
    mobject_IDMap.insert(MObject_IDPair(string(typeid(MLocalization).name()), mLocalization));
}

Memory::~Memory() {
    cout << "Memory destructor" << endl;
}

void Memory::update(boost::shared_ptr<Object> obj) {
    obj->update();
}

void Memory::updateVision() {
    update(mVision);
}

Memory::Object::const_ptr Memory::getMObject(MObject_ID id) const {
    MObject_IDMap::const_iterator it = mobject_IDMap.find(id);

    if (it != mobject_IDMap.end()) {
        return it->second;
    } else {
        return Memory::Object::const_ptr();
    }
}

Memory::Object::ptr Memory::getMutableMObject(MObject_ID id) {
    MObject_IDMap::iterator it = mobject_IDMap.find(id);

    if (it != mobject_IDMap.end()) {
        return it->second;
    } else {
        return Memory::Object::ptr();
    }
}

}
}
