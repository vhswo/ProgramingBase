using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Gmr : MonoBehaviour
{
    private static Gmr instance = null;
    public static Gmr Instance
    {
        get
        {
            if(!instance)
            {
                instance = FindObjectOfType<Gmr>();
                if (!instance) instance = new GameObject("Gmr").AddComponent<Gmr>();
                DontDestroyOnLoad(Instance.gameObject);
            }
            return instance;
        }
    }

    [SerializeField] private int maxWave = 10;
    public int wave { get; private set; } = 0;
    public int enemySpawnCount { get { return Mathf.RoundToInt(wave * 1.5f); } }
    public bool NextWave()
    {
        if(maxWave > wave)
        {
            wave++;
            return true;
        }
        return false;
    }

    private void Awake()
    {
        if (this != Instance) Destroy(gameObject);
    }

    private int score = 0;

    public void AddScore(int value)
    {
        score += value;
        UIMgr.Instance.UpdateScoreText(score);
    }

    private void Update()
    {
        if (ItemMgr.Instance) ItemMgr.Instance.Update();
        if (EnemyMgr.Instance) EnemyMgr.Instance.Update();
    }
}
