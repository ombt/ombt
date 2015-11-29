/**
 *  notify.c - testing the Linux inotify
 *
 *  Copyright (c) 2008 by Ido Kanner - LINESIP <idok  @ a t @  linesip . com>
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* {{{ */
/* {{{ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <libgen.h>
/* }}} */

/* {{{ */
#define VERSION ("0.1b")
/** the flags we require and wish to monitor when a file has issued a message of some sort */
#define FLAGS (IN_CLOSE_WRITE | IN_MODIFY)
/** size of the event structure, not counting name */
#define EVENT_SIZE  (sizeof (struct inotify_event))
/** reasonable guess as to size of 1024 events */
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))
/** the length of the action */
#define ACTION_LENGTH (50+FILENAME_MAX)

/** the linked list of the path we wish to listen to */
struct file_record
{
  /** the file/path name */
  char pathname[FILENAME_MAX];
  /** the file descriptor of the file that is being listened */
  int wd;
  /** the flags we should listen to with the current file */
  uint32_t listen_to;
  /** the next record */
  struct file_record *next;
};

/** the record that stores information regarding inotify and the linked list */
struct file_list
{
   /** the file descriptor of inotify */
   int fd;
   /** the linked list for each requested files */
   struct file_record *files;
} ;

typedef struct file_record filerecord;
typedef struct file_list filelist;
/* }}} */

/* {{{ */
/* {{{ */
/**
 * Register a file to the inotify events
 * 
 * @param list      - a pointer to a file_list that contain the linked list and the file descriptor of inotify
 * @param pathname  - a null zero string with the path/file name to monitor
 * @param flags     - the flags to tell inotify what to monitor
 *
 * @return 1 if the operation was successful, or 0 if not
 */
unsigned char register_file(struct file_list *list, const char *pathname, uint32_t flags) {
  filerecord *file_rec = (filerecord *)malloc(sizeof(filerecord));
  
  int length          = strlen(pathname) +1;
  memset(file_rec->pathname, 0, FILENAME_MAX);
  strncpy(file_rec->pathname, pathname, length);
  file_rec->listen_to = flags;
  file_rec->next      = list->files;
  file_rec->wd        = inotify_add_watch (list->fd, pathname, flags);

  if (file_rec->wd < 0)
  {
    free(file_rec);
    perror("inotify_add_watch");
    return 0;
  }

  list->files = file_rec;
  
  return 1;
} /* }}} */

/* {{{ */
/**
 * Remove the first file at list from the inotify and then free the file from the list
 *
 * @param list - a pointer to the linked list
 */ 
void unregister_file(struct file_list *list)
{
  filerecord *next_file_rec, *cur_file_rec;
  if (NULL == list->files)
  {
     perror("list->files is null");
     return;
  }

  cur_file_rec = list->files;
  if (NULL != list->files->next)
    next_file_rec = list->files->next;
  else 
    next_file_rec = NULL;

  if (inotify_rm_watch(list->fd, cur_file_rec->wd) < 0)
  {
      perror("inotify_rm_watch");
      return;
  }

  list->files = next_file_rec;

  free(cur_file_rec);
} /* }}} */

/* {{{ */
/**
 * Initialize the file_list with the inotify 
 *
 * @return NULL if there was a problem, or an allocated file_list based struct
 */
struct file_list * init_list(void)
{
   int fd         = inotify_init();
   if (fd < 0)
   {
     perror("inotify_init");
     return NULL;
   }

   filelist *list = (filelist *) malloc(sizeof(filelist));
   list->fd       = fd;
   list->files    = NULL;
   return list;
} /* }}} */

/* {{{ */
/**
 * Free the memory of the file list 
 *
 * @param list - the list of the files
 *
 * @return 1 if freeing of the list was successful 
 */
unsigned char free_list(filelist *list)
{
  if (NULL == list)
  {
    perror("list is null");
    return 0;
  }

  while (NULL != list->files)
    unregister_file(list);

  free(list);
  return 1;
} /* }}} */

/* {{{ */
/**
 * Check if the given path exists or not
 *
 * @param path - The given path to check
 *
 * @return 1 if the path is exists or 0 other wise
 */
unsigned char path_exists(const char *path)
{
   struct stat buf;
   return (stat(path, &buf) == 0 || errno != ENOENT);
}  /* }}} */

/* {{{ */
/**
 * Populate the list with the command line path that was given
 *
 * @param list - a basic initialized list
 * @param argc - the number of arguments from the command line
 * @param argv - the array of chars with the command line
 */
void populate_list(filelist *list, int argc, char *argv[])
{
   int counter;
   for (counter = 1; counter < argc; counter++)
   {
      char path[FILENAME_MAX]; 
      memset(&path, 0, FILENAME_MAX);
      strncpy(path, argv[counter], FILENAME_MAX - 1);
      if (path_exists(path))
      {
        if (! register_file(list, path, FLAGS))
        {
           perror("register_file");
        } /* if (! register_file(list, path, FLAGS)) */
      } /* if (path_exists(path)) */
      else
        fprintf(stderr,"The path \"%s\" does not exists.\n", path);
   } /* for (counter = 0; counter < argc; counter++) */
} /* }}} */

