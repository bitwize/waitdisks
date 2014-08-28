#ifndef __WAITDISKS_H
#define __WAITDISKS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <mntent.h>

#ifdef LJBAD_DEBUG
#define BUGOUT(...) fprintf(stderr, ...)
#else
#define BUGOUT(...)
#endif


struct waitdisks_watchfile
{
	char path[PATH_MAX];
	bool found;
};

struct waitdisks_watchlist
{
	size_t n_files;
	size_t max_files;
	struct waitdisks_watchfile *files;
};


bool has_dev_name(char *str);
void init_watchlist(struct waitdisks_watchlist *wl);
void add_to_watchlist(struct waitdisks_watchlist *wl, const char *filename);
void process_watchlist(struct waitdisks_watchlist *wl);
bool watchlist_all_found(struct waitdisks_watchlist *wl);

#endif
