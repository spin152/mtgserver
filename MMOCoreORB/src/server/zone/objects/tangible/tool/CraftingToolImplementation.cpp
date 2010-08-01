/*
 Copyright (C) 2010 <SWGEmu>

 This File is part of Core3.

 This program is free software; you can redistribute
 it and/or modify it under the terms of the GNU Lesser
 General Public License as published by the Free Software
 Foundation; either version 3 of the License,
 or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for
 more details.

 You should have received a copy of the GNU Lesser General
 Public License along with this program; if not, write to
 the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

 Linking Engine3 statically or dynamically with other modules
 is making a combined work based on Engine3.
 Thus, the terms and conditions of the GNU Lesser General Public License
 cover the whole combination.

 In addition, as a special exception, the copyright holders of Engine3
 give you permission to combine Engine3 program with free software
 programs or libraries that are released under the GNU LGPL and with
 code included in the standard release of Core3 under the GNU LGPL
 license (or modified versions of such code, with unchanged license).
 You may copy and distribute such a system following the terms of the
 GNU LGPL for Engine3 and the licenses of the other code concerned,
 provided that you include the source code of that other code when
 and as the GNU LGPL requires distribution of source code.

 Note that people who make modified versions of Engine3 are not obligated
 to grant this special exception for their modified versions;
 it is their choice whether to do so. The GNU Lesser General Public License
 gives permission to release a modified version without this exception;
 this exception also makes it possible to release a modified version
 which carries forward this exception.
 */

#include "engine/engine.h"

#include "CraftingTool.h"
#include "server/zone/Zone.h"
#include "server/zone/objects/player/PlayerCreature.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/templates/tangible/tool/CraftingToolTemplate.h"
#include "server/zone/objects/manufactureschematic/ManufactureSchematic.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/IngredientSlot.h"
#include "server/zone/managers/crafting/CraftingManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/tangible/tool/events/CreateObjectTask.h"
#include "server/zone/objects/tangible/tool/events/UpdateToolCountdownTask.h"

#include "server/zone/packets/scene/AttributeListMessage.h"
#include "server/zone/packets/player/PlayerObjectDeltaMessage9.h"
#include "server/zone/packets/tangible/TangibleObjectDeltaMessage3.h"
#include "server/zone/packets/manufactureschematic/ManufactureSchematicObjectDeltaMessage3.h"
#include "server/zone/packets/manufactureschematic/ManufactureSchematicObjectDeltaMessage6.h"
#include "server/zone/packets/manufactureschematic/ManufactureSchematicObjectDeltaMessage7.h"
#include "server/zone/packets/manufactureschematic/ManufactureSchematicObjectMessage7.h"

void CraftingToolImplementation::loadTemplateData(
		SharedObjectTemplate* templateData) {
	TangibleObjectImplementation::loadTemplateData(templateData);

	CraftingToolTemplate* craftingToolData =
			dynamic_cast<CraftingToolTemplate*> (templateData);

	if (craftingToolData == NULL) {
		throw Exception("invalid template for CraftingTool");
	}

	type = craftingToolData->getToolType();

	complexityLevel = craftingToolData->getComplexityLevel();

	for (int i = 0; i < craftingToolData->getTabs().size(); ++i)
		enabledTabs.add(craftingToolData->getTabs().get(i));
}

void CraftingToolImplementation::fillObjectMenuResponse(
		ObjectMenuResponse* menuResponse, PlayerCreature* player) {
	TangibleObjectImplementation::fillObjectMenuResponse(menuResponse, player);

}

int CraftingToolImplementation::handleObjectMenuSelect(
		PlayerCreature* playerCreature, byte selectedID) {
	PlayerObject* playerObject = playerCreature->getPlayerObject();

	if (selectedID == 20) { // use object

	}

	return 1;
}

void CraftingToolImplementation::fillAttributeList(AttributeListMessage* alm,
		PlayerCreature* object) {
	TangibleObjectImplementation::fillAttributeList(alm, object);

	alm->insertAttribute("craft_tool_effectiveness", Math::getPrecision(
			effectiveness, 2));

	alm->insertAttribute("craft_tool_status", status);

	if (craftersName != "") {

		alm->insertAttribute("crafter", craftersName);
	}
	if (craftersSerial != "") {

		alm->insertAttribute("serial_number", craftersSerial);
	}
}

void CraftingToolImplementation::updateCraftingValues(ManufactureSchematic* schematic) {
	/// useModifer is the effectiveness


	CraftingValues* craftingValues = schematic->getCraftingValues();

	effectiveness = craftingValues->getCurrentValue("usemodifier");
	//craftingValues->toString();
}

Vector<uint32>* CraftingToolImplementation::getToolTabs() {
	return &enabledTabs;
}

void CraftingToolImplementation::requestCraftingSession(PlayerCreature* player,
		CraftingStation* station) {

	/// pre: _this locked

	if(status == "@crafting:tool_status_finished") {
		player->sendSystemMessage("system_msg", "crafting_tool_full");
		sendToolStartFailure(player);
		return;
	}

	if(status == "@crafting:tool_status_working") {
		player->sendSystemMessage("system_msg", "crafting_tool_creating_prototype");
		sendToolStartFailure(player);
		return;
	}

	Locker _locker(player);

	craftingStation = station;

	if(craftingManager == NULL)
		craftingManager = player->getZoneServer()->getCraftingManager();

	//Locate closest crafting station, if exists.
	if (craftingStation == NULL)
		locateCraftingStation(player, getToolType());

	sendStart(player);
}

