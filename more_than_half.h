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

#ifndef MORE_THAN_HALF_H
#define MORE_THAN_HALF_H

#include "deviation_history.h"

struct sync {
	/* history of the last N deviations */
	struct deviation_history deviation_history;

	unsigned int half_history_size;

	/* number of out-of-sync elements in history */
	unsigned int misses;
	/* sum of the deviations of out-of-sync elements in history */
	int64_t out_of_sync_sum;
	/* total sum of the deviations */
	int64_t total_sum;

	/*
	 * We know network latency is rather unstable
	 * and Linux doesn't guarantee hard real-time.
	 *
	 * A deviation up to max_deviation is assumed
	 * to be synchronized.
	 *
	 * NOTE: max_deviation is in nanoseconds and
	 * it's subject to the limit of long type
	 * (LONG_MAX)
	 */
	unsigned long max_deviation;
};

long
more_than_half_do_sync(struct sync *s, long deviation);

int
more_than_half_reset(struct sync *s, void *history_addr,
                     unsigned int history_size,
                     unsigned long allowed_deviation);

#endif /* MORE_THAN_HALF_H */
