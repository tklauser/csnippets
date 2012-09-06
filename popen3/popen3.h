#ifndef _POPEN3_H
#define _POPEN3_H

extern pid_t popen3(const char *cmd, int *pipes);
extern int pclose3(pid_t pid, int *pipes);

#endif /* _POPEN3_H */