void CraftingToolImplementation::sendStart(PlayerCreature* player) {

	/// pre: player and _this locked

	insertCounter = 1;
	experimentationPointsTotal = 0;
	assemblyResult = 8;

	PlayerObject* playerObject = player->getPlayerObject();
	int complexity = complexityLevel;  ///

	if (craftingStation != NULL)
		complexity = craftingStation->getComplexityLevel();

	currentSchematicList.removeAll();
	currentSchematicList = playerObject->filterSchematicList(getToolTabs(), complexity);

	/// Packet Sending Start
	/// DPlay9
	PlayerObjectDeltaMessage9* dplay9 = new PlayerObjectDeltaMessage9(
			playerObject);
	dplay9->setExperimentationEnabled(craftingStation != NULL);
	dplay9->setCraftingState(1);
	state = 1;

	if (craftingStation != NULL)
		dplay9->setClosestCraftingStation(craftingStation->getObjectID());
	else
		dplay9->setClosestCraftingStation(0);

	dplay9->setExperimentationPoints(experimentationPointsTotal);
	dplay9->close();
	player->sendMessage(dplay9);
	// End dplay9***********************************

	/// Object Controller Message 102 - Schematic List
	ObjectControllerMessage* ocm = new ObjectControllerMessage(
			player->getObjectID(), 0x0B, 0x102);
	ocm->insertLong(getObjectID());

	if (craftingStation != NULL)
		ocm->insertLong(craftingStation->getObjectID());
	else
		ocm->insertLong(0);

	ocm->insertInt(currentSchematicList.size());
	for (int i = 0; i < currentSchematicList.size(); ++i) {

		DraftSchematic* draftSchematic = currentSchematicList.get(i).get();

		ocm->insertInt(draftSchematic->getSchematicID());
		ocm->insertInt(draftSchematic->getClientObjectCRC());
		ocm->insertInt(draftSchematic->getToolTab()); // this number decides what tab the schematic goes in (ex: 4 = food tab in crafting window)
	}
	player->sendMessage(ocm);

	/// Send all the ingredients to the player
	for (int i = 0; i < currentSchematicList.size(); ++i) {

		DraftSchematic* draftSchematic = currentSchematicList.get(i).get();

		/// Object Controller Message 1BF
		draftSchematic->sendDraftSlotsTo(player);
		// End OBJC 1BF***********************************
	}

	/// Send all the experimental properties to the player
	for (int i = 0; i < currentSchematicList.size(); ++i) {

		DraftSchematic* draftSchematic = currentSchematicList.get(i).get();

		/// Object Controller Message 207
		draftSchematic->sendResourceWeightsTo(player);
		// End OBJC 207***********************************
	}

	player->setLastCraftingToolUsed(_this);
}

void CraftingToolImplementation::sendToolStartFailure(PlayerCreature* player) {

	// Start Object Controller **(Failed to start crafting Session************
	ObjectControllerMessage* objMsg = new ObjectControllerMessage(
			player->getObjectID(), 0x0B, 0x010C);
	objMsg->insertInt(0x10F);
	objMsg->insertInt(0);
	objMsg->insertByte(0);

	player->sendMessage(objMsg);
}

void CraftingToolImplementation::cancelCraftingSession(PlayerCreature* player) {

	/// pre: _this locked

	if (manufactureSchematic != NULL) {

		if(manufactureSchematic->getParent() == _this) {
			removeObject(manufactureSchematic);
			manufactureSchematic->setDraftSchematic(NULL);
		}

		manufactureSchematic = NULL;
	}

	if (prototype != NULL) {
		if(status == "@crafting:tool_status_ready") {

			if(prototype->getParent() == _this)
				removeObject(prototype);

			prototype = NULL;
		}
	}

	if (player != NULL) {
		// DPlay9
		PlayerObjectDeltaMessage9* dplay9 = new PlayerObjectDeltaMessage9(
				player->getPlayerObject());
		dplay9->setCraftingState(0);
		state = 0;
		dplay9->close();
		player->sendMessage(dplay9);

		player->setLastCraftingToolUsed(NULL);
	}
}

void CraftingToolImplementation::closeCraftingWindow(PlayerCreature* player, int clientCounter) {

	ObjectControllerMessage* objMsg = new ObjectControllerMessage(player->getObjectID(), 0x0B, 0x010C);
	objMsg->insertInt(0x10A);
	objMsg->insertInt(1);
	objMsg->insertByte(clientCounter);

	player->sendMessage(objMsg);

	objMsg = new ObjectControllerMessage(player->getObjectID(), 0x0B, 0x010C);
	objMsg->insertInt(0x10A);
	objMsg->insertInt(0);
	objMsg->insertByte(clientCounter);

	player->sendMessage(objMsg);

	objMsg = new ObjectControllerMessage(player->getObjectID(), 0x0B, 0x01C2);
	objMsg->insertByte(clientCounter);

	player->sendMessage(objMsg);
}

void CraftingToolImplementation::locateCraftingStation(PlayerCreature* player,
		int toolType) {

	/// pre: player and _this locked

	ZoneServer* server = player->getZone()->getZoneServer();
	Zone* zone = player->getZone();

	Locker zoneLocker(zone);
	ManagedReference<CraftingStation*> station = NULL;

	for (int i = 0; i < player->inRangeObjectCount(); ++i) {
		SceneObjectImplementation* scno =
				(SceneObjectImplementation*) player->getInRangeObject(i);

		if (scno->isCraftingStation() && player->isInRange(scno, 7.0f)) {

			station = (CraftingStation*) server->getObject(scno->getObjectID());

			if (station == NULL)
				continue;

			if (toolType == station->getStationType() || (toolType
					== CraftingTool::JEDI && station->getStationType()
					== CraftingTool::WEAPON)) {
				craftingStation = station;
				return;
			}
		}
	}
}

