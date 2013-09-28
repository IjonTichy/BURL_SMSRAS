script SAMSARA_BURL (int class, int slot, int dropped)
{
    int ret = 0;

    switch (slot)
    {
      case SLOT_CHAINSAW:
        if (!CheckInventory("Eager Beaver")) { ret = 1; }
        if (!CheckInventory("Accurate Bullet Gun")) { ret = 1; }
        if (CheckInventory("Clip") != GetAmmoCapacity("Clip")) { ret = 1; }

        if (ret == 0) { break; }

        GiveInventory("Eager Beaver", 1);
        GiveInventory("Accurate Bullet Gun", 1);
        break;
    }

    SetResultValue(ret);
}

#define EXCOUNT 1
int Exhausts[EXCOUNT]  = {"FartExhaust"};
int Exhausts2[EXCOUNT] = {"LolExhaust"};
int Explodes[EXCOUNT]  = {"Exploder"};

script 528 (int exhaust)
{
    int i, j, k, l;
    int myTID = defaultTID(-1);
    exhaust = middle(0, exhaust, EXCOUNT-1);

    if (!SetActivatorToTarget(0)) { terminate; }

    int yourTID = defaultTID(-1);
    int camTID  = unusedTID(55000, 65000);

    if (GameType() == GAME_NET_COOPERATIVE && PlayerNumber() != -1) { terminate; }

    if (PlayerNumber() != -1) { SetPlayerProperty(0, 0, PROP_FROZEN); }
    SetActorVelocity(0, 0,0,16.0, 0,0);

    for (l = 0; l < 15; l++)
    {
        i = random(0, 1.0);
        j = random(0, 1.0);
        k = unusedTID(20000, 25000);

        Spawn(Exhausts2[exhaust], GetActorX(0), GetActorY(0), GetActorZ(0), k);
        SetActorVelocity(k, 6 * FixedMul(cos(i), cos(j)), 6 * FixedMul(sin(i), cos(j)), 1 * sin(j) + 2.0, 0,0);
    }

    if (PlayerNumber() != -1)
    {
        Spawn("FuckYourselfKillCam", GetActorX(0), GetActorY(0), GetActorZ(0) + GetActorViewHeight(myTID), camTID);
        ACS_ExecuteAlways(529, 0, PlayerNumber(), camTID, yourTID);
    }

    Delay(12);
    GiveInventory("NoInteractionOn", 1);

    j = random(2.0,3.0);

    for (i = 0; i < 105; i++)
    {
        SetActorVelocity(0, 5*sin(itof(i)/18),5*cos(itof(i)/18),i*j, 0,0);
        GiveInventory(Exhausts[exhaust], 1);
        Delay(1);
    }

    GiveInventory(Explodes[exhaust], 1);
    SetActorProperty(0, APROP_Health, 1);

    i = random(0, 1.0);
    j = random(0, 1.0);

    SetActorVelocity(0, 50 * FixedMul(cos(i), cos(j)), 50 * FixedMul(sin(i), cos(j)), 50 * sin(j), 0,0);

    k = 0;
    if (GetActorCeilingZ(0) >= GetActorZ(0)) { GiveInventory("NoInteractionOff", 1); k = 1; }

    SetActivator(myTID);

    Thing_Damage2(yourTID, 2*max(100, GetActorProperty(yourTID, APROP_SpawnHealth)), "Normal");

    SetActivator(yourTID);
    DamageThing(0);

    Delay(35);
    Thing_Remove(camTID);
}

script 529 (int pln, int camTID, int targetTID) clientside
{
    int x, y, z;
    int dist, xydist;
    int newang, newpitch;

    ChangeCamera(camTID, 0, 0);

    while (ThingCount(0, camTID))
    {
        x = GetActorX(targetTID) - GetActorX(camTID);
        y = GetActorY(targetTID) - GetActorY(camTID);
        z = GetActorZ(targetTID) - GetActorZ(camTID);
    
        dist   = magnitudeThree_f(x, y, z);
        xydist = magnitudeTwo_f(x, y); 
        newang = VectorAngle(x, y);
        newpitch = FixedMul(0.25, FixedDiv(xydist, dist)) - 0.25;
        if (z < 0) { newpitch = -newpitch; }

        //Log(s:"(", f:x, s:", ", f:y, s:", ", f:z, s:") ", f:dist, s:", ", f:newpitch);

        SetActorAngle(camTID, newang);
        SetActorPitch(camTID, newpitch);

        Delay(1);
    }
}
