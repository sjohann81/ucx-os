/* file:          corotine.c
 * description:   corotine implementation
 * date:          12/2024
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

/* initialize a corotine group */
struct cgroup_s *ucx_cr_ginit(void)
{
	struct cgroup_s *cgroup;
	
	cgroup = malloc(sizeof(struct cgroup_s));
	
	if (!cgroup)
		return 0;
		
	cgroup->crlist = list_create();
	
	if (!cgroup->crlist) {
		free(cgroup);
		return 0;
	}
	
	return cgroup;
}

/* destroy a corotine group */
int32_t ucx_cr_gdestroy(struct cgroup_s *cgroup)
{
	if (cgroup->fibers > 0)
		return -1;
	
	free(cgroup->crlist);
	free(cgroup);
	
	return 0;
}

/* add a corotine to a group */
int32_t ucx_cr_add(struct cgroup_s *cgroup, void *(corotine)(void *), uint8_t priority)
{
	struct ccb_s *cr;
	struct node_s *node;
	
	cr = malloc(sizeof(struct ccb_s));
	
	if (!cr)
		return -1;
		
	cr->corotine = corotine;
	cr->priority = priority;
	cr->pcounter = priority;
	
	node = list_pushback(cgroup->crlist, cr);
	
	if (!node) {
		free(cr);
		return -1;
	}
	
	cgroup->fibers++;

	return 0;
}

/* remove a corotine from a group */
static struct node_s *cr_trycancel(struct node_s *node, void *arg)
{
	struct ccb_s *cr = node->data;
	struct ccb_s *crcmp = (struct ccb_s *)arg;
	
	if (cr->corotine == crcmp->corotine)
		return node->data;
	
	return 0;
}

int32_t ucx_cr_cancel(struct cgroup_s *cgroup, void *(corotine)(void *))
{
	struct node_s *node;
	struct ccb_s *cr;
	
	node = list_foreach(cgroup->crlist, cr_trycancel, corotine);
	
	if (node) {
		cr = node->data;
		free(cr);
		list_remove(cgroup->crlist, node);
	}
	
	return node ? 1 : 0;
}

/* schedule a group of corotines */
static struct node_s *cr_trysched(struct node_s *node, void *arg)
{
	struct ccb_s *cr = node->data;
	
	if (!--cr->pcounter) {
		cr->pcounter = cr->priority;
		cr->corotine(arg);
		
		return node;
	}
	
	return 0;
}

int32_t ucx_cr_schedule(struct cgroup_s *cgroup, void *arg)
{
	struct node_s *node;
	
	node = list_foreach(cgroup->crlist, cr_trysched, arg);
	
	return node ? 1 : 0;
}