/* {{{ */
/**
 * Try to find the path descriptor at the file list 
 *
 * @param list - the file list to search the given file 
 * @param wd - the path file descriptor given by inotify 
 *
 * @return the path name or \0 if not found
 */
char * find_path_name(struct file_list *list, int wd)
{
  /* save starting position */
  filerecord *filepos = list->files;
  char * name = (char *) malloc(FILENAME_MAX);
  memset(name, 0, FILENAME_MAX);

  while (NULL != filepos)
  {
     if (wd == filepos->wd)
     {
       strncpy(name, filepos->pathname, FILENAME_MAX -1);
       break;
     }
     filepos = filepos->next;
  } /* while (filepos) */

  if (strlen(name) > 0)
    return name;

  return (char*)"\0";
} /* }}} */

/* {{{ */
/**
 * listen to inotify events 
 *
 * @param list - A list of the files we are listen to
 *
 */
void listen(struct file_list *list)
{
  ssize_t len, i           = 0;
  char buff[EVENT_BUF_LEN] = {0};

  len = read (list->fd, buff, EVENT_BUF_LEN -1);
   
  while (i < len)
  {
    struct inotify_event *pevent    = (struct inotify_event *) & buff[i];
    char action[ACTION_LENGTH]      = {0};
    char *pathname                  = find_path_name(list, pevent->wd);

    if (pevent->len)
      strncpy (action, pevent->name, ACTION_LENGTH -1);
    else
      strncpy(action, pathname, FILENAME_MAX -1);

    free(pathname);
    
    if (pevent->mask & IN_ACCESS)
      strncat(action, " was read", ACTION_LENGTH -1);
    if (pevent->mask & IN_ATTRIB)
      strncat(action, " Metadata changed", ACTION_LENGTH -1);
    if (pevent->mask & IN_CLOSE_WRITE)
      strncat(action, " was opened for writing and is now closed", ACTION_LENGTH -1);
    if (pevent->mask & IN_CLOSE_NOWRITE)
      strncat(action, " was not opened for writing and is now closed", ACTION_LENGTH -1);
    if (pevent->mask & IN_CREATE)
      strncat(action, " created in watched directory", ACTION_LENGTH -1);
    if (pevent->mask & IN_DELETE)
      strncat(action, " deleted from watched directory", ACTION_LENGTH -1);
    if (pevent->mask & IN_DELETE_SELF)
      strncat(action, "Watched file/directory was itself deleted", ACTION_LENGTH -1);
    if (pevent->mask & IN_MODIFY)
      strncat(action, " was modified", ACTION_LENGTH -1);
    if (pevent->mask & IN_MOVE_SELF)
      strncat(action, "Watched file/directory was itself moved", ACTION_LENGTH -1);
    if (pevent->mask & IN_MOVED_FROM)
      strncat(action, " moved out of watched directory", ACTION_LENGTH -1);
    if (pevent->mask & IN_MOVED_TO)
      strncat(action, " moved into watched directory", ACTION_LENGTH -1);
    if (pevent->mask & IN_OPEN)
      strncat(action, " was opened", ACTION_LENGTH -1);
    
    printf ("Action: [%s]\n\twd=%d mask=%d cookie=%d len=%d dir=%s ", 
            action, pevent->wd, pevent->mask, pevent->cookie, pevent->len, (pevent->mask & IN_ISDIR) ? "yes":"no");

    if (pevent->len)
      printf ("name=%s\n", pevent->name);
    else 
      printf("\n");

    i += sizeof(struct inotify_event) + pevent->len;
    
    printf("\n");
  }
} /* }}} */

/* {{{ */
/**
 * Display how to use the program
 *
 * @param program_name - the name that should be displaced as the program name
 */
void print_help(const char program_name[])
{
  printf("Usage: %s <path> [path path path ...]\n", program_name);
} /* }}} */
/* }}} */ 

/** a global variable to be used by signal trapping to determine if the program should stop running */
int to_run = 1;

/* {{{ */
/**
 * Capture the ^C signal and tell the program to stop the main loop
 */
void sigbreak(int)
{
  signal(SIGINT, SIG_IGN);
  to_run = 0;
}  /* }}} */

/* {{{ */
int main(int argc, char *argv[])
{
   printf ("notify  Copyright (C) 2008  Ido Kanner LINESIP\n");
   printf ("This program comes with ABSOLUTELY NO WARRANTY; for details read the license.\n");
   printf ("This is free software, and you are welcome to redistribute it\n");
   printf ("under certain conditions; read the license for details.\n\n");

   printf ("notify testing version %s\n", VERSION);
   if (2 > argc)
   {
      print_help(basename(argv[0]));
      return 1;
   }

   filelist *list = init_list();
   if (NULL == list)
     return -errno;
   
   populate_list(list, argc, argv);

   signal(SIGINT, sigbreak);
   printf("Press on ^C to stop the program from running\n");

   while (1 == to_run)
   {
      listen (list);
   }

   return free_list(list);
}
/* }}} */
/* }}} */