void CraftingToolImplementation::selectDraftSchematic(PlayerCreature* player,
		int index) {

	/// pre: _this locked


	DraftSchematic* draftschematic = currentSchematicList.get(index);
	//DraftSchematic* draftschematic = player->getPlayerObject()->getSchematic(
	//		index);

	if (draftschematic == NULL) {
		player->sendSystemMessage("ui_craft", "err_no_draft_schematic");
		return;
	}

	try {

		manufactureSchematic
				= (ManufactureSchematic*) draftschematic->createManufactureSchematic();
		manufactureSchematic->createChildObjects();


		prototype = dynamic_cast<TangibleObject*> (player->getZoneServer()->createObject(
				draftschematic->getTanoCRC(), 0));

		if (prototype == NULL )
			return;


		prototype->createChildObjects();


		if (manufactureSchematic == NULL) {
			player->sendSystemMessage("ui_craft", "err_no_manf_schematic");
			return;
		}

		if (prototype == NULL) {
			player->sendSystemMessage("ui_craft", "err_no_prototype");
			return;
		}

		addObject(manufactureSchematic, 0x4, false);
		manufactureSchematic->sendTo(player, true);

		addObject(prototype, 0xFFFFFFFF, false);
		prototype->sendTo(player, true);

		// Dplay9 ********************************************************
		// Sets the Crafting state to 2, which is the Resource screen
		PlayerObjectDeltaMessage9* dplay9 = new PlayerObjectDeltaMessage9(
				player->getPlayerObject());
		dplay9->setExperimentationPoints(0xFFFFFFFF);
		dplay9->setCraftingState(2);
		state = 2;
		dplay9->close();

		player->sendMessage(dplay9);
		// End Dplay9 *****************************************************

		// Dtano3 ********************************************************
		// Update Condition Damage
		TangibleObjectDeltaMessage3* dtano3 = new TangibleObjectDeltaMessage3(prototype);
		dtano3->updateConditionDamage();
		dtano3->close();
		player->sendMessage(dtano3);
		// End Dtano3 *****************************************************

		// Start the insert count so inserts and removals work
		insertCounter = 1;

	} catch (...) {
		player->sendSystemMessage("ui_craft", "err_no_prototype");
	}
}

void CraftingToolImplementation::synchronizedUIListenForSchematic(PlayerCreature* player) {

	if(manufactureSchematic == NULL) {
		player->sendSystemMessage("ui_craft", "err_no_manf_schematic");
		return;
	}

	if (prototype == NULL) {
		player->sendSystemMessage("ui_craft", "err_no_prototype");
		return;
	}

	ManufactureSchematicObjectMessage7* mcso7 =
			new ManufactureSchematicObjectMessage7(manufactureSchematic);
	player->sendMessage(mcso7);

	// Object Controller w/ Ingredients ***************************
	ObjectControllerMessage* objMsg = new ObjectControllerMessage(
			player->getObjectID(), 0x0B, 0x0103);
	objMsg->insertLong(getObjectID()); // Crafting Tool Object ID
	objMsg->insertLong(manufactureSchematic->getObjectID());// Manufacture Schematic Object ID
	objMsg->insertLong(prototype->getObjectID()); // Crafting Tangible Object ID
	objMsg->insertInt(2);
	objMsg->insertByte(1);

	ManagedReference<DraftSchematic*> draftSchematic =
			manufactureSchematic->getDraftSchematic();

	int draftSlotCount = draftSchematic->getDraftSlotCount();
	objMsg->insertInt(draftSlotCount);

	// Send all the ingredient data
	for (int i = 0; i < draftSlotCount; i++) {
		DraftSlot* slot = draftSchematic->getDraftSlot(i);

		if (slot != NULL)
			slot->insertToMessage(objMsg);
	}
	objMsg->insertShort(0);

	player->sendMessage(objMsg);
}

void CraftingToolImplementation::addIngredient(PlayerCreature* player, TangibleObject* tano, int slot, int clientCounter) {
	/// Tano can't be NULL

	Locker locker(_this);

	if(manufactureSchematic == NULL) {
		sendSlotMessage(player, clientCounter, IngredientSlot::NOSCHEMATIC);
		return;
	}

	if (prototype == NULL) {
		sendSlotMessage(player, clientCounter, IngredientSlot::PROTOTYPENOTFOUND);
		return;
	}

	Reference<IngredientSlot* > ingredientSlot = manufactureSchematic->getIngredientSlot(slot);

	if(ingredientSlot->add(player, tano)) {

		sendIngredientAddSuccess(player, slot, clientCounter);

	} else {

		sendSlotMessage(player, clientCounter, IngredientSlot::INVALIDINGREDIENT);

	}

	// Increment the insert counter
	insertCounter++;
}

