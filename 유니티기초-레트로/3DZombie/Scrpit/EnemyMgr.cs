using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMgr : Spawn<Enemy>
{
    public static EnemyMgr Instance { get; private set; }

    private void Awake()
    {
        if(!Instance)
        {
            Instance = this;
            return;
        }
        Destroy(gameObject);
    }

    [SerializeField] private float damageMax = 40f;
    [SerializeField] private float damageMin = 20f;

    [SerializeField] private float healthMax = 200f;
    [SerializeField] private float healthMin = 100f;

    [SerializeField] private float speedMax = 3f;
    [SerializeField] private float speenMin = 1f;

    [SerializeField] private Color strongColor = Color.red;

    private int spawnCount = 0;

    private void Start()
    {
        for(int i =0; i < 50; i++)
        {
            MakeObjPooling(0);
        }
    }

    public override void Update()
    {
        if(0 >= spawnCount)
        {
            SpawnObj();
        }
    }

    private void SetupEnemy(float intensity)
    {
        if (isEmptyPool) MakeObjPooling(0);

        var enemy = GetFirstObj();
        if(enemy)
        {
            spawnCount++;

            var damage = Mathf.Lerp(damageMin, damageMax, intensity);
            var health = Mathf.Lerp(healthMin, healthMax, intensity);
            var speed = Mathf.Lerp(speenMin, speedMax, intensity);
            var color = Color.Lerp(Color.white, strongColor, intensity);
            var pos = GetRandomPointOnNavMesh(Vector3.zero);
            enemy.Setup(damage, health, speed, color, pos);
        }
    }

    public void DecreaseSpawnCount()
    {
        UIMgr.Instance.UpdateWaveText(Gmr.Instance.wave, --spawnCount);
    }

    protected override void SpawnObj()
    {
        if(Gmr.Instance.NextWave())
        {
            var count = Gmr.Instance.enemySpawnCount;
            for (int i = 0; count > i; i++) SetupEnemy(Random.value);

            UIMgr.Instance.UpdateWaveText(Gmr.Instance.wave, spawnCount);
        }
    }
}
