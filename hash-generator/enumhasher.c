/**
 * Generate the hash table enum for os-release file parsing.
 *
 * @author longptr
 * @license MIT
 * @version 1.0.0
 */
#include <stdio.h>

/**
 * @brief The format string of the enum, entries may be appended to the end.
 */
const char *ENUM_FORMAT = "enum parser_hashed_keys_t {\n\tNAME = %u,\n\tID = %u,\n\tID_LIKE = %u,\n\tVERSION = %u,\n\tVERSION_ID = %u,\n\tCPE_NAME = %u,\n\tBUILD_ID = %u,\n\tPRETTY_NAME = %u,\n\tANSI_COLOR = %u,\n\tREDHAT_BUGZILLA_PRODUCT = %u,\n\tREDHAT_BUGZILLA_PRODUCT_VERSION = %u,\n\tREDHAT_SUPPORT_PRODUCT = %u,\n\tREDHAT_SUPPORT_PRODUCT_VERSION = %u,\n\tHOME_URL = %u,\n\tDOCUMENTATION_URL = %u,\n\tSUPPORT_URL = %u,\n\tBUG_REPORT_URL = %u,\n\tPRIVACY_POLICY_URL = %u,\n\tLOGO = %u,\n\tVARIANT = %u,\n\tVARIANT_ID = %u\n};\n";

/**
 * @brief A simple implementation of the sdbm hash function.
 * @param d The c string to hash.
 * @return The hash representated as an unsigned 32bit integer.
 */
unsigned int
hash(const char *d)
{
    unsigned int i = 0;

    while (*d)
    {
        i = *d++ + (i << 6) + (i << 16) - i;
    }


    return i;
}

/**
 * @brief The main entry point.
 * @return Always returns zero.
 */
int
main(void)
{
    printf(ENUM_FORMAT, hash("NAME="), hash("ID="), hash("ID_LIKE="), hash("VERSION="), hash("VERSION_ID="), hash("CPE_NAME="),
           hash("BUILD_ID="), hash("PRETTY_NAME="), hash("ANSI_COLOR="), hash("REDHAT_BUGZILLA_PRODUCT="), hash("REDHAT_BUGZILLA_PRODUCT_VERSION="),
           hash("REDHAT_SUPPORT_PRODUCT="), hash("REDHAT_SUPPORT_PRODUCT_VERSION="), hash("HOME_URL="), hash("DOCUMENTATION_URL="),
           hash("SUPPORT_URL="),  hash("BUG_REPORT_URL="), hash("PRIVACY_POLICY_URL="), hash("LOGO="), hash("VARIANT="), hash("VARIANT_ID="));

    return 0;
}

