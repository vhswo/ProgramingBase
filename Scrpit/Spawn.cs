using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

abstract public class Spawn<T> : MonoBehaviour where T: MonoBehaviour
{
    [SerializeField] protected T[] prefabs;
    [SerializeField] private float maxDistance = 5f;

    private List<T> pooling = new List<T>();
    public bool isEmptyPool { get { return !(0 < pooling.Count); } }

    abstract public void Update();
    abstract protected void SpawnObj();

    protected void MakeObjPooling(int index)
    {
        var select = Mathf.Min(prefabs.Length - 1, index);
        if( 0 <= select)
        {
            var obj = Instantiate(prefabs[select]); 
            if(obj)
            {
                pooling.Add(obj);
                obj.gameObject.SetActive(false);
            }
        }
    }
    public void SetPooling(T obj)
    {
        if (!pooling.Contains(obj)) pooling.Add(obj);
    }

    private T GetObject(int index)
    {
        T obj = null;
        if(0 < pooling.Count)
        {
            obj = pooling[index];
            pooling.RemoveAt(index);
        }
        return obj;
    }
    protected T GetObjRandom()
    {
        var select = Random.Range(0, pooling.Count);
        return GetObject(select);
    }
    protected T GetFirstObj()
    {
        return GetObject(0);
    }
    protected Vector3 GetRandomPointOnNavMesh(Vector3 center)
    {
        Vector3 randomPos = Random.insideUnitSphere * maxDistance + center;
        NavMeshHit navHit;
        NavMesh.SamplePosition(randomPos, out navHit, maxDistance, NavMesh.AllAreas);

        return navHit.position;
    }
}
