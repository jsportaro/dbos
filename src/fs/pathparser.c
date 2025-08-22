#include <fs/pathparser.h>
#include <memory/heap/kheap.h>
#include <memory/memory.h>
#include <memory/string.h>
#include <status.h>

static int PathValidFormat(const char *filename)
{
    int len = strnlen(filename, MAX_PATH_LENGTH);

    return (len >= 3 && IsDigit(filename[0]) && memcmp((void *)&filename[1], ":/", 2) == 0);
}

static int PathGetDriveByPath(const char **path)
{
    if (!PathValidFormat(*path))
    {
        return -EBADPATH;
    }

    int driveNumber = ToNumericDigit(*path[0]);

    //  Add 3 bytes to skip drive number
    *path += 3;

    return driveNumber;
}

static PathRoot *CreateRoot(int driveNumber)
{
    PathRoot *pathRoot = KZAlloc(sizeof(PathRoot));

    pathRoot->driveNumber = driveNumber;
    pathRoot->first = NULL;

    return pathRoot;
}

static const char *GetPathPart(const char **path)
{
    char *resultPathPart = KZAlloc(MAX_PATH_LENGTH);
    int i = 0;

    while (**path != '/' && **path != 0)
    {
        resultPathPart[i] = **path;
        *path += 1;
        i++;
    }

    if (**path == '/')
    {
        *path += 1;
    }

    if (i == 0)
    {
        KFree(resultPathPart);
        resultPathPart = NULL;
    }

    return resultPathPart;
}

static PathPart *ParsePathPart(PathPart *last, const char **path)
{
    const char *pathPartString = GetPathPart(path);

    if (!pathPartString)
    {
        return NULL;
    }

    PathPart *part = KZAlloc(sizeof(PathPart));

    part->part = pathPartString;
    part->next = NULL;

    if (last)
    {
        last->next = part;
    }

    return part;
}

PathRoot *ParsePath(const char *path, const char *current_directory_path)
{
    int result = 0;

    const char *tempPath = path;
    PathRoot *root = NULL;

    if (strlen(path) > MAX_PATH_LENGTH)
    {
        goto out;
    }

    result = PathGetDriveByPath(&tempPath);
    if (result < 0)
    {
        goto out;
    }

    root = CreateRoot(result);
    if (root == NULL)
    {
        goto out;
    }

    PathPart *firstPart = ParsePathPart(NULL, &tempPath);
    if (!firstPart)
    {
        goto out;
    }

    root->first = firstPart;

    PathPart *part = ParsePathPart(firstPart, &tempPath);

    while (part)
    {
        part = ParsePathPart(part, &tempPath);
    }

out:
    return root;
}

void PathPartFree(PathRoot *root)
{
    PathPart *part = root->first;

    while (part)
    {
        PathPart *next = part->next;
        KFree((void *)part->part);
        KFree(part);
        part = next;
    }

    KFree(root);
}