void CraftingToolImplementation::sendIngredientAddSuccess(PlayerCreature* player, int slot, int clientCounter) {

	/// pre: _this locked

	// DMSCO6 ***************************************************
	// Prepares the slot for insert
	ManufactureSchematicObjectDeltaMessage6* dMsco6 =
			new ManufactureSchematicObjectDeltaMessage6(
					manufactureSchematic->getObjectID());

	dMsco6->insertToResourceSlot(insertCounter);
	dMsco6->close();

	player->sendMessage(dMsco6);
	// End DMSCO6 ********************************************F*******

	// DMSCO7 ***************************************************
	// Updates the slot
	ManufactureSchematicObjectDeltaMessage7 * dMsco7 =
			new ManufactureSchematicObjectDeltaMessage7(
					manufactureSchematic->getObjectID());

	if (insertCounter == 1)
		// If it's the first resource inserted, we need to fully update all the slots
		dMsco7->fullUpdate(manufactureSchematic, slot);
	else
		// If it's not the first resources, slots are updates, and only insert needs done
		dMsco7->partialUpdate(manufactureSchematic, slot, insertCounter);


	manufactureSchematic->increaseComplexity();
	dMsco7->close();

	player->sendMessage(dMsco7);
	// End DMSCO7 ***************************************************

	// Start DMSCO3 ***********************************************************
	// Updates the Complexity
	ManufactureSchematicObjectDeltaMessage3* dMsco3 =
			new ManufactureSchematicObjectDeltaMessage3(
					manufactureSchematic->getObjectID());
	dMsco3->updateComplexity(manufactureSchematic->getComplexity());
	dMsco3->close();

	player->sendMessage(dMsco3);
	// End DMSCO3 *************************************************************

	sendSlotMessage(player, clientCounter, IngredientSlot::OK);
}

void CraftingToolImplementation::removeIngredient(PlayerCreature* player, TangibleObject* tano, int slot, int clientCounter) {
	/// Tano can't be NULL

	Locker _locker(_this);

	if(manufactureSchematic == NULL) {
		sendSlotMessage(player, clientCounter, IngredientSlot::NOSCHEMATIC);
		return;
	}

	if (prototype == NULL) {
		sendSlotMessage(player, clientCounter, IngredientSlot::PROTOTYPENOTFOUND);
		return;
	}

	Reference<IngredientSlot* > ingredientSlot = manufactureSchematic->getIngredientSlot(slot);

	if(ingredientSlot->remove(player)) {

		sendIngredientRemoveSuccess(player, slot, clientCounter);

	} else {

		sendSlotMessage(player, clientCounter, IngredientSlot::NOCOMPONENTTRANSFER);

	}

	// Increment the insert counter
	insertCounter++;

}

void CraftingToolImplementation::sendIngredientRemoveSuccess(PlayerCreature* player, int slot, int clientCounter) {

	/// pre: _this locked

	// DMCSO7 ******************************************************
	// Removes resource from client slot
	ManufactureSchematicObjectDeltaMessage7* dMsco7 =
			new ManufactureSchematicObjectDeltaMessage7(
					manufactureSchematic->getObjectID());

	dMsco7->removeResource(slot, manufactureSchematic->getSlotCount()
			+ insertCounter);

	dMsco7->close();

	player->sendMessage(dMsco7);
	// End DMCSO7 ***************************************************


	// Object Controller ********************************************
	// Updates the screen with the resource removal
	ObjectControllerMessage* objMsg = new ObjectControllerMessage(
			player->getObjectID(), 0x0B, 0x010C);
	objMsg->insertInt(0x108);
	objMsg->insertInt(0);
	objMsg->insertByte(clientCounter);

	player->sendMessage(objMsg);
	// End Object Controller *****************************************

	if (insertCounter > 0)
		manufactureSchematic->decreaseComplexity();

	// Start DMSCO3 ***********************************************************
	// Updates the Complexity
	ManufactureSchematicObjectDeltaMessage3* dMsco3 =
			new ManufactureSchematicObjectDeltaMessage3(
					manufactureSchematic->getObjectID());
	dMsco3->updateComplexity(manufactureSchematic->getComplexity());
	dMsco3->close();

	player->sendMessage(dMsco3);
	// End DMSCO3 *************************************************************
}

void CraftingToolImplementation::sendSlotMessage(PlayerCreature* player,
		int counter, int message) {

	// Object Controller ********************************************
	// Send Bad Slot message
	ObjectControllerMessage* objMsg =
			new ObjectControllerMessage(player->getObjectID(), 0x0B, 0x010C);
	objMsg->insertInt(0x107);
	objMsg->insertInt(message);
	objMsg->insertByte(counter);

	player->sendMessage(objMsg);
	//End Object Controller ******************************************
}

void CraftingToolImplementation::nextCraftingStage(PlayerCreature* player, int clientCounter) {

	if(manufactureSchematic == NULL) {
		sendSlotMessage(player, 0, IngredientSlot::NOSCHEMATIC);
		return;
	}

	if (prototype == NULL) {
		sendSlotMessage(player, 0, IngredientSlot::PROTOTYPENOTFOUND);
		return;
	}

	// Make sure all the require resources are there, if not, return them to inventory and close tool
	if(!manufactureSchematic->isReadyForAssembly()){

		closeCraftingWindow(player, clientCounter);

		cancelCraftingSession(player);

		return;
	}

	Locker _locker(_this);

	manufactureSchematic->setAssembled();

	if (state == 2) {

		// Calculate exp failure for red bars
		experimentalFailureRate = craftingManager->calculateExperimentationFailureRate(player, manufactureSchematic, 0);

		// Flag to get the experimenting window
		if (craftingStation != NULL) {

			// Assemble with Experimenting
			state = 3;
			initialAssembly(player, clientCounter);

		} else {

			// Assemble without Experimenting
			state = 4;
			initialAssembly(player, clientCounter);

		}

	} else if (state == 3) {

		finishAssembly(player, clientCounter);

	} else if (state == 5) {

		finishStage1(player, clientCounter);

	} else if (state == 6) {

		finishStage2(player, clientCounter);
	}
}

