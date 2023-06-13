using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealPack : BaseItem
{
    public override bool Use(GameObject target)
    {
        if(base.Use(target))
        {
            var health = target.GetComponent<LivingEntity>();
            if (health) health.RestoreHealth(value);
            return true;
        }
        return false;
    }
}
