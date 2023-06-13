using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerInput : MonoBehaviour
{
    [SerializeField] private string moveAxisName = "Vertical";
    [SerializeField] private string rotateAxisName = "Horizontal";
    [SerializeField] private string fireBtnName = "Fire1";
    [SerializeField] private string reloadBtnName = "Reload";

    public float move { get; private set; }
    public float rotate { get; private set; }

    public bool fire { get; private set; }
    public bool reload { get; private set; }
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        move = Input.GetAxis(moveAxisName);
        rotate = Input.GetAxis(rotateAxisName);

        fire = Input.GetButton(fireBtnName);
        reload = Input.GetButtonDown(reloadBtnName);
    }
}
