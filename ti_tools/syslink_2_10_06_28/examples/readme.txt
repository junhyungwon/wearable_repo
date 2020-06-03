Note that these examples are a preview of what's coming in a future SysLink release.
They aren't fully validated, so you may encounter minor issues.  For fully validated
samples, please refer to packages/ti/syslink/samples.

They have intentionally been separated into their own stand-alone .zip files and
build independent of the SysLink makefiles.

Use the unzip command to unpack the examples. Remember to protect
wildcard characters from the shell (unzip must see the wildcard).

Some examples:

1. Unpack all ti_platform_evm3530 examples into the current folder.

% unzip archive/ti_platform_evm3530/\*.zip


2. From the platform folder, unpack all examples to a destination folder.

% cd archive/ti_platforms_evmTI816X
% unzip \*.zip -d /home/examples
