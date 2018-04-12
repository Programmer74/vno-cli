#ifndef PATHS_H
#define PATHS_H

#define WORKING_DIR         "."
#define REPO_DIR            "./.vno"

#define SETTINGS_DIR        ".vno"
#define SETTINGS_FILE       SETTINGS_DIR "/settings"

#define LAST_COMMIT_ID_FILE REPO_DIR "/head"
#define CUR_BRANCH_ID_FILE  REPO_DIR "/current_branch"
#define TMP_COMMIT_FILE     REPO_DIR "/commits/tmpcommit"

#define BRANCHES_DIR        REPO_DIR "/branches/"
#define COMMITS_DIR         REPO_DIR "/commits/"
#define BLOBS_DIR           REPO_DIR "/blobs/"

#define SRV_ADDRESS         "localhost:8080"

#endif // PATHS_H
