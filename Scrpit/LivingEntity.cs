using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public interface IDamageable
{
    void OnDamage(float damage, Vector3 hiPoint, Vector3 hitNormal);
}

public class LivingEntity : MonoBehaviour, IDamageable
{
    [SerializeField] protected float maxHealth = 100;
    public float health { get; protected set; }
    public bool isDead { get { return (0 >= health);  } }
    public event System.Action OnDeath;

    protected virtual void OnEnable()
    {
        health = maxHealth;
    }
    public virtual void OnDamage(float damage, Vector3 hiPoint, Vector3 hitNormal)
    {
        if (isDead) return;

        health -= damage;
        if (isDead) Die();
    }

    private void Die()
    {
        if (null != OnDeath) OnDeath();
    }

    public virtual void RestoreHealth(float value)
    {
        if (isDead) return;
        health = Mathf.Clamp(health + value, 0, maxHealth);
    }
}
