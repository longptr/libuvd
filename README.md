## libuvd
A simple library to provide support for parsing the `os-release` on any linux distribution that has adopted the `os-release` standard [reference](https://www.freedesktop.org/software/systemd/man/os-release.html). 

The `uname` routine is used to obtain the following information:
1. System Name
2. Node name
3. Release
4. Version
5. Machine

## Example
```C
    struct unix_version_t *version = get_unix_information();

    if (version)
    {
        if (version->os_release)
        {
            if (version->os_release->pretty_name) // This may want to be checked as the memory is allocated.
            {
                printf("os %s", version->os_release->pretty_name);
            }
        }

        printf("kernel %s\n", version->uname_information->sysname);
        printf("kernel version %s\n", version->uname_information->release);
        printf("network identifier %s\n", version->uname_information->nodename);
        free_unix_information(version);
    }
```

## License
libuvd is licensed under the MIT license.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
