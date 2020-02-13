/* 
 * File:   logfile.h
 * Author: Tasos198
 *
 * Created on April 23, 2017, 2:39 PM
 */
#ifndef LOGFILE_H
#define LOGFILE_H

#include <sys/stat.h>

#define LOGFILE "/tmp/csd3166_logfile.txt"

/*Define function*/
void initLogFile(void);

void appendLogFile(uid_t userID, const char* filePath, int openStatus, int actionStatus);

unsigned char* calculateHash(const char* filePath);

char* getFilePath(int fp);

#endif /* LOGFILE_H */

