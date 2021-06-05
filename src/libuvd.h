#ifndef LIBUVD_H
#define LIBUVD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/utsname.h>

#define OS_RELEASE "/etc/os-release"

struct os_release_t
{
    char *name;
    char *id;
    char *id_like;
    char *version;
    char *version_id;
    char *cpe_name;
    char *build_id;
    char *pretty_name;
    char *ansi_color;
    char *home_url;
    char *documentation_url;
    char *support_url;
    char *bug_report_url;
    char *privacy_policy_url;
    char *logo;
    char *variant;
    char *variant_id;
    char *redhat_bugzilla;
    char *redhat_bugzilla_version;
    char *redhat_support;
    char *redhat_support_version;
};

struct unix_version_t
{
    struct os_release_t *os_release;
    struct utsname *uname_information;
};

enum parser_hashed_keys_t {
    NAME = 2825891282,
    ID = 607550658,
    ID_LIKE = 834434786,
    VERSION = 218387749,
    VERSION_ID = 3303394491,
    CPE_NAME = 3518911211,
    BUILD_ID = 2884964881,
    PRETTY_NAME = 3758580745,
    ANSI_COLOR = 995869526,
    REDHAT_BUGZILLA_PRODUCT = 849262906,
    REDHAT_BUGZILLA_PRODUCT_VERSION = 4214314625,
    REDHAT_SUPPORT_PRODUCT = 3750338035,
    REDHAT_SUPPORT_PRODUCT_VERSION = 3471739194,
    HOME_URL = 3871004686,
    DOCUMENTATION_URL = 2574235251,
    SUPPORT_URL = 3066209982,
    BUG_REPORT_URL = 323944174,
    PRIVACY_POLICY_URL = 2587417252,
    LOGO = 2836135858,
    VARIANT = 2557366552,
    VARIANT_ID = 1893742344
};

enum parser_token_t
{
    TOKEN_NEWLINE = 0x0A,
    TOKEN_EQUAL = 0x3D,
    TOKEN_QUOTE = 0x22,
    TOKEN_NULL = 0x00
};

enum parser_phase_t
{
    PARSE_KEY = 1 << 2,
    PARSE_VALUE = 1 << 4
};

/**
 * @brief Attempts to get the version of the current unix operating system.
 *     This routine will attempt to parse the "os-release" and call the "uname"
 *     routine, if either fail a null pointer will be returned.
 *
 * @return The unix_version_t struct containing parsed information,
 *     otherwise this routine can return a null pointer.
 */
extern struct unix_version_t *get_unix_information(void);

/**
 * @brief Attempts to free any and all memory allocated for the version detection.
 *
 * @param The unix_version_t struct to free allocated memory from.
 */
extern void free_unix_information(struct unix_version_t *p);

#ifdef __cplusplus
}
#endif
#endif
