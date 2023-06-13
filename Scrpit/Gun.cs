using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum State
{
    Ready,
    Empty,
    Reloading
}

public class Gun : MonoBehaviour
{
 
    [SerializeField] private float damage = 25;
    [Header("SFX")]
    [SerializeField] private ParticleSystem muzzleFlashEffect;
    [SerializeField] private ParticleSystem shellEjectEffect;
    [SerializeField] private AudioClip shootSound;
    [SerializeField] private AudioClip reloadSound;
    private AudioSource audioSource;
    private Transform pivot;
    private State state = State.Ready;
    public void Fire() 
    {
        if(state.Equals(State.Ready) && Time.time >= lastFireTime +timeBetFire)
        {
            lastFireTime = Time.time;
            Shot();
        }
    }
    public bool Reload() 
    {
        if (state.Equals(State.Reloading) || 0 >= ammoRemain || magCapacity <= magAmmo) return false;

        StartCoroutine(ReloadRoutine());
        return true; 
    }

    private void Shot()
    {
        if(firePos)
        {
            RaycastHit hit;
            Vector3 hitPos = firePos.position + firePos.position + firePos.forward * hitRange;
            if(Physics.Raycast(firePos.position, firePos.forward, out hit, hitRange))
            {
                IDamageable target = hit.collider.GetComponent<IDamageable>();
                if (null != target) target.OnDamage(damage, hit.point, hit.normal);
                hitPos = hit.point;
            }
            StartCoroutine(ShotEffect(hitPos));
        }
    }
    private IEnumerator ShotEffect(Vector3 hitPosition)
    {
        if(bulletLineRenderer)
        {
            bulletLineRenderer.SetPosition(0, firePos.position);
            bulletLineRenderer.SetPosition(1, hitPosition);
            bulletLineRenderer.enabled = true;
        }
        yield return new WaitForSeconds(0.03f);
        if (bulletLineRenderer) bulletLineRenderer.enabled = false;
    }

    private IEnumerator ReloadRoutine()
    {
        state = State.Reloading;

        if (audioSource && reloadSound) audioSource.PlayOneShot(reloadSound);
        yield return new WaitForSeconds(reloadTime);

        int ammoTofill = Mathf.Min(magCapacity - magAmmo, ammoRemain);
        ammoRemain -= ammoTofill;
        magAmmo += ammoTofill;

        state = State.Ready;
    }

    [Header("Gun")]
    [SerializeField] private Transform firePos;
    [SerializeField] private Transform leftHandMount;
    [SerializeField] private Transform rightHandMount;

    [Header("IK")]
    [SerializeField] [Range(0, 1)] private float leftHandPosWeight = 1f;
    [SerializeField] [Range(0, 1)] private float leftHandRoWeight = 1f;
    [SerializeField] [Range(0, 1)] private float rightHandPosWeight = 1f;
    [SerializeField] [Range(0, 1)] private float rightHandRoWeight = 1f;

    public Vector3 Pivot { get { return (pivot) ? pivot.position : Vector3.zero; } set { if (pivot) pivot.position = value; } }
    public Vector3 LeftHandMountPos { get { return (leftHandMount) ? leftHandMount.position : Vector3.zero; } }
    public Quaternion LeftHandMonutRo { get { return (leftHandMount) ? leftHandMount.rotation : Quaternion.identity; } }
    public Vector3 RightHandMountPos { get { return (rightHandMount) ? rightHandMount.position : Vector3.zero; } }
    public Quaternion RightHandMountRo { get { return (rightHandMount) ? rightHandMount.rotation : Quaternion.identity; } }

    public float LeftHandPosWeight { get { return leftHandPosWeight; } }
    public float LeftHandRoWeight { get { return leftHandRoWeight; } }
    public float RightHandPosWeight { get { return rightHandPosWeight; } }
    public float RightHandRoWeight { get { return rightHandRoWeight; } }

    [Header("Gun ¼Ó¼º")]
    [SerializeField] private float hitRange = 50f;
    [SerializeField] private float timeBetFire = 0.12f;
    private float lastFireTime;
    private LineRenderer bulletLineRenderer;

    private readonly int magCapacity = 25;
    public int ammoRemain { get; private set; } = 100;
    public int magAmmo { get; private set; }

    [SerializeField] private float reloadTime = 0.9f;

    private void Awake()
    {
        audioSource = gameObject.AddComponent<AudioSource>();
        audioSource.playOnAwake = false;
        pivot = transform.parent;

        bulletLineRenderer = GetComponent<LineRenderer>();
        if(bulletLineRenderer)
        {
            bulletLineRenderer.positionCount = 2;
            bulletLineRenderer.enabled = false;
        }

        magAmmo = magCapacity;
        state = State.Ready;
        lastFireTime = Time.time - timeBetFire;
    }

    public void AddAmmo(int value)
    {
        ammoRemain += value;
    }
}