void CraftingToolImplementation::initialAssembly(PlayerCreature* player, int clientCounter) {

	/// pre: _this locked

	// Get the appropriate number of Experimentation points from Skill
	ManagedReference<DraftSchematic* > draftSchematic = manufactureSchematic->getDraftSchematic();

	String expskill = draftSchematic->getExperimentationSkill();
	experimentationPointsTotal = int(player->getSkillMod(expskill) / 10);
	experimentationPointsUsed = 0;

	// Get the level of customization
	String custskill = draftSchematic->getCustomizationSkill();
	int custpoints = int(player->getSkillMod(custskill));

	// Start DPLAY9 ***********************************************************
	// Updates the stage of crafting, sets the number of experimentation points
	PlayerObjectDeltaMessage9* dplay9 =
		new PlayerObjectDeltaMessage9(player->getPlayerObject());
	dplay9->setCraftingState(state); // 3 If Experimenting is active, 4 if already experimented/ No experimenting
	dplay9->setExperimentationPoints(experimentationPointsTotal);
	dplay9->close();

	player->sendMessage(dplay9);
	// End DPLAY9 *************************************************************

	// Determine the outcome of the craft, Amazing through Critical
	assemblyResult = craftingManager->calculateAssemblySuccess(player, draftSchematic, effectiveness);

	//Set initial crafting percentages
	setInitialCraftingValues();

	// Set Crafter name and generate serial number
	String name = player->getFirstName();
	prototype->setCraftersName(name);

	String serial = craftingManager->generateSerial();
	prototype->setCraftersSerial(serial);

	// Update the prototype with new values
	prototype->updateCraftingValues(manufactureSchematic);

	// Set crafter
	manufactureSchematic->setCrafter(player);

	// Set default customization
	Vector<byte>* customizationOptions = draftSchematic->getCustomizationOptions();
	Vector<byte>* customizationDefaultValues = draftSchematic->getCustomizationDefaultValues();

	for (int i = 0; i < customizationOptions->size(); ++i) {
		prototype->setCustomizationVariable(customizationOptions->get(i), customizationDefaultValues->get(i));
	}

	// Start DMSCO3 ***********************************************************
	// Sends the updated values to the crafting screen
	ManufactureSchematicObjectDeltaMessage3* dMsco3 =
		new ManufactureSchematicObjectDeltaMessage3(manufactureSchematic->getObjectID());
	dMsco3->updateCraftingValues(manufactureSchematic);
	dMsco3->close();

	player->sendMessage(dMsco3);
	// End DMSCO3 *************************************************************

	// Start DMSCO7 ***********************************************************
	// Sends the experimental properties and experimental percentages
	ManufactureSchematicObjectDeltaMessage7* dMsco7 =
		new ManufactureSchematicObjectDeltaMessage7(manufactureSchematic->getObjectID());
	dMsco7->updateForAssembly(manufactureSchematic, experimentalFailureRate);
	if (custpoints > 0) {
		dMsco7->updateCustomizationOptions(manufactureSchematic, custpoints);
	}
	dMsco7->close();

	player->sendMessage(dMsco7);
	// End DMSCO7 *************************************************************

	// Start DTANO3 ***********************************************************
	// Updates the Complexity and the condition
	TangibleObjectDeltaMessage3* dtano3 = new TangibleObjectDeltaMessage3(prototype);
	dtano3->updateComplexity(manufactureSchematic->getComplexity());
	dtano3->updateMaxCondition();
	dtano3->close();

	player->sendMessage(dtano3);
	// End DTANO3 *************************************************************


	// Start Object Controller *******************************************
	// Send the results
	ObjectControllerMessage* objMsg =
		new ObjectControllerMessage(player->getObjectID(), 0x0B, 0x01BE);
	objMsg->insertInt(0x109);

	objMsg->insertInt(assemblyResult); // Assembly Results

	objMsg->insertByte(clientCounter);

	player->sendMessage(objMsg);
	// End Object Controller ******************************************

	// If the window is closed now, this sets the resources to no be recoverable
	manufactureSchematic->setAssembled();


	// Remove all resources - Not recovering them
	if (assemblyResult == CraftingManager::CRITICALFAILURE) {

		//manufactureSchematic->resetCraftingValues();

		state = 2;

		// re-setup the slots and ingredients
		manufactureSchematic->synchronizedUIListen(player, 0);
		manufactureSchematic->initializeIngredientSlots(manufactureSchematic->getDraftSchematic());

		// Start Dplay9 **************************************
		// Reset crafting state
		PlayerObjectDeltaMessage9* dplay9 =
			new PlayerObjectDeltaMessage9(player->getPlayerObject());
		dplay9->setCraftingState(state);

		dplay9->close();

		player->sendMessage(dplay9);
		// End DPLAY9 ****************************************

		// Reset inserts
		insertCounter = 1;
	}
}

