using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Enemy : LivingEntity
{
    [SerializeField] private LayerMask targetLayer;
    [SerializeField] [Range(0, 100)] private float searchRange = 20;

    [SerializeField] private NavMeshAgent agent;
    [SerializeField] private AudioClip deathSound;
    [SerializeField] private AudioClip hitSound;
    [SerializeField] private ParticleSystem hitEffect;

    [SerializeField] private Renderer enemyRenderer;
    private AudioSource audioSource;

    private Animator anim;
    private new Collider collider;
    private float damage = 20f;

    public void Setup(float damage, float maxHealth, float speed,Color color, Vector3 pos)
    {
        this.damage = damage;
        this.maxHealth = maxHealth;
        if (agent) agent.speed = speed;
        if (enemyRenderer) enemyRenderer.material.color = color;
        transform.position = pos;
        gameObject.SetActive(true);
    }
    private void OnDrawGizmosSelected()
    {
        Gizmos.DrawWireSphere(transform.position, searchRange);
    }

    private void Awake()
    {
        anim = GetComponent<Animator>();
        collider = GetComponent<Collider>();

        audioSource = gameObject.AddComponent<AudioSource>();
        audioSource.playOnAwake = false;
        OnDeath += () =>
         {
             if (collider) collider.enabled = false;
             if (agent) agent.isStopped = true;
             if (anim) anim.SetBool("isDead", isDead);
             if (audioSource && deathSound) audioSource.PlayOneShot(deathSound);
             Gmr.Instance.AddScore(100);
             EnemyMgr.Instance.DecreaseSpawnCount();
         };

    }

    protected override void OnEnable()
    {
        base.OnEnable();

        if (anim) anim.SetBool("isDead", isDead);
        if (collider) collider.enabled = true;
        if (agent) agent.isStopped = false;
        StartCoroutine(UpdatePath());
    }

    private IEnumerator UpdatePath()
    {
        while(!isDead)
        {
            if(agent)
            {
                var targets = Physics.OverlapSphere(transform.position, searchRange, targetLayer);
                if(null != targets && 0 < targets.Length)
                {
                    var livingEntity = targets[0].GetComponent<LivingEntity>();
                    if (livingEntity && !livingEntity.isDead)
                    {
                        var targetPos = livingEntity.transform.position;

                        agent.SetDestination(targetPos);
                        if (Vector3.Distance(targetPos, transform.position) <= agent.stoppingDistance)
                        {
                            targetPos.y = transform.position.y;
                            var dir = (targetPos - transform.position).normalized;
                            transform.rotation = Quaternion.LookRotation(dir);
                            StartCoroutine(Attack(livingEntity));
                            yield break;
                        }
                    }
                }
                if (anim) anim.SetFloat("Magnitude", agent.velocity.magnitude);

            }
            yield return new WaitForSeconds(0.04f);
        }
    }

    private IEnumerator Attack(LivingEntity target)
    {
        if(agent && target)
        {
            var trTarget = target.transform;
            while(!isDead && !target.isDead)
            {
                if (anim) anim.SetTrigger("Attack");
                yield return new WaitForSeconds(1.1f);

                if (Vector3.Distance(trTarget.position, transform.position) > agent.stoppingDistance) break;

                //플레이어 받은 데미지 추가
                if (isDead || target.isDead) yield break;
                var hitNormal = transform.position - trTarget.position;
                target.OnDamage(damage, Vector3.zero, hitNormal);

                yield return new WaitForSeconds(1.2f);

                if (Vector3.Distance(trTarget.position, transform.position) > agent.stoppingDistance) break;
            }
        }
        if(!isDead) StartCoroutine(UpdatePath());
    }

    public override void OnDamage(float damage, Vector3 hiPoint, Vector3 hitNormal)
    {
        base.OnDamage(damage, hiPoint, hitNormal);

        if(anim && !isDead)
        {
            if(hitEffect)
            {
                var hitEffectTR = hitEffect.transform;
                hitEffectTR.position = hiPoint;
                hitEffectTR.rotation = Quaternion.LookRotation(hitNormal);
                hitEffect.Play();
            }
            if (audioSource && hitSound) audioSource.PlayOneShot(hitSound);
            anim.SetTrigger("Damaged");
        }
    }

    public void UnactiveObject()
    {
        gameObject.SetActive(false);
    }

    private void OnDisable()
    {
        if (EnemyMgr.Instance) EnemyMgr.Instance.SetPooling(this);
    }
}
