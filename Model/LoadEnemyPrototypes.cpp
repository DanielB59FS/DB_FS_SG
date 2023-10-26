void LoadEnemyPrototypes(const std::string &_filename)
{
	irr::io::IrrXMLReader *xmlReader = irr::io::createIrrXMLReader(_filename.c_str());
	while(xmlReader->read())
	{
		if(xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
		{
			if(!strcmp("Enemy", xmlReader->getNodeName()))
			{
				Enemy *e = new Enemy();
				e->SetShip(NULL);
				e->SetScore(xmlReader->getAttributeValueAsInt("score"));
				e->SetShipID(xmlReader->getAttributeValueAsInt("ship"));
				e->SetWeaponID(xmlReader->getAttributeValueAsInt("weapon"));
				e->SetWeaponLevel(xmlReader->getAttributeValueAsInt("level"));
				e->SetShieldID(xmlReader->getAttributeValueAsInt("shield"));
				e->SetPropulsionID(xmlReader->getAttributeValueAsInt("propulsion"));
				e->SetGeneratorID(xmlReader->getAttributeValueAsInt("generator"));
				e->SetFireFrequency(xmlReader->getAttributeValueAsFloat("fire"));
				e->SetMinDistance(xmlReader->getAttributeValueAsFloat("min"));
				e->SetMaxDistance(xmlReader->getAttributeValueAsFloat("max"));
				std::string behavior;
				if(xmlReader->getAttributeValue("behavior"))
					behavior = xmlReader->getAttributeValue("behavior");
				if(behavior == "flee")
					e->SetMovementBehavior(Enemy::FLEE);
				else if(behavior == "follow")
					e->SetMovementBehavior(Enemy::FOLLOW);
				else if(behavior == "joust")
				{
					e->SetMovementBehavior(Enemy::FOLLOW);
					e->SetJoust(true);
				}
				std::string powerup = xmlReader->getAttributeValue("powerUp");
				if(powerup == "score")
					e->SetPowerUpID(Enemy::SCORE);
				else if(powerup == "energy")
					e->SetPowerUpID(Enemy::ENERGY);
				else if(powerup == "weapon")
					e->SetPowerUpID(Enemy::WEAPON);
				e->SetPowerUpAmount(xmlReader->getAttributeValueAsInt("powerUpAmount"));
				SaiphApp::AddEnemyPrototype(e);
			}
		}
	}
	delete xmlReader;
}
