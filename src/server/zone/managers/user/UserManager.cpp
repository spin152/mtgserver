/*
 *	server/zone/managers/user/UserManager.cpp generated by engine3 IDL compiler 0.55
 */

#include "UserManager.h"

#include "UserManagerImplementation.h"

#include "server/zone/ZoneServer.h"

/*
 *	UserManagerStub
 */

UserManager::UserManager(ZoneServer* server) {
	_impl = new UserManagerImplementation(server);
	_impl->_setStub(this);
}

UserManager::UserManager(DummyConstructorParameter* param) {
	_impl = NULL;
}

UserManager::~UserManager() {
}

bool UserManager::checkUser(unsigned int ipid) {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 6);
		method.addUnsignedIntParameter(ipid);

		return method.executeWithBooleanReturn();
	} else
		return ((UserManagerImplementation*) _impl)->checkUser(ipid);
}

bool UserManager::isAdmin(const string& name) {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 7);
		method.addAsciiParameter(name);

		return method.executeWithBooleanReturn();
	} else
		return ((UserManagerImplementation*) _impl)->isAdmin(name);
}

void UserManager::parseBanList() {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 8);

		method.executeWithVoidReturn();
	} else
		((UserManagerImplementation*) _impl)->parseBanList();
}

void UserManager::banUser(const string& ipaddr) {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 9);
		method.addAsciiParameter(ipaddr);

		method.executeWithVoidReturn();
	} else
		((UserManagerImplementation*) _impl)->banUser(ipaddr);
}

bool UserManager::banUserByName(string& name, string& admin) {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 10);
		method.addAsciiParameter(name);
		method.addAsciiParameter(admin);

		return method.executeWithBooleanReturn();
	} else
		return ((UserManagerImplementation*) _impl)->banUserByName(name, admin);
}

bool UserManager::kickUser(string& name, string& admin) {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 11);
		method.addAsciiParameter(name);
		method.addAsciiParameter(admin);

		return method.executeWithBooleanReturn();
	} else
		return ((UserManagerImplementation*) _impl)->kickUser(name, admin);
}

void UserManager::changeUserCap(int amount) {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 12);
		method.addSignedIntParameter(amount);

		method.executeWithVoidReturn();
	} else
		((UserManagerImplementation*) _impl)->changeUserCap(amount);
}

bool UserManager::isBannedUser(unsigned int ipid) {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 13);
		method.addUnsignedIntParameter(ipid);

		return method.executeWithBooleanReturn();
	} else
		return ((UserManagerImplementation*) _impl)->isBannedUser(ipid);
}

int UserManager::getUserCap() {
	if (_impl == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 14);

		return method.executeWithSignedIntReturn();
	} else
		return ((UserManagerImplementation*) _impl)->getUserCap();
}

/*
 *	UserManagerAdapter
 */

UserManagerAdapter::UserManagerAdapter(UserManagerImplementation* obj) : DistributedObjectAdapter((DistributedObjectServant*) obj) {
}

Packet* UserManagerAdapter::invokeMethod(uint32 methid, DistributedMethod* inv) {
	Packet* resp = new MethodReturnMessage(0);

	switch (methid) {
	case 6:
		resp->insertBoolean(checkUser(inv->getUnsignedIntParameter()));
		break;
	case 7:
		resp->insertBoolean(isAdmin(inv->getAsciiParameter(_param0_isAdmin__string_)));
		break;
	case 8:
		parseBanList();
		break;
	case 9:
		banUser(inv->getAsciiParameter(_param0_banUser__string_));
		break;
	case 10:
		resp->insertBoolean(banUserByName(inv->getAsciiParameter(_param0_banUserByName__string_string_), inv->getAsciiParameter(_param1_banUserByName__string_string_)));
		break;
	case 11:
		resp->insertBoolean(kickUser(inv->getAsciiParameter(_param0_kickUser__string_string_), inv->getAsciiParameter(_param1_kickUser__string_string_)));
		break;
	case 12:
		changeUserCap(inv->getSignedIntParameter());
		break;
	case 13:
		resp->insertBoolean(isBannedUser(inv->getUnsignedIntParameter()));
		break;
	case 14:
		resp->insertSignedInt(getUserCap());
		break;
	default:
		return NULL;
	}

	return resp;
}

bool UserManagerAdapter::checkUser(unsigned int ipid) {
	return ((UserManagerImplementation*) impl)->checkUser(ipid);
}

bool UserManagerAdapter::isAdmin(const string& name) {
	return ((UserManagerImplementation*) impl)->isAdmin(name);
}

void UserManagerAdapter::parseBanList() {
	return ((UserManagerImplementation*) impl)->parseBanList();
}

void UserManagerAdapter::banUser(const string& ipaddr) {
	return ((UserManagerImplementation*) impl)->banUser(ipaddr);
}

bool UserManagerAdapter::banUserByName(string& name, string& admin) {
	return ((UserManagerImplementation*) impl)->banUserByName(name, admin);
}

bool UserManagerAdapter::kickUser(string& name, string& admin) {
	return ((UserManagerImplementation*) impl)->kickUser(name, admin);
}

void UserManagerAdapter::changeUserCap(int amount) {
	return ((UserManagerImplementation*) impl)->changeUserCap(amount);
}

bool UserManagerAdapter::isBannedUser(unsigned int ipid) {
	return ((UserManagerImplementation*) impl)->isBannedUser(ipid);
}

int UserManagerAdapter::getUserCap() {
	return ((UserManagerImplementation*) impl)->getUserCap();
}

/*
 *	UserManagerHelper
 */

UserManagerHelper* UserManagerHelper::staticInitializer = UserManagerHelper::instance();

UserManagerHelper::UserManagerHelper() {
	className = "UserManager";

	DistributedObjectBroker::instance()->registerClass(className, this);
}

void UserManagerHelper::finalizeHelper() {
	UserManagerHelper::finalize();
}

DistributedObject* UserManagerHelper::instantiateObject() {
	return new UserManager(DummyConstructorParameter::instance());
}

DistributedObjectAdapter* UserManagerHelper::createAdapter(DistributedObjectStub* obj) {
	DistributedObjectAdapter* adapter = new UserManagerAdapter((UserManagerImplementation*) obj->_getImplementation());

	obj->_setClassName(className);
	obj->_setClassHelper(this);

	adapter->setStub(obj);

	return adapter;
}

/*
 *	UserManagerServant
 */

UserManagerServant::UserManagerServant() {
	_classHelper = UserManagerHelper::instance();
}

UserManagerServant::~UserManagerServant() {
}

void UserManagerServant::_setStub(DistributedObjectStub* stub) {
	_this = (UserManager*) stub;
}

DistributedObjectStub* UserManagerServant::_getStub() {
	return _this;
}



