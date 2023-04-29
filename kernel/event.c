/* file:          event.c
 * description:   event management
 * date:          04/2023
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

int32_t ucx_event_queue(uint16_t events)
{
	// if event queue not created, do it
	// if events not changed but this API is called, leave it alone
	// if events is changed, recreate it (drop all events)
	// if events is changed to zero, destroy queue
}

int32_t ucx_event_add(void *callback, uint16_t delay)
{
}

int32_t ucx_event_remove(void *callback)
{
}
