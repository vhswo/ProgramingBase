using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIMgr : MonoBehaviour
{
    public static UIMgr Instance { get; private set; }
    private void Awake()
    {
        if(null == Instance)
        {
            Instance = this;
            DontDestroyOnLoad(gameObject);
            return;
        }
        Destroy(gameObject);
    }
    [SerializeField] private Text ammoText;
    [SerializeField] private Text scoreText;
    [SerializeField] private Text waveText;
    [SerializeField] private GameObject gameoverUI;
    public event System.Action RestartEvent;

    public void UpdateAmmoText(int magAmmo, int remainAmmo)
    {
        ammoText.text = string.Format("{0} / {1}", magAmmo, remainAmmo);
    }

    public void UpdateScoreText(int newScore)
    {
        scoreText.text = string.Format("Score : {0}", newScore);
    }

    public void UpdateWaveText(int waves, int count)
    {
        waveText.text = string.Format("Wave : {0}\nEnemy Left : {1}", waves, count);
    }
    public void GameOver()
    {
        gameoverUI.SetActive(true);
    }
    public void Restart()
    {
        if (null != RestartEvent) RestartEvent();
    }
}
