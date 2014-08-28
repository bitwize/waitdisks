#include "waitdisks.h"

void usage();

int main(int argc, char **argv)
{
	FILE *fstab;
	int opt;
	char fn[PATH_MAX] = "/etc/fstab";
	struct mntent *ment;
	int timeout_sec = 30;
	struct waitdisks_watchlist wlist;
	struct timeval tv_start, tv_now;
	while((opt=getopt(argc, argv, "f:")) != -1)
	{
		switch(opt)
		{
		case 'f':
			strncpy(fn, optarg, PATH_MAX);
			fn[PATH_MAX - 1] = '\0';
			break;
		case 't':
			timeout_sec = atoi(optarg);
		case 'h':
		case '?':
			usage();
			exit(255);
			break;
		}		
	}
	init_watchlist(&wlist);
	fstab = fopen(fn, "r");
	gettimeofday(&tv_start, NULL);
	if(fstab == NULL) {
		BUGOUT("cannot open fstab file: %s, errno %d, exiting!\n",
		       fn,
		       errno);
		exit(255);
	}

	/* Read the fstab file one line at a time, see if it
	   references a device node we're interested in, and if so add
	   it to the watch list. */
	
	while((ment=getmntent(fstab)) != NULL)
	{
		
		if(has_dev_name(ment->mnt_fsname) &&
		   !hasmntopt(ment, MNTOPT_NOAUTO))

		{
			add_to_watchlist(&wlist, ment->mnt_fsname);
		}
	}

	/* Go down the watch list and see which device nodes have
	   appeared. If all have appeared, exit with success; if
	   timeout_sec seconds have gone by or an error occurs, exit
	   with failure. */

	while(!watchlist_all_found(&wlist))
	{
		process_watchlist(&wlist);
		gettimeofday(&tv_now, NULL);
		if(tv_now.tv_sec - tv_start.tv_sec >= timeout_sec)
		{
			exit(1);
		}		
	}
	exit(0);
}

void usage()
{
	printf("Usage: waitdisks [ -f fstabname ]  [ -t timeout_secs ]\n\n");
}
