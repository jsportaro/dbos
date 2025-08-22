#ifndef __OS_PATHPARSER_H__
#define __OS_PATHPARSER_H__

#define MAX_PATH_LENGTH 108

typedef struct PathPart PathPart;

struct PathPart
{
    const char *part;
    PathPart *next;
};

typedef struct PathRoot
{
    int driveNumber;
    PathPart *first;
} PathRoot;


PathRoot *ParsePath(const char *path, const char *current_directory_path);
void PathPartFree(PathRoot *root);

#endif