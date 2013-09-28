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
