#include "waitdisks.h"

bool has_dev_name(char *str)
{
	/* if a device name begins with a single slash
	   we assume it's a local device node */
	return str[0] == '/' && str[1] != '/';
}

void init_watchlist(struct waitdisks_watchlist *wl)
{
	wl->n_files = 0;
	wl->max_files = 64;
	wl->files = malloc(sizeof(struct waitdisks_watchfile) * wl->max_files);
	if(wl->files == NULL) {
		BUGOUT("could not allocate watchlist\n");
		exit(255);
	}
}

void add_to_watchlist(struct waitdisks_watchlist *wl, const char *filename)
{
	size_t i, j;
	wl->n_files++;
	if(wl->n_files >= wl->max_files)
	{
		wl->max_files *= 2;
		wl->files = realloc(wl->files,sizeof(struct waitdisks_watchfile)
				    * wl->max_files);
		if(wl->files == NULL) {
			BUGOUT("could not allocate watchlist\n");
			exit(255);
		}
	}
	strncpy(wl->files[wl->n_files - 1].path,
		filename,
		PATH_MAX);
	wl->files[wl->n_files - 1].path[PATH_MAX - 1] = '\0';
	wl->files[wl->n_files - 1].found = 0;
}

void process_watchlist(struct waitdisks_watchlist *wl)
{
	size_t i;
	for(i=0;i<wl->n_files;i++)
	{
		if(!access(wl->files[i].path, F_OK))
		{
			wl->files[i].found = 1;
		}
	}
	usleep(250000);
}

bool watchlist_all_found(struct waitdisks_watchlist *wl)
{
	size_t i;
	for(i=0;i<wl->n_files;i++)
	{
		if(!wl->files[i].found)
		{
			return 0;
		}
	}
	return 1;
}
