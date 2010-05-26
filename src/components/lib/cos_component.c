/**
 * Copyright 2007 by Gabriel Parmer, gabep1@cs.bu.edu
 *
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 */

#include <cos_component.h>

/* FIXME:  I Should now be able to remove this */
int stkmgr_stack_space[ALL_STACK_SZ];
//struct cos_stack cos_stack_space[MAX_NUM_THREADS];
//struct cos_stack *cos_stack_free_list = NULL;
void * cos_stack_free_list = NULL;


/* 
 * This is initialized at load time with the spd id of the current
 * spd, and is passed into all system calls to identify the calling
 * service.
 */
long cos_this_spd_id = 0;
void *cos_heap_ptr = NULL;

struct cos_sched_data_area cos_sched_notifications = {
	.cos_next = {.next_thd_id = 0, .next_thd_flags = 0},
	.cos_locks = {.owner_thd = 0, .queued_thd = 0},
	.cos_events = {}
};

__attribute__ ((weak))
void cos_init(void *arg)
{
	return;
}

__attribute__ ((weak))
void cos_upcall_exec(void *arg)
{
	return;
}

__attribute__ ((weak))
void cos_upcall_fn(upcall_type_t t, void *arg1, void *arg2, void *arg3)
{
	switch (t) {
	case COS_UPCALL_BRAND_EXEC:
	{
		cos_upcall_exec(arg1);
		break;
	}
	case COS_UPCALL_BOOTSTRAP:
	{
		cos_argreg_init();
		cos_init(arg1);
		break;
	}
	default:
		*(int*)NULL = 0;
		return;
	}
	return;
}

__attribute__ ((weak))
int main(void)
{
	return 0;
}

__attribute__((weak)) vaddr_t ST_user_caps;

extern const vaddr_t cos_atomic_cmpxchg, cos_atomic_cmpxchg_end, 
	cos_atomic_user1, cos_atomic_user1_end, 
	cos_atomic_user2, cos_atomic_user2_end, 
	cos_atomic_user3, cos_atomic_user3_end, 
	cos_atomic_user4, cos_atomic_user4_end;
extern const vaddr_t cos_upcall_entry;

/* 
 * Much of this is either initialized at load time, or passed to the
 * loader though this structure.
 */
struct cos_component_information cos_comp_info = {
	.cos_this_spd_id = 0,
	.cos_heap_ptr = 0,
	.cos_stacks.freelists[0] = {.freelist = 0, .thd_id = 0},
	.cos_upcall_entry = (vaddr_t)&cos_upcall_entry,
	.cos_sched_data_area = &cos_sched_notifications,
	.cos_user_caps = (vaddr_t)&ST_user_caps,
	.cos_ras = {{.start = (vaddr_t)&cos_atomic_cmpxchg, .end = (vaddr_t)&cos_atomic_cmpxchg_end}, 
		    {.start = (vaddr_t)&cos_atomic_user1, .end = (vaddr_t)&cos_atomic_user1_end},
		    {.start = (vaddr_t)&cos_atomic_user2, .end = (vaddr_t)&cos_atomic_user2_end},
		    {.start = (vaddr_t)&cos_atomic_user3, .end = (vaddr_t)&cos_atomic_user3_end},
		    {.start = (vaddr_t)&cos_atomic_user4, .end = (vaddr_t)&cos_atomic_user4_end}},
	.cos_poly = {0, }
};