void CraftingToolImplementation::setInitialCraftingValues() {

	ManagedReference<DraftSchematic* > draftSchematic = manufactureSchematic->getDraftSchematic();
	CraftingValues* craftingValues = manufactureSchematic->getCraftingValues();

	float value, maxPercentage, currentPercentage, weightedSum;
	String itemName;

	// These 2 values are pretty standard, adding these
	itemName = "xp";
	value = float(draftSchematic->getXpAmount());
	craftingValues->addExperimentalProperty("", itemName, value, value, 0, 1);

	itemName = "complexity";
	value = manufactureSchematic->getComplexity();
	craftingValues->addExperimentalProperty("", itemName, value, value, 0, 1);

	int subtitleCounter = 0;

	float modifier = craftingManager->calculateAssemblyValueModifier(assemblyResult);

	for (int i = 0; i < draftSchematic->getResourceWeightCount(); ++i) {

		// Grab the first weight group
		Reference<ResourceWeight* > resourceWeight = draftSchematic->getResourceWeight(i);

		// Getting the title ex: expDamage
		String experimentalTitle = resourceWeight->getExperimentalTitle();

		// Getting the subtitle ex: minDamage
		String property = resourceWeight->getPropertyName();

		weightedSum = 0;

		craftingValues->addExperimentalProperty(experimentalTitle, property,
				resourceWeight->getMinValue(), resourceWeight->getMaxValue(),
				resourceWeight->getPrecision(), resourceWeight->isFiller());

		for (int ii = 0; ii < resourceWeight->getPropertyListSize(); ++ii) {

			// Based on the script we cycle through each exp group

			// Get the type from the type/weight
			int type = (resourceWeight->getTypeAndWeight(ii) >> 4);

			// Get the calculation percentage
			float percentage = resourceWeight->getPropertyPercentage(ii);

			// add to the weighted sum based on type and percentage
			weightedSum += craftingManager->getWeightedValue(manufactureSchematic, type) * percentage;
		}

		// > 0 ensures that we don't add things when there is NaN value
		if (weightedSum > 0) {

			// This is the formula for max experimenting percentages
			maxPercentage = ((weightedSum / 10.0f) * .01f);

			// Based on the weighted sum, we can get the initial %
			currentPercentage = (craftingManager->getAssemblyPercentage(weightedSum)) * modifier;

			craftingValues->setMaxPercentage(property, maxPercentage);
			craftingValues->setCurrentPercentage(property, currentPercentage);

			subtitleCounter++;

		}
	}

	craftingValues->recalculateValues(true);

	//if (applyComponentBoost(draftSchematic, craftingTool))
	//	craftingValues->recalculateValues(draftSchematic, true);

	//craftingValues->toString();
}

void CraftingToolImplementation::finishAssembly(PlayerCreature* player, int clientCounter) {

	// Start Dplay9 **************************************
	// Move crafting to State 4
	PlayerObjectDeltaMessage9* dplay9 =
			new PlayerObjectDeltaMessage9(player->getPlayerObject());
	dplay9->setCraftingState(4);
	state = 4;
	dplay9->close();

	player->sendMessage(dplay9);
	// End DPLAY9

	// Start Object Controller **************************************
	ObjectControllerMessage* objMsg =
			new ObjectControllerMessage(player->getObjectID(), 0x0B, 0x01BE);
	objMsg->insertInt(0x109);
	objMsg->insertInt(4);

	objMsg->insertByte(clientCounter);

	player->sendMessage(objMsg);
	// End Object Controller **************************************

}

void CraftingToolImplementation::experiment(PlayerCreature* player, int numRowsAttempted, String& expString, int clientCounter) {

	if(manufactureSchematic == NULL) {
		sendSlotMessage(player, 0, IngredientSlot::NOSCHEMATIC);
		return;
	}

	if (prototype == NULL) {
		sendSlotMessage(player, 0, IngredientSlot::PROTOTYPENOTFOUND);
		return;
	}

	StringTokenizer tokenizer(expString);

	int rowEffected, pointsAttempted, failure;
	int lowestExpSuccess = 0;
	int totalPoints = 0;

	CraftingValues* craftingValues = manufactureSchematic->getCraftingValues();

	craftingValues->clear();

	// Loop through all the lines of experimentation
	for (int i = 0; i < numRowsAttempted; ++i) {

		rowEffected = tokenizer.getIntToken();
		pointsAttempted = tokenizer.getIntToken();

		totalPoints += pointsAttempted;

		// Each line gets it's own rolls
		// Calcualte a new failure rate for each line of experimentation
		failure = craftingManager->calculateExperimentationFailureRate(player, manufactureSchematic,
				pointsAttempted);

			// Set the experimentation result ie:  Amazing Success
		experimentationResult = craftingManager->calculateExperimentationSuccess(player,
				manufactureSchematic->getDraftSchematic(), failure);

		// Make sure to store the lowest roll to display (Effect the multiline rolls
		if (lowestExpSuccess < experimentationResult)
			lowestExpSuccess = experimentationResult;

		manufactureSchematic->increaseComplexity();

		// Do the experimenting - sets new percentages
		experimentRow(craftingValues, rowEffected, pointsAttempted, failure);

		}

	manufactureSchematic->setExperimentingCounter(manufactureSchematic->getExperimentingCounter() + numRowsAttempted);

	// Use percentages to recalculate the values
	craftingValues->recalculateValues(false);

	// Update the Tano with new values
	prototype->updateCraftingValues(manufactureSchematic);

	// Sets the result for display
	experimentationResult = lowestExpSuccess;

	/*if(experimentationPointsTotal > experimentationPointsLeft) {

		craftingValues->clearAll();
		tano = new TangibleObject(player, 0xBC03F94, UnicodeString("a Viewscreen (broken)"),
				"object/tangible/loot/tool/shared_viewscreen_broken_s2.iff", TangibleObjectImplementation::GENERICITEM);
			craftingTool->setWorkingTano(tano);
			draftSchematic->setExpPoints(0);
			craftingTool->setAssemblyResults(8);
			player->sendSystemMessage("Trying to scam the system I see, take this!");

		} else {

			// Set new exp points subtracting those used above
			draftSchematic->setExpPoints(expPoints - totalPoints);
		}*/

	experimentationPointsUsed += totalPoints;

	// Start Player Object Delta **************************************
	PlayerObjectDeltaMessage9* dplay9 =
			new PlayerObjectDeltaMessage9(player->getPlayerObject());
	dplay9->setExperimentationPoints(experimentationPointsTotal - experimentationPointsUsed);
	dplay9->close();

	player->sendMessage(dplay9);
	// End Player Object Delta **************************************


	ManufactureSchematicObjectDeltaMessage3* dMsco3 =
			new ManufactureSchematicObjectDeltaMessage3(manufactureSchematic->getObjectID());
	dMsco3->updateComplexity(manufactureSchematic->getComplexity());
	dMsco3->updateCraftingValues(manufactureSchematic);
	dMsco3->close();

	player->sendMessage(dMsco3);

	ManufactureSchematicObjectDeltaMessage7* dMsco7 =
			new ManufactureSchematicObjectDeltaMessage7(manufactureSchematic->getObjectID());
	dMsco7->update9(manufactureSchematic, false);
	dMsco7->close();

	player->sendMessage(dMsco7);

	TangibleObjectDeltaMessage3* dtano3 =
			new TangibleObjectDeltaMessage3(prototype);
	dtano3->updateComplexity(float(manufactureSchematic->getComplexity()));
	dtano3->close();

	player->sendMessage(dtano3);

	ObjectControllerMessage* objMsg =
			new ObjectControllerMessage(player->getObjectID(), 0x0B, 0x0113);
	objMsg->insertInt(0x105);

	objMsg->insertInt(experimentationResult); // Experimentation Result
	objMsg->insertByte(clientCounter);

	player->sendMessage(objMsg);
}

