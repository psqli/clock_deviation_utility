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
 * 18/07/2018
 *
 * Smooth synchronization
 *
 * Smooth sync is used to distribute a correction among
 * regularly runs of an application.
 */

#ifndef SMOOTH_SYNC_H
#define SMOOTH_SYNC_H

/*
 * The correction is divided by 'count' and stored in
 * 'sum' ('count' is defined by application). Then, the
 * value of 'sum' is applied 'count' runs.
 *
 * 'skip' defines the number of runs to skip before the
 * next 'sum' is applied. 'skip_step' just keep track of
 * the skipped runs.
 */

struct smooth_sync {
	/* correction = count * sum */
	unsigned long count;
	/* value to add for every run */
	int sum;

	/* runs to skip */
	int skip;
	/* counter to keep track of the runs to skip */
	int skip_step;
};

static inline void
smooth_sync_reset(struct smooth_sync *s)
{
	s->count = 0;
}

static void
smooth_sync_start(struct smooth_sync *s, long deviation,
                  unsigned long elapsed_runs)
{
	long ratio;

	s->count = labs(deviation);
	s->sum = deviation > 0 ? 1 : -1;
	s->skip = 0;

	/*
	 * check the deviation ratio
	 *
	 * Elapsed runs per one deviated.
	 */
	ratio = elapsed_runs / labs(deviation);

	if (ratio < 2) {
		/* TODO: it's a debug */
		printf("smooth sync: WARNING: deviation ratio too low\n");

		/* amount deviated per run */
		ratio = labs(deviation) / elapsed_runs;
		s->sum = deviation > 0 ? ratio : -ratio;
		s->count = elapsed_runs;
	} else {
		/*
		 * make the smooth synchronization finish
		 * its job twice as fast as the application
		 * tooks to get out of sync
		 */
		s->skip = ratio / 2 - 1;
	}

	s->skip_step = 0;
}

static inline int
smooth_sync(struct smooth_sync *s)
{
	int diff;

	if (!s->count)
		return 0;

	if (!s->skip) {
		diff = s->sum;
		/* TODO: remove debug */
		printf("smooth sync: %d\n", diff);
		s->count--;
	} else {
		if (!s->skip_step--) {
			diff = s->sum;
			/* TODO: remove debug */
			printf("smooth sync: %d\n", diff);
			s->count--;
			s->skip_step = s->skip;
		} else {
			diff = 0;
		}
	}

	return diff;
}

#endif /* SMOOTH_SYNC_H */
