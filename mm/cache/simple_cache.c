#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

#define MAX_NAME 20

struct identity {
	char name[MAX_NAME];
	int id;
	bool busy;
	struct list_head id_list;
};

static struct list_head superhero_list;
static struct kmem_cache *identity_cachep;

/*
 * The constructor seem to gurantee the creation of new cache.
 */
static void identity_ctor(void *p)
{
}

static int identity_create(char *name, int id)
{
	struct identity *p;

	p = kmem_cache_alloc(identity_cachep, GFP_KERNEL);
	if (!p)
		return -ENOMEM;

	strcpy(p->name, name);
	p->id = id;
	p->busy = false;

	list_add(&p->id_list, &superhero_list);
	return 0;
}

static struct identity *identity_find(int id)
{
	struct identity *p;

	list_for_each_entry(p, &superhero_list, id_list) {
		if (p->id == id)
			return p;
	}

	return NULL;
}

static void identity_destroy(int id)
{
	struct identity *p, *next;

	list_for_each_entry_safe(p, next, &superhero_list, id_list) {
		if (p->id == id) {
			list_del(&p->id_list);
			kmem_cache_free(identity_cachep, p);
			break;
		}
	}
}

static int my_init(void)
{
	struct identity *temp;
	void (*fp)(void *);

	fp = &identity_ctor;
	identity_cachep = kmem_cache_create("superhero_cache",
				sizeof(struct identity),
				0, SLAB_HWCACHE_ALIGN, fp);
	if (!identity_cachep)
		return -ENOMEM;

	INIT_LIST_HEAD(&superhero_list);

	/* No need for strict ordering. only for simplification */
	identity_create("Superman", 0);
	identity_create("Spiderman", 1);
	identity_create("Batman", 3);
	identity_create("Hulk", 4);
	identity_create("Wolverin", 5);

	/* Batman, you must respond */
	temp = identity_find(4);
	pr_debug("id 4: %s", temp->name);

	/* Negative test */
	temp = identity_find(8);
	if (temp == NULL)
		pr_debug("id 8 not found");

	identity_destroy(3);
	identity_destroy(1);
	identity_destroy(0);
	identity_destroy(4);
	identity_destroy(5);

	return 0;
}

static void my_exit(void)
{
	/* Negative test */
	identity_destroy(2);
	kmem_cache_destroy(identity_cachep);
}

module_init(my_init);
module_exit(my_exit);