void CraftingToolImplementation::experimentRow(CraftingValues* craftingValues,
		int rowEffected, int pointsAttempted, float failure) {

	float modifier, newValue;

	String title, subtitle, subtitlesTitle;

	title = craftingValues->getVisibleExperimentalPropertyTitle(rowEffected);

	for (int i = 0; i < craftingValues->getExperimentalPropertySubtitleSize(); ++i) {

		subtitlesTitle = craftingValues->getExperimentalPropertySubtitlesTitle(i);

		if (subtitlesTitle == title) {

			subtitle = craftingValues->getExperimentalPropertySubtitle(i);

			modifier = craftingManager->calculateExperimentationValueModifier(experimentationResult,
					pointsAttempted);

			newValue = craftingValues->getCurrentPercentage(subtitle)
					+ modifier;

			if (newValue > craftingValues->getMaxPercentage(subtitle))
				newValue = craftingValues->getMaxPercentage(subtitle);

			craftingValues->setCurrentPercentage(subtitle, newValue);
		}
	}
}


void CraftingToolImplementation::customization(PlayerCreature* player, String& name, int schematicCount, String& customization) {

	Locker _locker(_this);

	manufactureSchematic->setManufactureLimit(schematicCount);

	StringTokenizer tokenizer(customization);
	byte customizationindex, customizationvalue, customizationtype;

	Database::escapeString(name);

	UnicodeString customName(name);
	prototype->setCustomObjectName(customName, false);

	/// Set Name
	manufactureSchematic->getObjectName()->setStringId(prototype->getObjectNameStringIdFile(), prototype->getObjectNameStringIdName());

	/// Set Manufacture Schematic Custom name
	if(name != "")
		manufactureSchematic->setCustomObjectName(customName, false);

	Vector<byte>* customizationOptions = manufactureSchematic->getDraftSchematic()->getCustomizationOptions();

		while (tokenizer.hasMoreTokens()) {

			customizationindex = (byte)tokenizer.getIntToken();
			customizationtype = customizationOptions->get(customizationindex);

			customizationvalue = (byte)tokenizer.getIntToken();

			prototype->setCustomizationVariable(customizationtype, customizationvalue);
		}

	TangibleObjectDeltaMessage3* dtano3 = new TangibleObjectDeltaMessage3(prototype);
	dtano3->updateName(name);
	dtano3->updateCustomizationString();
	dtano3->close();

	player->sendMessage(dtano3);

	ManufactureSchematicObjectDeltaMessage3 * dMsco3 =
			new ManufactureSchematicObjectDeltaMessage3(manufactureSchematic->getObjectID());
	dMsco3->updateName(name);
	dMsco3->updateCondition(schematicCount);
	dMsco3->close();

	player->sendMessage(dMsco3);

	//Object Controller
	ObjectControllerMessage* objMsg =
			new ObjectControllerMessage(player->getObjectID(), 0x0B, 0x010C);
	objMsg->insertInt(0x15A);
	objMsg->insertInt(0);
	objMsg->insertByte(0);

	player->sendMessage(objMsg);

	state = 5;
}

void CraftingToolImplementation::finishStage1(PlayerCreature* player, int clientCounter) {

	/// pre: _this locked

	PlayerObjectDeltaMessage9* dplay9 =
			new PlayerObjectDeltaMessage9(player->getPlayerObject());
	dplay9->setCraftingState(state);
	dplay9->close();

	player->sendMessage(dplay9);

	//Object Controller
	ObjectControllerMessage* objMsg =
			new ObjectControllerMessage(player->getObjectID(), 0x0B, 0x01BE);
	objMsg->insertInt(0x109);
	objMsg->insertInt(4);
	objMsg->insertByte(clientCounter);

	player->sendMessage(objMsg);

	state = 6;
}

