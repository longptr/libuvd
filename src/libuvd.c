#include "libuvd.h"

static inline void
buffer_reassignment(char *buffer, struct os_release_t *release_info, unsigned int *hash)
{
    switch (*hash)
    {
        case NAME:
            release_info->name = buffer;
            break;
        case ID:
            release_info->id = buffer;
            break;
        case ID_LIKE:
            release_info->id_like = buffer;
            break;
        case VERSION:
            release_info->version = buffer;
            break;
        case VERSION_ID:
            release_info->version_id = buffer;
            break;
        case CPE_NAME:
            release_info->cpe_name = buffer;
            break;
        case BUILD_ID:
            release_info->build_id = buffer;
            break;
        case PRETTY_NAME:
            release_info->pretty_name = buffer;
            break;
        case ANSI_COLOR:
            release_info->ansi_color = buffer;
            break;
        case REDHAT_BUGZILLA_PRODUCT:
            release_info->redhat_bugzilla = buffer;
            break;
        case REDHAT_BUGZILLA_PRODUCT_VERSION:
            release_info->redhat_bugzilla_version = buffer;
            break;
        case REDHAT_SUPPORT_PRODUCT:
            release_info->redhat_support = buffer;
            break;
        case REDHAT_SUPPORT_PRODUCT_VERSION:
            release_info->redhat_support_version = buffer;
            break;
        case HOME_URL:
            release_info->home_url = buffer;
            break;
        case DOCUMENTATION_URL:
            release_info->documentation_url = buffer;
            break;
        case SUPPORT_URL:
            release_info->support_url = buffer;
            break;
        case BUG_REPORT_URL:
            release_info->bug_report_url = buffer;
            break;
        case PRIVACY_POLICY_URL:
            release_info->privacy_policy_url = buffer;
            break;
        case LOGO:
            release_info->logo = buffer;
            break;
        case VARIANT:
            release_info->variant = buffer;
            break;
        case VARIANT_ID:
            release_info->variant_id = buffer;
            break;
        default:
            free(buffer);
            break;
        }
}

static inline long
get_os_release_file_size(void)
{
    struct stat *buf = (struct stat *) malloc(sizeof(struct stat));

    if (buf)
    {
        stat(OS_RELEASE, buf);
        long ret = buf->st_size;
        free(buf);
        return ret;
    }

    return 0;
}


static inline u_int32_t
parser_token_length(const char *p, enum parser_token_t token)
{
    const char *tmp = p;
    u_int32_t length = 0;

    while (*tmp)
    {
        length++;
        if (*tmp++ == token)
        {
            break;
        }
    }

    tmp = NULL;

    return length;
}

static struct os_release_t *
parse_os_release(void)
{
    const int handle = open(OS_RELEASE, O_RDONLY);
    long size = get_os_release_file_size();

    if (size <= 0)
    {
        if (handle)
        {
            close(handle);
        }

        return NULL;
    }

    struct os_release_t *release_info = (struct os_release_t *) malloc(sizeof(struct os_release_t));
    char *buffer = (char *) malloc(size + sizeof(char));
    char *value = NULL;
    unsigned int parse_flag = PARSE_KEY;

    switch (handle)
    {
    case -1:
        if (release_info)
        {
            free(release_info);
            release_info = NULL;
        }

        if (buffer)
        {
            free(buffer);
            buffer = NULL;
        }

        return NULL;
    default:
        if (release_info && buffer)
        {
            const char *read_buffer = buffer;
            const ssize_t bytes = read(handle, buffer, size);
            unsigned int hash = 0;
            close(handle);

            if (bytes == size) // data read.
            {
                while (*read_buffer && size)
                {
                    switch (parse_flag)
                    {
                    case PARSE_KEY: {
                        u_int32_t key_length = parser_token_length(read_buffer, TOKEN_EQUAL);
                        size -= key_length;

                        while (key_length--)
                        {
                            hash = *read_buffer++ + (hash << 6) + (hash << 16) - hash;
                        }

                        parse_flag = PARSE_VALUE;
                        continue;
                    }
                    case PARSE_VALUE: {
                        u_int32_t value_length = parser_token_length(read_buffer, TOKEN_NEWLINE) ;
                        value = (char *) malloc(value_length + sizeof(char));
                        size -= value_length;

                        if (value)
                        {
                            char *tmp = value;
                            char v = 0;

                            while (value_length--)
                            {
                                v = *read_buffer++;

                                switch (v)
                                {
                                case TOKEN_QUOTE:
                                    continue;
                                default:
                                    *tmp++ = v;
                                }
                            }

                            buffer_reassignment(value, release_info, &hash);
                            tmp = NULL;
                        }

                        // Reset flag and hash regardless if the value allocation was successful.
                        parse_flag = PARSE_KEY;
                        hash = 0;
                        continue;
                    }
                    }
                }

                read_buffer = NULL;
                value = NULL;

                if (buffer)
                {
                    free(buffer);
                    buffer = NULL;
                }

                return release_info;
            }


            read_buffer = NULL;

            if (release_info)
            {
                free(release_info);
                release_info = NULL;
            }

            if (value)
            {
                free(value);
                value = NULL;
            }

            if (buffer)
            {
                free(buffer);
                buffer = NULL;
            }

            return NULL;
        }

        if (handle)
        {
            close(handle);
        }

        if (release_info)
        {
            free(release_info);
            release_info = NULL;
        }

        if (value)
        {
            free(value);
            value = NULL;
        }

        if (buffer)
        {
            free(buffer);
            buffer = NULL;
        }

        return NULL;
    }
}

