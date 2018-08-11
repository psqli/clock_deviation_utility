/*
 * clock deviation utility
 * Copyright (C) 2018  Ricardo Biehl Pasquali
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * 28/03/2018
 */

#ifndef SYNC_DEVIATION_HISTORY_H
#define SYNC_DEVIATION_HISTORY_H

/*
 * single ring buffer
 * ==================
 */

struct single_ring_buffer {
	int is_full;
	unsigned int current;
	unsigned int size;
};

#define single_ring_buffer_reset(b) \
do { \
	(b)->is_full = 0; \
	(b)->current = 0; \
} while (0)

//#define single_ring_buffer_get_last(b) ((b)->is_full ? (b)->current : 0)

static inline void
single_ring_buffer_update(struct single_ring_buffer *b)
{
	b->current++;
	if (b->current == b->size) {
		b->current = 0;

		/*
		 * once the buffer gets full it is always
		 * full until be reset
		 */
		if (!b->is_full)
			b->is_full = 1;
	}
}

/* ---------------------------------------- */

/*
 * deviation history
 * =================
 */

/* ring buffer of time deviations that overwrites */
struct deviation_history {
	long *buffer;
	struct single_ring_buffer ring_buffer;
};

#define deviation_history_get_last(h) \
        ((h)->ring_buffer.is_full \
         ? (h)->buffer[(h)->ring_buffer.current] \
         : 0)

/*
 * register the new deviation and update
 * ring buffer indexes
 */
#define deviation_history_insert(h, deviation)  do { \
	(h)->buffer[(h)->ring_buffer.current] = deviation; \
	single_ring_buffer_update(&(h)->ring_buffer); \
} while (0)

#define deviation_history_get_size(h) \
        (h)->ring_buffer.size

#define deviation_history_reset_ring_buffer(h) \
	single_ring_buffer_reset(&(h)->ring_buffer)

#define deviation_history_reset(h, history_addr, history_size)  do { \
	(h)->buffer = history_addr; \
	(h)->ring_buffer.size = history_size; \
	deviation_history_reset_ring_buffer(h); \
} while (0)

#endif /* SYNC_DEVIATION_HISTORY_H */
