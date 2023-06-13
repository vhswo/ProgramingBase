using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmmoPack : BaseItem
{
    public override bool Use(GameObject target)
    {
        if(base.Use(target))
        {
            var shooter = target.GetComponent<PlayerShooter>();
            if (shooter) shooter.AddAmmo(value);
            return true;
        }
        return false;
    }
}
