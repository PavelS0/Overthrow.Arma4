class OVT_BaseUpgradeDefensePatrol : OVT_BasePatrolUpgrade
{
	
	
	protected override void AddWaypoints(SCR_AIGroup aigroup)
	{
		array<AIWaypoint> queueOfWaypoints = new array<AIWaypoint>();
		AIWaypoint firstWP;
		
			
		for(int i=0; i< 4; i++)
		{
			vector randompos = OVT_Global.GetRandomNonOceanPositionNear(m_BaseController.GetOwner().GetOrigin(), m_Config.m_Difficulty.baseRange);			
			AIWaypoint wp = m_Config.SpawnPatrolWaypoint(randompos);
			if(i==0) firstWP = wp;
			queueOfWaypoints.Insert(wp);
			
			wp = m_Config.SpawnSearchAndDestroyWaypoint(randompos);			
			queueOfWaypoints.Insert(wp);			
		}			
		AIWaypointCycle cycle = AIWaypointCycle.Cast(m_Config.SpawnWaypoint(m_Config.m_pCycleWaypointPrefab, firstWP.GetOrigin()));
		cycle.SetWaypoints(queueOfWaypoints);
		cycle.SetRerunCounter(-1);
		aigroup.AddWaypoint(cycle);		
	}
	
	override int Spend(int resources, float threat)
	{
		int spent = 0;
		
		while(resources > 0)
		{
			OVT_GroupType type = OVT_GroupType.LIGHT_INFANTRY;
			
			if(m_iNumGroups == 0 || threat > 50)
			{
				type = OVT_GroupType.ANTI_TANK;
			}else if(threat > 25){
				type = OVT_GroupType.HEAVY_INFANTRY;
			}
			
			int newres = m_Config.m_Difficulty.baseResourceCost * 4;
			
			OVT_Faction faction = m_Config.GetOccupyingFaction();
			ResourceName res = faction.GetRandomGroupByType(type);
			m_iProxedResources += newres;
			m_ProxiedGroups.Insert(res);
			vector spawnpos = OVT_Global.GetRandomNonOceanPositionNear(m_BaseController.GetOwner().GetOrigin(), 50);			
			m_ProxiedPositions.Insert(spawnpos);			
			
			if(newres > resources){
				newres = resources;
				//todo: delete some soldiers when overspending
			}
			
			spent += newres;
			resources -= newres;
			m_iNumGroups++;
		}
		
		return spent;
	}
}