struct unix_version_t *
get_unix_information(void)
{
    struct utsname *uname_information = (struct utsname *) malloc(sizeof(struct utsname));
    struct unix_version_t *version = (struct unix_version_t *) malloc(sizeof(struct unix_version_t));

    if (uname_information && version && uname(uname_information))
    {
        version->uname_information = uname_information;
        version->os_release = parse_os_release();
        return version;
    }

    if (uname_information)
    {
        free(uname_information);
        uname_information = NULL;
    }

    if (version)
    {
        free(version);
        version = NULL;
    }

    return NULL;
}

void
free_unix_information(struct unix_version_t *p)
{
    if (p)
    {
        if (p->os_release)
        {
            if (p->os_release->ansi_color)
            {
                free(p->os_release->ansi_color);
            }

            if (p->os_release->bug_report_url)
            {
                free(p->os_release->bug_report_url);
            }

            if (p->os_release->build_id)
            {
                free(p->os_release->build_id);
            }

            if (p->os_release->cpe_name)
            {
                free(p->os_release->cpe_name);
            }

            if (p->os_release->documentation_url)
            {
                free(p->os_release->documentation_url);
            }

            if (p->os_release->home_url)
            {
                free(p->os_release->home_url);
            }

            if (p->os_release->id)
            {
                free(p->os_release->id);
            }

            if (p->os_release->id_like)
            {
                free(p->os_release->id_like);
            }

            if (p->os_release->logo)
            {
                free(p->os_release->logo);
            }

            if (p->os_release->name)
            {
                free(p->os_release->name);
            }

            if (p->os_release->pretty_name)
            {
                free(p->os_release->pretty_name);
            }

            if (p->os_release->privacy_policy_url)
            {
                free(p->os_release->privacy_policy_url);
            }

            if (p->os_release->redhat_bugzilla)
            {
                free(p->os_release->redhat_bugzilla);
            }

            if (p->os_release->redhat_bugzilla_version)
            {
                free(p->os_release->redhat_bugzilla_version);
            }

            if (p->os_release->redhat_support)
            {
                free(p->os_release->redhat_support);
            }

            if (p->os_release->redhat_support_version)
            {
                free(p->os_release->redhat_support_version);
            }

            if (p->os_release->support_url)
            {
                free(p->os_release->support_url);
            }

            if (p->os_release->variant)
            {
                free(p->os_release->variant);
            }

            if (p->os_release->variant_id)
            {
                free(p->os_release->variant_id);
            }

            if (p->os_release->version)
            {
                free(p->os_release->version);
            }

            if (p->os_release->version_id)
            {
                free(p->os_release->version_id);
            }

            free(p->os_release);
            p->os_release = NULL;
        }

        if (p->uname_information)
        {
            free(p->uname_information);
        }

        free(p);
    }
}
