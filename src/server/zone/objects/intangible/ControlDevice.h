/*
 *	server/zone/objects/intangible/ControlDevice.h generated by engine3 IDL compiler 0.60
 */

#ifndef CONTROLDEVICE_H_
#define CONTROLDEVICE_H_

#include "engine/core/Core.h"

#include "engine/core/ManagedReference.h"

#include "engine/core/ManagedWeakReference.h"

namespace server {
namespace zone {
namespace objects {
namespace creature {

class VehicleObject;

} // namespace creature
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::creature;

namespace server {
namespace zone {
namespace objects {
namespace player {

class PlayerCreature;

} // namespace player
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::player;

namespace server {
namespace zone {
namespace objects {
namespace creature {

class CreatureObject;

} // namespace creature
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::creature;

namespace server {
namespace zone {
namespace objects {
namespace scene {

class SceneObject;

} // namespace scene
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::scene;

namespace server {
namespace zone {

class Zone;

} // namespace zone
} // namespace server

using namespace server::zone;

#include "server/zone/managers/radial/RadialOptions.h"

#include "engine/lua/LuaObject.h"

#include "server/zone/objects/intangible/IntangibleObject.h"

namespace server {
namespace zone {
namespace objects {
namespace intangible {

class ControlDevice : public IntangibleObject {
public:
	ControlDevice();

	void updateToDatabaseAllObjects(bool startTask);

	void storeObject(PlayerCreature* player);

	void generateObject(PlayerCreature* player);

	void setControlledObject(CreatureObject* object);

	CreatureObject* getControlledObject();

	bool isControlDevice();

	DistributedObjectServant* _getImplementation();

	void _setImplementation(DistributedObjectServant* servant);

protected:
	ControlDevice(DummyConstructorParameter* param);

	virtual ~ControlDevice();

	friend class ControlDeviceHelper;
};

} // namespace intangible
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::intangible;

namespace server {
namespace zone {
namespace objects {
namespace intangible {

class ControlDeviceImplementation : public IntangibleObjectImplementation {
protected:
	ManagedReference<CreatureObject* > controlledObject;

public:
	ControlDeviceImplementation();

	ControlDeviceImplementation(DummyConstructorParameter* param);

	void updateToDatabaseAllObjects(bool startTask);

	virtual void storeObject(PlayerCreature* player);

	virtual void generateObject(PlayerCreature* player);

	void setControlledObject(CreatureObject* object);

	CreatureObject* getControlledObject();

	bool isControlDevice();

	ControlDevice* _this;

	operator const ControlDevice*();

	DistributedObjectStub* _getStub();
	virtual void readObject(ObjectInputStream* stream);
	virtual void writeObject(ObjectOutputStream* stream);
protected:
	virtual ~ControlDeviceImplementation();

	void finalize();

	void _initializeImplementation();

	void _setStub(DistributedObjectStub* stub);

	void lock(bool doLock = true);

	void lock(ManagedObject* obj);

	void rlock(bool doLock = true);

	void wlock(bool doLock = true);

	void wlock(ManagedObject* obj);

	void unlock(bool doLock = true);

	void runlock(bool doLock = true);

	void _serializationHelperMethod();
	bool readObjectMember(ObjectInputStream* stream, const String& name);
	int writeObjectMembers(ObjectOutputStream* stream);

	friend class ControlDevice;
};

class ControlDeviceAdapter : public IntangibleObjectAdapter {
public:
	ControlDeviceAdapter(ControlDeviceImplementation* impl);

	Packet* invokeMethod(sys::uint32 methid, DistributedMethod* method);

	void updateToDatabaseAllObjects(bool startTask);

	void storeObject(PlayerCreature* player);

	void generateObject(PlayerCreature* player);

	void setControlledObject(CreatureObject* object);

	CreatureObject* getControlledObject();

	bool isControlDevice();

};

class ControlDeviceHelper : public DistributedObjectClassHelper, public Singleton<ControlDeviceHelper> {
	static ControlDeviceHelper* staticInitializer;

public:
	ControlDeviceHelper();

	void finalizeHelper();

	DistributedObject* instantiateObject();

	DistributedObjectServant* instantiateServant();

	DistributedObjectAdapter* createAdapter(DistributedObjectStub* obj);

	friend class Singleton<ControlDeviceHelper>;
};

} // namespace intangible
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::intangible;

#endif /*CONTROLDEVICE_H_*/
