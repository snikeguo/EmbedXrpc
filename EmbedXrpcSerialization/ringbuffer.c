#include "ringbuffer.h"

enum rt_ringbuffer_state rt_ringbuffer_status(struct rt_ringbuffer* rb)
{
	if (rb->read_index == rb->write_index)
	{
		if (rb->read_mirror == rb->write_mirror)
			return RT_RINGBUFFER_EMPTY;
		else
			return RT_RINGBUFFER_FULL;
	}
	return RT_RINGBUFFER_HALFFULL;
}

void rt_ringbuffer_init(struct rt_ringbuffer* rb,
	uint8_t* pool,
	int16_t            size)
{
	El_Assert(rb != NULL);
	El_Assert(size > 0);

	/* initialize read and write index */
	rb->read_mirror = rb->read_index = 0;
	rb->write_mirror = rb->write_index = 0;

	/* set buffer pool and size */
	rb->buffer_ptr = pool;
	rb->buffer_size = RT_ALIGN_DOWN(size, RT_ALIGN_SIZE);
}
//RTM_EXPORT(rt_ringbuffer_init);

/**
 * put a block of data into ring buffer
 */
uint32_t rt_ringbuffer_put(struct rt_ringbuffer* rb,
	const uint8_t* ptr,
	uint16_t           length)
{
	uint16_t size;

	El_Assert(rb != NULL);

	/* whether has enough space */
	size = rt_ringbuffer_space_len(rb);

	/* no space */
	if (size == 0)
		return 0;

	/* drop some data */
	if (size < length)
		return 0;//length = size;

	if (rb->buffer_size - rb->write_index > length)
	{
		/* read_index - write_index = empty space */
		El_Memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
		/* this should not cause overflow because there is enough space for
		 * length of data in current mirror */
		rb->write_index += length;
		return length;
	}

	El_Memcpy(&rb->buffer_ptr[rb->write_index],
		&ptr[0],
		rb->buffer_size - rb->write_index);
	El_Memcpy(&rb->buffer_ptr[0],
		&ptr[rb->buffer_size - rb->write_index],
		length - (rb->buffer_size - rb->write_index));

	/* we are going into the other side of the mirror */
	rb->write_mirror = ~rb->write_mirror;
	rb->write_index = length - (rb->buffer_size - rb->write_index);

	return length;
}
//RTM_EXPORT(rt_ringbuffer_put);

/**
 * put a block of data into ring buffer
 *
 * When the buffer is full, it will discard the old data.
 */
uint32_t rt_ringbuffer_put_force(struct rt_ringbuffer* rb,
	const uint8_t* ptr,
	uint16_t           length)
{
	uint16_t space_length;

	El_Assert(rb != NULL);

	space_length = rt_ringbuffer_space_len(rb);

	if (length > rb->buffer_size)
	{
		ptr = &ptr[length - rb->buffer_size];
		length = rb->buffer_size;
	}

	if (rb->buffer_size - rb->write_index > length)
	{
		/* read_index - write_index = empty space */
		El_Memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
		/* this should not cause overflow because there is enough space for
		 * length of data in current mirror */
		rb->write_index += length;

		if (length > space_length)
			rb->read_index = rb->write_index;

		return length;
	}

	El_Memcpy(&rb->buffer_ptr[rb->write_index],
		&ptr[0],
		rb->buffer_size - rb->write_index);
	El_Memcpy(&rb->buffer_ptr[0],
		&ptr[rb->buffer_size - rb->write_index],
		length - (rb->buffer_size - rb->write_index));

	/* we are going into the other side of the mirror */
	rb->write_mirror = ~rb->write_mirror;
	rb->write_index = length - (rb->buffer_size - rb->write_index);

	if (length > space_length)
	{
		rb->read_mirror = ~rb->read_mirror;
		rb->read_index = rb->write_index;
	}

	return length;
}
//RTM_EXPORT(rt_ringbuffer_put_force);

/**
 *  get data from ring buffer
 */
