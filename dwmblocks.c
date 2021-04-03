#include <X11/Xlib.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIGPLUS SIGRTMIN
#define SIGMINUS SIGRTMIN
#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
#define CMDLENGTH 50
#define MIN(a, b) ((a < b) ? a : b)
#define STATUSLENGTH (LENGTH(blocks) * CMDLENGTH + 1)

typedef struct {
  char *command;
  unsigned int interval;
  unsigned int signal;
} Block;

void sighandler(int num);
void buttonhandler(int sig, siginfo_t *si, void *ucontext);
void getcmds(int time);
void getsigcmds(unsigned int signal);
void setupsignals();
void sighandler(int signum);
int getstatus(char *str, char *last);
void statusloop();
void termhandler();
void pstdout();
void setroot();
static void (*writestatus)() = setroot;
static int setupX();

static Display *dpy;
static int screen;
static Window root;

#include "config.h"

static char statusbar[LENGTH(blocks)][CMDLENGTH] = {0};
static char statusstr[2][STATUSLENGTH];
static char button[] = "\0";
static int statusContinue = 1;
static int returnStatus = 0;

/* opens process *cmd and stores output in *output */
void getcmd(const Block *block, char *output) {
  if (block->signal) {
    output[0] = block->signal;
    output++;
  }

  char *cmd = block->command;
  FILE *cmdf;
  if (*button) {
    setenv("BUTTON", button, 1);
    cmdf = popen(cmd, "r");
    *button = '\0';
    unsetenv("BUTTON");
  } else
    cmdf = popen(cmd, "r");

  if (!cmdf)
    return;

  fgets(output, CMDLENGTH - delimLen, cmdf);
  int i = strlen(output);
  if (i == 0) {
    /* return if block and command output are both empty */
    pclose(cmdf);
    return;
  }

  if (delim[0] != '\0') {
    /* only chop off newline if one is present at the end */
    i = output[i - 1] == '\n' ? i - 1 : i;
    strncpy(output + i, delim, delimLen);
  } else
    output[i++] = '\0';
  pclose(cmdf);
}

void getcmds(int time) {
  const Block *current;
  for (unsigned int i = 0; i < LENGTH(blocks); i++) {
    current = blocks + i;
    if ((current->interval != 0 && time % current->interval == 0) || time == -1)
      getcmd(current, statusbar[i]);
  }
}

void getsigcmds(unsigned int signal) {
  const Block *current;
  for (unsigned int i = 0; i < LENGTH(blocks); i++) {
    current = blocks + i;
    if (current->signal == signal)
      getcmd(current, statusbar[i]);
  }
}

void setupsignals() {

  struct sigaction sa;
  for (unsigned int i = 0; i < LENGTH(blocks); i++) {
    if (blocks[i].signal > 0) {

      signal(SIGMINUS + blocks[i].signal, sighandler);
      sigaddset(&sa.sa_mask,
                SIGRTMIN +
                    blocks[i].signal); /* ignore signal when handling SIGUSR1 */
    }
  }

  sa.sa_sigaction = buttonhandler;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &sa, NULL);
}

int getstatus(char *str, char *last) {
  strcpy(last, str);
  str[0] = '\0';

  for (unsigned int i = 0; i < LENGTH(blocks); i++)
    strcat(str, statusbar[i]);

  str[strlen(str) - strlen(delim)] = '\0';
  return strcmp(str, last); /* 0 if they are the same */
}

void setroot() {
  if (!getstatus(statusstr[0],
                 statusstr[1])) /* Only set root if text has changed. */
    return;
  XStoreName(dpy, root, statusstr[0]);
  XFlush(dpy);
}

int setupX() {
  dpy = XOpenDisplay(NULL);
  if (!dpy) {
    fprintf(stderr, "dwmblocks: Failed to open display\n");
    return 0;
  }
  screen = DefaultScreen(dpy);
  root = RootWindow(dpy, screen);
  return 1;
}

void buttonhandler(int sig, siginfo_t *si, void *ucontext) {
  *button = '0' + si->si_value.sival_int & 0xff;
  getsigcmds(si->si_value.sival_int >> 8);
  writestatus();
}

void pstdout() {
  if (!getstatus(statusstr[0],
                 statusstr[1])) /* Only write out if text has changed. */
    return;
  printf("%s\n", statusstr[0]);
  fflush(stdout);
}

void statusloop() {
  setupsignals();
  int i = 0;
  getcmds(-1);
  while (1) {
    getcmds(i++);
    writestatus();
    if (!statusContinue)
      break;
    sleep(1.0);
  }
}

void sighandler(int signum) {
  getsigcmds(signum - SIGPLUS);
  writestatus();
}

void termhandler() { statusContinue = 0; }

int main(int argc, char **argv) {
  for (int i = 0; i < argc; i++) { /* Handle command line arguments */
    if (!strcmp("-d", argv[i]))
      strncpy(delim, argv[++i], delimLen);
    else if (!strcmp("-p", argv[i]))
      writestatus = pstdout;
  }
  if (!setupX())
    return 1;
  delimLen = MIN(delimLen, strlen(delim));
  delim[delimLen++] = '\0';

  signal(SIGTERM, termhandler);
  signal(SIGINT, termhandler);
  statusloop();
  XCloseDisplay(dpy);
  return 0;
}
