/*
 *	server/zone/objects/factorycrate/FactoryCrate.cpp generated by engine3 IDL compiler 0.60
 */

#include "FactoryCrate.h"

#include "server/zone/objects/scene/SceneObject.h"

#include "server/zone/Zone.h"

/*
 *	FactoryCrateStub
 */

FactoryCrate::FactoryCrate() : TangibleObject(DummyConstructorParameter::instance()) {
	_impl = new FactoryCrateImplementation();
	_impl->_setStub(this);
}

FactoryCrate::FactoryCrate(DummyConstructorParameter* param) : TangibleObject(param) {
}

FactoryCrate::~FactoryCrate() {
}


void FactoryCrate::loadTemplateData(SharedObjectTemplate* templateData) {
	if (_impl == NULL) {
		throw ObjectNotLocalException(this);

	} else
		((FactoryCrateImplementation*) _impl)->loadTemplateData(templateData);
}

void FactoryCrate::initializeTransientMembers() {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 6);

		method.executeWithVoidReturn();
	} else
		((FactoryCrateImplementation*) _impl)->initializeTransientMembers();
}

void FactoryCrate::sendBaselinesTo(SceneObject* player) {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 7);
		method.addObjectParameter(player);

		method.executeWithVoidReturn();
	} else
		((FactoryCrateImplementation*) _impl)->sendBaselinesTo(player);
}

bool FactoryCrate::isFactoryCrate() {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 8);

		return method.executeWithBooleanReturn();
	} else
		return ((FactoryCrateImplementation*) _impl)->isFactoryCrate();
}

TangibleObject* FactoryCrate::getPrototype() {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 9);

		return (TangibleObject*) method.executeWithObjectReturn();
	} else
		return ((FactoryCrateImplementation*) _impl)->getPrototype();
}

/*
 *	FactoryCrateImplementation
 */

FactoryCrateImplementation::FactoryCrateImplementation(DummyConstructorParameter* param) : TangibleObjectImplementation(param) {
	_initializeImplementation();
}

FactoryCrateImplementation::~FactoryCrateImplementation() {
}


void FactoryCrateImplementation::finalize() {
}

void FactoryCrateImplementation::_initializeImplementation() {
	_setClassHelper(FactoryCrateHelper::instance());

	_serializationHelperMethod();
}

void FactoryCrateImplementation::_setStub(DistributedObjectStub* stub) {
	_this = (FactoryCrate*) stub;
	TangibleObjectImplementation::_setStub(stub);
}

DistributedObjectStub* FactoryCrateImplementation::_getStub() {
	return _this;
}

FactoryCrateImplementation::operator const FactoryCrate*() {
	return _this;
}

void FactoryCrateImplementation::lock(bool doLock) {
	_this->lock(doLock);
}

void FactoryCrateImplementation::lock(ManagedObject* obj) {
	_this->lock(obj);
}

void FactoryCrateImplementation::rlock(bool doLock) {
	_this->rlock(doLock);
}

void FactoryCrateImplementation::wlock(bool doLock) {
	_this->wlock(doLock);
}

void FactoryCrateImplementation::wlock(ManagedObject* obj) {
	_this->wlock(obj);
}

void FactoryCrateImplementation::unlock(bool doLock) {
	_this->unlock(doLock);
}

void FactoryCrateImplementation::runlock(bool doLock) {
	_this->runlock(doLock);
}

void FactoryCrateImplementation::_serializationHelperMethod() {
	TangibleObjectImplementation::_serializationHelperMethod();

	_setClassName("FactoryCrate");

	addSerializableVariable("prototype", &prototype);
}

FactoryCrateImplementation::FactoryCrateImplementation() {
	_initializeImplementation();
	// server/zone/objects/factorycrate/FactoryCrate.idl(61):  		Logger.setLoggingName("FactoryCrate");
	Logger::setLoggingName("FactoryCrate");
}

bool FactoryCrateImplementation::isFactoryCrate() {
	// server/zone/objects/factorycrate/FactoryCrate.idl(72):  		return true;
	return true;
}

TangibleObject* FactoryCrateImplementation::getPrototype() {
	// server/zone/objects/factorycrate/FactoryCrate.idl(76):  		return prototype;
	return prototype;
}

/*
 *	FactoryCrateAdapter
 */

FactoryCrateAdapter::FactoryCrateAdapter(FactoryCrateImplementation* obj) : TangibleObjectAdapter(obj) {
}

Packet* FactoryCrateAdapter::invokeMethod(uint32 methid, DistributedMethod* inv) {
	Packet* resp = new MethodReturnMessage(0);

	switch (methid) {
	case 6:
		initializeTransientMembers();
		break;
	case 7:
		sendBaselinesTo((SceneObject*) inv->getObjectParameter());
		break;
	case 8:
		resp->insertBoolean(isFactoryCrate());
		break;
	case 9:
		resp->insertLong(getPrototype()->_getObjectID());
		break;
	default:
		return NULL;
	}

	return resp;
}

void FactoryCrateAdapter::initializeTransientMembers() {
	((FactoryCrateImplementation*) impl)->initializeTransientMembers();
}

void FactoryCrateAdapter::sendBaselinesTo(SceneObject* player) {
	((FactoryCrateImplementation*) impl)->sendBaselinesTo(player);
}

bool FactoryCrateAdapter::isFactoryCrate() {
	return ((FactoryCrateImplementation*) impl)->isFactoryCrate();
}

TangibleObject* FactoryCrateAdapter::getPrototype() {
	return ((FactoryCrateImplementation*) impl)->getPrototype();
}

/*
 *	FactoryCrateHelper
 */

FactoryCrateHelper* FactoryCrateHelper::staticInitializer = FactoryCrateHelper::instance();

FactoryCrateHelper::FactoryCrateHelper() {
	className = "FactoryCrate";

	DistributedObjectBroker::instance()->registerClass(className, this);
}

void FactoryCrateHelper::finalizeHelper() {
	FactoryCrateHelper::finalize();
}

DistributedObject* FactoryCrateHelper::instantiateObject() {
	return new FactoryCrate(DummyConstructorParameter::instance());
}

DistributedObjectServant* FactoryCrateHelper::instantiateServant() {
	return new FactoryCrateImplementation(DummyConstructorParameter::instance());
}

DistributedObjectAdapter* FactoryCrateHelper::createAdapter(DistributedObjectStub* obj) {
	DistributedObjectAdapter* adapter = new FactoryCrateAdapter((FactoryCrateImplementation*) obj->_getImplementation());

	obj->_setClassName(className);
	obj->_setClassHelper(this);

	adapter->setStub(obj);

	return adapter;
}

