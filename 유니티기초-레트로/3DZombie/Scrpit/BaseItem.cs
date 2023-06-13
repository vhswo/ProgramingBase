using System.Collections;
using System.Collections.Generic;
using UnityEngine;

abstract public class BaseItem : MonoBehaviour
{
    [SerializeField] protected LayerMask targetLayer;
    [SerializeField] protected int value = 0;

    virtual public bool Use(GameObject target)
    {
        if ((1 << target.layer).Equals(targetLayer))
        {
            gameObject.SetActive(false);
            return true;
        }
        return false;
    }

    public void SetPosition(Vector3 pos)
    {
        transform.position = pos;
        gameObject.SetActive(true);
    }

    virtual protected void OnDisable()
    {
        if (ItemMgr.Instance) ItemMgr.Instance.SetPooling(this);
    }
}
