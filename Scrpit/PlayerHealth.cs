using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerHealth : LivingEntity
{
    [SerializeField] private Slider healthSlider;
    [SerializeField] private ParticleSystem hitEffect;
    [SerializeField] private AudioClip hitSound;
    [SerializeField] private AudioClip deathSound;
    [SerializeField] private AudioClip pickUpItemSound;
    private AudioSource audioSource;
    private PlayerInput playerInput;
    private Animator anim;

    private void OnTriggerEnter(Collider other)
    {
        var item = other.GetComponent<BaseItem>();
        if(item)
        {
            item.Use(gameObject);
            if (audioSource && pickUpItemSound) audioSource.PlayOneShot(pickUpItemSound);
        }
    }

    protected override void OnEnable()
    {
        base.OnEnable();
        if(healthSlider)
        {
            healthSlider.maxValue = maxHealth;
            healthSlider.value = health;
        }
    }

    public override void OnDamage(float damage, Vector3 hiPoint, Vector3 hitNormal)
    {
        base.OnDamage(damage, hiPoint, hitNormal);
        if(!isDead && hitEffect)
        {
            hitEffect.transform.rotation = Quaternion.LookRotation(hitNormal);
            hitEffect.Play();

            if (audioSource && hitSound) audioSource.PlayOneShot(hitSound);
        }
        healthSlider.value = health;
    }

    public override void RestoreHealth(float value)
    {
        base.RestoreHealth(value);
        healthSlider.value = health;
    }

    private void Awake()
    {
        playerInput = GetComponent<PlayerInput>();
        anim = GetComponent<Animator>();

        audioSource = gameObject.AddComponent<AudioSource>();
        audioSource.playOnAwake = false;

        OnDeath += () =>
        {
            UIMgr.Instance.GameOver();
            if (playerInput) playerInput.enabled = false;
            if (anim) anim.SetTrigger("Die");
            if (audioSource && deathSound) audioSource.PlayOneShot(deathSound);
        };
    }
}
