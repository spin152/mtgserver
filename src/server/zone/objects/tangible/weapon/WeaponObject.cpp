/*
 *	server/zone/objects/tangible/weapon/WeaponObject.cpp generated by engine3 IDL compiler 0.60
 */

#include "WeaponObject.h"

#include "server/zone/objects/scene/SceneObject.h"

/*
 *	WeaponObjectStub
 */

WeaponObject::WeaponObject(LuaObject* templateData) : TangibleObject(DummyConstructorParameter::instance()) {
	_impl = new WeaponObjectImplementation(templateData);
	_impl->_setStub(this);
}

WeaponObject::WeaponObject(DummyConstructorParameter* param) : TangibleObject(param) {
}

WeaponObject::~WeaponObject() {
}


void WeaponObject::initializeTransientMembers() {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 6);

		method.executeWithVoidReturn();
	} else
		((WeaponObjectImplementation*) _impl)->initializeTransientMembers();
}

void WeaponObject::sendBaselinesTo(SceneObject* player) {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 7);
		method.addObjectParameter(player);

		method.executeWithVoidReturn();
	} else
		((WeaponObjectImplementation*) _impl)->sendBaselinesTo(player);
}

int WeaponObject::getAttackType() {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 8);

		return method.executeWithSignedIntReturn();
	} else
		return ((WeaponObjectImplementation*) _impl)->getAttackType();
}

/*
 *	WeaponObjectImplementation
 */

WeaponObjectImplementation::WeaponObjectImplementation(DummyConstructorParameter* param) : TangibleObjectImplementation(param) {
	_initializeImplementation();
}

WeaponObjectImplementation::~WeaponObjectImplementation() {
}


void WeaponObjectImplementation::finalize() {
}

void WeaponObjectImplementation::_initializeImplementation() {
	_setClassHelper(WeaponObjectHelper::instance());

	_serializationHelperMethod();
}

void WeaponObjectImplementation::_setStub(DistributedObjectStub* stub) {
	_this = (WeaponObject*) stub;
	TangibleObjectImplementation::_setStub(stub);
}

DistributedObjectStub* WeaponObjectImplementation::_getStub() {
	return _this;
}

WeaponObjectImplementation::operator const WeaponObject*() {
	return _this;
}

void WeaponObjectImplementation::lock(bool doLock) {
	_this->lock(doLock);
}

void WeaponObjectImplementation::lock(ManagedObject* obj) {
	_this->lock(obj);
}

void WeaponObjectImplementation::rlock(bool doLock) {
	_this->rlock(doLock);
}

void WeaponObjectImplementation::wlock(bool doLock) {
	_this->wlock(doLock);
}

void WeaponObjectImplementation::wlock(ManagedObject* obj) {
	_this->wlock(obj);
}

void WeaponObjectImplementation::unlock(bool doLock) {
	_this->unlock(doLock);
}

void WeaponObjectImplementation::runlock(bool doLock) {
	_this->runlock(doLock);
}

void WeaponObjectImplementation::_serializationHelperMethod() {
	TangibleObjectImplementation::_serializationHelperMethod();

	_setClassName("WeaponObject");

	addSerializableVariable("attackType", &attackType);
	addSerializableVariable("weaponEffect", &weaponEffect);
	addSerializableVariable("weaponEffectIndex", &weaponEffectIndex);
	addSerializableVariable("certificationsRequired", &certificationsRequired);
}

WeaponObjectImplementation::WeaponObjectImplementation(LuaObject* templateData) : TangibleObjectImplementation(templateData) {
	_initializeImplementation();
	// server/zone/objects/tangible/weapon/WeaponObject.idl(63):  attackType = templateData.getIntField("attackType");
	attackType = templateData->getIntField("attackType");
	// server/zone/objects/tangible/weapon/WeaponObject.idl(64):  weaponEffect = templateData.getStringField("weaponEffect");
	weaponEffect = templateData->getStringField("weaponEffect");
	// server/zone/objects/tangible/weapon/WeaponObject.idl(65):  weaponEffectIndex = templateData.getIntField("weaponEffectIndex");
	weaponEffectIndex = templateData->getIntField("weaponEffectIndex");
	// server/zone/objects/tangible/weapon/WeaponObject.idl(67):  Logger.setLoggingName("WeaponObject");
	Logger::setLoggingName("WeaponObject");
}

int WeaponObjectImplementation::getAttackType() {
	// server/zone/objects/tangible/weapon/WeaponObject.idl(75):  return attackType;
	return attackType;
}

/*
 *	WeaponObjectAdapter
 */

WeaponObjectAdapter::WeaponObjectAdapter(WeaponObjectImplementation* obj) : TangibleObjectAdapter(obj) {
}

Packet* WeaponObjectAdapter::invokeMethod(uint32 methid, DistributedMethod* inv) {
	Packet* resp = new MethodReturnMessage(0);

	switch (methid) {
	case 6:
		initializeTransientMembers();
		break;
	case 7:
		sendBaselinesTo((SceneObject*) inv->getObjectParameter());
		break;
	case 8:
		resp->insertSignedInt(getAttackType());
		break;
	default:
		return NULL;
	}

	return resp;
}

void WeaponObjectAdapter::initializeTransientMembers() {
	((WeaponObjectImplementation*) impl)->initializeTransientMembers();
}

void WeaponObjectAdapter::sendBaselinesTo(SceneObject* player) {
	((WeaponObjectImplementation*) impl)->sendBaselinesTo(player);
}

int WeaponObjectAdapter::getAttackType() {
	return ((WeaponObjectImplementation*) impl)->getAttackType();
}

/*
 *	WeaponObjectHelper
 */

WeaponObjectHelper* WeaponObjectHelper::staticInitializer = WeaponObjectHelper::instance();

WeaponObjectHelper::WeaponObjectHelper() {
	className = "WeaponObject";

	DistributedObjectBroker::instance()->registerClass(className, this);
}

void WeaponObjectHelper::finalizeHelper() {
	WeaponObjectHelper::finalize();
}

DistributedObject* WeaponObjectHelper::instantiateObject() {
	return new WeaponObject(DummyConstructorParameter::instance());
}

DistributedObjectServant* WeaponObjectHelper::instantiateServant() {
	return new WeaponObjectImplementation(DummyConstructorParameter::instance());
}

DistributedObjectAdapter* WeaponObjectHelper::createAdapter(DistributedObjectStub* obj) {
	DistributedObjectAdapter* adapter = new WeaponObjectAdapter((WeaponObjectImplementation*) obj->_getImplementation());

	obj->_setClassName(className);
	obj->_setClassHelper(this);

	adapter->setStub(obj);

	return adapter;
}