uint32_t rt_ringbuffer_get(struct rt_ringbuffer* rb,
	uint8_t* ptr,
	uint16_t           length)
{
	uint32_t size;

	El_Assert(rb != NULL);

	/* whether has enough data  */
	size = rt_ringbuffer_data_len(rb);

	/* no data */
	if (size == 0)
		return 0;

	/* less data */
	if (size < length)
		length = size;

	if (rb->buffer_size - rb->read_index > length)
	{
		/* copy all of data */
		if (ptr != NULL)
		{
			El_Memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
		}
		/* this should not cause overflow because there is enough space for
		 * length of data in current mirror */
		rb->read_index += length;
		return length;
	}

	if (ptr != NULL)
	{
		El_Memcpy(&ptr[0],
			&rb->buffer_ptr[rb->read_index],
			rb->buffer_size - rb->read_index);
		El_Memcpy(&ptr[rb->buffer_size - rb->read_index],
			&rb->buffer_ptr[0],
			length - (rb->buffer_size - rb->read_index));
	}


	/* we are going into the other side of the mirror */
	rb->read_mirror = ~rb->read_mirror;
	rb->read_index = length - (rb->buffer_size - rb->read_index);

	return length;
}
//RTM_EXPORT(rt_ringbuffer_get);

/**
 * put a character into ring buffer
 */
uint32_t rt_ringbuffer_putchar(struct rt_ringbuffer* rb, const uint8_t ch)
{
	El_Assert(rb != NULL);

	/* whether has enough space */
	if (!rt_ringbuffer_space_len(rb))
		return 0;

	rb->buffer_ptr[rb->write_index] = ch;

	/* flip mirror */
	if (rb->write_index == rb->buffer_size - 1)
	{
		rb->write_mirror = ~rb->write_mirror;
		rb->write_index = 0;
	}
	else
	{
		rb->write_index++;
	}

	return 1;
}
//RTM_EXPORT(rt_ringbuffer_putchar);

/**
 * put a character into ring buffer
 *
 * When the buffer is full, it will discard one old data.
 */
uint32_t rt_ringbuffer_putchar_force(struct rt_ringbuffer* rb, const uint8_t ch)
{
	enum rt_ringbuffer_state old_state;

	El_Assert(rb != NULL);

	old_state = rt_ringbuffer_status(rb);

	rb->buffer_ptr[rb->write_index] = ch;

	/* flip mirror */
	if (rb->write_index == rb->buffer_size - 1)
	{
		rb->write_mirror = ~rb->write_mirror;
		rb->write_index = 0;
		if (old_state == RT_RINGBUFFER_FULL)
		{
			rb->read_mirror = ~rb->read_mirror;
			rb->read_index = rb->write_index;
		}
	}
	else
	{
		rb->write_index++;
		if (old_state == RT_RINGBUFFER_FULL)
			rb->read_index = rb->write_index;
	}

	return 1;
}
//RTM_EXPORT(rt_ringbuffer_putchar_force);

/**
 * get a character from a ringbuffer
 */
uint32_t rt_ringbuffer_getchar(struct rt_ringbuffer* rb, uint8_t* ch)
{
	El_Assert(rb != NULL);

	/* ringbuffer is empty */
	if (!rt_ringbuffer_data_len(rb))
		return 0;

	/* put character */
	if (ch != NULL)
	{
		*ch = rb->buffer_ptr[rb->read_index];
	}
	if (rb->read_index == rb->buffer_size - 1)
	{
		rb->read_mirror = ~rb->read_mirror;
		rb->read_index = 0;
	}
	else
	{
		rb->read_index++;
	}
	return 1;
}
uint32_t rt_ringbuffer_viewchar(struct rt_ringbuffer* rb, uint8_t* ch, uint16_t offset)
{
	El_Assert(rb != NULL);

	/* ringbuffer is empty */
	if (!rt_ringbuffer_data_len(rb))
		return 0;

	/* put character */
	if (ch != NULL)
	{
		*ch = rb->buffer_ptr[rb->read_index + offset];
	}
	return 1;
}
//RTM_EXPORT(rt_ringbuffer_getchar);

/**
 * get the size of data in rb
 */
uint32_t rt_ringbuffer_data_len(struct rt_ringbuffer* rb)
{
	switch (rt_ringbuffer_status(rb))
	{
	case RT_RINGBUFFER_EMPTY:
		return 0;
	case RT_RINGBUFFER_FULL:
		return rb->buffer_size;
	case RT_RINGBUFFER_HALFFULL:
	default:
		if (rb->write_index > rb->read_index)
			return rb->write_index - rb->read_index;
		else
			return rb->buffer_size - (rb->read_index - rb->write_index);
	};
}
//RTM_EXPORT(rt_ringbuffer_data_len);

/**
 * empty the rb
 */
void rt_ringbuffer_reset(struct rt_ringbuffer* rb)
{
	El_Assert(rb != NULL);

	rb->read_mirror = 0;
	rb->read_index = 0;
	rb->write_mirror = 0;
	rb->write_index = 0;
}