void CraftingToolImplementation::finishStage2(PlayerCreature* player, int clientCounter) {

	/// pre: _this locked

	state = 0;

	PlayerObjectDeltaMessage9* dplay9 =
			new PlayerObjectDeltaMessage9(player->getPlayerObject());
	dplay9->insertShort(5);
	dplay9->insertInt(0xFFFFFFFF);
	dplay9->setCraftingState(0);

	dplay9->close();
	player->sendMessage(dplay9);

	//Object Controller
	ObjectControllerMessage* objMsg =
			new ObjectControllerMessage(player->getObjectID(), 0x0B, 0x010C);
	objMsg->insertInt(0x10A);
	objMsg->insertInt(1);
	objMsg->insertByte(clientCounter); //?!?!
	player->sendMessage(objMsg);
}

void CraftingToolImplementation::createPrototype(PlayerCreature* player,
		int clientCounter, int practice) {

	if(manufactureSchematic == NULL) {
		sendSlotMessage(player, 0, IngredientSlot::NOSCHEMATIC);
		return;
	}

	if (prototype == NULL) {
		sendSlotMessage(player, 0, IngredientSlot::PROTOTYPENOTFOUND);
		return;
	}

	if (manufactureSchematic->isAssembled() && !manufactureSchematic->isCompleted()) {

		closeCraftingWindow(player, clientCounter);

		String xpType = manufactureSchematic->getDraftSchematic()->getXpType();
		int xp = manufactureSchematic->getDraftSchematic()->getXpAmount();

		if (practice != 0) {

			createObject(player, manufactureSchematic->getComplexity() * 2, false);

		} else {

			// This is for practiceing
			createObject(player, manufactureSchematic->getComplexity() * 2, true);
			xp *= 1.05f;
		}

		PlayerManager* playerManager = server->getPlayerManager();
		playerManager->awardExperience(player, xpType, xp, false);

		manufactureSchematic->setCompleted();

	} else {

		closeCraftingWindow(player, clientCounter);

		sendSlotMessage(player, clientCounter, IngredientSlot::WEIRDFAILEDMESSAGE);
	}
}

void CraftingToolImplementation::createManfSchematic(PlayerCreature* player,
		int clientCounter) {

	if(manufactureSchematic == NULL) {
		sendSlotMessage(player, 0, IngredientSlot::NOSCHEMATIC);
		return;
	}

	if (prototype == NULL) {
		sendSlotMessage(player, 0, IngredientSlot::PROTOTYPENOTFOUND);
		return;
	}

	if (manufactureSchematic->isAssembled() && !manufactureSchematic->isCompleted()) {

		//Object Controller
		ObjectControllerMessage* objMsg = new ObjectControllerMessage(
				player->getObjectID(), 0x0B, 0x010C);
		objMsg->insertInt(0x10B);
		objMsg->insertInt(1);
		objMsg->insertByte(clientCounter);

		player->sendMessage(objMsg);

		SceneObject* datapad = player->getSlottedObject("datapad");
		removeObject(manufactureSchematic);
		removeObject(prototype);

		manufactureSchematic->setPersistent(2);
		prototype->setPersistent(2);

		datapad->addObject(manufactureSchematic, -1, true);
		manufactureSchematic->setPrototype(prototype);
		manufactureSchematic->updateToDatabase();
		prototype->updateToDatabase();

		updateToDatabaseAllObjects(true);
		datapad->updateToDatabaseAllObjects(true);

	} else {

		closeCraftingWindow(player, clientCounter);

		sendSlotMessage(player, clientCounter, IngredientSlot::WEIRDFAILEDMESSAGE);
	}
}

void CraftingToolImplementation::createObject(PlayerCreature* player,
		int timer, bool practice) {

	/// pre: _this locked

	int timer2 = 1;

	Reference<UpdateToolCountdownTask* > updateToolCountdownTask;

	Reference<CreateObjectTask* > createObjectTask = new CreateObjectTask(player, _this, practice);

	if (server != NULL) {

		status = "@crafting:tool_status_working";

		while (timer > 0) {
			updateToolCountdownTask = new UpdateToolCountdownTask(player, _this, timer);
			updateToolCountdownTask->schedule(timer2);
			timer -= 5;
			timer2 += 5000;
		}

		if (timer < 0) {
			timer2 += (timer * 1000);
			timer = 0;
		}

		updateToolCountdownTask
				= new UpdateToolCountdownTask(player, _this, timer);

		updateToolCountdownTask->schedule(timer2);
		createObjectTask->schedule(timer2);

	} else {
		System::out << "server == NULL\n";
	}
}

void CraftingToolImplementation::depositObject(PlayerCreature* player, bool practice) {

	if(practice) {
	    removeObject(prototype);
		prototype = NULL;
		status = "@crafting:tool_status_ready";
		return;
	}

	ManagedReference<SceneObject* > inventory = player->getSlottedObject("inventory");

	if (inventory->getContainerObjectsSize() < 80) {

		player->sendSystemMessage("system_msg", "prototype_transferred");
		removeObject(prototype);
		updateToDatabaseAllObjects(true);

		inventory->addObject(prototype, -1, true);
		prototype->setPersistent(2);

		prototype->updateToDatabase();
		inventory->updateToDatabaseAllObjects(true);

		status = "@crafting:tool_status_ready";

	} else {

		player->sendSystemMessage("system_msg", "prototype_not_transferred");
		status = "@crafting:tool_status_finished";
	}
}
